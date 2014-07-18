$ v = f$verify(0)
$!+  DIRDEF.COM  - do directory differences between two directories.
$!
$!  Mike Bergsma  6/25/85
$!
$! Description:
$!  Does two main tasks:
$!    1. Tells of any differences in file specs between
$!       two directories, ie: what's been added, what's been
$!       deleted, excluding objects, images, librarys, etc
$!    2. For matched fileSpecs, does a VMS differences between
$!       them, excluding objects, images, librarys, etc.
$!
$! Arguments:
$!  p1,p2	- the source and release directories respec. FROM which the
$!   		  differences are to be compared
$!  p3		- the differences directory.
$!  p4		- if specified, p4 is the starting file from the source account
$!		  to begin comparing from. Newsource and newrel are opened for
$!		  append.
$!
$!
$! Modifications:
$!
$!   $Log: dirdif.com,v $
$!   Revision 1.2  2003/01/16 14:40:47  bergsma
$!   V3.1.0
$!   Added Modification tag.
$!
$!
$!-
$  set noon
$  on control_y then goto ERROR
$  set message /nosev/noid/nofac/notext
$  dif :== differences
$  say = "write sys$output"
$  keepTypes =	".C" + ".H" + ".FOR" + ".COM" + ".SCR" + ".HYP"
$!		".SBM" + -
$!		".MAR" + -
$!		".BLD" + -
$!		".BAT" + -
$!		".DEF" + -
$!		".CMD" + -
$!		".CLD" + -
$!		".DLC" + -
$!		".DAT" + -
$!		".NLS" + -
$!		".MAS" + -
$!		".SOP"
$
$  keepTypesLen = f$length(keepTypes)
$  say "DIRDEF.COM - Directory differences"
$  line = ""
$!
$! If p1 is not present prompt for the source directory
$!
$  if p1 .nes. "" then goto GOT_P1
$ REASK_P1:
$  inquire p1 "Source or Development directory"
$  if p1 .eqs. "" then goto REASK_P1
$ GOT_P1:
$!
$! Check the source dir
$!
$  fullSpec = "''p1'"
$  source = "''f$parse(fullSpec,,,"DEVICE")'" + -
            "''f$parse(fullSpec,,,"DIRECTORY")'" - "]"
$  howcum = "Bad source directory syntax"
$  dir/out=nl: 'source']
$  if $severity .eq. 2 then goto ERROR
$!
$! If p2 is not present prompt for the source directory
$!
$  if p2 .nes. "" then goto GOT_P2
$ REASK_P2:
$  inquire p2 "Release or Upgrade directory"
$  if p2 .eqs. "" then goto REASK_p2
$ GOT_P2:
$!
$! Check the release directory
$!
$  fullSpec = "''p2'"
$  release = "''f$parse(fullSpec,,,"DEVICE")'" + -
          "''f$parse(fullSpec,,,"DIRECTORY")'" - "]"
$  howcum = "Bad release directory syntax"
$  dir/out=nl: 'release']
$  if $severity .eq. 2 then goto ERROR
$
$  howcum = "Source and release directories are identical"
$  if ( release .eqs. source ) then goto ERROR
$!
$! If p3 is not present prompt for the differences directory
$!
$  if p3 .nes. "" then goto GOT_P3
$ REASK_P3:
$  inquire p3 "Differences directory"
$  if p3 .eqs. "" then goto REASK_p3
$ GOT_P3:
$!
$! Check the default directory
$!
$  fullSpec = "''p3'"
$  default = "''f$parse(fullSpec,,,"DEVICE")'" + -
          "''f$parse(fullSpec,,,"DIRECTORY")'" - "]"
$  howcum = "Bad default (default) directory syntax"
$  dir/out=nl: 'default']
$  if $severity .eq. 2 then goto ERROR
$  sd 'default']
$
$  howcum = "Default directory can't be either source or release directories"
$  if f$locate(default,source) .eq. 0 .or. -
      f$locate(default,release) .eq. 0 then goto ERROR
