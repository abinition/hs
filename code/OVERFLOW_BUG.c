


20110208:025630:uv-3_equip:%DBG002|084| |Checking for idle connections
20110208:025719:uv-3_equip:%DBG128|683| |<-ENQ
20110208:025719:uv-3_equip:%DBG128|684| |->EOT
20110208:025719:uv-3_equip:%DBG128|772| |<-13 received (13/13 bytes)
20110208:025719:uv-3_equip:%DBG128|772| |<-BLK [0a 80 01 81 01 80 01 00 00 14 b9 02 51 ]
20110208:025719:uv-3_equip:%DBG128|772| |->ACK
20110208:025719:uv-3_equip:%DBG128|773| |Received S1F1 (Device 1) (TID=0x14b9) (SID=0x00) 
20110208:025719:uv-3_equip:%DBG128|773| |Saving TID=0x14b9, SID=0x00
20110208:025719:uv-3_equip:%DBG002|773| |uv-3_equip returning to IDLE state
20110208:025719:uv-3_equip:%DBG128|773| |Found queued event or query message(27) 'SECS_S1F1' for uv-3_equip
20110208:025719:uv-3_equip:%DBG128|773| |Created default secs reply variable 'S1F2<>  = < 0x00 >' at outgoing depth 27
20110208:025719:uv-3_equip:%DBG128|773| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S1F1, TID=000014b9, isReply=0, isSECS=1
20110208:025719:uv-3_equip:%DBG128|773| |proto: outgoing reply depth++ = 28
20110208:025719:uv-3_equip:%DBG002|773| |diag : Handling msg interrupt
20110208:025719:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S1F1
20110208:025719:uv-3_equip:%DBG002|774| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025719:uv-3_equip:%DBG002|774| |diag : return STATUS{}  = { "$ACK" }
20110208:025719:uv-3_equip:%DBG002|774| |...checking STATUS{}  = { "$ACK" }
20110208:025719:uv-3_equip:%DBG002|774| |diag : Handling msg call for 'SECS_S1F1'
20110208:025719:uv-3_equip:%DBG128|774| |diag : Storing outgoing reply for SECS_S1F1 at depth 27
20110208:025719:uv-3_equip:Received S1F1 from 1, TID=1, SID=0
20110208:025719:uv-3_equip:list 'S1F1' = {};
20110208:025719:uv-3_equip:%DBG002|778| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025719:uv-3_equip:Executing local definition for S1F1
%METHOD: warning: No method for S1F1
20110208:025719:uv-3_equip:list 'S1F2' = {};
20110208:025719:uv-3_equip:%DBG002|779| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025719:uv-3_equip:%DBG128|780| |diag : Looking for outgoing reply at depth 27
20110208:025719:uv-3_equip:%DBG128|780| |diag : outgoing reply depth-- = 27
20110208:025719:uv-3_equip:%DBG128|780| |diag : Looking for outgoing secs reply S1F2 at depth 27
20110208:025719:uv-3_equip:%DBG128|780| |Method data is SECS_S1F1{}  = { args , S1F1 }
20110208:025719:uv-3_equip:%DBG128|780| |Setting/Restoring TID=0x14b9, SID=0x00
20110208:025719:uv-3_equip:%DBG128|780| |Sending S1F2 (device 1) (TID=0x14b9) (SID=0x00)
20110208:025719:uv-3_equip:%DBG128|780| |->ENQ
20110208:025719:uv-3_equip:%DBG128|962| |<-EOT
20110208:025719:uv-3_equip:%DBG128|962| |->BLK [0c 00 01 01 02 80 01 00 00 14 b9 01 00 01 53 ]
20110208:025720:uv-3_equip:%DBG128|130| |<-ACK
20110208:025720:uv-3_equip:%DBG002|130| |uv-3_equip returning to IDLE state
20110208:025819:uv-3_equip:%DBG128|665| |<-ENQ
20110208:025819:uv-3_equip:%DBG128|665| |->EOT
20110208:025819:uv-3_equip:%DBG128|833| |<-13 received (13/13 bytes)
20110208:025819:uv-3_equip:%DBG128|834| |<-BLK [0a 80 01 81 01 80 01 00 00 14 ba 02 52 ]
20110208:025819:uv-3_equip:%DBG128|834| |->ACK
20110208:025819:uv-3_equip:%DBG128|834| |Received S1F1 (Device 1) (TID=0x14ba) (SID=0x00) 
20110208:025819:uv-3_equip:%DBG128|834| |Saving TID=0x14ba, SID=0x00
20110208:025819:uv-3_equip:%DBG002|834| |uv-3_equip returning to IDLE state



20110208:025819:uv-3_equip:%DBG128|834| |Found queued event or query message(28) 'SECS_S1F1' for uv-3_equip
20110208:025819:uv-3_equip:%DBG128|834| |Created default secs reply variable 'S1F2<>  = < 0x00 >' at outgoing depth 27
20110208:025819:uv-3_equip:%DBG128|834| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S1F1, TID=000014ba, isReply=0, isSECS=1
20110208:025819:uv-3_equip:%DBG128|834| |proto: outgoing reply depth++ = 28
20110208:025819:uv-3_equip:%DBG002|834| |diag : Handling msg interrupt
20110208:025819:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S1F1
20110208:025819:uv-3_equip:%DBG002|835| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025819:uv-3_equip:%DBG002|835| |diag : return STATUS{}  = { "$ACK" }
20110208:025819:uv-3_equip:%DBG002|835| |...checking STATUS{}  = { "$ACK" }
20110208:025819:uv-3_equip:%DBG002|835| |diag : Handling msg call for 'SECS_S1F1'
20110208:025819:uv-3_equip:%DBG128|835| |diag : Storing outgoing reply for SECS_S1F1 at depth 27
20110208:025819:uv-3_equip:Received S1F1 from 1, TID=1, SID=0
20110208:025819:uv-3_equip:list 'S1F1' = {};
20110208:025819:uv-3_equip:%DBG002|839| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025819:uv-3_equip:Executing local definition for S1F1
%METHOD: warning: No method for S1F1
20110208:025819:uv-3_equip:list 'S1F2' = {};
20110208:025819:uv-3_equip:%DBG002|841| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025819:uv-3_equip:%DBG128|841| |diag : Looking for outgoing reply at depth 27
20110208:025819:uv-3_equip:%DBG128|841| |diag : outgoing reply depth-- = 27
20110208:025819:uv-3_equip:%DBG128|841| |diag : Looking for outgoing secs reply S1F2 at depth 27
20110208:025819:uv-3_equip:%DBG128|841| |Method data is SECS_S1F1{}  = { args , S1F1 }
20110208:025819:uv-3_equip:%DBG128|841| |Setting/Restoring TID=0x14ba, SID=0x00
20110208:025819:uv-3_equip:%DBG128|841| |Sending S1F2 (device 1) (TID=0x14ba) (SID=0x00)
20110208:025819:uv-3_equip:%DBG128|841| |->ENQ
20110208:025820:uv-3_equip:%DBG128|012| |<-EOT
20110208:025820:uv-3_equip:%DBG128|012| |->BLK [0c 00 01 01 02 80 01 00 00 14 ba 01 00 01 54 ]
20110208:025820:uv-3_equip:%DBG128|191| |<-ACK

20110208:025820:uv-3_equip:%DBG002|191| |uv-3_equip returning to IDLE state

20110208:025919:uv-3_equip:%DBG128|666| |<-ENQ
20110208:025919:uv-3_equip:%DBG128|666| |->EOT
20110208:025919:uv-3_equip:%DBG128|836| |<-13 received (13/13 bytes)
20110208:025919:uv-3_equip:%DBG128|836| |<-BLK [0a 80 01 81 01 80 01 00 00 14 bb 02 53 ]
20110208:025919:uv-3_equip:%DBG128|837| |->ACK
20110208:025919:uv-3_equip:%DBG128|837| |Received S1F1 (Device 1) (TID=0x14bb) (SID=0x00) 
20110208:025919:uv-3_equip:%DBG128|837| |Saving TID=0x14bb, SID=0x00


20110208:025919:uv-3_equip:%DBG002|837| |uv-3_equip returning to IDLE state


20110208:025919:uv-3_equip:%DBG128|837| |Found queued event or query message(29) 'SECS_S1F1' for uv-3_equip
20110208:025919:uv-3_equip:%DBG128|837| |Created default secs reply variable 'S1F2<>  = < 0x00 >' at outgoing depth 27
20110208:025919:uv-3_equip:%DBG128|837| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S1F1, TID=000014bb, isReply=0, isSECS=1
20110208:025919:uv-3_equip:%DBG128|837| |proto: outgoing reply depth++ = 28
20110208:025919:uv-3_equip:%DBG002|837| |diag : Handling msg interrupt
20110208:025919:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S1F1
20110208:025919:uv-3_equip:%DBG002|838| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025919:uv-3_equip:%DBG002|838| |diag : return STATUS{}  = { "$ACK" }
20110208:025919:uv-3_equip:%DBG002|838| |...checking STATUS{}  = { "$ACK" }
20110208:025919:uv-3_equip:%DBG002|838| |diag : Handling msg call for 'SECS_S1F1'
20110208:025919:uv-3_equip:%DBG128|838| |diag : Storing outgoing reply for SECS_S1F1 at depth 27
20110208:025919:uv-3_equip:Received S1F1 from 1, TID=1, SID=0
20110208:025919:uv-3_equip:list 'S1F1' = {};
20110208:025919:uv-3_equip:%DBG002|842| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025919:uv-3_equip:Executing local definition for S1F1
%METHOD: warning: No method for S1F1
20110208:025919:uv-3_equip:list 'S1F2' = {};
20110208:025919:uv-3_equip:%DBG002|844| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025919:uv-3_equip:%DBG128|844| |diag : Looking for outgoing reply at depth 27
20110208:025919:uv-3_equip:%DBG128|844| |diag : outgoing reply depth-- = 27
20110208:025919:uv-3_equip:%DBG128|844| |diag : Looking for outgoing secs reply S1F2 at depth 27
20110208:025919:uv-3_equip:%DBG128|844| |Method data is SECS_S1F1{}  = { args , S1F1 }
20110208:025919:uv-3_equip:%DBG128|844| |Setting/Restoring TID=0x14bb, SID=0x00
20110208:025919:uv-3_equip:%DBG128|844| |Sending S1F2 (device 1) (TID=0x14bb) (SID=0x00)
20110208:025919:uv-3_equip:%DBG128|844| |->ENQ
20110208:025920:uv-3_equip:%DBG128|013| |<-EOT
20110208:025920:uv-3_equip:%DBG128|013| |->BLK [0c 00 01 01 02 80 01 00 00 14 bb 01 00 01 55 ]
20110208:025920:uv-3_equip:%DBG128|181| |<-ACK
20110208:025920:uv-3_equip:%DBG002|181| |uv-3_equip returning to IDLE state
20110208:025920:uv-3_equip:==FIFO Read==
--To /local/fifo/.router/uv-3_equip ----
|uv-3_equip|query|CONNECT|1e1d8437#uv-3_into|2e098ddb|20110208:025920|||
20110208:025920:uv-3_equip:%DBG002|258| |Found instance uv-3_equip#uv-3_equip
-------------




20110208:025920:uv-3_equip:%DBG002|258| |uv-3_equip returning to IDLE state



