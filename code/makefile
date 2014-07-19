# HyperScript makefile
#
# Modifications:
#
#	$Log: makefile,v $
#	Revision 1.7  2004/06/13 14:15:22  bergsma
#	Comments about AS_SSL, AS_JNI, AS_DLL
#	
#	Revision 1.6  2004/05/03 18:07:47  bergsma
#	Remove ^M
#	
#	Revision 1.5  2004/04/29 08:24:53  bergsma
#	Add comments for POSTGRESSQL
#	
#	Revision 1.4  2003/06/05 22:15:06  bergsma
#	Include library for pow() function, exponentation.
#	
#	Revision 1.3  2002/09/20 00:03:20  bergsma
#	Comment about MEMTRACK
#	
#	Revision 1.2  2002/09/03 21:24:15  bergsma
#	Cleaned up comment section and consolidated compiler definitions.
#	
#
# Description of Compiler OPTIONS:
#
#	Automatically set
#	AS_UNIX		UNIX OS
#	AS_VMS		VMS OS
#	AS_WINDOWS	Windows NT/2000/XP
#	AS_PROMIS	PROMIS Application
#
#	Must be set
#	AS_SOLARIS	Solaris UNIX
#	AS_HPUX		HP-UX UNIX
#	AS_DMBX		FASTech's DMBX
#	AS_SQL		SQL access 
#		+ AS_SQLSERVER	- SQLSERVER	
#		+ AS_MYSQL	- MySQL
#		+ AS_PROC	- ORACLE, using proC pre-compiler
#		+ AS_PGSQL	- POSTgresSQL
#
#	Other features:
#	AS_SSL	OpenSSL
#	AS_DLL	Windows DLL
#	AS_JNI	Java JNI
#
#	Optional
#	AS_MEMTRACK	Memory Leak Detector
#
# include file dependancies
INCLUDES = auto.h hypdef.h 
OBJECTS = aimsg.o data.o dateparse.o hash.o channel.o signal.o tcp.o util.o hs.o branch.o cgi.o env.o fileio.o frame.o function.o hsms.o http.o hyp.o concept.o instance.o label.o load.o memtrack.o method.o operand.o operator.o parse.o port.o promis.o route.o router.o secs.o secs1.o secs2.o sock.o sort.o ssl.o stack.o stmt.o system.o type.o sql.o
PROC_FLAGS = sqlcheck=full define=AS_UNIX define=AS_HS

# SOLARIS
#CC = gcc
#CFLAGS = -g -O -Wall -I.
#CDEFS = -DAS_SOLARIS 
#CDEFS = -DAS_SOLARIS -DAS_DMBX -DAS_SQL
#CDEFS = -DAS_SOLARIS -DAS_DMBX
#CDEFS = -DAS_SOLARIS 
#LIBS  = -lsocket -lnsl -L $(CELLworks)/FASTech/SDT/lib -lCw 
#LIBS  = -lsocket -lnsl
#LIBS = -lsocket -lnsl
#LIBS = -L$(ORACLE_HOME)/lib -lclntsh 

# HP-UX
#CC = c89
#CFLAGS = -Ac -g -w -I.
#CDEFS = -DAS_HPUX 
#LIBS = 

# DEC UNIX
CC = cc
CFLAGS =  -g3 -O -Wall -I.
CDEFS = -DAS_SQL -DAS_MYSQL -DAS_TRUE64
LIBS  = -lm -L/usr/lib/mysql -lmysqlclient 
#LIBS  = -L $(CELLworks)/FASTech/SDT/lib -lCw
#LIBS = -L$(ORACLE_HOME)/lib -lclntsh 

