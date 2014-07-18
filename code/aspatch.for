c
c Apply a patch to a source. Output to stdout.
c The patch file contains the following delimiters:
c    >BS - start of text before patch
c    >BE - end of text before patch
c    >IS - start of text to insert
c    >IE - end of text to insert
c    >CS - start of text to comment out
c    >CE - end of text to comment out
c    >ES - start of text after patch
c    >EE - end of text after patch
c
      program aspatch

      implicit none

      integer*4 maxline, maxpatch, maxmatch
      parameter (maxline=256, maxpatch=256, maxmatch=256)
c
c Operation states
c
      integer*4 PATCH, HOLD, SEARCH
      parameter (PATCH=1, HOLD=2, SEARCH=3)
c
c Patch file text types
c    N - None
c    B - Before text to match
c    E - End text to match
c    I - Insert text to patch
c    C - Comment-out text to match
c
c Modifications
c
c	$Log: aspatch.for,v $
c	Revision 1.3  2003-02-18 02:01:47  bergsma
c	Misplaced continuation mark.
c	
c	Revision 1.2  2002/11/12 20:06:18  bergsma
c	Add recl=256,recordtype='variable' to output file.
c	
c
c
      integer*4 N, B, E, I, C
      parameter (N=0, B=1, E=2, I=3, C=4)

      character*132 patfile, srcfile, outfile, errfile
      character*132 c_header, c_footer
      character*(maxline) patlines(maxpatch), bmatchlines(maxmatch)
      character*(maxline) ematchlines(maxmatch), patchline
      character*(maxline) patline, srcline, matchline
      character*(maxline) patchline2, srcline2, matchline2
      integer*4 npat, nbmatch, nematch, nmatch, ntomatch, npatch
      integer*4 state, pattype, matchtype, k, j
      integer*4 nargs, stdin, stdout, stderr
      integer*4 patlun, srclun, outlun, errlun, opstat
      integer*4 string_len, patline_len, srcline_len, matchline_len
      integer*4 string_len2, patchline_len2, srcline_len2, matchline_len2
      integer*4 patchline_len, c_header_len, c_footer_len
      integer*4 srcline_len_read, patline_len_read
      logical matchblank, foundblank, matchimmediate
      integer*4 anchor

      data stdin, stdout, stderr / 5, 6, 6 /
      data patlun, srclun, outlun, errlun / 10, 11, 12, 13 /
      data c_header /'!B-HS! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'/
      data c_footer /'!E-HS! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!'/
      data c_header_len, c_footer_len / 48, 48 /
c
c Two logicals must exist.  
c 1. as_source
c 2. as_patch
c
c Need to change this for VMS?
c
c      nargs = iargc()
c      if (nargs .ne. 2) then
c         write (errlun, *) 'Usage: aspatch patchfile sourcefile'
c         stop
c      endif
c
c      call getarg (1, patfile)
c      call getarg (2, srcfile)
c
        patfile = 'as_patch'
        srcfile = 'as_source'
        outfile = 'as_output'
        errfile = 'as_error'

c Open the input files
c
      open (unit=patlun, file=patfile, status='old',
     1      form='formatted', access='sequential', iostat=opstat)
      if (opstat.ne.0) then
         write (errlun, *) 'ERROR opening patch file '//patfile
         stop
      endif

      open (unit=srclun, file=srcfile, status='old',
     1      form='formatted', access='sequential', iostat=opstat)
      if (opstat.ne.0) then
         write (errlun, *) 'ERROR opening source file '//srcfile
         close (unit=patlun)
         stop
      endif
c
c Open the output files
c
      open (unit=outlun, file=outfile, status='new',
     1      form='formatted', carriageControl='list', access='sequential', 
     1		iostat=opstat,recl=256,recordType='variable')
      if (opstat.ne.0) then
         write (errlun, *) 'ERROR opening output file '//outfile
         close (unit=patlun)
         close (unit=srclun)
         stop
      endif

      open (unit=errlun, file=errfile, status='new',
     1      form='formatted', carriageControl='list', access='sequential', iostat=opstat)
      if (opstat.ne.0) then
         write (errlun, *) 'ERROR opening error file '//errfile
         close (unit=patlun)
         close (unit=srclun)
         close (unit=outlun,disp='delete')
         stop
      endif