20110208:025920:uv-3_equip:%DBG128|258| |Found queued event or query message(30) 'CONNECT' for uv-3_equip
20110208:025920:uv-3_equip:%DBG128|258| |diag : Target=uv-3_equip, Sender=1e1d8437#uv-3_into, Method=CONNECT, TID=2e098ddb, isReply=0, isSECS=0
20110208:025920:uv-3_equip:%DBG128|258| |proto: outgoing reply depth++ = 28
20110208:025920:uv-3_equip:%DBG002|258| |diag : Handling msg interrupt
20110208:025920:uv-3_equip:Servicing incoming message from 1e1d8437#uv-3_into requesting method CONNECT
20110208:025920:uv-3_equip:%DBG002|259| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|260| |diag : return STATUS{}  = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|260| |...checking STATUS{}  = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|260| |diag : Handling msg call for 'CONNECT'
20110208:025920:uv-3_equip:%DBG128|260| |diag : Storing outgoing reply for CONNECT at depth 27
--To /local/fifo/router ----
|router|event|CONNECT|uv-3_equip|1a947f03|20110208:025920|||
-------------
20110208:025920:uv-3_equip:list 'S1F13' = {};
20110208:025920:uv-3_equip:Sending S1F13, TID=00000129
20110208:025920:uv-3_equip:%DBG128|269| |Setting/Restoring TID=0x129, SID=0x00
20110208:025920:uv-3_equip:%DBG128|269| |Sending S1F13 (device 1) (TID=0x129) (SID=0x00)
20110208:025920:uv-3_equip:%DBG128|270| |->ENQ
20110208:025920:uv-3_equip:%DBG128|429| |<-EOT
20110208:025920:uv-3_equip:%DBG128|429| |->BLK [0c 00 01 81 0d 80 01 00 00 01 29 01 00 01 3b ]
20110208:025920:uv-3_equip:%DBG128|608| |<-ACK
20110208:025920:uv-3_equip:%DBG128|608| |<-ENQ(ACK)
20110208:025920:uv-3_equip:%DBG128|608| |ENQ contention. Relinquishing...
20110208:025920:uv-3_equip:%DBG128|608| |->EOT
20110208:025920:uv-3_equip:%DBG128|707| |<-35 received (35/35 bytes)
20110208:025920:uv-3_equip:%DBG128|707| |<-BLK [20 80 01 01 0e 80 01 00 00 01 29 01 02 21 01 00 01 02 41 06 32 30 32 50 43 55 41 05 76 34 2e 33 30 04 a7 ]
20110208:025920:uv-3_equip:%DBG128|707| |->ACK
20110208:025920:uv-3_equip:%DBG128|707| |Received S1F14 (Device 1) (TID=0x129) (SID=0x00) 
20110208:025920:uv-3_equip:%DBG128|707| |diag : incoming reply depth++ = 1
20110208:025920:uv-3_equip:%DBG128|707| |diag : Setting expected reply for sender:method:tid 1#secs:S1F14:00000129 at incoming depth 0
20110208:025920:uv-3_equip:%DBG128|707| |diag : Setting SECS reply S1F14 at incoming depth 0
20110208:025920:uv-3_equip:...waiting for S1F14 reply from device '1', timeout in 30 seconds
20110208:025920:uv-3_equip:%DBG002|708| |uv-3_equip returning to QUERY state
20110208:025920:uv-3_equip:%DBG128|708| |Found queued reply message(31) 'S1F14' for uv-3_equip
20110208:025920:uv-3_equip:%DBG128|708| |diag : Target=uv-3_equip, Sender=1#secs, Method=S1F14, TID=00000129, isReply=1, isSECS=1
20110208:025920:uv-3_equip:%DBG128|708| |diag : Matching 1#secs=1#secs, S1F14=S1F14, 00000129=00000129
20110208:025920:uv-3_equip:Matched TID=00000129, Method=S1F14, Depth=0, Sender=1#secs
20110208:025920:uv-3_equip:%DBG128|708| |diag : Matched current reply S1F14 at incoming depth 0
20110208:025920:uv-3_equip:%DBG128|708| |diag : Matched 1#secs=1#secs, S1F14=S1F14, 00000129=00000129
20110208:025920:uv-3_equip:%DBG128|708| |diag : Matched incoming reply at depth 0
20110208:025920:uv-3_equip:%DBG128|708| |diag : Consuming reply message at incoming depth 0
20110208:025920:uv-3_equip:%DBG128|708| |proto: incoming reply depth-- = 0
20110208:025920:uv-3_equip:%DBG002|708| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025920:uv-3_equip:list 'S1F14' = {
20110208:025920:uv-3_equip:  binary 'BN_0'[1] = {0x00},
20110208:025920:uv-3_equip:  list 'L_1' = {
20110208:025920:uv-3_equip:    char 'A_0'[6] = {"202PCU"},
20110208:025920:uv-3_equip:    char 'A_1'[5] = {"v4.30"}
20110208:025920:uv-3_equip:    
20110208:025920:uv-3_equip:  }
20110208:025920:uv-3_equip:  
20110208:025920:uv-3_equip:};
20110208:025920:uv-3_equip:%DBG002|710| |diag : return COMM_REQUEST_S1F13{} (1) = { "$ACK: model " , "202PCU" , " revision " , "v4.30" }
20110208:025920:uv-3_equip:%DBG002|711| |diag : return HEARTBEAT{} (1) = { "$ACK: model " , "202PCU" , " revision " , "v4.30" }
20110208:025920:uv-3_equip:%DBG128|712| |diag : Looking for outgoing reply at depth 27
20110208:025920:uv-3_equip:%DBG128|712| |diag : outgoing reply depth-- = 27
--To /local/fifo/router ----
|1e1d8437#uv-3_into|reply|CONNECT|uv-3_equip|2e098ddb|20110208:025920||STATUS|$ACK: model |202PCU| revision |v4.30|||
-------------
20110208:025920:uv-3_equip:%DBG002|712| |uv-3_equip returning to IDLE state


20110208:025920:uv-3_equip:==FIFO Read==
--To /local/fifo/.router/uv-3_equip ----
|uv-3_equip|query|EQP_STATUS|1e1d8437#uv-3_into|51d9760e|20110208:025920||EQPID|UV-3|||
20110208:025920:uv-3_equip:%DBG002|714| |Found instance uv-3_equip#uv-3_equip
-------------

20110208:025920:uv-3_equip:%DBG002|714| |uv-3_equip returning to IDLE state

20110208:025920:uv-3_equip:%DBG128|715| |Found queued event or query message(0) 'EQP_STATUS' for uv-3_equip
20110208:025920:uv-3_equip:%DBG128|715| |diag : Target=uv-3_equip, Sender=1e1d8437#uv-3_into, Method=EQP_STATUS, TID=51d9760e, isReply=0, isSECS=0
20110208:025920:uv-3_equip:%DBG128|715| |proto: outgoing reply depth++ = 28
20110208:025920:uv-3_equip:%DBG002|715| |diag : Handling msg interrupt
20110208:025920:uv-3_equip:Servicing incoming message from 1e1d8437#uv-3_into requesting method EQP_STATUS
20110208:025920:uv-3_equip:%DBG002|715| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|716| |diag : return STATUS{}  = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|716| |...checking STATUS{}  = { "$ACK" }
20110208:025920:uv-3_equip:%DBG002|716| |diag : Handling msg call for 'EQP_STATUS'
20110208:025920:uv-3_equip:%DBG128|716| |diag : Storing outgoing reply for EQP_STATUS at depth 27
20110208:025920:uv-3_equip:Sending S1F3, TID=0000012a
20110208:025920:uv-3_equip:%DBG128|717| |Setting/Restoring TID=0x12a, SID=0x00
20110208:025920:uv-3_equip:%DBG128|717| |Sending S1F3 (device 1) (TID=0x12a) (SID=0x00)
20110208:025920:uv-3_equip:%DBG128|717| |->ENQ
20110208:025920:uv-3_equip:%DBG128|887| |<-EOT
20110208:025920:uv-3_equip:%DBG128|887| |->BLK [14 00 01 81 03 80 01 00 00 01 2a 01 02 a9 02 03 01 a9 02 fe 08 03 94 ]
20110208:025921:uv-3_equip:%DBG128|056| |<-ACK
20110208:025921:uv-3_equip:%DBG128|056| |<-ENQ(ACK)
20110208:025921:uv-3_equip:%DBG128|056| |ENQ contention. Relinquishing...
20110208:025921:uv-3_equip:%DBG128|057| |->EOT
20110208:025921:uv-3_equip:%DBG128|166| |<-22 received (22/22 bytes)
20110208:025921:uv-3_equip:%DBG128|166| |<-BLK [13 80 01 01 04 80 01 00 00 01 2a 01 02 a9 02 1c aa a5 01 05 03 51 ]
20110208:025921:uv-3_equip:%DBG128|166| |->ACK
20110208:025921:uv-3_equip:%DBG128|166| |Received S1F4 (Device 1) (TID=0x12a) (SID=0x00) 
20110208:025921:uv-3_equip:%DBG128|166| |diag : incoming reply depth++ = 1
20110208:025921:uv-3_equip:%DBG128|166| |diag : Setting expected reply for sender:method:tid 1#secs:S1F4:0000012a at incoming depth 0
20110208:025921:uv-3_equip:%DBG128|166| |diag : Setting SECS reply S1F4 at incoming depth 0
20110208:025921:uv-3_equip:...waiting for S1F4 reply from device '1', timeout in 30 seconds
20110208:025921:uv-3_equip:%DBG002|166| |uv-3_equip returning to QUERY state
20110208:025921:uv-3_equip:%DBG128|166| |Found queued reply message(1) 'S1F4' for uv-3_equip
20110208:025921:uv-3_equip:%DBG128|166| |diag : Target=uv-3_equip, Sender=1#secs, Method=S1F4, TID=0000012a, isReply=1, isSECS=1
20110208:025921:uv-3_equip:%DBG128|166| |diag : Matching 1#secs=1#secs, S1F4=S1F4, 0000012a=0000012a
20110208:025921:uv-3_equip:Matched TID=0000012a, Method=S1F4, Depth=0, Sender=1#secs
20110208:025921:uv-3_equip:%DBG128|166| |diag : Matched current reply S1F4 at incoming depth 0
20110208:025921:uv-3_equip:%DBG128|166| |diag : Matched 1#secs=1#secs, S1F4=S1F4, 0000012a=0000012a
20110208:025921:uv-3_equip:%DBG128|167| |diag : Matched incoming reply at depth 0
20110208:025921:uv-3_equip:%DBG128|167| |diag : Consuming reply message at incoming depth 0
20110208:025921:uv-3_equip:%DBG128|167| |proto: incoming reply depth-- = 0
20110208:025921:uv-3_equip:%DBG002|167| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025921:uv-3_equip:list 'S1F4' = {
20110208:025921:uv-3_equip:  ushort 'UW_0'[1] = {7338},
20110208:025921:uv-3_equip:  ubyte 'UB_1'[1] = {5}
20110208:025921:uv-3_equip:  
20110208:025921:uv-3_equip:};
20110208:025921:uv-3_equip:%DBG002|168| |diag : return EQP_STATUS{} (1) = { "$ACK" }
20110208:025921:uv-3_equip:%DBG128|168| |diag : Looking for outgoing reply at depth 27
20110208:025921:uv-3_equip:%DBG128|168| |diag : outgoing reply depth-- = 27
--To /local/fifo/router ----
|1e1d8437#uv-3_into|reply|EQP_STATUS|uv-3_equip|51d9760e|20110208:025920||STATUS|$ACK|||
-------------
20110208:025921:uv-3_equip:%DBG002|168| |uv-3_equip returning to IDLE state
20110208:025927:uv-3_equip:==FIFO Read==
--To /local/fifo/.router/uv-3_equip ----
|uv-3_equip|query|START|1e1d8437#uv-3_into|077c881a|20110208:025927||REC|REBAKE A||LL|LEFT PORT||LOTID|B05361.1|||
20110208:025927:uv-3_equip:%DBG002|332| |Found instance uv-3_equip#uv-3_equip
-------------


20110208:025927:uv-3_equip:%DBG002|332| |uv-3_equip returning to IDLE state


20110208:025927:uv-3_equip:%DBG128|332| |Found queued event or query message(2) 'START' for uv-3_equip
20110208:025927:uv-3_equip:%DBG128|332| |diag : Target=uv-3_equip, Sender=1e1d8437#uv-3_into, Method=START, TID=077c881a, isReply=0, isSECS=0
20110208:025927:uv-3_equip:%DBG128|332| |proto: outgoing reply depth++ = 28
20110208:025927:uv-3_equip:%DBG002|332| |diag : Handling msg interrupt
20110208:025927:uv-3_equip:Servicing incoming message from 1e1d8437#uv-3_into requesting method START
20110208:025927:uv-3_equip:%DBG002|333| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025927:uv-3_equip:%DBG002|333| |diag : return STATUS{}  = { "$ACK" }
20110208:025927:uv-3_equip:%DBG002|333| |...checking STATUS{}  = { "$ACK" }
20110208:025927:uv-3_equip:%DBG002|333| |diag : Handling msg call for 'START'
20110208:025927:uv-3_equip:%DBG128|334| |diag : Storing outgoing reply for START at depth 27
20110208:025927:uv-3_equip:Sending S1F3, TID=0000012b
20110208:025927:uv-3_equip:%DBG128|335| |Setting/Restoring TID=0x12b, SID=0x00
20110208:025927:uv-3_equip:%DBG128|335| |Sending S1F3 (device 1) (TID=0x12b) (SID=0x00)
20110208:025927:uv-3_equip:%DBG128|335| |->ENQ
20110208:025927:uv-3_equip:%DBG128|479| |<-EOT
20110208:025927:uv-3_equip:%DBG128|479| |->BLK [14 00 01 81 03 80 01 00 00 01 2b 01 02 a9 02 03 01 a9 02 fe 08 03 95 ]
20110208:025927:uv-3_equip:%DBG128|658| |<-ACK
20110208:025927:uv-3_equip:%DBG128|658| |<-ENQ(ACK)
20110208:025927:uv-3_equip:%DBG128|658| |ENQ contention. Relinquishing...
20110208:025927:uv-3_equip:%DBG128|658| |->EOT
20110208:025927:uv-3_equip:%DBG128|746| |<-22 received (22/22 bytes)
20110208:025927:uv-3_equip:%DBG128|746| |<-BLK [13 80 01 01 04 80 01 00 00 01 2b 01 02 a9 02 1c aa a5 01 05 03 52 ]
20110208:025927:uv-3_equip:%DBG128|746| |->ACK
20110208:025927:uv-3_equip:%DBG128|747| |Received S1F4 (Device 1) (TID=0x12b) (SID=0x00) 
20110208:025927:uv-3_equip:%DBG128|747| |diag : incoming reply depth++ = 1
20110208:025927:uv-3_equip:%DBG128|747| |diag : Setting expected reply for sender:method:tid 1#secs:S1F4:0000012b at incoming depth 0
20110208:025927:uv-3_equip:%DBG128|747| |diag : Setting SECS reply S1F4 at incoming depth 0
20110208:025927:uv-3_equip:...waiting for S1F4 reply from device '1', timeout in 30 seconds
20110208:025927:uv-3_equip:%DBG002|747| |uv-3_equip returning to QUERY state
20110208:025927:uv-3_equip:%DBG128|747| |Found queued reply message(3) 'S1F4' for uv-3_equip
20110208:025927:uv-3_equip:%DBG128|747| |diag : Target=uv-3_equip, Sender=1#secs, Method=S1F4, TID=0000012b, isReply=1, isSECS=1
20110208:025927:uv-3_equip:%DBG128|747| |diag : Matching 1#secs=1#secs, S1F4=S1F4, 0000012b=0000012b
20110208:025927:uv-3_equip:Matched TID=0000012b, Method=S1F4, Depth=0, Sender=1#secs
20110208:025927:uv-3_equip:%DBG128|747| |diag : Matched current reply S1F4 at incoming depth 0
20110208:025927:uv-3_equip:%DBG128|747| |diag : Matched 1#secs=1#secs, S1F4=S1F4, 0000012b=0000012b
20110208:025927:uv-3_equip:%DBG128|747| |diag : Matched incoming reply at depth 0
20110208:025927:uv-3_equip:%DBG128|747| |diag : Consuming reply message at incoming depth 0
20110208:025927:uv-3_equip:%DBG128|747| |proto: incoming reply depth-- = 0
20110208:025927:uv-3_equip:%DBG002|748| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025927:uv-3_equip:0x00001caa
20110208:025927:uv-3_equip:%DBG002|754| |diag : return START{} (1) = { "$ACK" }
20110208:025927:uv-3_equip:%DBG128|754| |diag : Looking for outgoing reply at depth 27
20110208:025927:uv-3_equip:%DBG128|754| |diag : outgoing reply depth-- = 27
--To /local/fifo/router ----
|1e1d8437#uv-3_into|reply|START|uv-3_equip|077c881a|20110208:025927||STATUS|$ACK|||
-------------
20110208:025927:uv-3_equip:%DBG002|754| |uv-3_equip returning to IDLE state