# LINUX
##CC = gcc 
#CC = g++
#CC = insure 
##CFLAGS = -g -O -Wall -I. -I/usr/include
##CDEFS =   -DAS_SQL -DAS_MYSQL -DAS_MEMTRACK -DAS_SSL
##CPPFLAGS=-I/usr/kerberos/include LDFLAGS=-L/usr/kerberos/lib
##LIBS = -lc -L/usr/lib/mysql -lmysqlclient -lz -lm -lssl -lcrypto
#LIBS = -L/usr/lib -L$(ORACLE_HOME)/lib -lclntsh 
#LIBS = -lefence

## POSTGRES
##CFLAGS = -g -O -Wall -I. -I/usr/kerberos/include -I/usr/include
##CDEFS = -DAS_SSL  -DAS_SQL -DAS_PGSQL
##LIBS = -lc -lz -lm -lssl -lcrypto -L/usr/kerberos/lib -L/usr/lib/pgsql/lib -lpq


all: hyperscript
hyperscript: hs
hs : $(OBJECTS)
	$(CC) \
	$(OBJECTS) \
	$(LIBS) -o $@

clean :
	rm -f *.o *~ *.pid *.dcl *.cod *.cud hs core 
#
# Compiles common to AutoRouter and HyperScript (ie: AutoServer)
#
aimsg.o : aimsg.c  $(INCLUDES) aimsg.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

data.o : data.c   $(INCLUDES) data.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

dateparse.o : dateparse.c  $(INCLUDES)
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

hash.o : hash.c   $(INCLUDES) hash.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

channel.o : channel.c   $(INCLUDES) channel.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

sock.o : sock.c $(INCLUDES) sock.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

signal.o : signal.c   $(INCLUDES) signl.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

tcp.o : tcp.c $(INCLUDES) tcp.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

util.o : util.c $(INCLUDES) util.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

router.o : router.c $(INCLUDES) router.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

hs.o : hs.c  $(INCLUDES)
	$(CC) $(CFLAGS) $(CDEFS)  -c $*.c -o $@	

branch.o : branch.c  $(INCLUDES) branch.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

env.o : env.c  $(INCLUDES) env.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

cgi.o : cgi.c  $(INCLUDES) cgi.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

concept.o : concept.c  $(INCLUDES) concept.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

fileio.o : fileio.c  $(INCLUDES) fileio.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

frame.o : frame.c  $(INCLUDES)  frame.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

function.o : function.c  $(INCLUDES) function.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

hyp.o : hyp.c  $(INCLUDES) hyp.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

hsms.o : hsms.c  $(INCLUDES) hsms.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

http.o : http.c  $(INCLUDES) http.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

instance.o : instance.c  $(INCLUDES) instance.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

label.o : label.c  $(INCLUDES) label.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

load.o : load.c  $(INCLUDES) load.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

method.o : method.c  $(INCLUDES) method.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

memtrack.o : memtrack.c  $(INCLUDES) memtrack.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

operand.o : operand.c  $(INCLUDES) operand.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

operator.o : operator.c  $(INCLUDES) operator.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

parse.o : parse.c  $(INCLUDES) parse.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

port.o : port.c  $(INCLUDES) port.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

promis.o : promis.c  $(INCLUDES) promis.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

route.o : route.c  $(INCLUDES) route.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

secs.o : secs.c  $(INCLUDES) secs.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

secs1.o : secs1.c  $(INCLUDES) secs1.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

secs2.o : secs2.c  $(INCLUDES) secs2.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

sort.o : sort.c  $(INCLUDES) sort.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

ssl.o : ssl.c  $(INCLUDES) ssl.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

stack.o : stack.c  $(INCLUDES) stack.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

stmt.o : stmt.c  $(INCLUDES) stmt.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

sql.o : sql.c  $(INCLUDES) sql.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

##########################################################
# For PROC support, uncomment this section
# 
#sql.c : sql.pc
#	proc iname=$*.pc $(PROC_FLAGS) 
#
##########################################################

system.o : system.c  $(INCLUDES) system.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@

type.o : type.c  $(INCLUDES) type.h
	$(CC) $(CFLAGS) $(CDEFS) -c $*.c -o $@