$
$  oldpriv = f$setprv("SYSPRV")
$  startFile = "*"
$  qual = "/write"
$  if p4 .eqs. "" then goto BEGIN
$     howcum = "P4 file specification invalid or does not exist"
$     startfile = f$search(p4)
$     if startFile .eqs. "" then goto ERROR
$     if f$search("''default']newSource.txt") .nes. "" then qual = "/append"
$     if f$search("''default']newRel.txt") .nes. "" then qual = "/append"
$ BEGIN:
$  say "%% WARNING - only top level file versions are compared"
$  say " "
$  say "...starting"
$  open 'qual' newSource 'default']newSource.txt
$  open 'qual' newrel 'default']newrel.txt
$  if qual .eqs. "/append" then goto INIT
$  write newSource "Files found in ''source'] not found in ''release']"
$  write newSource -
"---------------------------------------------------------------------------"
$  write newrel "Files found in ''release'] not found in ''source']"
$  write newrel -
"---------------------------------------------------------------------------"
$ INIT:
$  ! Initialize starting files in source and release directorys
$  sourceSpec = "''source'...]*.*"
$  releaseSpec = "''release'...]*.*"
$  ! Get first files in source and release directories
$  sourceFile = "''f$search(sourceSpec,1)'"
$  releaseFile = "''f$search(releaseSpec,2)'"
$  howcum = "Control-Y abort"
$  if startFile .eqs. "*" then goto LOOP
$ SCAN:
$   if sourceFile .eqs. startFile then goto LOOP
$   sourceFile = "''f$search(sourceSpec,1)'"
$   goto SCAN
$ LOOP:
$  ! Exit only when both directory searches are complete
$  say "''sourceFile'	''releaseFile'"
$  if "''sourceFile'" .eqs. "" .or. "''releaseFile'" .eqs. "" then goto DONE
$  ! Get the common parts of the file names and remove the version code.
$  sourceName = sourceFile - source
$  sourceName = "''f$extract(0,f$locate(";",sourceName),sourceName)'"
$  releaseName = releaseFile - release
$  releaseName = "''f$extract(0,f$locate(";",releaseName),releaseName)'"
$  ! Determine the current directory for the release and source files.
$  ! Whichever directory is ahead must wait for the other to catch up.
$  releaseDir = f$extract(0,f$locate("]",releaseName),releaseName)
$  sourceDir = f$extract(0,f$locate("]",sourceName),sourceName)
$  ! Test if the directories are not equal - if so we must figure out which
$  ! one is ahead. 
$  if "''sourceDir'" .eqs. "''releaseDir'" then goto SAME_DIR
$  ! Else directories are not the same. The source and/or release 
$  ! directory have just changed. The one that is alphabetically greater is 
$  ! the one ahead.
$  if sourceDir .gts. releaseDir then goto SKIP_SOURCE
$  ! Otherwise the release is ahead
$  goto SKIP_RELEASE
$ SAME_DIR:
$  ! Determine file types and set the keep flag if type is in the keep list.
$  sourcetype = f$parse(sourceFile,,,"TYPE")
$  releasetype = f$parse(releaseFile,,,"TYPE")
$  ! Assume we will not keep (skip) the file(s).
$  keepSource = 0
$  keepRelease = 0                                       
$  if f$locate(sourceType,keepTypes) .lt. keepTypesLen then keepSource = 1
$  if f$locate(releaseType,keepTypes) .lt. keepTypesLen then keepRelease = 1
$  ! Are the files identical?
$  if "''sourceName'" .nes. "''releaseName'" then goto NEW_FILE ! IF .NES. NO
$  ! Else files are identical. 
$  if ( .not. keepSource .or. .not. keepRelease ) then goto IGNORE
$  dirSpec = default + "''f$extract(0,f$locate("]",sourceName),sourceName)'"
$  dir/out=nl:  'dirSpec']
$  if $severity .eq. 2 then create/dir 'dirSpec']
$  dif/out='default''sourceName' 'sourceFile' 'releaseFile'
$  open/read temp 'default''sourceName' 
$  read temp line
$  difs = f$locate("*****",line) 
$  close temp
$  if difs .ne. 0 then delete 'default''sourceName';*
$ IGNORE:
$  ! Before proceeding, get next file specs from both directories
$  sourceFile = "''f$search(sourceSpec,1)'"
$  releaseFile = "''f$search(releaseSpec,2)'"
$  goto LOOP   
$ NEW_FILE:
$  if sourceName .gts. releaseName then goto NEW_RELEASE
$ NEW_SOURCE:
$  if ( keepSource ) then write newSource "''sourceFile'"
$ SKIP_RELEASE:
$  sourceFile = "''f$search(sourceSpec,1)'"
$  goto LOOP
$ NEW_RELEASE:
$  if ( keepRelease ) then write newrel "''releaseFile'"
$ SKIP_SOURCE:
$  releaseFile = "''f$search(releaseSpec,2)'"
$  goto LOOP
$
$ DONE:
$  close newSource 
$  close newrel 
$  tmp = f$setprv(oldpriv)
$  sd 'default']
$  dirl/out=dirdif.lis [...]*.*
$  say "...finished!"
$  set message /sev/id/fac/text
$  if v then set ver
$  exit
$
$ ERROR:
$  say "%DirDif aborted"
$  say "- ''howcum'"
$  close newSource 
$  close newrel 
$  set message /sev/id/fac/text
$  exit