%ALARM: warning: Timer signal:20110208:025929: instance = uv-3_equip
20110208:025929:uv-3_equip:%DBG002|756| |Event: Heartbeat


20110208:025929:uv-3_equip:%DBG002|756| |uv-3_equip returning to IDLE state
20110208:025929:uv-3_equip:%DBG002|756| |Checking for idle connections
20110208:025945:uv-3_equip:==FIFO Read==
--To /local/fifo/.router/uv-3_equip ----
|uv-3_equip|query|HEARTBEAT|heartbeat|66400320|20110208:025945|||
20110208:025945:uv-3_equip:%DBG002|535| |Found instance uv-3_equip#uv-3_equip
-------------
20110208:025945:uv-3_equip:%DBG002|535| |uv-3_equip returning to IDLE state
20110208:025945:uv-3_equip:%DBG128|535| |Found queued event or query message(4) 'HEARTBEAT' for uv-3_equip
20110208:025945:uv-3_equip:%DBG128|535| |diag : Target=uv-3_equip, Sender=heartbeat, Method=HEARTBEAT, TID=66400320, isReply=0, isSECS=0
20110208:025945:uv-3_equip:%DBG128|535| |proto: outgoing reply depth++ = 28
20110208:025945:uv-3_equip:%DBG002|535| |diag : Handling msg interrupt
20110208:025945:uv-3_equip:Requeing incoming message from heartbeat requesting method HEARTBEAT
20110208:025945:uv-3_equip:%DBG128|536| |diag : outgoing reply depth-- = 27
20110208:025945:uv-3_equip:%DBG128|536| |diag : Incoming message queued
20110208:025945:uv-3_equip:%DBG002|537| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|537| |diag : return STATUS{}  = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|537| |uv-3_equip returning to IDLE state
20110208:025945:uv-3_equip:Found re-queued message(13) 'HEARTBEAT'
20110208:025945:uv-3_equip:%DBG128|537| |diag : Target=uv-3_equip, Sender=heartbeat, Method=HEARTBEAT, TID=66400320, isReply=0, isSECS=0
20110208:025945:uv-3_equip:%DBG128|537| |proto: outgoing reply depth++ = 28
20110208:025945:uv-3_equip:%DBG002|537| |diag : Handling msg interrupt
20110208:025945:uv-3_equip:Servicing incoming message from heartbeat requesting method HEARTBEAT
20110208:025945:uv-3_equip:%DBG002|538| |diag : return MESSAGE_HANDLER2{} (1) = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|538| |diag : return STATUS{}  = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|538| |...checking STATUS{}  = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|538| |diag : Handling msg call for 'HEARTBEAT'
20110208:025945:uv-3_equip:%DBG128|538| |diag : Storing outgoing reply for HEARTBEAT at depth 27
20110208:025945:uv-3_equip:list 'S1F13' = {};
20110208:025945:uv-3_equip:Sending S1F13, TID=0000012c
20110208:025945:uv-3_equip:%DBG128|540| |Setting/Restoring TID=0x12c, SID=0x00
20110208:025945:uv-3_equip:%DBG128|540| |Sending S1F13 (device 1) (TID=0x12c) (SID=0x00)
20110208:025945:uv-3_equip:%DBG128|540| |->ENQ
20110208:025945:uv-3_equip:%DBG128|644| |<-EOT
20110208:025945:uv-3_equip:%DBG128|644| |->BLK [0c 00 01 81 0d 80 01 00 00 01 2c 01 00 01 3e ]
20110208:025945:uv-3_equip:%DBG128|813| |<-ACK
20110208:025945:uv-3_equip:%DBG128|813| |<-ENQ(ACK)
20110208:025945:uv-3_equip:%DBG128|813| |ENQ contention. Relinquishing...
20110208:025945:uv-3_equip:%DBG128|813| |->EOT
20110208:025945:uv-3_equip:%DBG128|924| |<-35 received (35/35 bytes)
20110208:025945:uv-3_equip:%DBG128|925| |<-BLK [20 80 01 01 0e 80 01 00 00 01 2c 01 02 21 01 00 01 02 41 06 32 30 32 50 43 55 41 05 76 34 2e 33 30 04 aa ]
20110208:025945:uv-3_equip:%DBG128|925| |->ACK
20110208:025945:uv-3_equip:%DBG128|925| |Received S1F14 (Device 1) (TID=0x12c) (SID=0x00) 
20110208:025945:uv-3_equip:%DBG128|925| |diag : incoming reply depth++ = 1
20110208:025945:uv-3_equip:%DBG128|925| |diag : Setting expected reply for sender:method:tid 1#secs:S1F14:0000012c at incoming depth 0
20110208:025945:uv-3_equip:%DBG128|925| |diag : Setting SECS reply S1F14 at incoming depth 0
20110208:025945:uv-3_equip:...waiting for S1F14 reply from device '1', timeout in 30 seconds
20110208:025945:uv-3_equip:%DBG002|925| |uv-3_equip returning to QUERY state
20110208:025945:uv-3_equip:%DBG128|925| |Found queued reply message(5) 'S1F14' for uv-3_equip
20110208:025945:uv-3_equip:%DBG128|925| |diag : Target=uv-3_equip, Sender=1#secs, Method=S1F14, TID=0000012c, isReply=1, isSECS=1
20110208:025945:uv-3_equip:%DBG128|925| |diag : Matching 1#secs=1#secs, S1F14=S1F14, 0000012c=0000012c
20110208:025945:uv-3_equip:Matched TID=0000012c, Method=S1F14, Depth=0, Sender=1#secs
20110208:025945:uv-3_equip:%DBG128|925| |diag : Matched current reply S1F14 at incoming depth 0
20110208:025945:uv-3_equip:%DBG128|925| |diag : Matched 1#secs=1#secs, S1F14=S1F14, 0000012c=0000012c
20110208:025945:uv-3_equip:%DBG128|925| |diag : Matched incoming reply at depth 0
20110208:025945:uv-3_equip:%DBG128|925| |diag : Consuming reply message at incoming depth 0
20110208:025945:uv-3_equip:%DBG128|925| |proto: incoming reply depth-- = 0
20110208:025945:uv-3_equip:%DBG002|926| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025945:uv-3_equip:list 'S1F14' = {
20110208:025945:uv-3_equip:  binary 'BN_0'[1] = {0x00},
20110208:025945:uv-3_equip:  list 'L_1' = {
20110208:025945:uv-3_equip:    char 'A_0'[6] = {"202PCU"},
20110208:025945:uv-3_equip:    char 'A_1'[5] = {"v4.30"}
20110208:025945:uv-3_equip:    
20110208:025945:uv-3_equip:  }
20110208:025945:uv-3_equip:  
20110208:025945:uv-3_equip:};
20110208:025945:uv-3_equip:%DBG002|928| |diag : return COMM_REQUEST_S1F13{} (1) = { "$ACK: model " , "202PCU" , " revision " , "v4.30" }
20110208:025945:uv-3_equip:%DBG002|929| |diag : return HEARTBEAT{} (1) = { "$ACK: model " , "202PCU" , " revision " , "v4.30" }
20110208:025945:uv-3_equip:%DBG128|929| |diag : Looking for outgoing reply at depth 27
20110208:025945:uv-3_equip:%DBG128|929| |diag : outgoing reply depth-- = 27
--To /local/fifo/router ----
|heartbeat|reply|HEARTBEAT|uv-3_equip|66400320|20110208:025945||STATUS|$ACK: model |202PCU| revision |v4.30|||
-------------
20110208:025945:uv-3_equip:%DBG002|929| |uv-3_equip returning to IDLE state




20110208:025945:uv-3_equip:==FIFO Read==
--To /local/fifo/.router/uv-3_equip ----
|uv-3_equip|query|EQP_STATUS|heartbeat|1d93bdf6|20110208:025945||EQPID|UV-3|||
20110208:025945:uv-3_equip:%DBG002|935| |Found instance uv-3_equip#uv-3_equip
-------------
20110208:025945:uv-3_equip:%DBG002|935| |uv-3_equip returning to IDLE state
20110208:025945:uv-3_equip:%DBG128|935| |Found queued event or query message(6) 'EQP_STATUS' for uv-3_equip
20110208:025945:uv-3_equip:%DBG128|935| |diag : Target=uv-3_equip, Sender=heartbeat, Method=EQP_STATUS, TID=1d93bdf6, isReply=0, isSECS=0
20110208:025945:uv-3_equip:%DBG128|935| |proto: outgoing reply depth++ = 28
20110208:025945:uv-3_equip:%DBG002|935| |diag : Handling msg interrupt
20110208:025945:uv-3_equip:Servicing incoming message from heartbeat requesting method EQP_STATUS
20110208:025945:uv-3_equip:%DBG002|936| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|936| |diag : return STATUS{}  = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|936| |...checking STATUS{}  = { "$ACK" }
20110208:025945:uv-3_equip:%DBG002|936| |diag : Handling msg call for 'EQP_STATUS'
20110208:025945:uv-3_equip:%DBG128|936| |diag : Storing outgoing reply for EQP_STATUS at depth 27
20110208:025945:uv-3_equip:Sending S1F3, TID=0000012d
20110208:025945:uv-3_equip:%DBG128|938| |Setting/Restoring TID=0x12d, SID=0x00
20110208:025945:uv-3_equip:%DBG128|938| |Sending S1F3 (device 1) (TID=0x12d) (SID=0x00)
20110208:025945:uv-3_equip:%DBG128|938| |->ENQ
20110208:025946:uv-3_equip:%DBG128|091| |<-EOT
20110208:025946:uv-3_equip:%DBG128|091| |->BLK [14 00 01 81 03 80 01 00 00 01 2d 01 02 a9 02 03 01 a9 02 fe 08 03 97 ]
20110208:025946:uv-3_equip:%DBG128|270| |<-ACK
20110208:025946:uv-3_equip:%DBG128|270| |<-ENQ(ACK)
20110208:025946:uv-3_equip:%DBG128|270| |ENQ contention. Relinquishing...
20110208:025946:uv-3_equip:%DBG128|270| |->EOT
20110208:025946:uv-3_equip:%DBG128|361| |<-22 received (22/22 bytes)
20110208:025946:uv-3_equip:%DBG128|361| |<-BLK [13 80 01 01 04 80 01 00 00 01 2d 01 02 a9 02 1c aa a5 01 05 03 54 ]
20110208:025946:uv-3_equip:%DBG128|362| |->ACK
20110208:025946:uv-3_equip:%DBG128|362| |Received S1F4 (Device 1) (TID=0x12d) (SID=0x00) 
20110208:025946:uv-3_equip:%DBG128|362| |diag : incoming reply depth++ = 1
20110208:025946:uv-3_equip:%DBG128|362| |diag : Setting expected reply for sender:method:tid 1#secs:S1F4:0000012d at incoming depth 0
20110208:025946:uv-3_equip:%DBG128|362| |diag : Setting SECS reply S1F4 at incoming depth 0
20110208:025946:uv-3_equip:...waiting for S1F4 reply from device '1', timeout in 30 seconds
20110208:025946:uv-3_equip:%DBG002|362| |uv-3_equip returning to QUERY state
20110208:025946:uv-3_equip:%DBG128|362| |Found queued reply message(7) 'S1F4' for uv-3_equip
20110208:025946:uv-3_equip:%DBG128|362| |diag : Target=uv-3_equip, Sender=1#secs, Method=S1F4, TID=0000012d, isReply=1, isSECS=1
20110208:025946:uv-3_equip:%DBG128|362| |diag : Matching 1#secs=1#secs, S1F4=S1F4, 0000012d=0000012d
20110208:025946:uv-3_equip:Matched TID=0000012d, Method=S1F4, Depth=0, Sender=1#secs
20110208:025946:uv-3_equip:%DBG128|362| |diag : Matched current reply S1F4 at incoming depth 0
20110208:025946:uv-3_equip:%DBG128|362| |diag : Matched 1#secs=1#secs, S1F4=S1F4, 0000012d=0000012d
20110208:025946:uv-3_equip:%DBG128|362| |diag : Matched incoming reply at depth 0
20110208:025946:uv-3_equip:%DBG128|362| |diag : Consuming reply message at incoming depth 0
20110208:025946:uv-3_equip:%DBG128|362| |proto: incoming reply depth-- = 0
20110208:025946:uv-3_equip:%DBG002|363| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025946:uv-3_equip:list 'S1F4' = {
20110208:025946:uv-3_equip:  ushort 'UW_0'[1] = {7338},
20110208:025946:uv-3_equip:  ubyte 'UB_1'[1] = {5}
20110208:025946:uv-3_equip:  
20110208:025946:uv-3_equip:};
20110208:025946:uv-3_equip:%DBG002|363| |diag : return EQP_STATUS{} (1) = { "$ACK" }
20110208:025946:uv-3_equip:%DBG128|363| |diag : Looking for outgoing reply at depth 27
20110208:025946:uv-3_equip:%DBG128|364| |diag : outgoing reply depth-- = 27
--To /local/fifo/router ----
|heartbeat|reply|EQP_STATUS|uv-3_equip|1d93bdf6|20110208:025945||STATUS|$ACK|||
-------------
20110208:025946:uv-3_equip:%DBG002|364| |uv-3_equip returning to IDLE state