c
c Start reading the patch file
c
      read (patlun, 2000, iostat=opstat) patline_len_read,patline(1:maxline)

      pattype = N
      state = HOLD

      do while (opstat .eq. 0)
      patline_len = string_len (patline, maxline)
c
c Look for tags in the patch file. Set the state accordingly
c 
         if ('>BS' .eq. patline(1:3)) then
            pattype = B
            nbmatch = 0
            state = HOLD
         elseif ('>BE' .eq. patline(1:3)) then
            state = SEARCH
            matchtype = B
         elseif ('>ES' .eq. patline(1:3)) then
            pattype = E
            nematch = 0
            state = HOLD
         elseif ('>EE' .eq. patline(1:3)) then
            state = SEARCH
            matchtype = E
         elseif ('>IS' .eq. patline(1:3)) then
            pattype = I
            npat = 0
            state = HOLD
         elseif ('>IE' .eq. patline(1:3)) then
            state = PATCH
         elseif ('>CS' .eq. patline(1:3)) then
            pattype = C
            npat = 0
            state = HOLD
         elseif ('>CE' .eq. patline(1:3)) then
            state = PATCH

         else
            if (pattype .eq. B) then
               nbmatch = nbmatch + 1
               if (nbmatch .gt. maxmatch) then
                  write (errlun, *)
     1                 'ERROR: exceeded maximum number of match lines'
                  stop 'Increase maxmatch'
               endif
               bmatchlines(nbmatch) = patline(1:patline_len_read)
            elseif (pattype .eq. E) then
               nematch = nematch + 1
               if (nematch .gt. maxmatch) then
                  write (errlun, *)
     1                 'ERROR: exceeded maximum number of match lines'
                  stop 'Increase maxmatch'
               endif
               ematchlines(nematch) = patline(1:patline_len_read)
            elseif (pattype .eq. I .or. pattype .eq. C) then
               npat = npat + 1
               if (npat .gt. maxpatch) then
                  write (errlun, *)
     1                 'ERROR: exceeded maximum number of patch lines'
                  stop 'Increase maxpatch'
               endif
               patlines(npat) = patline(1:patline_len_read)
            endif

         endif

         read (patlun, 2000, iostat=opstat) patline_len_read,patline(1:maxline)
c
c Search the source file for matching lines
c
         if (state .eq. SEARCH) then
c
c Match Before lines starting anywhere in the source file
c
            if (matchtype .eq. B) then
               ntomatch = nbmatch
               matchimmediate = .false.
c               write (stdout, *) '>BS'
            elseif (matchtype .eq. E) then
               ntomatch = nematch
               matchimmediate = .true.
c               write (stdout, *) '>ES'
            else
               ntomatch = 0
            endif

            matchblank = .false.
            foundblank = .false.
            nmatch = 0
            do while (nmatch .lt. ntomatch)
c
c Don't get the next source line if the previous matchline was blank and
c the match failed. We want to try the match again with the next match line.
c
               if (.not. matchblank) then
                  read (srclun, 2000, err=100, end=100)
     1                 srcline_len_read,srcline(1:maxline)
               srcline_len = string_len (srcline, maxline)
               endif
c
c Don't get the next matchline if the previous source line was blank and
c the match failed. We want to try the match again with the next source line.
c
               if (.not. foundblank) then
                  if (matchtype .eq. B) then
                     matchline = bmatchlines(nmatch+1)
                  elseif (matchtype .eq. E) then
                     matchline = ematchlines(nmatch+1)
                  endif
                  matchline_len = string_len(matchline, maxline)
c
c Set lastblank if the current matchline is not blank but the
c previous matchline was blank
c
                  if (matchline_len .eq. 0) then
                     matchblank = .true.
                  else
                     matchblank = .false.
                  endif
               endif
