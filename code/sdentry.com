$ type sys$input

To start all automation processes:	@aeq_ssp:autostart
To start an automation server:		@aeq_ssp:start
To stop all automation processes:	@aeq_ssp:autostop
To stop an automation server:		@aeq_ssp:stop 

For node...		Run AUTOSTART.COM on ...
-----------		------------------------
VAX1Z      		PROMISOPS
VAX2Z      		PROMDEV or PROM50DEM
TBACK      		PROMISDEM
TPRIM       		PROMISOPS

$ show logical /system MBX_*
$ exit