20110208:025950:uv-3_equip:%DBG128|629| |<-ENQ
20110208:025950:uv-3_equip:%DBG128|629| |->EOT
20110208:025950:uv-3_equip:%DBG128|729| |<-29 received (29/29 bytes)
20110208:025950:uv-3_equip:%DBG128|730| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 bc 01 03 b1 04 00 00 11 3b b1 04 00 00 03 03 01 00 04 24 ]
20110208:025950:uv-3_equip:%DBG128|730| |->ACK
20110208:025950:uv-3_equip:%DBG128|730| |Received S6F11 (Device 1) (TID=0x14bc) (SID=0x00) 
20110208:025950:uv-3_equip:%DBG128|730| |Saving TID=0x14bc, SID=0x00
20110208:025950:uv-3_equip:%DBG002|730| |uv-3_equip returning to IDLE state
20110208:025950:uv-3_equip:%DBG128|730| |Found queued event or query message(8) 'SECS_S6F11' for uv-3_equip
20110208:025950:uv-3_equip:%DBG128|730| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 27
20110208:025950:uv-3_equip:%DBG128|730| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014bc, isReply=0, isSECS=1
20110208:025950:uv-3_equip:%DBG128|730| |proto: outgoing reply depth++ = 28
20110208:025950:uv-3_equip:%DBG002|730| |diag : Handling msg interrupt
20110208:025950:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025950:uv-3_equip:%DBG002|731| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025950:uv-3_equip:%DBG002|731| |diag : return STATUS{}  = { "$ACK" }
20110208:025950:uv-3_equip:%DBG002|731| |...checking STATUS{}  = { "$ACK" }
20110208:025950:uv-3_equip:%DBG002|731| |diag : Handling msg call for 'SECS_S6F11'
20110208:025950:uv-3_equip:%DBG128|731| |diag : Storing outgoing reply for SECS_S6F11 at depth 27
20110208:025950:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025950:uv-3_equip:list 'S6F11' = {
20110208:025950:uv-3_equip:  ulong 'UL_0'[1] = {4411},
20110208:025950:uv-3_equip:  ulong 'UL_1'[1] = {771},
20110208:025950:uv-3_equip:  list 'L_2' = {}
20110208:025950:uv-3_equip:  
20110208:025950:uv-3_equip:};
20110208:025950:uv-3_equip:%DBG002|734| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025950:uv-3_equip:Executing local definition for S6F11
20110208:025950:uv-3_equip:Sending S2F41, TID=0000012e
20110208:025950:uv-3_equip:%DBG128|737| |Setting/Restoring TID=0x12e, SID=0x00
20110208:025950:uv-3_equip:%DBG128|737| |Sending S2F41 (device 1) (TID=0x12e) (SID=0x00)
20110208:025950:uv-3_equip:%DBG128|737| |->ENQ
20110208:025950:uv-3_equip:%DBG128|910| |<-EOT
20110208:025950:uv-3_equip:%DBG128|910| |->BLK [31 00 01 82 29 80 01 00 00 01 2e 01 02 41 0b 50 50 20 55 4e 53 45 4c 45 43 54 01 01 01 02 41 07 50 4f 52 54 20 49 44 41 09 4c 45 46 54 20 50 4f 52 54 09 e7 ]
20110208:025951:uv-3_equip:%DBG128|087| |<-ACK
20110208:025951:uv-3_equip:%DBG128|087| |diag : incoming reply depth++ = 1
20110208:025951:uv-3_equip:%DBG128|087| |diag : Setting expected reply for sender:method:tid 1#secs:S2F42:0000012e at incoming depth 0
20110208:025951:uv-3_equip:%DBG128|088| |diag : Setting SECS reply S2F42 at incoming depth 0
20110208:025951:uv-3_equip:...waiting for S2F42 reply from device '1', timeout in 30 seconds
20110208:025951:uv-3_equip:%DBG002|088| |uv-3_equip returning to QUERY state
20110208:025951:uv-3_equip:%DBG128|166| |<-ENQ
20110208:025951:uv-3_equip:%DBG128|166| |->EOT
20110208:025951:uv-3_equip:%DBG128|277| |<-20 received (20/20 bytes)
20110208:025951:uv-3_equip:%DBG128|277| |<-BLK [11 80 01 02 2a 80 01 00 00 01 2e 01 02 21 01 04 01 00 01 87 ]
20110208:025951:uv-3_equip:%DBG128|277| |->ACK
20110208:025951:uv-3_equip:%DBG128|277| |Received S2F42 (Device 1) (TID=0x12e) (SID=0x00) 
20110208:025951:uv-3_equip:%DBG002|278| |uv-3_equip returning to QUERY state
20110208:025951:uv-3_equip:%DBG128|278| |Found queued reply message(9) 'S2F42' for uv-3_equip
20110208:025951:uv-3_equip:%DBG128|278| |diag : Target=uv-3_equip, Sender=1#secs, Method=S2F42, TID=0000012e, isReply=1, isSECS=1
20110208:025951:uv-3_equip:%DBG128|278| |diag : Matching 1#secs=1#secs, S2F42=S2F42, 0000012e=0000012e
20110208:025951:uv-3_equip:Matched TID=0000012e, Method=S2F42, Depth=0, Sender=1#secs
20110208:025951:uv-3_equip:%DBG128|278| |diag : Matched current reply S2F42 at incoming depth 0
20110208:025951:uv-3_equip:%DBG128|278| |diag : Matched 1#secs=1#secs, S2F42=S2F42, 0000012e=0000012e
20110208:025951:uv-3_equip:%DBG128|278| |diag : Matched incoming reply at depth 0
20110208:025951:uv-3_equip:%DBG128|278| |diag : Consuming reply message at incoming depth 0
20110208:025951:uv-3_equip:%DBG128|278| |proto: incoming reply depth-- = 0
20110208:025951:uv-3_equip:%DBG002|278| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025951:uv-3_equip:list 'S2F42' = {
20110208:025951:uv-3_equip:  binary 'BN_0'[1] = {0x04},
20110208:025951:uv-3_equip:  list 'L_1' = {}
20110208:025951:uv-3_equip:  
20110208:025951:uv-3_equip:};
20110208:025951:uv-3_equip:Sending S2F41, TID=0000012f
20110208:025951:uv-3_equip:%DBG128|281| |Setting/Restoring TID=0x12f, SID=0x00
20110208:025951:uv-3_equip:%DBG128|281| |Sending S2F41 (device 1) (TID=0x12f) (SID=0x00)
20110208:025951:uv-3_equip:%DBG128|281| |->ENQ
20110208:025951:uv-3_equip:%DBG128|376| |<-ENQ(EOT)
20110208:025951:uv-3_equip:%DBG128|376| |ENQ contention. Relinquishing...
20110208:025951:uv-3_equip:%DBG128|376| |->EOT
20110208:025951:uv-3_equip:%DBG128|476| |<-29 received (29/29 bytes)
20110208:025951:uv-3_equip:%DBG128|476| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 bd 01 03 b1 04 00 00 11 3c b1 04 00 00 01 08 01 00 04 29 ]
20110208:025951:uv-3_equip:%DBG128|476| |->ACK
20110208:025951:uv-3_equip:%DBG128|476| |Received S6F11 (Device 1) (TID=0x14bd) (SID=0x00) 
20110208:025951:uv-3_equip:%DBG128|476| |Saving TID=0x14bd, SID=0x00
20110208:025951:uv-3_equip:%DBG002|477| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:025951:uv-3_equip:...re-sending interrupted S2F41, TID=0000012f
20110208:025951:uv-3_equip:%DBG128|477| |Setting/Restoring TID=0x12f, SID=0x00
20110208:025951:uv-3_equip:%DBG128|477| |Sending S2F41 (device 1) (TID=0x12f) (SID=0x00)
20110208:025951:uv-3_equip:%DBG128|477| |->ENQ
20110208:025951:uv-3_equip:%DBG128|733| |<-EOT
20110208:025951:uv-3_equip:%DBG128|733| |->BLK [55 00 01 82 29 80 01 00 00 01 2f 01 02 41 09 50 50 20 53 45 4c 45 43 54 01 03 01 02 41 07 50 4f 52 54 20 49 44 41 09 4c 45 46 54 20 50 4f 52 54 01 02 41 04 50 50 49 44 41 08 52 45 42 41 4b 45 20 41 01 02 41 06 4c 4f 54 20 49 44 41 08 42 30 35 33 36 31 2e 31 10 dd ]
20110208:025951:uv-3_equip:%DBG128|922| |<-ACK
20110208:025951:uv-3_equip:%DBG128|922| |diag : incoming reply depth++ = 1
20110208:025951:uv-3_equip:%DBG128|922| |diag : Setting expected reply for sender:method:tid 1#secs:S2F42:0000012f at incoming depth 0
20110208:025951:uv-3_equip:%DBG128|923| |diag : Setting SECS reply S2F42 at incoming depth 0
20110208:025951:uv-3_equip:...waiting for S2F42 reply from device '1', timeout in 30 seconds
20110208:025951:uv-3_equip:%DBG002|923| |uv-3_equip returning to QUERY state
20110208:025951:uv-3_equip:%DBG128|923| |Found queued event or query message(10) 'SECS_S6F11' for uv-3_equip
20110208:025951:uv-3_equip:%DBG128|923| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:025951:uv-3_equip:%DBG128|923| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014bd, isReply=0, isSECS=1
%MESSAGE: warning: Message interrupted query at incoming depth 1
20110208:025951:uv-3_equip:%DBG128|923| |proto: outgoing reply depth++ = 29
20110208:025951:uv-3_equip:%DBG002|923| |diag : Handling msg interrupt
20110208:025951:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025951:uv-3_equip:%DBG002|924| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025951:uv-3_equip:%DBG002|924| |diag : return STATUS{}  = { "$ACK" }
20110208:025951:uv-3_equip:%DBG002|924| |...checking STATUS{}  = { "$ACK" }
20110208:025951:uv-3_equip:%DBG002|924| |diag : Handling msg call for 'SECS_S6F11'
20110208:025951:uv-3_equip:%DBG128|924| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:025951:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025951:uv-3_equip:list 'S6F11' = {
20110208:025951:uv-3_equip:  ulong 'UL_0'[1] = {4412},
20110208:025951:uv-3_equip:  ulong 'UL_1'[1] = {264},
20110208:025951:uv-3_equip:  list 'L_2' = {}
20110208:025951:uv-3_equip:  
20110208:025951:uv-3_equip:};
20110208:025951:uv-3_equip:%DBG002|928| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025951:uv-3_equip:Executing local definition for S6F11
20110208:025951:uv-3_equip:binary 'S6F12' = {0x00};
20110208:025951:uv-3_equip:%DBG002|932| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025951:uv-3_equip:%DBG128|932| |diag : Looking for outgoing reply at depth 28
20110208:025951:uv-3_equip:%DBG128|932| |diag : outgoing reply depth-- = 28
20110208:025951:uv-3_equip:%DBG128|932| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:025951:uv-3_equip:%DBG128|932| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:025951:uv-3_equip:%DBG128|932| |Setting/Restoring TID=0x14bd, SID=0x00
20110208:025951:uv-3_equip:%DBG128|932| |Sending S6F12 (device 1) (TID=0x14bd) (SID=0x00)
20110208:025951:uv-3_equip:%DBG128|933| |->ENQ
20110208:025952:uv-3_equip:%DBG128|090| |<-EOT
20110208:025952:uv-3_equip:%DBG128|090| |->BLK [0d 00 01 06 0c 80 01 00 00 14 bd 21 01 00 01 87 ]
20110208:025952:uv-3_equip:%DBG128|258| |<-ACK
20110208:025952:uv-3_equip:%DBG002|258| |uv-3_equip returning to QUERY state
20110208:025952:uv-3_equip:%DBG128|486| |<-ENQ
20110208:025952:uv-3_equip:%DBG128|486| |->EOT
20110208:025952:uv-3_equip:%DBG128|586| |<-29 received (29/29 bytes)
20110208:025952:uv-3_equip:%DBG128|586| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 be 01 03 b1 04 00 00 11 3d b1 04 00 00 01 08 01 00 04 2b ]
20110208:025952:uv-3_equip:%DBG128|586| |->ACK
20110208:025952:uv-3_equip:%DBG128|586| |Received S6F11 (Device 1) (TID=0x14be) (SID=0x00) 
20110208:025952:uv-3_equip:%DBG128|587| |Saving TID=0x14be, SID=0x00
20110208:025952:uv-3_equip:%DBG002|587| |uv-3_equip returning to QUERY state
20110208:025952:uv-3_equip:%DBG128|587| |Found queued event or query message(11) 'SECS_S6F11' for uv-3_equip
20110208:025952:uv-3_equip:%DBG128|587| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:025952:uv-3_equip:%DBG128|587| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014be, isReply=0, isSECS=1
%MESSAGE: warning: Message interrupted query at incoming depth 1
20110208:025952:uv-3_equip:%DBG128|587| |proto: outgoing reply depth++ = 29
20110208:025952:uv-3_equip:%DBG002|587| |diag : Handling msg interrupt
20110208:025952:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025952:uv-3_equip:%DBG002|588| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025952:uv-3_equip:%DBG002|588| |diag : return STATUS{}  = { "$ACK" }
20110208:025952:uv-3_equip:%DBG002|588| |...checking STATUS{}  = { "$ACK" }
20110208:025952:uv-3_equip:%DBG002|588| |diag : Handling msg call for 'SECS_S6F11'
20110208:025952:uv-3_equip:%DBG128|588| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:025952:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025952:uv-3_equip:list 'S6F11' = {
20110208:025952:uv-3_equip:  ulong 'UL_0'[1] = {4413},
20110208:025952:uv-3_equip:  ulong 'UL_1'[1] = {264},
20110208:025952:uv-3_equip:  list 'L_2' = {}
20110208:025952:uv-3_equip:  
20110208:025952:uv-3_equip:};
20110208:025952:uv-3_equip:%DBG002|592| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025952:uv-3_equip:Executing local definition for S6F11
20110208:025952:uv-3_equip:binary 'S6F12' = {0x00};
20110208:025952:uv-3_equip:%DBG002|595| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025952:uv-3_equip:%DBG128|596| |diag : Looking for outgoing reply at depth 28
20110208:025952:uv-3_equip:%DBG128|596| |diag : outgoing reply depth-- = 28
20110208:025952:uv-3_equip:%DBG128|596| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:025952:uv-3_equip:%DBG128|596| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:025952:uv-3_equip:%DBG128|596| |Setting/Restoring TID=0x14be, SID=0x00
20110208:025952:uv-3_equip:%DBG128|596| |Sending S6F12 (device 1) (TID=0x14be) (SID=0x00)
20110208:025952:uv-3_equip:%DBG128|596| |->ENQ
20110208:025952:uv-3_equip:%DBG128|686| |<-ENQ(EOT)
20110208:025952:uv-3_equip:%DBG128|686| |ENQ contention. Relinquishing...
20110208:025952:uv-3_equip:%DBG128|687| |->EOT
20110208:025952:uv-3_equip:%DBG128|785| |<-20 received (20/20 bytes)
20110208:025952:uv-3_equip:%DBG128|786| |<-BLK [11 80 01 02 2a 80 01 00 00 01 2f 01 02 21 01 04 01 00 01 88 ]
20110208:025952:uv-3_equip:%DBG128|786| |->ACK
20110208:025952:uv-3_equip:%DBG128|786| |Received S2F42 (Device 1) (TID=0x12f) (SID=0x00) 
20110208:025952:uv-3_equip:%DBG128|786| |proto: outgoing reply depth++ = 29
20110208:025952:uv-3_equip:%DBG128|786| |Found queued reply message(12) 'S2F42' for uv-3_equip
20110208:025952:uv-3_equip:%DBG128|786| |diag : Target=uv-3_equip, Sender=1#secs, Method=S2F42, TID=0000012f, isReply=1, isSECS=1
20110208:025952:uv-3_equip:%DBG128|786| |diag : Matching 1#secs=1#secs, S2F42=S2F42, 0000012f=0000012f
20110208:025952:uv-3_equip:Matched TID=0000012f, Method=S2F42, Depth=0, Sender=1#secs
20110208:025952:uv-3_equip:%DBG128|786| |diag : Matched current reply S2F42 at incoming depth 0
20110208:025952:uv-3_equip:%DBG128|786| |diag : Matched 1#secs=1#secs, S2F42=S2F42, 0000012f=0000012f
20110208:025952:uv-3_equip:%DBG128|786| |diag : outgoing reply depth-- = 28
20110208:025952:uv-3_equip:%DBG128|786| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:025952:uv-3_equip:Re-sending interrupted S6F12 reply message
20110208:025952:uv-3_equip:%DBG128|786| |Setting/Restoring TID=0x14be, SID=0x00
20110208:025952:uv-3_equip:%DBG128|786| |Sending S6F12 (device 1) (TID=0x14be) (SID=0x00)
20110208:025952:uv-3_equip:%DBG128|786| |->ENQ
20110208:025952:uv-3_equip:%DBG128|976| |<-EOT
20110208:025952:uv-3_equip:%DBG128|976| |->BLK [0d 00 01 06 0c 80 01 00 00 14 be 21 01 00 01 88 ]
20110208:025953:uv-3_equip:%DBG128|144| |<-ACK
20110208:025953:uv-3_equip:%DBG002|144| |uv-3_equip returning to QUERY state
20110208:025953:uv-3_equip:%DBG128|144| |diag : Matched incoming reply at depth 0
20110208:025953:uv-3_equip:%DBG128|144| |diag : Consuming reply message at incoming depth 0
20110208:025953:uv-3_equip:%DBG128|144| |proto: incoming reply depth-- = 0
20110208:025953:uv-3_equip:%DBG002|145| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025953:uv-3_equip:list 'S2F42' = {
20110208:025953:uv-3_equip:  binary 'BN_0'[1] = {0x04},
20110208:025953:uv-3_equip:  list 'L_1' = {}
20110208:025953:uv-3_equip:  
20110208:025953:uv-3_equip:};
20110208:025953:uv-3_equip:binary 'S6F12' = {0x00};
20110208:025953:uv-3_equip:%DBG002|147| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025953:uv-3_equip:%DBG128|148| |diag : Looking for outgoing reply at depth 27
20110208:025953:uv-3_equip:%DBG128|148| |diag : outgoing reply depth-- = 27
20110208:025953:uv-3_equip:%DBG128|148| |diag : Looking for outgoing secs reply S6F12 at depth 27
20110208:025953:uv-3_equip:%DBG128|148| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:025953:uv-3_equip:%DBG128|148| |Setting/Restoring TID=0x14bc, SID=0x00
20110208:025953:uv-3_equip:%DBG128|148| |Sending S6F12 (device 1) (TID=0x14bc) (SID=0x00)
20110208:025953:uv-3_equip:%DBG128|148| |->ENQ
20110208:025953:uv-3_equip:%DBG128|243| |<-EOT
20110208:025953:uv-3_equip:%DBG128|244| |->BLK [0d 00 01 06 0c 80 01 00 00 14 bc 21 01 00 01 86 ]
20110208:025953:uv-3_equip:%DBG128|431| |<-ACK
20110208:025953:uv-3_equip:%DBG002|431| |uv-3_equip returning to IDLE state
20110208:025957:uv-3_equip:%DBG128|500| |<-ENQ
20110208:025957:uv-3_equip:%DBG128|500| |->EOT
20110208:025957:uv-3_equip:%DBG128|610| |<-29 received (29/29 bytes)
20110208:025957:uv-3_equip:%DBG128|610| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 bf 01 03 b1 04 00 00 11 3e b1 04 00 00 03 05 01 00 04 2c ]
20110208:025957:uv-3_equip:%DBG128|610| |->ACK
20110208:025957:uv-3_equip:%DBG128|610| |Received S6F11 (Device 1) (TID=0x14bf) (SID=0x00) 
20110208:025957:uv-3_equip:%DBG128|610| |Saving TID=0x14bf, SID=0x00
20110208:025957:uv-3_equip:%DBG002|610| |uv-3_equip returning to IDLE state
20110208:025957:uv-3_equip:%DBG128|611| |Found queued event or query message(13) 'SECS_S6F11' for uv-3_equip
20110208:025957:uv-3_equip:%DBG128|611| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 27
20110208:025957:uv-3_equip:%DBG128|611| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014bf, isReply=0, isSECS=1
20110208:025957:uv-3_equip:%DBG128|611| |proto: outgoing reply depth++ = 28
20110208:025957:uv-3_equip:%DBG002|611| |diag : Handling msg interrupt
20110208:025957:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025957:uv-3_equip:%DBG002|612| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025957:uv-3_equip:%DBG002|612| |diag : return STATUS{}  = { "$ACK" }
20110208:025957:uv-3_equip:%DBG002|612| |...checking STATUS{}  = { "$ACK" }
20110208:025957:uv-3_equip:%DBG002|612| |diag : Handling msg call for 'SECS_S6F11'
20110208:025957:uv-3_equip:%DBG128|612| |diag : Storing outgoing reply for SECS_S6F11 at depth 27
20110208:025957:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025957:uv-3_equip:list 'S6F11' = {
20110208:025957:uv-3_equip:  ulong 'UL_0'[1] = {4414},
20110208:025957:uv-3_equip:  ulong 'UL_1'[1] = {773},
20110208:025957:uv-3_equip:  list 'L_2' = {}
20110208:025957:uv-3_equip:  
20110208:025957:uv-3_equip:};
20110208:025957:uv-3_equip:%DBG002|616| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025957:uv-3_equip:Executing local definition for S6F11
20110208:025957:uv-3_equip:Sending S2F41, TID=00000131
20110208:025957:uv-3_equip:%DBG128|619| |Setting/Restoring TID=0x131, SID=0x00
20110208:025957:uv-3_equip:%DBG128|619| |Sending S2F41 (device 1) (TID=0x131) (SID=0x00)
20110208:025957:uv-3_equip:%DBG128|619| |->ENQ
20110208:025957:uv-3_equip:%DBG128|818| |<-EOT
20110208:025957:uv-3_equip:%DBG128|818| |->BLK [32 00 01 82 29 80 01 00 00 01 31 01 02 41 0b 50 50 20 55 4e 53 45 4c 45 43 54 01 01 01 02 41 07 50 4f 52 54 20 49 44 41 0a 52 49 47 48 54 20 50 4f 52 54 0a 3e ]
20110208:025958:uv-3_equip:%DBG128|029| |<-ACK
20110208:025958:uv-3_equip:%DBG128|029| |<-ENQ(ACK)
20110208:025958:uv-3_equip:%DBG128|029| |ENQ contention. Relinquishing...
20110208:025958:uv-3_equip:%DBG128|029| |->EOT
20110208:025958:uv-3_equip:%DBG128|152| |<-29 received (29/29 bytes)
20110208:025958:uv-3_equip:%DBG128|152| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 c0 01 03 b1 04 00 00 11 3f b1 04 00 00 01 09 01 00 04 30 ]
20110208:025958:uv-3_equip:%DBG128|152| |->ACK
20110208:025958:uv-3_equip:%DBG128|152| |Received S6F11 (Device 1) (TID=0x14c0) (SID=0x00) 
20110208:025958:uv-3_equip:%DBG128|152| |Saving TID=0x14c0, SID=0x00
20110208:025958:uv-3_equip:%DBG128|152| |diag : incoming reply depth++ = 1
20110208:025958:uv-3_equip:%DBG128|152| |diag : Setting expected reply for sender:method:tid 1#secs:S2F42:00000131 at incoming depth 0
20110208:025958:uv-3_equip:%DBG128|152| |diag : Setting SECS reply S2F42 at incoming depth 0
20110208:025958:uv-3_equip:...waiting for S2F42 reply from device '1', timeout in 30 seconds
20110208:025958:uv-3_equip:%DBG002|152| |uv-3_equip returning to QUERY state
20110208:025958:uv-3_equip:%DBG128|152| |Found queued event or query message(14) 'SECS_S6F11' for uv-3_equip
20110208:025958:uv-3_equip:%DBG128|152| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:025958:uv-3_equip:%DBG128|152| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c0, isReply=0, isSECS=1
%MESSAGE: warning: Message interrupted query at incoming depth 1
20110208:025958:uv-3_equip:%DBG128|152| |proto: outgoing reply depth++ = 29
20110208:025958:uv-3_equip:%DBG002|152| |diag : Handling msg interrupt
20110208:025958:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025958:uv-3_equip:%DBG002|153| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025958:uv-3_equip:%DBG002|154| |diag : return STATUS{}  = { "$ACK" }
20110208:025958:uv-3_equip:%DBG002|154| |...checking STATUS{}  = { "$ACK" }
20110208:025958:uv-3_equip:%DBG002|154| |diag : Handling msg call for 'SECS_S6F11'
20110208:025958:uv-3_equip:%DBG128|154| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:025958:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025958:uv-3_equip:list 'S6F11' = {
20110208:025958:uv-3_equip:  ulong 'UL_0'[1] = {4415},
20110208:025958:uv-3_equip:  ulong 'UL_1'[1] = {265},
20110208:025958:uv-3_equip:  list 'L_2' = {}
20110208:025958:uv-3_equip:  
20110208:025958:uv-3_equip:};
20110208:025958:uv-3_equip:%DBG002|157| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025958:uv-3_equip:Executing local definition for S6F11
20110208:025958:uv-3_equip:binary 'S6F12' = {0x00};
20110208:025958:uv-3_equip:%DBG002|161| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025958:uv-3_equip:%DBG128|162| |diag : Looking for outgoing reply at depth 28
20110208:025958:uv-3_equip:%DBG128|162| |diag : outgoing reply depth-- = 28
20110208:025958:uv-3_equip:%DBG128|162| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:025958:uv-3_equip:%DBG128|162| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:025958:uv-3_equip:%DBG128|162| |Setting/Restoring TID=0x14c0, SID=0x00
20110208:025958:uv-3_equip:%DBG128|162| |Sending S6F12 (device 1) (TID=0x14c0) (SID=0x00)
20110208:025958:uv-3_equip:%DBG128|162| |->ENQ
20110208:025958:uv-3_equip:%DBG128|258| |<-ENQ(EOT)
20110208:025958:uv-3_equip:%DBG128|258| |ENQ contention. Relinquishing...
20110208:025958:uv-3_equip:%DBG128|258| |->EOT
20110208:025958:uv-3_equip:%DBG128|438| |<-20 received (20/20 bytes)
20110208:025958:uv-3_equip:%DBG128|438| |<-BLK [11 80 01 02 2a 80 01 00 00 01 31 01 02 21 01 04 01 00 01 8a ]
20110208:025958:uv-3_equip:%DBG128|439| |->ACK
20110208:025958:uv-3_equip:%DBG128|439| |Received S2F42 (Device 1) (TID=0x131) (SID=0x00) 
20110208:025958:uv-3_equip:%DBG128|439| |proto: outgoing reply depth++ = 29
20110208:025958:uv-3_equip:%DBG128|439| |Found queued reply message(15) 'S2F42' for uv-3_equip
20110208:025958:uv-3_equip:%DBG128|439| |diag : Target=uv-3_equip, Sender=1#secs, Method=S2F42, TID=00000131, isReply=1, isSECS=1
20110208:025958:uv-3_equip:%DBG128|439| |diag : Matching 1#secs=1#secs, S2F42=S2F42, 00000131=00000131
20110208:025958:uv-3_equip:Matched TID=00000131, Method=S2F42, Depth=0, Sender=1#secs
20110208:025958:uv-3_equip:%DBG128|439| |diag : Matched current reply S2F42 at incoming depth 0
20110208:025958:uv-3_equip:%DBG128|439| |diag : Matched 1#secs=1#secs, S2F42=S2F42, 00000131=00000131
20110208:025958:uv-3_equip:%DBG128|439| |diag : outgoing reply depth-- = 28
20110208:025958:uv-3_equip:%DBG128|439| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:025958:uv-3_equip:Re-sending interrupted S6F12 reply message
20110208:025958:uv-3_equip:%DBG128|439| |Setting/Restoring TID=0x14c0, SID=0x00
20110208:025958:uv-3_equip:%DBG128|439| |Sending S6F12 (device 1) (TID=0x14c0) (SID=0x00)
20110208:025958:uv-3_equip:%DBG128|439| |->ENQ
20110208:025958:uv-3_equip:%DBG128|627| |<-EOT
20110208:025958:uv-3_equip:%DBG128|627| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c0 21 01 00 01 8a ]
20110208:025958:uv-3_equip:%DBG128|815| |<-ACK
20110208:025958:uv-3_equip:%DBG002|815| |uv-3_equip returning to QUERY state
20110208:025958:uv-3_equip:%DBG128|815| |diag : Matched incoming reply at depth 0
20110208:025958:uv-3_equip:%DBG128|815| |diag : Consuming reply message at incoming depth 0
20110208:025958:uv-3_equip:%DBG128|815| |proto: incoming reply depth-- = 0
20110208:025958:uv-3_equip:%DBG002|816| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:025958:uv-3_equip:list 'S2F42' = {
20110208:025958:uv-3_equip:  binary 'BN_0'[1] = {0x04},
20110208:025958:uv-3_equip:  list 'L_1' = {}
20110208:025958:uv-3_equip:  
20110208:025958:uv-3_equip:};
20110208:025958:uv-3_equip:Sending S2F41, TID=00000132
20110208:025958:uv-3_equip:%DBG128|818| |Setting/Restoring TID=0x132, SID=0x00
20110208:025958:uv-3_equip:%DBG128|818| |Sending S2F41 (device 1) (TID=0x132) (SID=0x00)
20110208:025958:uv-3_equip:%DBG128|818| |->ENQ
20110208:025958:uv-3_equip:%DBG128|914| |<-EOT
20110208:025958:uv-3_equip:%DBG128|915| |->BLK [46 00 01 82 29 80 01 00 00 01 32 01 02 41 09 50 50 20 53 45 4c 45 43 54 01 03 01 02 41 07 50 4f 52 54 20 49 44 41 0a 52 49 47 48 54 20 50 4f 52 54 01 02 41 04 50 50 49 44 41 00 01 02 41 06 4c 4f 54 20 49 44 41 00 0d 79 ]
20110208:025959:uv-3_equip:%DBG128|115| |<-ACK
20110208:025959:uv-3_equip:%DBG128|115| |diag : incoming reply depth++ = 1
20110208:025959:uv-3_equip:%DBG128|115| |diag : Setting expected reply for sender:method:tid 1#secs:S2F42:00000132 at incoming depth 0
20110208:025959:uv-3_equip:%DBG128|115| |diag : Setting SECS reply S2F42 at incoming depth 0
20110208:025959:uv-3_equip:...waiting for S2F42 reply from device '1', timeout in 30 seconds
20110208:025959:uv-3_equip:%DBG002|115| |uv-3_equip returning to QUERY state
20110208:025959:uv-3_equip:%DBG128|352| |<-ENQ
20110208:025959:uv-3_equip:%DBG128|352| |->EOT
20110208:025959:uv-3_equip:%DBG128|453| |<-29 received (29/29 bytes)
20110208:025959:uv-3_equip:%DBG128|453| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 c1 01 03 b1 04 00 00 11 40 b1 04 00 00 01 09 01 00 04 32 ]
20110208:025959:uv-3_equip:%DBG128|453| |->ACK
20110208:025959:uv-3_equip:%DBG128|453| |Received S6F11 (Device 1) (TID=0x14c1) (SID=0x00) 
20110208:025959:uv-3_equip:%DBG128|453| |Saving TID=0x14c1, SID=0x00
20110208:025959:uv-3_equip:%DBG002|453| |uv-3_equip returning to QUERY state
20110208:025959:uv-3_equip:%DBG128|453| |Found queued event or query message(16) 'SECS_S6F11' for uv-3_equip
20110208:025959:uv-3_equip:%DBG128|453| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:025959:uv-3_equip:%DBG128|453| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c1, isReply=0, isSECS=1
%MESSAGE: warning: Message interrupted query at incoming depth 1
20110208:025959:uv-3_equip:%DBG128|453| |proto: outgoing reply depth++ = 29
20110208:025959:uv-3_equip:%DBG002|453| |diag : Handling msg interrupt
20110208:025959:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:025959:uv-3_equip:%DBG002|454| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:025959:uv-3_equip:%DBG002|454| |diag : return STATUS{}  = { "$ACK" }
20110208:025959:uv-3_equip:%DBG002|454| |...checking STATUS{}  = { "$ACK" }
20110208:025959:uv-3_equip:%DBG002|454| |diag : Handling msg call for 'SECS_S6F11'
20110208:025959:uv-3_equip:%DBG128|454| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:025959:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:025959:uv-3_equip:list 'S6F11' = {
20110208:025959:uv-3_equip:  ulong 'UL_0'[1] = {4416},
20110208:025959:uv-3_equip:  ulong 'UL_1'[1] = {265},
20110208:025959:uv-3_equip:  list 'L_2' = {}
20110208:025959:uv-3_equip:  
20110208:025959:uv-3_equip:};
20110208:025959:uv-3_equip:%DBG002|458| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:025959:uv-3_equip:Executing local definition for S6F11
20110208:025959:uv-3_equip:binary 'S6F12' = {0x00};
20110208:025959:uv-3_equip:%DBG002|462| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:025959:uv-3_equip:%DBG128|462| |diag : Looking for outgoing reply at depth 28
20110208:025959:uv-3_equip:%DBG128|462| |diag : outgoing reply depth-- = 28
20110208:025959:uv-3_equip:%DBG128|462| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:025959:uv-3_equip:%DBG128|462| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:025959:uv-3_equip:%DBG128|462| |Setting/Restoring TID=0x14c1, SID=0x00
20110208:025959:uv-3_equip:%DBG128|462| |Sending S6F12 (device 1) (TID=0x14c1) (SID=0x00)
20110208:025959:uv-3_equip:%DBG128|462| |->ENQ
20110208:025959:uv-3_equip:%DBG128|563| |<-ENQ(EOT)
20110208:025959:uv-3_equip:%DBG128|564| |ENQ contention. Relinquishing...
20110208:025959:uv-3_equip:%DBG128|564| |->EOT
20110208:025959:uv-3_equip:%DBG128|673| |<-20 received (20/20 bytes)
20110208:025959:uv-3_equip:%DBG128|673| |<-BLK [11 80 01 02 2a 80 01 00 00 01 32 01 02 21 01 04 01 00 01 8b ]
20110208:025959:uv-3_equip:%DBG128|673| |->ACK
20110208:025959:uv-3_equip:%DBG128|673| |Received S2F42 (Device 1) (TID=0x132) (SID=0x00) 
20110208:025959:uv-3_equip:%DBG128|673| |proto: outgoing reply depth++ = 29
20110208:025959:uv-3_equip:%DBG128|673| |Found queued reply message(17) 'S2F42' for uv-3_equip
20110208:025959:uv-3_equip:%DBG128|673| |diag : Target=uv-3_equip, Sender=1#secs, Method=S2F42, TID=00000132, isReply=1, isSECS=1
20110208:025959:uv-3_equip:%DBG128|673| |diag : Matching 1#secs=1#secs, S2F42=S2F42, 00000132=00000132
20110208:025959:uv-3_equip:Matched TID=00000132, Method=S2F42, Depth=0, Sender=1#secs
20110208:025959:uv-3_equip:%DBG128|673| |diag : Matched current reply S2F42 at incoming depth 0
20110208:025959:uv-3_equip:%DBG128|673| |diag : Matched 1#secs=1#secs, S2F42=S2F42, 00000132=00000132
20110208:025959:uv-3_equip:%DBG128|673| |diag : outgoing reply depth-- = 28
20110208:025959:uv-3_equip:%DBG128|674| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:025959:uv-3_equip:Re-sending interrupted S6F12 reply message
20110208:025959:uv-3_equip:%DBG128|674| |Setting/Restoring TID=0x14c1, SID=0x00
20110208:025959:uv-3_equip:%DBG128|674| |Sending S6F12 (device 1) (TID=0x14c1) (SID=0x00)
20110208:025959:uv-3_equip:%DBG128|674| |->ENQ
20110208:025959:uv-3_equip:%DBG128|872| |<-EOT
20110208:025959:uv-3_equip:%DBG128|872| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c1 21 01 00 01 8b ]
20110208:030000:uv-3_equip:%DBG128|051| |<-ACK
20110208:030000:uv-3_equip:%DBG002|051| |uv-3_equip returning to QUERY state
20110208:030000:uv-3_equip:%DBG128|051| |diag : Matched incoming reply at depth 0
20110208:030000:uv-3_equip:%DBG128|051| |diag : Consuming reply message at incoming depth 0
20110208:030000:uv-3_equip:%DBG128|051| |proto: incoming reply depth-- = 0
20110208:030000:uv-3_equip:%DBG002|052| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:030000:uv-3_equip:list 'S2F42' = {
20110208:030000:uv-3_equip:  binary 'BN_0'[1] = {0x04},
20110208:030000:uv-3_equip:  list 'L_1' = {}
20110208:030000:uv-3_equip:  
20110208:030000:uv-3_equip:};
20110208:030000:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030000:uv-3_equip:%DBG002|054| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030000:uv-3_equip:%DBG128|055| |diag : Looking for outgoing reply at depth 27
20110208:030000:uv-3_equip:%DBG128|055| |diag : outgoing reply depth-- = 27
20110208:030000:uv-3_equip:%DBG128|055| |diag : Looking for outgoing secs reply S6F12 at depth 27
20110208:030000:uv-3_equip:%DBG128|055| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030000:uv-3_equip:%DBG128|055| |Setting/Restoring TID=0x14bf, SID=0x00
20110208:030000:uv-3_equip:%DBG128|055| |Sending S6F12 (device 1) (TID=0x14bf) (SID=0x00)
20110208:030000:uv-3_equip:%DBG128|055| |->ENQ
20110208:030000:uv-3_equip:%DBG128|161| |<-EOT
20110208:030000:uv-3_equip:%DBG128|161| |->BLK [0d 00 01 06 0c 80 01 00 00 14 bf 21 01 00 01 89 ]
20110208:030000:uv-3_equip:%DBG128|328| |<-ACK
20110208:030000:uv-3_equip:%DBG002|328| |uv-3_equip returning to IDLE state






