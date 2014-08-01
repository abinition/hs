# HS Automation Scripting Language

## UNIX Build Procedures

Although HS now has AUTOCONF integration, the features of SSL, SQL are not yet integrated.  Therefore, to use these features, you must use the older makefile copies in the directory.  Otherwise, simply do:

```
./configure
make
```

## WIN32 (VS 2008) Hyperscript Build Procedures

### HS Standalone, HS.EXE

The following combinations are available.
```
AS_SQL, AS_SQLSERVER
AS_SQL, AS_SQLSERVER, AS_SSL
AS_SQL, AS_MYSQL
AS_SQL, AS_MYSQL, AS_SSL
AS_SQL, AS_ORACLE
AS_SQL, AS_ORACLE, AS_SSL
AS_SQL, AS_PGSQL
AS_SQL, AS_PGSQL, AS_SSL
AS_SSL
(none)  ->  to make ROUTER.EXE
```

### HS-DOM, ActiveX control HSX.DLL deployed using webpickle.cab

* From VS, select “Build hsx”
AS_ATL and AS_SSL are the directives used

* You get HSX.DLL in the Release directory
Copy it up one level to the hs\hsx and CVS it

* cd to \abinition\hs\hsx and execute MAKECABSIGNED.BAT
password is Abinition

* You get WebPickle.cab in \Abinition\hs\hsx
CVS it, then copy it to \hs\deploy and CVS it

### HS-DOM, JAVA control HS.DLL, deployed using abinition.jar

* From VS, select "Build hsdll"
AS_DLL, AS_JNI, AS_SSL are the directives used

* You get HS.DLL and HSDLL.LIB in the Release directory
CVS them

* cd to \abinition\hs\ and execute the ANT sequences.
(make sure JAVA is 1.6 or recent)

```
		ant clean
		ant init
		ant genkey
		
		ant jar		<=== repeatable
```

You get Abinition.jar in deploy
	
### HS/GD

Installing supporting packages
		
: For Windows, obtain the gd.lib, either generating from the gd.dll 
: file, or getting a download of gd.lib off of the internet.  Alternatively,
: it is possible to build gd.lib from the gd sources, but that has not been
: done on windows.

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
``
	## Modify gd_jpeg.c to point to jpeg include files
	## Change the following:
```	
		#include "jpeglib.h"
		#include "jerror.h"
```
	## To the following:
```
		#include <jpeg/jpeglib.h>
		#include <jpeg/jerror.h>
```
```
	make
	
	su
	make install
```
	## Copy the library files to /usr/lib
```
	cd /usr/local/lib
	cp -rp libgd* /usr/lib
```

#### Building HS/GD with SQL support

In the makefile.  For MySQL
```
		CC = gcc

		CFLAGS = -g -O -Wall -I. -I/usr/include -I/var/www/securebob/gd/gd-2.0.35

		CDEFS =   -DAS_SQL -DAS_MYSQL -DAS_SSL -DAS_GD

		LIBS = -lc -L/usr/lib/mysql -lmysqlclient -lz -lm -lssl -lcrypto -lgd -ljpeg
``
	In the makefile, for SQLSERVER aka FreeTDS
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
```
		make

		su
		make install

		cd src/dblib/.libs
		cp -rp libsy* /usr/lib
		cd src/ctlib/.libs
		cp -rp libct* /usr/lib
```






			
