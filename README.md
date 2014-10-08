# HS

Refer to the Wiki for detailed user guide, installation guides, and language guides.

Otherwise, to build HS, the following README notes describe the process.

# Sticky Notes

## UNIX Build Procedures

To compile and link HS 

```
./configure
make
```

However, the AUTOCONF does not yet handle some of the compiler directives, such as AS_SSL,AS_GD,AS_SQL,AS_ORACLE,AS_SQLSERVER,and AS_MYSQL.

To build in these features, edit the original makefile and change the CDEF and LIBS variables.
For example:

```
CDEFS =  -DAS_SSL -DAS_SQL -DAS_MYSQL -DAS_GD
LIBS = -lc -lm -lssl -lcrypto -lgd -ljpeg -L/usr/lib/mysql -lmysqlclient
```

The full list of compiler directives are listed here:
```
#	Automatically set
#	AS_UNIX		UNIX OS
#	AS_VMS		VMS OS
#	AS_WINDOWS	Windows NT/2000/XP
#	AS_PROMIS	PROMIS Application
#	AS_PPC		Win32 WCE
#	AS_ALPHA	Alpha Processor
#
#	Must be set
#	AS_SOLARIS	Solaris UNIX
#	AS_HPUX		HP-UX UNIX
#	AS_TRU64	Tru64 UNIX
#	AS_DMBX		FASTech's DMBX
#	AS_SQL		SQL access 
#		+ AS_SQLSERVER	- SQLSERVER	
#		+ AS_MYSQL	- MySQL
#		+ AS_PROC	- ORACLE, using proC pre-compiler
#		+ AS_PGSQL	- POSTgresSQL
#
#	TCP/IP Stack related
#	AS_UCX
#	AS_MULTINET
#	AS_
#
#	Other features:
#	AS_SSL	OpenSSL
#	AS_DLL	Windows DLL (for both ActiveX and JAVA controls)
#	AS_JNI	Java JNI (for JAVA control)
#	AS_ATL  Windows ATL (for ActiveX control)
#	AS_GD   GDlib graphics api
#	AS_MAPI Windows RPC
#	AS_OWL  Semantic Web (future)
#	
#	Optional
#	AS_MEMTRACK	Memory Leak Detector
```

## WIN32 (VS 2008) HS Build Procedures

### HS Standalone, HS.EXE

Change the compiler (preprocessor) directives to create an image with different features.
For example, here are some combinations:
```
(none)  ->  to make base HS.EXE
AS_SQL, AS_SQLSERVER  -> make HS.EXE with SQLSERVER functionality
AS_SQL, AS_SQLSERVER, AS_SSL -> make HS.EXE with SQLSERVER and SSL functionality
AS_SQL, AS_MYSQL -> make HS.EXE with MYSQL functionality
AS_SQL, AS_MYSQL, AS_SSL -> make HS.EXE with SQLSERVER and SSL functionality
AS_SQL, AS_ORACLE -> make HS.EXE with ORACLE functionality
AS_SQL, AS_ORACLE, AS_SSL -> make HS.EXE with ORACLE and SSL functionality
AS_SQL, AS_PGSQL -> make HS.EXE with POSTGRES functionality
AS_SQL, AS_PGSQL, AS_SSL -> make HS.EXE with POSTGRES and SSL functionality
AS_SSL -> make HS.EXE with SSL functionality

```

Naturally, you will need the linker to find the required libraries.  For example:

```
ws2_32.lib   <- for basic app TCP access
libmysql.lib  < for AS_MYSQL
ntwdblib.lib  <- for AS_SQLSERVER
ssleay32.lib <- for AS_SSL
libeay32.lib <- for AS_SSL
oci.lib <- for AS_ORACLE
odbc32.lib <- for AS_ORACLE
odbccp32.lib <- for AS_ORACLE
```

### HS-DOM (Document Object Model), ActiveX control HSX.DLL deployed using webpickle.cab


* From VS, select “Build hsx”  
 AS_ATL and AS_SSL are the directives used

* You get HSX.DLL in the Release directory  
Copy it up one level to the hs\hsx and CVS it

* cd to \abinition\hs\hsx and execute MAKECABSIGNED.BAT  
password is Abinition

* You get WebPickle.cab in \Abinition\hs\hsx  
CVS it, then copy it to \hs\deploy and CVS it

### HS-DOM (Document Object Model), JAVA control HS.DLL, deployed using abinition.jar

* From VS, select "Build hsdll"  
AS_DLL, AS_JNI, AS_SSL are the directives used

* You get HS.DLL and HSDLL.LIB in the Release directory  
CVS them

* cd to \abinition\hs\ and execute the ANT sequences.  
(make sure JAVA is 1.6 or recent)  
You get Abinition.jar in deploy
```
		ant clean
		ant init
		ant genkey
		
		ant jar		<=== repeatable
```
	