20110208:030000:uv-3_equip:%DBG128|656| |<-ENQ
20110208:030000:uv-3_equip:%DBG128|656| |->EOT
20110208:030000:uv-3_equip:%DBG128|779| |<-29 received (29/29 bytes)
20110208:030000:uv-3_equip:%DBG128|780| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 c2 01 03 b1 04 00 00 11 41 b1 04 00 00 fe 0b 01 00 05 33 ]
20110208:030000:uv-3_equip:%DBG128|780| |->ACK
20110208:030000:uv-3_equip:%DBG128|780| |Received S6F11 (Device 1) (TID=0x14c2) (SID=0x00) 
20110208:030000:uv-3_equip:%DBG128|780| |Saving TID=0x14c2, SID=0x00
20110208:030000:uv-3_equip:%DBG002|780| |uv-3_equip returning to IDLE state

SMOKING



20110208:030000:uv-3_equip:%DBG128|780| |Found queued event or query message(18) 'SECS_S6F11' for uv-3_equip
20110208:030000:uv-3_equip:%DBG128|780| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 27
20110208:030000:uv-3_equip:%DBG128|780| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c2, isReply=0, isSECS=1
20110208:030000:uv-3_equip:%DBG128|780| |proto: outgoing reply depth++ = 28
20110208:030000:uv-3_equip:%DBG002|780| |diag : Handling msg interrupt
20110208:030000:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:030000:uv-3_equip:%DBG002|781| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:030000:uv-3_equip:%DBG002|781| |diag : return STATUS{}  = { "$ACK" }
20110208:030000:uv-3_equip:%DBG002|781| |...checking STATUS{}  = { "$ACK" }
20110208:030000:uv-3_equip:%DBG002|782| |diag : Handling msg call for 'SECS_S6F11'
20110208:030000:uv-3_equip:%DBG128|782| |diag : Storing outgoing reply for SECS_S6F11 at depth 27
20110208:030000:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:030000:uv-3_equip:list 'S6F11' = {
20110208:030000:uv-3_equip:  ulong 'UL_0'[1] = {4417},
20110208:030000:uv-3_equip:  ulong 'UL_1'[1] = {65035},
20110208:030000:uv-3_equip:  list 'L_2' = {}
20110208:030000:uv-3_equip:  
20110208:030000:uv-3_equip:};
20110208:030000:uv-3_equip:%DBG002|785| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:030000:uv-3_equip:Executing local definition for S6F11
20110208:030000:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030000:uv-3_equip:%DBG002|789| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030000:uv-3_equip:%DBG128|789| |diag : Looking for outgoing reply at depth 27
20110208:030000:uv-3_equip:%DBG128|789| |diag : outgoing reply depth-- = 27
20110208:030000:uv-3_equip:%DBG128|789| |diag : Looking for outgoing secs reply S6F12 at depth 27
20110208:030000:uv-3_equip:%DBG128|789| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030000:uv-3_equip:%DBG128|789| |Setting/Restoring TID=0x14c2, SID=0x00
20110208:030000:uv-3_equip:%DBG128|790| |Sending S6F12 (device 1) (TID=0x14c2) (SID=0x00)
20110208:030000:uv-3_equip:%DBG128|790| |->ENQ
20110208:030000:uv-3_equip:%DBG128|896| |<-ENQ(EOT)
20110208:030000:uv-3_equip:%DBG128|896| |ENQ contention. Relinquishing...
20110208:030000:uv-3_equip:%DBG128|896| |->EOT
20110208:030001:uv-3_equip:%DBG128|008| |<-63 received (63/63 bytes)
20110208:030001:uv-3_equip:%DBG128|008| |<-BLK [3c 80 01 86 0b 80 01 00 00 14 c3 01 03 b1 04 00 00 11 42 b1 04 00 00 01 10 01 01 01 02 b1 04 00 00 00 02 01 02 41 08 42 30 35 33 36 31 2e 31 41 0c 52 45 42 41 4b 45 20 41 2e 72 63 70 0a af ]
20110208:030001:uv-3_equip:%DBG128|008| |->ACK
20110208:030001:uv-3_equip:%DBG128|008| |Received S6F11 (Device 1) (TID=0x14c3) (SID=0x00) 
20110208:030001:uv-3_equip:%DBG128|008| |Saving TID=0x14c3, SID=0x00
20110208:030001:uv-3_equip:%DBG128|008| |proto: outgoing reply depth++ = 28
20110208:030001:uv-3_equip:%DBG128|008| |Found queued event or query message(19) 'SECS_S6F11' for uv-3_equip
20110208:030001:uv-3_equip:%DBG128|008| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:030001:uv-3_equip:%DBG128|008| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c3, isReply=0, isSECS=1
20110208:030001:uv-3_equip:%DBG128|008| |proto: outgoing reply depth++ = 29
20110208:030001:uv-3_equip:%DBG002|008| |diag : Handling msg interrupt
20110208:030001:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:030001:uv-3_equip:%DBG002|010| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:030001:uv-3_equip:%DBG002|010| |diag : return STATUS{}  = { "$ACK" }
20110208:030001:uv-3_equip:%DBG002|010| |...checking STATUS{}  = { "$ACK" }
20110208:030001:uv-3_equip:%DBG002|010| |diag : Handling msg call for 'SECS_S6F11'
20110208:030001:uv-3_equip:%DBG128|010| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:030001:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:030001:uv-3_equip:list 'S6F11' = {
20110208:030001:uv-3_equip:  ulong 'UL_0'[1] = {4418},
20110208:030001:uv-3_equip:  ulong 'UL_1'[1] = {272},
20110208:030001:uv-3_equip:  list 'L_2' = {
20110208:030001:uv-3_equip:    list 'L_0' = {
20110208:030001:uv-3_equip:      ulong 'UL_0'[1] = {2},
20110208:030001:uv-3_equip:      list 'L_1' = {
20110208:030001:uv-3_equip:        char 'A_0'[8] = {"B05361.1"},
20110208:030001:uv-3_equip:        char 'A_1'[12] = {"REBAKE A.rcp"}
20110208:030001:uv-3_equip:        
20110208:030001:uv-3_equip:      }
20110208:030001:uv-3_equip:      
20110208:030001:uv-3_equip:    }
20110208:030001:uv-3_equip:    
20110208:030001:uv-3_equip:  }
20110208:030001:uv-3_equip:  
20110208:030001:uv-3_equip:};
20110208:030001:uv-3_equip:%DBG002|015| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:030001:uv-3_equip:Executing local definition for S6F11
20110208:030001:uv-3_equip:Sending S2F41, TID=00000133
20110208:030001:uv-3_equip:%DBG128|019| |Setting/Restoring TID=0x133, SID=0x00
20110208:030001:uv-3_equip:%DBG128|019| |Sending S2F41 (device 1) (TID=0x133) (SID=0x00)
20110208:030001:uv-3_equip:%DBG128|019| |->ENQ
20110208:030001:uv-3_equip:%DBG128|297| |<-EOT
20110208:030001:uv-3_equip:%DBG128|297| |->BLK [2b 00 01 82 29 80 01 00 00 01 33 01 02 41 05 53 54 41 52 54 01 01 01 02 41 07 50 4f 52 54 20 49 44 41 09 4c 45 46 54 20 50 4f 52 54 08 51 ]
20110208:030001:uv-3_equip:%DBG128|488| |<-ACK
20110208:030001:uv-3_equip:%DBG128|488| |<-ENQ(ACK)
20110208:030001:uv-3_equip:%DBG128|488| |ENQ contention. Relinquishing...
20110208:030001:uv-3_equip:%DBG128|488| |->EOT
20110208:030001:uv-3_equip:%DBG128|585| |<-20 received (20/20 bytes)
20110208:030001:uv-3_equip:%DBG128|586| |<-BLK [11 80 01 02 2a 80 01 00 00 01 33 01 02 21 01 04 01 00 01 8c ]
20110208:030001:uv-3_equip:%DBG128|586| |->ACK
20110208:030001:uv-3_equip:%DBG128|586| |Received S2F42 (Device 1) (TID=0x133) (SID=0x00) 
20110208:030001:uv-3_equip:%DBG128|586| |diag : incoming reply depth++ = 1
20110208:030001:uv-3_equip:%DBG128|586| |diag : Setting expected reply for sender:method:tid 1#secs:S2F42:00000133 at incoming depth 0
20110208:030001:uv-3_equip:%DBG128|586| |diag : Setting SECS reply S2F42 at incoming depth 0
20110208:030001:uv-3_equip:...waiting for S2F42 reply from device '1', timeout in 30 seconds
20110208:030001:uv-3_equip:%DBG002|586| |uv-3_equip returning to QUERY state
20110208:030001:uv-3_equip:%DBG128|586| |Found queued reply message(20) 'S2F42' for uv-3_equip
20110208:030001:uv-3_equip:%DBG128|586| |diag : Target=uv-3_equip, Sender=1#secs, Method=S2F42, TID=00000133, isReply=1, isSECS=1
20110208:030001:uv-3_equip:%DBG128|586| |diag : Matching 1#secs=1#secs, S2F42=S2F42, 00000133=00000133
20110208:030001:uv-3_equip:Matched TID=00000133, Method=S2F42, Depth=0, Sender=1#secs
20110208:030001:uv-3_equip:%DBG128|586| |diag : Matched current reply S2F42 at incoming depth 0
20110208:030001:uv-3_equip:%DBG128|586| |diag : Matched 1#secs=1#secs, S2F42=S2F42, 00000133=00000133
20110208:030001:uv-3_equip:%DBG128|586| |diag : Matched incoming reply at depth 0
20110208:030001:uv-3_equip:%DBG128|586| |diag : Consuming reply message at incoming depth 0
20110208:030001:uv-3_equip:%DBG128|586| |proto: incoming reply depth-- = 0
20110208:030001:uv-3_equip:%DBG002|587| |diag : return SECS_QUERY{} (1) = { "$ACK" }
20110208:030001:uv-3_equip:list 'S2F42' = {
20110208:030001:uv-3_equip:  binary 'BN_0'[1] = {0x04},
20110208:030001:uv-3_equip:  list 'L_1' = {}
20110208:030001:uv-3_equip:  
20110208:030001:uv-3_equip:};
20110208:030001:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030001:uv-3_equip:%DBG002|588| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030001:uv-3_equip:%DBG128|589| |diag : Looking for outgoing reply at depth 28
20110208:030001:uv-3_equip:%DBG128|589| |diag : outgoing reply depth-- = 28
20110208:030001:uv-3_equip:%DBG128|589| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:030001:uv-3_equip:%DBG128|589| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030001:uv-3_equip:%DBG128|589| |Setting/Restoring TID=0x14c3, SID=0x00
20110208:030001:uv-3_equip:%DBG128|589| |Sending S6F12 (device 1) (TID=0x14c3) (SID=0x00)
20110208:030001:uv-3_equip:%DBG128|589| |->ENQ
20110208:030001:uv-3_equip:%DBG128|854| |<-EOT
20110208:030001:uv-3_equip:%DBG128|854| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c3 21 01 00 01 8d ]
20110208:030001:uv-3_equip:%DBG128|972| |<-ACK
20110208:030001:uv-3_equip:%DBG002|972| |uv-3_equip returning to IDLE state