c
c Try to find a match. Be forgiving about blank lines.
c
               if (srcline_len .eq. 0) then
                  foundblank = .true.
               endif

	       anchor = 0
	       j = 0
	       do k=1,srcline_len
		 if ( srcline(k:k) .eq. ' ' .or. srcline(k:k) .eq. CHAR(9) ) then
		   ! Whitespace encountered.  
		   if ( anchor .eq. 0 ) then
		     ! First occurance
	             j = j + 1
		     srcline2(j:j) = ' '
		     anchor = j
		   endif
		 else
	           j = j + 1
		   srcline2(j:j) = srcline(k:k) 
		   anchor = 0
		 endif
	       enddo
	       srcline_len2 = j	

	       anchor = 0
	       j = 0
	       do k=1,matchline_len
		 if ( matchline(k:k) .eq. ' ' .or. matchline(k:k) .eq. CHAR(9) ) then
		   ! Whitespace encountered.  
		   if ( anchor .eq. 0 ) then
		     ! First occurance
	             j = j + 1
		     matchline2(j:j) = ' '
		     anchor = j
		   endif
		 else
	           j = j + 1
		   matchline2(j:j) = matchline(k:k) 
		   anchor = 0
		 endif
	       enddo
	       matchline_len2 = j	

               if ( !!!srcline_len .ne. 0 .and.
     1              srcline_len2 .eq. matchline_len2 .and.
     2              srcline2(1:srcline_len2) .eq.
     3              matchline2(1:matchline_len2)) then
                  matchblank = .false.
                  foundblank = .false.
                  nmatch = nmatch + 1
c
c If the source line is blank then ignore it and try to match then
c next source line.
c
               elseif (foundblank) then
                  continue
c
c If the matchline is blank then ignore it and try to match the next
c matchline.
c
               elseif (matchblank) then
                  nmatch = nmatch + 1
c
c Match End lines immediately. Stop search if any lines fail to match.
c
               elseif (matchimmediate) then
                  matchblank = .false.
                  foundblank = .false.
                  goto 100
c
c Match Before lines as a set. That is, all Before lines must match
c consecutive lines in the source file for a positive match. If any
c lines fail to match, reset the counter and try again.
c
               else
                  matchblank = .false.
                  foundblank = .false.
                  nmatch = 0

               endif
c
c Write out the source line unless we are skipping a blank matchline
c
               if (.not. matchblank) then
                 write (outlun, 2001) srcline(1:srcline_len_read)
               endif

            enddo

 100        continue
            if (nmatch .eq. ntomatch) then
               state = HOLD
c               if (matchtype .eq. B) then
c                  write (stdout, *) '>BE'
c               else
c                  write (stdout, *) '>EE'
c               endif
            else
               if (matchtype .eq. B) then
                  write (errlun, *)
     1                 'ERROR: failed to match Begin lines:'
               else
                  write (errlun, *)
     1                 'ERROR: failed to match End lines:'
               endif
               do j=1, ntomatch
                  if (matchtype .eq. B) then
                     matchline = bmatchlines(j)
                  elseif (matchtype .eq. E) then
                     matchline = ematchlines(j)
                  endif
                  matchline_len = string_len(matchline, maxline)
                  write (errlun, *) matchline(1:matchline_len)
               enddo
               stop 'patch failed'
            endif
c
c Patch the source file
c
         elseif (state .eq. PATCH) then
c            if (pattype .eq. C) then
c               write (stdout, *) '>CS'
c            else
c               write (stdout, *) '>IS'
c            endif
            matchblank = .false.
            foundblank = .false.
            npatch = 0
            do while (npatch .lt. npat)
c
c Don't get a new patchline if a blank source line was found while
c trying to match comment lines. We want to try the match again
c with the next source line
c
               if (.not. foundblank) then
                  patchline = patlines(npatch+1)
                  patchline_len = string_len (patchline, maxline)
               endif
c
c Comment-out the lines.
c Make sure they match exactly in the source file.
c Ignore blank lines
c
               if (pattype .eq. C) then
