/*
  https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command/
  https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development/
  https://docs.rakwireless.com/RUI3/LoRaWAN
  

  https://manuals.plus/lacuna/ls200-sensor-and-relay-manual#axzz8IXdHpl1t

  https://github.com/XuanMinh201/RF210
  https://github.com/RFThings/rfthings-stm32l4
  https://github.com/FabienFerrero/SUCA

  https://github.com/RAKWireless/RAK-APOLLO3-RUI/

  https://github.com/stevemarple/MicroNMEA -> GNSS LC76F
  https://github.com/automote/LTR303

Digital humidity and temperature sensor - SHTC3
Tri-Axis Accelerometer - KX023-1025
Ambient Light Sensor - LTR-303ALS-01
GNSS LC76F

AT for RAK3172_v1.0.4_Boot+App_20220218.hex
Default baudrate 9200
  AT?
  AT+<CMD>?        : Help on <CMD>
  AT+<CMD>         : Run <CMD>
  AT+<CMD>=<value> : Set the value
  AT+<CMD>=?       : Get the value
  ATZ:          Triggers a reset of the MCU
  ATR:          Restore default parameters
  ATE:          Set command echo
  AT+DEVEUI:    Get or set the device EUI
  AT+APPEUI:    Get or set the application EUI
  AT+APPKEY:    Get or set the application key
  AT+DEVADDR:   Get or set the device address
  AT+NWKSKEY:   Get or set the network session key
  AT+APPSKEY:   Get or set the application session key
  AT+CFM:       Get or set the confirm mode (0:unconfirmed, 1:confirmed)
  AT+CFS:       Get or set the confirmation status of the last AT+SEND(0-1)
  AT+JOIN:      Join network
  AT+NJM:       Get or set the network join mode (0:ABP, 1:OTAA)
  AT+NJS:       Get the join status
  AT+RECV:      Print the last received data in hex format
  AT+SEND:      Send data along with the application port
  AT+ADR:       Get or set the adaptive data rate setting (0:disable, 1:enable)
  AT+CLASS:     Get or set the device class (A, B, C)
  AT+DCS:       Get or set the ETSI duty cycle setting (0:disable, 1:enable)
  AT+DUTYTIME:  Get the duty cycle time
  AT+DR:        Get or set the data rate (0-7 corresponding to DR_X)
  AT+PNM:       Get or set the public network mode (0=off,1=on)
  AT+JN1DL:     Get or set the join accept delay between the end of the Tx and the join Rx window 1 in ms
  AT+JN2DL:     Get or set the join accept delay between the end of the Tx and the join Rx window 2 in ms
  AT+RX1DL:     Get or set the delay between the end of the Tx and the Rx Window 1 in ms
  AT+RX2DL:     Get or set the delay between the end of the Tx and the Rx window 2 in ms
  AT+RX2FQ:     Get or set the Rx2 window Frequency
  AT+RX2DR:     Get or set the Rx2 window data rate (0-7) corresponding to DR_X
  AT+TXP:       Get or set the transmit power
  AT+RETY:      Set the number of retransmissions of Confirm packet data
  AT+MASK:      Set the channel mask, close or open the channel. Only for US915,AU915,CN470
  AT+BAND:      Set number corresponding to active regions (0: EU433 1: CN470 2: RU864 3: IN865 4: EU868 5: US915 6: AU915 7: KR920 8: AS923)
  AT+LPSEND:    Send long packet data (Maximum is 1024 bytes)
  AT+LINKCHECK: Verify Network Link Status
  AT+TIMEREQ:   Request the current date and time
  AT+USEND:     Unified send data along with the application port
  AT+PGSLOT:    PS:periodicity
  AT+BFREQ:     Get the Beacon frequency
  AT+LTIME:     Get the local time in UTC format
  AT+RSSI:      Get the RSSI of the last received packet
  // AT+SNR:       Get the SNR of the last received packet
  AT+VER:       Get the version of the firmware
  AT+SN:        Read the device serial number
  AT+TRSSI:     Starts RF RSSI tone test
  AT+TTONE:     Starts RF Tone test
  AT+TTX:       Set number of packets sent with RF LoRa test
  AT+TRX:       Set number of packets received with RF LoRa test
  AT+TCONF:     Configure LoRa RF test
  AT+TTHX:      Starts RF Tx hopping test from Fstart to Fstop randomly, with Fdelta steps
  AT+TTH:       Starts RF Tx hopping test from Fstart to Fstop, with Fdelta steps
  AT+TOFF:      Stops ongoing RF test
  AT+CERTIF:    Set the module in LoraWAN Certification mode
  AT+CW:        Send continuous wave
  AT+NWM:       Get or set the network work NWM (0:P2P, 1:LoRaWAN)
  AT+PFREQ:     Configure P2P frequency
  AT+PSF:       Configure P2P SpreadingFactor (6,7,8,9,10,11,12)
  AT+PBW:       Configure P2P Bandwidth(125, 250, 500)
  AT+PCR:       Configure P2P code rate(0:4/5, 1:4/6, 2:4/7, 3:4/8)
  AT+PPL:       Configure P2P Preamble Length (2-65535)
  AT+PTP:       Configure P2P Power (5-22)
  AT+P2P:       Configure P2P all parameters  // 868000000:12:125:0:8:15
                AT+P2P=868000000:12:125:0:8:15
  AT+PSEND:     P2P send data
  AT+PRECV:     Set the timeout period for P2P window reception (0-65535) ms
  AT+ADDMULC:   Add a new multicast group
  AT+RMVMULC:   Delete multicast group
  AT+LSTMULC:   View multicast group information
  ATD:          Enter data transparent transmission mode
  AT+BAUD:      Get or set the the uart baud rate(4800,9600,115200)
  OK


  https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/
  https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Quickstart/#rak3172-as-a-stand-alone-device-using-rui3
  https://docs.rakwireless.com/Knowledge-Hub/Learn/AT-Migration-Guide/

  Lasted firmware         https://downloads.rakwireless.com/LoRa/RAK3172/
  Firmware Upload tool    https://downloads.rakwireless.com/LoRa/Tools/RAK_Device_Firmware_Upgrade_tool/

  1. Download lested firmware, upgrade to RUI3, ex Version: RUI_4.0.6_RAK3172-T
  2. Use firmware upload tool to upload
  3. Use STM32CubeProgrammer to upload Arduino HEX file.
      Adress: 0x08006000

AT+<CMD>?: help on <CMD>
AT+<CMD>: run <CMD>
AT+<CMD>=<value>: set the value
AT+<CMD>=?: get the value

AT+BOOT,		        R: enter bootloader mode for firmware upgrade
AT+BOOTVER,		      R: get the version of RUI Bootloader
ATZ,			          R: triggers a reset of the MCU
ATR,			          R: restore default parameters
AT+DEBUG,		        RW: set debug log
ATE,			          R: show or hide the AT command input (setting is save & not change after reset)
AT+SN,			        R: get the serial number of the device (max 18 char)
AT+BAT,			        R: get the battery level (volt)
AT+BUILDTIME,	      R: get the build time of the firmware
AT+REPOINFO,	      R: get the commit ID of the firmware
AT+VER,			        R: get the version of the firmware
AT+FIRMWAREVER,	    R: get the Customized version of the firmware
AT+CLIVER,		      R: get the version of the AT command
AT+APIVER,		      R: get the version of the RUI API
AT+HWMODEL,		      R: get the string of the hardware model
AT+HWID,		        R: get the string of the hardware id
AT+ALIAS,		        RW: add an alias name to the device
AT+SYSV,		        R: get the System Voltage
AT+SLEEP,		        W: enter sleep mode for a period of time (ms)
AT+LPM,			        RW: get or set the low power mode (0 = off, 1 = on)
AT+LPMLVL,		      RW: get or set the low power mode level
AT+LOCK,		        R: lock the serial port
AT+PWORD,		        RW: set the serial port locking password (max 8 char)
AT+BAUD,		        RW: get or set the serial port baudrate
AT+ATM,			        R: switch to AT command mode
AT+APPEUI,		      RW: get or set the application EUI (8 bytes in hex)
AT+APPKEY,		      RW: get or set the application key (16 bytes in hex)
AT+APPSKEY,		      RW: get or set the application session key (16 bytes in hex)
AT+DEVADDR,		      RW: get or set the device address (4 bytes in hex)
AT+DEVEUI,		      RW: get or set the device EUI (8 bytes in hex)
AT+NETID,		        RW: get the network identifier (NetID) (3 bytes in hex)
AT+NWKSKEY,		      RW: get or set the network session key (16 bytes in hex)
AT+MCROOTKEY,	      RW: get the Mc Root key (16 bytes in hex)
AT+CFM,			        RW: get or set the confirm mode (0 = off, 1 = on)
AT+CFS,			        R: get the confirmation status of the last AT+SEND (0 = failure, 1 = success)
AT+JOIN,		        RW: join network
AT+NJM,			        RW: get or set the network join mode (0 = ABP, 1 = OTAA)
AT+NJS,			        R: get the join status (0 = not joined, 1 = joined)
AT+RECV,		        R: print the last received data in hex format
AT+SEND,		        W: send data along with the application port
AT+LPSEND,		      W: send long packet data (max 1000 bytes)
AT+RETY,		        RW: get or set the number of retransmission of confirm packet data
AT+ADR,			        RW: get or set the adaptive data rate setting (0 = off, 1 = on)
AT+CLASS,		        RW: get or set the device class (A = class A, B = class B, C = class C)
AT+DCS,			        RW: get the ETSI duty cycle setting (0 = disabled, 1 = enabled)
AT+DR,			        RW: get or set the data rate
AT+JN1DL,		        RW: get or set the join accept delay between the end of TX and the join RX window 1 in seconds
AT+JN2DL,		        RW: get or set the join accept delay between the end of TX and the join RX window 2 in seconds
AT+PNM,			        RW: get or set the public network mode (0 = off, 1 = on)
AT+RX1DL,		        RW: get or set the delay between the end of TX and the RX window 1 in seconds (1-15)
AT+RX2DL,		        RW: get or set the delay between the end of TX and the RX window 2 in seconds (2-16)
AT+RX2DR,		        RW: get or set the RX2 window data rate
AT+RX2FQ,		        RW: get the RX2 window frequency (Hz)
AT+TXP,			        RW: get or set the transmitting power
AT+LINKCHECK,	      RW: get or set the link check setting (0 = disabled, 1 = once, 2 = everytime)
AT+TIMEREQ,		      RW: request the current date and time (0 = disabled, 1 = enabled)
AT+LBT,			        RW: get or set the LoRaWAN LBT (support Korea Japan)
AT+LBTRSSI,		      RW: get or set the LoRaWAN LBT rssi (support Korea Japan)
AT+LBTSCANTIME,	    RW: get or set the LoRaWAN LBT scantime (support Korea Japan)
AT+PGSLOT,		      RW: get or set the unicast ping slot periodicity (0-7)
AT+BFREQ,		        R: get the data rate and beacon frequency (MHz)
AT+BTIME,		        R: get the beacon time (seconds since GPS Epoch time)
AT+BGW,			        R: get the gateway GPS coordinate, NetID and GwID
AT+LTIME,		        R: get the local time
AT+RSSI,		        R: get the RSSI of the last received packet
AT+ARSSI,		        R: access all open channel RSSI
AT+SNR,			        R: get the SNR of the last received packet
AT+MASK,		        RW: get or set the channel mask to close or open the channel (only for US915, AU915, CN470)
AT+CHE,			        RW: get or set eight channels mode (only for US915 AU915 CN470)
AT+CHS,			        RW: get or set single channels mode (only for US915 AU915 CN470)
AT+BAND,		        RW: get or set the active region (0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915, 6 = AU915, 7 = KR920, 8 = AS923-1 , 9 = AS923-2 ,
                                              10 = AS923-3 , 11 = AS923-4)
AT+NWM,			        RW: get or set the network working mode (0 = P2P_LORA, 1 = LoRaWAN, 2 = P2P_FSK)
AT+PFREQ,		        RW: configure P2P Frequency (Note:This command will be deleted in the future)
AT+PSF,			        RW: configure P2P Spreading Factor (5-12)(Note:This command will be deleted in the future)
AT+PBW,			        RW: configure P2P Bandwidth(LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83, 7 = 31.25, 8 = 41.67, 9 = 62.5  FSK:4800-467000)
                                                (Note:This command will be deleted in the future)
AT+PCR,			        RW: configure P2P Code Rate(0=4/5, 1=4/6, 2=4/7, 3=4/8)(Note:This command will be deleted in the future)
AT+PPL,			        RW: configure P2P Preamble Length (5-65535)(Note:This command will be deleted in the future)
AT+PTP,			        RW: configure P2P TX Power(5-22)(Note:This command will be deleted in the future)
AT+PSEND,		        W: send data in P2P mode
AT+PRECV,		        RW: enter P2P RX mode for a period of time (ms)
AT+ENCRY,		        RW: get or set the encryption status of P2P mode
AT+CAD,			        RW: get or set the Channel Activity Detection status of P2P mode
AT+ENCKEY,		      RW: get or set the encryption key of P2P mode (16 bytes in hex)
AT+CRYPIV,		      RW: get or set the encryption IV of P2P mode (16 bytes in hex)
AT+P2P,			        RW: get or set all P2P parameters
AT+PBR,			        RW: get or set the P2P FSK modem bitrate (600-300000 b/s)
AT+PFDEV,		        RW: get or set the P2P FSK modem frequency deviation (600-200000 hz)
AT+IQINVER,		      RW: get or set P2P IQ inversion (1 = on, 0 = off)
AT+SYNCWORD,	      RW: get or set P2P syncword (0x0000 - 0xffff)
AT+RFFREQUENCY,	    RW: get or set P2P Frequency
AT+TXOUTPUTPOWER,   RW: get or set P2P Tx Power(5-22)
AT+BANDWIDTH,	      RW: get or set P2P Bandwidth(LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83, 7 = 31.25, 8 = 41.67, 9 = 62.5  FSK:4800-467000)
AT+SPREADINGFACTOR, RW: get or set P2P Spreading Factor (5-12)
AT+CODINGRATE,	    RW: get or set P2P Code Rate(0=4/5, 1=4/6, 2=4/7, 3=4/8)
AT+PREAMBLELENGTH,  RW: get or set P2P Preamble Length (5-65535)
AT+SYMBOLTIMEOUT,   RW: get or set P2P symbolTimeout (0-248)
AT+FIXLENGTHPAYLOAD,RW: get or set P2P fix length payload on/off ( 1 = on, 0 = off)
AT+ADDMULC,		      W: add a new multicast group
AT+RMVMULC,		      W: delete a multicast group
AT+LSTMULC,		      R: view multicast group information
AT+TRSSI,		        RW: start RF RSSI tone test
AT+TTONE,		        RW: start RF tone test
AT+TTX,			        RW: set number of packets to be sent for PER RF TX test
AT+TRX,			        RW: set number of packets to be received for PER RF RX test
AT+TCONF,		        RW: configure LoRa RF test
AT+TTH,			        RW: start RF TX hopping test from Fstart to Fstop, with Fdelta steps
AT+TRTH,		        RW: start RF TX hopping test from Fstart to Fstop, with Fdelta interval in random sequence
AT+TOFF,		        RW: stop ongoing RF test
AT+CERTIF,		      RW: set the module in LoraWAN certification mode (0 = normal mode, 1 = certification mode)
AT+CW,			        RW: start continuous wave

///////////////////////////////////////////////////////////////////////////////////////////////////

ATC+VER,		        R: VER, Return firmware version

ATC+SHT,		        R: SHT, Return the status of the SHTC3 sensor. 1 if available.
ATC+TEMP,		        R: TEMP, Return the temperature value with 0.01° resolution
ATC+HUM,		        R: HUM, Return the humidity value with 1 % resolution

ATC+KX023,		      R: KX023, Return the status of the KX023 sensor. 1 if available.
ATC+AX,			        R: AX, Return the value of X acceleration with 0.01G resolution
ATC+AY,             R: AY, Return the value of Y acceleration with 0.01G resolution
ATC+AZ,             R: AZ, Return the value of Z acceleration with 0.01G resolution

ATC+LTR,            R: LTR, Return the status of the LTR - 303 sensor. 1 if available.
ATC+LUMCH0,         R: LUMCH0, Return the CHANNEL0 value of the LTR - 303 sensor
ATC+LUMCH1,         R: LUMCH1, Return the CHANNEL1 value of the LTR - 303 sensor
ATC+LUM,            R: LUM, Return the CHANNEL1 value of the LTR - 303 sensor

ATC+GPS,            R: GPS, Return the status of the GNSS module. 1 if available.
ATC+GPSON,          W: GPSON, Return the GNSS module power | = 1 : GNSS ON | = 0 : GNSS OFF
ATC+GPSPWR,         W: GPSPWR, Return the GNSS module power | = 1 : GNSS ON | = 0 : GNSS OFF
ATC+GPSSAT,         R: GPSSAT, Return the number of available satellite(s)
ATC+GPSTIME,        R: GPSTIME, Return GPS time in EPOCH format
ATC+GPSLAT,         R: GPSLAT, Return latitude | Return 0 if coordinate is not available
ATC+GPSLON,         R: GPSLON, Return longtitude | Return 0 if coordinate is not available
ATC+GPSALT,         R: GPSALT, Return altitude | Return 0 if coordinate is not available
ATC+GPSNMEA,        W: GPSON, Activate the NMEA log from GNSS module | = 1 : NMEA ON | = 0 : NMEA OFF
ATC+GPSLOG,         W: GPSLOG, Activate the results from GNSS module every 5s | = 1 : LOG ON | = 0 : LOG OFF
ATC+GPSDC,          W: GPSDC, Set GNSS module in duty cycle mode with 30sec sleep | = 1 : DC ON | = 0 : DC OFF
ATC+GPSCONST,       R: GPSCONST, Activate Galileo, Beidu, GPS and Glonass constellations

ATC+BAT,            R: BAT, Return battery voltage in mV | Return 0 if not available
ATC+LDO,            R: LDO, Return LDO voltage in mV | Return 0 if not available
OK

///////////////////////////////////////////////////////////////////////////////////////////////////
08:07:22.024 -> WIFI -> Connected successfully
08:07:22.024 -> IP address: 192.168.1.11
08:07:26.783 -> Setup AT command...
08:07:30.356 -> -->ATE
08:07:38.370 -> OK
08:07:41.230 -> ATC+SHT
08:07:41.230 -> 
ATC+SHT=
08:07:41.230 -> 
ATC+SHT=1
08:07:41.230 -> OK
08:07:41.230 -> 1
08:07:41.230 -> OK
08:07:44.110 -> ATC+KX023
08:07:44.110 -> 
ATC+KX023=1
08:07:44.110 -> OK
08:07:48.989 -> ATC+GPSON=1
08:07:48.989 -> 
OK
08:08:00.092 -> 
08:08:00.852 -> Setup done!
08:08:00.852 -> Client ID: RFThings-1C99014E76A0

# SHT temperature and humidity sensor

1. Activate the SHT sensor, type :
---        
ATC+SHT
---            
It returns : 
    ATC+SHT=1
    OK
2. Check temperature, type :
    ATC+TEMP=?
It returns a result in Celsius degree: 
     ATC+TEMP=xx.yy degree C
     OK
3. Check Humidity, type : 
    ATC+HUM=?
It returns a result in percent of humidity :
    ATC+HUM=xx%
    OK
# KX023 Accelerometer sensor
1. Activate the KX023 sensor, type :
    ATC+KX023
It returns : 
    ATC+KX023=1
    OK
2. Check the accelerometer value on x-axis, type :
    ATC+AX=?
It returns the result in G unit : 
    ATC+AX=-x.yy
    OK
3. Check the accelerometer value on y-axis, type :
    ATC+AY=?
It returns the result in G unit : 
    ATC+AY=x.yy
    OK
4. Check the accelerometer value on z-axis, type :
    ATC+AZ=?
It returns the result in G unit : 
    ATC+AZ=x.yy
    OK
# L76F GNSS sensor
1. GNSS experiment needs to be run with a clear view on the sky to capture signal from GPS, Galileo or Beidu Satellite.
2. Activate the GNSS LDO, it power-on the L76F module, type :
    ATC+GPSON=1
It returns : 
    OK
3. The GNSS module will need up to 1 minute to obtain a position. If LED D1 (close to L76F) is blinking, the module get a fix.
4. Read the GPS value, type
    ATC+GPS
It returns : 
    ATC+GPS=1
    OK
5. Read the number of satellites in view, type : 
    ATC+GPSSAT=?
It returns the number of satellites available : 
    ATC+GPSSAT=x
    OK
6. Read the latitude, type : 
    ATC+GPSLAT=?
It returns the latitude in degreees : 
    ATC+GPSLAT=xx.yyyy
    OK
7. Read the Longitude, type : 
    ATC+GPSLON=?
It returns the longitude in degrees : 
    ATC+GPSLON=xx.yyyy
    OK
8. Read the Altitude, type : 
    ATC+GPSALT=?
It returns the altitude in meter : 
    ATC+GPSALT=xx.yyy
    OK
9. Read the Time, type : 
    ATC+GPSTIME=?
It returns the time in second in Epoch format : 
    ATC+GPSTIME=xxxxxxxxxx
    OK
# Battery Voltage
1. Request battery voltage level, type :
    ATC+BAT=?
It returns the battery voltage level in millivolt : 
    ATC+BAT=xxxx
    OK
*/