SMOKING GUN - why isn't the S6F12 being send now for the first S6F11 ?



20110208:030002:uv-3_equip:%DBG128|666| |<-ENQ
20110208:030002:uv-3_equip:%DBG128|666| |->EOT
20110208:030002:uv-3_equip:%DBG128|797| |<-29 received (29/29 bytes)
20110208:030002:uv-3_equip:%DBG128|797| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 c4 01 03 b1 04 00 00 11 43 b1 04 00 00 01 08 01 00 04 37 ]
20110208:030002:uv-3_equip:%DBG128|797| |->ACK
20110208:030002:uv-3_equip:%DBG128|797| |Received S6F11 (Device 1) (TID=0x14c4) (SID=0x00) 
20110208:030002:uv-3_equip:%DBG128|797| |Saving TID=0x14c4, SID=0x00




20110208:030002:uv-3_equip:%DBG002|797| |uv-3_equip returning to IDLE state




20110208:030002:uv-3_equip:%DBG128|797| |Found queued event or query message(21) 'SECS_S6F11' for uv-3_equip
20110208:030002:uv-3_equip:%DBG128|797| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 28
20110208:030002:uv-3_equip:%DBG128|797| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c4, isReply=0, isSECS=1
20110208:030002:uv-3_equip:%DBG128|798| |proto: outgoing reply depth++ = 29
20110208:030002:uv-3_equip:%DBG002|798| |diag : Handling msg interrupt
20110208:030002:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:030002:uv-3_equip:%DBG002|799| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:030002:uv-3_equip:%DBG002|799| |diag : return STATUS{}  = { "$ACK" }
20110208:030002:uv-3_equip:%DBG002|799| |...checking STATUS{}  = { "$ACK" }
20110208:030002:uv-3_equip:%DBG002|799| |diag : Handling msg call for 'SECS_S6F11'
20110208:030002:uv-3_equip:%DBG128|799| |diag : Storing outgoing reply for SECS_S6F11 at depth 28
20110208:030002:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:030002:uv-3_equip:list 'S6F11' = {
20110208:030002:uv-3_equip:  ulong 'UL_0'[1] = {4419},
20110208:030002:uv-3_equip:  ulong 'UL_1'[1] = {264},
20110208:030002:uv-3_equip:  list 'L_2' = {}
20110208:030002:uv-3_equip:  
20110208:030002:uv-3_equip:};
20110208:030002:uv-3_equip:%DBG002|803| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:030002:uv-3_equip:Executing local definition for S6F11
20110208:030002:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030002:uv-3_equip:%DBG002|806| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030002:uv-3_equip:%DBG128|807| |diag : Looking for outgoing reply at depth 28
20110208:030002:uv-3_equip:%DBG128|807| |diag : outgoing reply depth-- = 28
20110208:030002:uv-3_equip:%DBG128|807| |diag : Looking for outgoing secs reply S6F12 at depth 28
20110208:030002:uv-3_equip:%DBG128|807| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030002:uv-3_equip:%DBG128|807| |Setting/Restoring TID=0x14c4, SID=0x00
20110208:030002:uv-3_equip:%DBG128|807| |Sending S6F12 (device 1) (TID=0x14c4) (SID=0x00)
20110208:030002:uv-3_equip:%DBG128|807| |->ENQ
20110208:030002:uv-3_equip:%DBG128|888| |<-ENQ(EOT)
20110208:030002:uv-3_equip:%DBG128|888| |ENQ contention. Relinquishing...
20110208:030002:uv-3_equip:%DBG128|888| |->EOT
20110208:030003:uv-3_equip:%DBG128|000| |<-45 received (45/45 bytes)
20110208:030003:uv-3_equip:%DBG128|000| |<-BLK [2a 80 01 86 0b 80 01 00 00 14 c5 01 03 b1 04 00 00 11 44 b1 04 00 00 fe 0a 01 01 01 02 b1 04 00 00 00 04 01 02 a5 01 04 a5 01 04 07 4c ]
20110208:030003:uv-3_equip:%DBG128|000| |->ACK
20110208:030003:uv-3_equip:%DBG128|000| |Received S6F11 (Device 1) (TID=0x14c5) (SID=0x00) 
20110208:030003:uv-3_equip:%DBG128|000| |Saving TID=0x14c5, SID=0x00
20110208:030003:uv-3_equip:%DBG128|000| |proto: outgoing reply depth++ = 29
20110208:030003:uv-3_equip:%DBG128|000| |Found queued event or query message(22) 'SECS_S6F11' for uv-3_equip
20110208:030003:uv-3_equip:%DBG128|000| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 29
20110208:030003:uv-3_equip:%DBG128|000| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c5, isReply=0, isSECS=1
20110208:030003:uv-3_equip:%DBG128|000| |proto: outgoing reply depth++ = 30
20110208:030003:uv-3_equip:%DBG002|000| |diag : Handling msg interrupt
20110208:030003:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:030003:uv-3_equip:%DBG002|001| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|001| |diag : return STATUS{}  = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|001| |...checking STATUS{}  = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|001| |diag : Handling msg call for 'SECS_S6F11'
20110208:030003:uv-3_equip:%DBG128|001| |diag : Storing outgoing reply for SECS_S6F11 at depth 29
20110208:030003:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:030003:uv-3_equip:list 'S6F11' = {
20110208:030003:uv-3_equip:  ulong 'UL_0'[1] = {4420},
20110208:030003:uv-3_equip:  ulong 'UL_1'[1] = {65034},
20110208:030003:uv-3_equip:  list 'L_2' = {
20110208:030003:uv-3_equip:    list 'L_0' = {
20110208:030003:uv-3_equip:      ulong 'UL_0'[1] = {4},
20110208:030003:uv-3_equip:      list 'L_1' = {
20110208:030003:uv-3_equip:        ubyte 'UB_0'[1] = {4},
20110208:030003:uv-3_equip:        ubyte 'UB_1'[1] = {4}
20110208:030003:uv-3_equip:        
20110208:030003:uv-3_equip:      }
20110208:030003:uv-3_equip:      
20110208:030003:uv-3_equip:    }
20110208:030003:uv-3_equip:    
20110208:030003:uv-3_equip:  }
20110208:030003:uv-3_equip:  
20110208:030003:uv-3_equip:};
20110208:030003:uv-3_equip:%DBG002|005| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:030003:uv-3_equip:Executing local definition for S6F11
20110208:030003:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030003:uv-3_equip:%DBG002|009| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030003:uv-3_equip:%DBG128|009| |diag : Looking for outgoing reply at depth 29
20110208:030003:uv-3_equip:%DBG128|009| |diag : outgoing reply depth-- = 29
20110208:030003:uv-3_equip:%DBG128|010| |diag : Looking for outgoing secs reply S6F12 at depth 29
20110208:030003:uv-3_equip:%DBG128|010| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030003:uv-3_equip:%DBG128|010| |Setting/Restoring TID=0x14c5, SID=0x00
20110208:030003:uv-3_equip:%DBG128|010| |Sending S6F12 (device 1) (TID=0x14c5) (SID=0x00)
20110208:030003:uv-3_equip:%DBG128|010| |->ENQ
20110208:030003:uv-3_equip:%DBG128|098| |<-ENQ(EOT)
20110208:030003:uv-3_equip:%DBG128|099| |ENQ contention. Relinquishing...
20110208:030003:uv-3_equip:%DBG128|099| |->EOT
20110208:030003:uv-3_equip:%DBG128|198| |<-29 received (29/29 bytes)
20110208:030003:uv-3_equip:%DBG128|198| |<-BLK [1a 80 01 86 0b 80 01 00 00 14 c6 01 03 b1 04 00 00 11 45 b1 04 00 00 01 86 01 00 04 b9 ]
20110208:030003:uv-3_equip:%DBG128|198| |->ACK
20110208:030003:uv-3_equip:%DBG128|198| |Received S6F11 (Device 1) (TID=0x14c6) (SID=0x00) 
20110208:030003:uv-3_equip:%DBG128|198| |Saving TID=0x14c6, SID=0x00
20110208:030003:uv-3_equip:%DBG128|198| |proto: outgoing reply depth++ = 30
20110208:030003:uv-3_equip:%DBG128|198| |Found queued event or query message(23) 'SECS_S6F11' for uv-3_equip
20110208:030003:uv-3_equip:%DBG128|199| |Created default secs reply variable 'S6F12<>  = < 0x00 >' at outgoing depth 30
20110208:030003:uv-3_equip:%DBG128|199| |diag : Target=uv-3_equip, Sender=1#secs, Method=SECS_S6F11, TID=000014c6, isReply=0, isSECS=1
20110208:030003:uv-3_equip:%DBG128|199| |proto: outgoing reply depth++ = 31
20110208:030003:uv-3_equip:%DBG002|199| |diag : Handling msg interrupt
20110208:030003:uv-3_equip:Servicing incoming message from 1#secs requesting method SECS_S6F11
20110208:030003:uv-3_equip:%DBG002|199| |diag : return MESSAGE_HANDLER{} (1) = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|200| |diag : return STATUS{}  = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|200| |...checking STATUS{}  = { "$ACK" }
20110208:030003:uv-3_equip:%DBG002|200| |diag : Handling msg call for 'SECS_S6F11'
20110208:030003:uv-3_equip:%DBG128|200| |diag : Storing outgoing reply for SECS_S6F11 at depth 30
20110208:030003:uv-3_equip:Received S6F11 from 1, TID=1, SID=0
20110208:030003:uv-3_equip:list 'S6F11' = {
20110208:030003:uv-3_equip:  ulong 'UL_0'[1] = {4421},
20110208:030003:uv-3_equip:  ulong 'UL_1'[1] = {390},
20110208:030003:uv-3_equip:  list 'L_2' = {}
20110208:030003:uv-3_equip:  
20110208:030003:uv-3_equip:};
20110208:030003:uv-3_equip:%DBG002|203| |diag : return IS_SECS_ID<> (1) = < 1 >
20110208:030003:uv-3_equip:Executing local definition for S6F11
20110208:030003:uv-3_equip:binary 'S6F12' = {0x00};
20110208:030003:uv-3_equip:%DBG002|206| |diag : return SECS_PROCESS{} (1) = { "$ACK" }
20110208:030003:uv-3_equip:%DBG128|206| |diag : Looking for outgoing reply at depth 30
20110208:030003:uv-3_equip:%DBG128|206| |diag : outgoing reply depth-- = 30
20110208:030003:uv-3_equip:%DBG128|206| |diag : Looking for outgoing secs reply S6F12 at depth 30
20110208:030003:uv-3_equip:%DBG128|206| |Method data is SECS_S6F11{}  = { args , S6F11 }
20110208:030003:uv-3_equip:%DBG128|206| |Setting/Restoring TID=0x14c6, SID=0x00
20110208:030003:uv-3_equip:%DBG128|206| |Sending S6F12 (device 1) (TID=0x14c6) (SID=0x00)
20110208:030003:uv-3_equip:%DBG128|206| |->ENQ
20110208:030003:uv-3_equip:%DBG128|386| |<-EOT
20110208:030003:uv-3_equip:%DBG128|386| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c6 21 01 00 01 90 ]
20110208:030003:uv-3_equip:%DBG128|497| |<-ACK
20110208:030003:uv-3_equip:%DBG128|497| |diag : outgoing reply depth-- = 29
20110208:030003:uv-3_equip:%DBG128|497| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:030003:uv-3_equip:Re-sending interrupted S6F12 reply message
20110208:030003:uv-3_equip:%DBG128|497| |Setting/Restoring TID=0x14c5, SID=0x00
20110208:030003:uv-3_equip:%DBG128|497| |Sending S6F12 (device 1) (TID=0x14c5) (SID=0x00)
20110208:030003:uv-3_equip:%DBG128|497| |->ENQ
20110208:030003:uv-3_equip:%DBG128|587| |<-EOT
20110208:030003:uv-3_equip:%DBG128|587| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c5 21 01 00 01 8f ]
20110208:030003:uv-3_equip:%DBG128|776| |<-ACK
20110208:030003:uv-3_equip:%DBG128|776| |diag : outgoing reply depth-- = 28
20110208:030003:uv-3_equip:%DBG128|776| |Checking re-send STATUS{}  = { "$ACKNOWLEDGE" }
20110208:030003:uv-3_equip:Re-sending interrupted S6F12 reply message
20110208:030003:uv-3_equip:%DBG128|776| |Setting/Restoring TID=0x14c4, SID=0x00
20110208:030003:uv-3_equip:%DBG128|776| |Sending S6F12 (device 1) (TID=0x14c4) (SID=0x00)
20110208:030003:uv-3_equip:%DBG128|776| |->ENQ
20110208:030003:uv-3_equip:%DBG128|878| |<-EOT
20110208:030003:uv-3_equip:%DBG128|878| |->BLK [0d 00 01 06 0c 80 01 00 00 14 c4 21 01 00 01 8e ]
20110208:030004:uv-3_equip:%DBG128|066| |<-ACK
20110208:030004:uv-3_equip:%DBG002|066| |uv-3_equip returning to IDLE state