### HS/GD

Installing supporting packages
	
> For Windows, obtain the gd.lib, either generating from the gd.dll 
> file, or getting a download of gd.lib off of the internet.  Alternatively,
> it is possible to build gd.lib from the gd sources, but that has not been
> done on windows.

For Linux, first install all the required support packages so that the include
areas and libraries can be found:

#### zlib
```
			tar -xvf zlib-1.2.3.tar.tar
			cd zlib-1.2.3/
			./configure
			make
			su
			make install
```
#### jpeglib

```
			tar -xvzf jpegsrc.v6b.tar.gz
			cd jpeg-6b/ 

			./configure
			make
			su
			make install
```
#### libpng
```
			tar -xvzf libpng-1.2.35.tar.gz
			cd libpng-1.2.35/
			./configure
			make
			su
			make install
```
#### FreeType
```
			tar -xvf freetype-2.1.10.tar.tar
			cd freetype-2.1.10
			./configure
			make
			su
			make install
```
#### OpenSSL
```
			tar -xvf openssl_0.9.8c.orig.tar.gz
			cd openssl-0.9.8c
			./config
			make
			make test
			su
			make install
```

Preparing supporting packages so GD and HS detects the include
files and link libraries.

```
	## INCLUDE AREAS
	cd /usr/include
	ln -sf /home/autoprod/packages/zlib-1.2.3 zlib
	ln -sf /home/autoprod/packages/libpng-1.2.35 png
	ln -sf /home/autoprod/packages/openssl-0.9.8c/include/openssl/ openssl
	ln -sf /home/autoprod/packages/jpeg-6b/ jpeg
	ln -sf /home/autoprod/packages/gd-2.0.35/ gd

	## LIBRARIES
	cd /usr/local/lib
	cp -rp * /usr/lib

	## SPECIAL CASE, MUST COPY JPEG LIBRARY
	cd /home/autoprod/packages/jpeg-6b
	cp libjpeg.a /usr/lib
```

#### Building GD

```
	tar -xvf gd-2.0.35.tar.gz
	cd gd-2.0.35
	./configure
```

If the supporting packages were installed
correctly, then the configuration summary for
GD should appear as follows

```
** Configuration summary for gd 2.0.35:
	   Support for PNG library:          yes
	   Support for JPEG library:         yes
	   Support for Freetype 2.x library: yes
	   Support for Fontconfig library:   no
	   Support for Xpm library:          no
	   Support for pthreads:             yes
```
Modify gd_jpeg.c to point to jpeg include files  
Change the following:
```	
		#include "jpeglib.h"
		#include "jerror.h"
```
To the following:
```
		#include <jpeg/jpeglib.h>
		#include <jpeg/jerror.h>
```
Then make it.
```
	make
	
	su
	make install
```
Copy the library files to /usr/lib
```
	cd /usr/local/lib
	cp -rp libgd* /usr/lib
```

#### Example: Building HS/GD with SQL support

In the makefile.
```
		CC = gcc
		CFLAGS = -g -O -Wall -I. -I/usr/include -I/usr/include/gd/gd-2.0.35
		CDEFS =   -DAS_SQL -DAS_MYSQL -DAS_SSL -DAS_GD
		LIBS = -lc -L/usr/lib/mysql -lmysqlclient -lz -lm -lssl -lcrypto -lgd -ljpeg
```
### HS on Linux with SQLSERVER support via FreeTDS library
```
		CC = gcc
		CFLAGS = -g -O -Wall 
			-I. 
			-I/usr/include 
			-I/home/autoprod/packages/freetds-0.82/include 
			-I/var/www/securebob/gd/gd-2.0.35
		CDEFS = -DAS_SSL  -DAS_SQL -DAS_SQLSERVER -DAS_GD
		LIBS = -lc -lm -lssl -lcrypto -L/home/autoprod/freetds-0.82/src/dblib/.libs -lsybdb
```
#### BUILDING FreeTDS
```
		TDSVER=7.0
		TDSDUMPCONFIG=$AUTOLOG/tdsconfig.log
		TDSDUMP=$AUTOLOG/tdsdump.log

		./configure  --with-tdsver=7.0
```
In freetds-0.82/src/tds/config.c, we must force the version to 7.0. 
Edit the file and look for the following
```	
	        	/* And finally apply anything from the login structure */
        		tds_config_login(connection, login);

	        	if (opened) {
        	        	connection->major_version = 7 ;   /* ADDED THIS LINE */
                		connection->minor_version = 0 ;	  /* ADDED THIS LINE */

	                	tdsdump_log(TDS_DBG_INFO1, "Final connection parameters:\n");

```
Then make it.
```
		make

		su
		make install

		cd src/dblib/.libs
		cp -rp libsy* /usr/lib
		cd src/ctlib/.libs
		cp -rp libct* /usr/lib
```






			