c
c Don't get a new source line if a blank patchline was just encountered.
c We want to try the match again with the next patchline.
c
                  if (.not. matchblank) then
                     read (srclun, 2000, err=200, end=200)
     1                    srcline_len_read,srcline(1:maxline)
                     srcline_len = string_len (srcline, maxline)
                     if (srcline_len .eq. 0) then
                        foundblank = .true.
                     else
                        foundblank = .false.
                     endif
                  endif

                  if (.not. foundblank) then
                     if (patchline_len .eq. 0) then
                        matchblank = .true.
                     else
                        matchblank = .false.
                     endif
                  endif

	       anchor = 0
	       j = 0
	       do k=1,srcline_len
		 if ( srcline(k:k) .eq. ' ' .or. srcline(k:k) .eq. CHAR(9) ) then
		   ! Whitespace encountered.  
		   if ( anchor .eq. 0 ) then
		     ! First occurance
	             j = j + 1
		     srcline2(j:j) = ' '
		     anchor = j
		   endif
		 else
	           j = j + 1
		   srcline2(j:j) = srcline(k:k) 
		   anchor = 0
		 endif
	       enddo
	       srcline_len2 = j	

	       anchor = 0
	       j = 0
	       do k=1,patchline_len
		 if ( patchline(k:k) .eq. ' ' .or. patchline(k:k) .eq. CHAR(9) ) then
		   ! Whitespace encountered.  
		   if ( anchor .eq. 0 ) then
		     ! First occurance
	             j = j + 1
		     patchline2(j:j) = ' '
		     anchor = j
		   endif
		 else
	           j = j + 1
		   patchline2(j:j) = patchline(k:k) 
		   anchor = 0
		 endif
	       enddo
	       patchline_len2 = j	

                  if ( !!!srcline_len .ne. 0 .and.
     1              srcline_len2 .eq. patchline_len2 .and.
     2              srcline2(1:srcline_len2) .eq.
     3              patchline2(1:patchline_len2)) then
                     npatch = npatch + 1
c
c Uncomment these lines to insert a header comment
c
c                     if (npatch .eq. 1) then
c                        write (outlun, 2001) c_header(1:c_header_len)
c                     endif
c
                     patchline = '!HS!'//patchline(1:patchline_len)
                     patchline_len = patchline_len + 4
                     write (outlun, 2001) patchline(1:patchline_len)
                     matchblank = .false.
                     foundblank = .false.
c
c Uncomment these lines to insert a footer comment
c
c                     if (npatch .eq. npat) then
c                        write (outlun, 2001) c_footer(1:c_footer_len)
c                     endif
c
c If a blank source line is found, then just write it out
c
                  elseif (foundblank) then
                    write (outlun, 2001) srcline(1:srcline_len_read)
c
c If the patchline is blank then ignore it and try to match the next
c patchline.
c
                  elseif (matchblank) then
                     npatch = npatch + 1
c
c No match, exit
c
                  else
                     go to 200
                  endif
c
c Insert the lines from the patch
c
               elseif (pattype .eq. I) then
                  write (outlun, 2001) patchline(1:patchline_len)
                  npatch = npatch + 1

               endif

            enddo

 200        continue
            if (npatch .eq. npat) then
               state = HOLD
c               if (pattype .eq. C) then
c                  write (stdout, *) '>CE'
c               else
c                  write (stdout, *) '>IE'
c               endif
            else
               if (pattype .eq. C) then
                  write (errlun, *)
     1                 'ERROR: failed to comment-out lines:'
               else
                  write (errlun, *)
     1                 'ERROR: failed to insert lines:'
               endif

               do j=1, npat
                  patchline = patlines(j)
                  patchline_len = string_len (patchline, maxline)
                  write (errlun, *) patchline(1:patchline_len)
               enddo
               stop 'Patch failed'
            endif

         endif

      enddo
c
c Finished with patches
c Cat the rest of the source file, and we're done
c                  
      read (srclun, 2000, iostat=opstat) srcline_len_read,srcline(1:maxline)
      do while (opstat .eq. 0)
         !!!srcline_len = string_len (srcline, maxline)
         write (outlun, 2001) srcline(1:srcline_len_read)
         read (srclun, 2000, iostat=opstat) srcline_len_read,srcline(1:maxline)
      enddo
      
      close (unit=patlun)
      close (unit=srclun)
      close (unit=outlun)
      close (unit=errlun,disp='delete')

 2000 format (q,a)
 2001 format (a)
 
      end
c
c Return the length of a string.
c max_string_len is the maximum length
c
      integer*4 function string_len (in_string, max_string_len)

      implicit none

      character*(*) in_string
      integer*4 max_string_len

      integer*4 i

      do i = max_string_len, 1, -1
         if (in_string(i:i) .ne. ' ' .and.
     1      ichar(in_string(i:i)) .ne. 0) goto 10
      enddo

 10   string_len = i

      return
      end
      
