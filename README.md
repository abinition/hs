hs
==

HyperScript Automation Scripting Language


WIN32 (VS 2008) Hyperscript Build Procedures

1.	HS Standalone, HS.EXE

	Rotate through the following combinations, building each combination of SQL server and SSL support

	i.	AS_SQL, AS_SQLSERVER
	ii.	AS_SQL, AS_SQLSERVER, AS_SSL
	iii.	AS_SQL, AS_MYSQL
	iv.	AS_SQL, AS_MYSQL, AS_SSL
	v.	AS_SQL, AS_ORACLE
	vi.	AS_SQL, AS_ORACLE, AS_SSL
	vii.	AS_SQL, AS_PGSQL
	viii.	AS_SQL, AS_PGSQL, AS_SSL
	ix.	AS_SSL
	x.	(none)  ->  to make ROUTER.EXE


2.	HS-DOM, ActiveX control HSX.DLL deployed using webpickle.cab

	a) From VS, select “Build hsx”
		AS_ATL and AS_SSL are the directives used

	b) You get HSX.DLL in the Release directory
		Copy it up one level to the hs\hsx and CVS it

	c) cd to \abinition\hs\hsx and execute MAKECABSIGNED.BAT
		password is Abinition

	e) You get WebPickle.cab in \Abinition\hs\hsx
		CVS it
		Copy it to \hs\deploy and CVS it


3.	HS-DOM, JAVA control HS.DLL, deployed using abinition.jar

	a) From VS, select "Build hsdll"
		AS_DLL, AS_JNI, AS_SSL are the directives used

	b) You get HS.DLL and HSDLL.LIB in the Release directory
		CVS them

	c) cd to \abinition\hs\ and execute the ANT sequences.
		(make sure JAVA is 1.6 or recent)

		ant clean
		ant init
		ant genkey
		
		ant jar		<=== repeatable

	d) You get Abinition.jar in deploy
	

4. HS/GD

	a) Installing supporting packages
		
		For Windows, obtain the gd.lib, either generating from the gd.dll
		file, or getting a download of gd.lib off of the internet.  Alternatively,
		it is possible to build gd.lib from the gd sources, but that has not been
		done on windows.

		For Linux, first install all the required support packages so that the include
		areas and libraries can be found:

			1. zlib

			tar -xvf zlib-1.2.3.tar.tar
			cd zlib-1.2.3/
			./configure
			make
			su
			make install

			2. jpeglib

			tar -xvzf jpegsrc.v6b.tar.gz
			cd jpeg-6b/ 

			./configure
			make
			su
			make install

			3. libpng

			tar -xvzf libpng-1.2.35.tar.gz
			cd libpng-1.2.35/
			./configure
			make
			su
			make install

			4. FreeType

			tar -xvf freetype-2.1.10.tar.tar
			cd freetype-2.1.10
			./configure
			make
			su
			make install

			5. OpenSSL

			tar -xvf openssl_0.9.8c.orig.tar.gz
			cd openssl-0.9.8c
			./config
			make
			make test
			su
			make install


	b) Preparing supporting packages so GD and HS detects the include
	files and link libraries.

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


	c) Building GD


	tar -xvf gd-2.0.35.tar.gz
	cd gd-2.0.35
	./configure

		If the supporting packages were installed
		correctly, then the configuration summary for
		GD should appear as follows

		** Configuration summary for gd 2.0.35:
		   Support for PNG library:          yes
		   Support for JPEG library:         yes
		   Support for Freetype 2.x library: yes
		   Support for Fontconfig library:   no
		   Support for Xpm library:          no
		   Support for pthreads:             yes

	## Modify gd_jpeg.c to point to jpeg include files
	## Change the following:
		#include "jpeglib.h"
		#include "jerror.h"
	## To the following:
		#include <jpeg/jpeglib.h>
		#include <jpeg/jerror.h>

	make
	
	su
	make install

	## Copy the library files to /usr/lib
	cd /usr/local/lib
	cp -rp libgd* /usr/lib


	d) Building HS/GD with SQL support

	In the makefile.  For MySQL

		CC = gcc

		CFLAGS = -g -O -Wall -I. -I/usr/include -I/var/www/securebob/gd/gd-2.0.35

		CDEFS =   -DAS_SQL -DAS_MYSQL -DAS_SSL -DAS_GD

		LIBS = -lc -L/usr/lib/mysql -lmysqlclient -lz -lm -lssl -lcrypto -lgd -ljpeg

	In the makefile, for SQLSERVER aka FreeTDS

		CC = gcc

		CFLAGS = -g -O -Wall 
			-I. 
			-I/usr/include 
			-I/home/autoprod/packages/freetds-0.82/include 
			-I/var/www/securebob/gd/gd-2.0.35

		CDEFS = -DAS_SSL  -DAS_SQL -DAS_SQLSERVER -DAS_GD

		LIBS = -lc -lm -lssl -lcrypto -L/home/autoprod/freetds-0.82/src/dblib/.libs -lsybdb


	e) BUILDING FreeTDS

		TDSVER=7.0
		TDSDUMPCONFIG=$AUTOLOG/tdsconfig.log
		TDSDUMP=$AUTOLOG/tdsdump.log

		./configure  --with-tdsver=7.0


		# In freetds-0.82/src/tds/config.c, we must force the version
		# to 7.0
		# Edit the file and look for the following
	
	        	/* And finally apply anything from the login structure */
        		tds_config_login(connection, login);

	        	if (opened) {
        	        	connection->major_version = 7 ;   /* ADDED THIS LINE */
                		connection->minor_version = 0 ;	  /* ADDED THIS LINE */

	                	tdsdump_log(TDS_DBG_INFO1, "Final connection parameters:\n");


		make

		su
		make install

		cd src/dblib/.libs
		cp -rp libsy* /usr/lib
		cd src/ctlib/.libs
		cp -rp libct* /usr/lib







			
