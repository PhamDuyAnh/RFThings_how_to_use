# RFThings_how_to_use

### Reference links
_**RFThings RF210**_
-   [https://github.com/XuanMinh201/RF210](https://github.com/XuanMinh201/RF210)
-   [https://github.com/FabienFerrero/SUCA](https://github.com/FabienFerrero/SUCA)
-   [https://github.com/RFThings/rfthings-stm32l4](https://github.com/RFThings/rfthings-stm32l4)

_**Arduino IDE & ESP32 board tuto**_
-   [https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions)
-   [https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0](https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0)
-   [https://randomnerdtutorials.com/getting-started-with-esp32](https://randomnerdtutorials.com/getting-started-with-esp32)

_**Other**_
-   [https://github.com/NamNamIoT/RAK3172_CANOPUS](https://github.com/NamNamIoT/RAK3172_CANOPUS)
-   [https://github.com/RAKWireless/RAK-APOLLO3-RUI](https://github.com/RAKWireless/RAK-APOLLO3-RUI)
-   [https://news.rakwireless.com/stm32wl-custom-firmware-via-arduino-using-rak3172-module-for-lorawan](https://news.rakwireless.com/stm32wl-custom-firmware-via-arduino-using-rak3172-module-for-lorawan)
-   [https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Deprecated-AT-Command)
-   [https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development](https://docs.rakwireless.com/Product-Categories/WisDuo/RAK3172-Module/Low-Level-Development)
-   [https://manuals.plus/lacuna/ls200-sensor-and-relay-manual#axzz8IXdHpl1t](https://manuals.plus/lacuna/ls200-sensor-and-relay-manual#axzz8IXdHpl1t)
-   [https://github.com/stevemarple/MicroNMEA](https://github.com/stevemarple/MicroNMEA)
-   [https://github.com/automote/LTR303](https://github.com/automote/LTR303)

### About the board
![Top](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/top.jpg)

![Bot](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/bot.jpg)

![USB port](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/usb.jpg)

### Installing the Arduino IDE & ESP32 Board

1.  Download and install the Arduino IDE (at least version v1.6.8)
2.  Start the Arduino IDE
3.  Go into File/Preferences Add --> **Additional Board Manager URLs** --> **Add URL**

```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```

![Additional Board Manager URLs](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/ESP32-URL-Arduino-IDE.webp)

4.  Open the Boards Manager. Go to Tools > Board > Boards Manager… --> Search for ESP32 and press install button for the **ESP32 by Espressif Systems** & **Install**

![install](https://github.com/PhamDuyAnh/RFThings_how_to_use/blob/main/REF/installing.webp)

### First test: RF210_RAK3172_bridge
1.  Connect your RF210 board (microUSB port) to your PC USB port (normally Windows will automatically install the driver)
2.  Download sample sketch from Github --> [RF210_RAK3172_bridge](https://github.com/FabienFerrero/SUCA/tree/main/Examples/RF210_RAK3172_bridge)
3.  Unzip and open the file RF210_RAK3172_bridge.ino (note that the file name must be the same as the folder name)
4.  Open the Boards Manager from the **Tools** --> **Board** --> **esp32** --> **ESP32C3 Dev Module** and Select.
5.  Open the Boards Manager from the **Tools** --> **Port** and Select Port (check step 1)
6.  Open the Boards Manager from the **Tools** --> **USB-CDC on boot** --> **Enable**
7.  Hold the **B_ESP1** (boot) button and press **ESP_EN1** (reset) button and release the B_ESP1 (boot) button to enter bootmode.
8.  Upload Sketch (Ctrl+U)
9.  After upload sketch, press **ESP_EN1** to reboot
10.  Use Serial Monitor to monitor Serial and send CMD
11.  Enter **AT?** and press **Enter** and see

### AT command
```
AT+<CMD>?: help on <CMD>
AT+<CMD>: run <CMD>
AT+<CMD>=<value>: set the value
AT+<CMD>=?: get the value

AT+BOOT             R: enter bootloader mode for firmware upgrade
AT+BOOTVER          R: get the version of RUI Bootloader
ATZ                 R: triggers a reset of the MCU
ATR                 R: restore default parameters
AT+DEBUG            RW: set debug log
ATE                 R: show or hide the AT command input (setting is save & not change after reset)
AT+SN               R: get the serial number of the device (max 18 char)
AT+BAT              R: get the battery level (volt)
AT+BUILDTIME        R: get the build time of the firmware
AT+REPOINFO         R: get the commit ID of the firmware
AT+VER              R: get the version of the firmware
AT+FIRMWAREVER      R: get the Customized version of the firmware
AT+CLIVE            R: get the version of the AT command
AT+APIVER           R: get the version of the RUI API
AT+HWMODEL          R: get the string of the hardware model
AT+HWID             R: get the string of the hardware id
AT+ALIAS            RW: add an alias name to the device
AT+SYSV             R: get the System Voltage
AT+SLEEP            W: enter sleep mode for a period of time (ms)
AT+LPM              RW: get or set the low power mode (0 = off, 1 = on)
AT+LPMLVL           RW: get or set the low power mode level
AT+LOCK             R: lock the serial port
AT+PWORD            RW: set the serial port locking password (max 8 char)
AT+BAUD             RW: get or set the serial port baudrate
AT+ATM              R: switch to AT command mode
AT+APPEUI           RW: get or set the application EUI (8 bytes in hex)
AT+APPKEY           RW: get or set the application key (16 bytes in hex)
AT+APPSKEY          RW: get or set the application session key (16 bytes in hex)
AT+DEVADDR          RW: get or set the device address (4 bytes in hex)
AT+DEVEUI           RW: get or set the device EUI (8 bytes in hex)
AT+NETID            RW: get the network identifier (NetID) (3 bytes in hex)
AT+NWKSKEY          RW: get or set the network session key (16 bytes in hex)
AT+MCROOTKEY        RW: get the Mc Root key (16 bytes in hex)
AT+CFM              RW: get or set the confirm mode (0 = off, 1 = on)
AT+CFS              R: get the confirmation status of the last AT+SEND (0 = failure, 1 = success)
AT+JOIN             RW: join network
AT+NJM              W: get or set the network join mode (0 = ABP, 1 = OTAA)
AT+NJS              R: get the join status (0 = not joined, 1 = joined)
AT+RECV             R: print the last received data in hex format
AT+SEND             W: send data along with the application port
AT+LPSEND           W: send long packet data (max 1000 bytes)
AT+RETY             RW: get or set the number of retransmission of confirm packet data
AT+ADR              RW: get or set the adaptive data rate setting (0 = off, 1 = on)
AT+CLASS            RW: get or set the device class (A = class A, B = class B, C = class C)
AT+DCS              RW: get the ETSI duty cycle setting (0 = disabled, 1 = enabled)
AT+DR               RW: get or set the data rate
AT+JN1DL            RW: get or set the join accept delay between the end of TX and the join RX window 1 in seconds
AT+JN2DL            RW: get or set the join accept delay between the end of TX and the join RX window 2 in seconds
AT+PNM              RW: get or set the public network mode (0 = off, 1 = on)
AT+RX1DL            RW: get or set the delay between the end of TX and the RX window 1 in seconds (1-15)
AT+RX2DL            RW: get or set the delay between the end of TX and the RX window 2 in seconds (2-16)
AT+RX2DR            RW: get or set the RX2 window data rate
AT+RX2FQ            RW: get the RX2 window frequency (Hz)
AT+TXP              RW: get or set the transmitting power
AT+LINKCHECK        RW: get or set the link check setting (0 = disabled, 1 = once, 2 = everytime)
AT+TIMEREQ          RW: request the current date and time (0 = disabled, 1 = enabled)
AT+LBT              RW: get or set the LoRaWAN LBT (support Korea Japan)
AT+LBTRSSI          RW: get or set the LoRaWAN LBT rssi (support Korea Japan)
AT+LBTSCANTIME      RW: get or set the LoRaWAN LBT scantime (support Korea Japan)
AT+PGSLOT           RW: get or set the unicast ping slot periodicity (0-7)
AT+BFREQ            R: get the data rate and beacon frequency (MHz)
AT+BTIME            R: get the beacon time (seconds since GPS Epoch time)
AT+BGW              R: get the gateway GPS coordinate, NetID and GwID
AT+LTIME            R: get the local time
AT+RSSI             R: get the RSSI of the last received packet
AT+ARSSI            R: access all open channel RSSI
AT+SNR              R: get the SNR of the last received packet
AT+MASK             RW: get or set the channel mask to close or open the channel (only for US915, AU915, CN470)
AT+CHE              RW: get or set eight channels mode (only for US915 AU915 CN470)
AT+CHS              RW: get or set single channels mode (only for US915 AU915 CN470)
AT+BAND             RW: get or set the active region (0 = EU433, 1 = CN470, 2 = RU864, 3 = IN865, 4 = EU868, 5 = US915,
                        6 = AU915, 7 = KR920, 8 = AS923-1 , 9 = AS923-2 , 10 = AS923-3 , 11 = AS923-4)
AT+NWM              RW: get or set the network working mode (0 = P2P_LORA, 1 = LoRaWAN, 2 = P2P_FSK)
AT+PFREQ            RW: configure P2P Frequency (Note:This command will be deleted in the future)
AT+PSF              RW: configure P2P Spreading Factor (5-12)(Note:This command will be deleted in the future)
AT+PBW              RW: configure P2P Bandwidth(LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83,
                        7 = 31.25, 8 = 41.67, 9 = 62.5  FSK:4800-467000)
                        (Note:This command will be deleted in the future)
AT+PCR              RW: configure P2P Code Rate(0=4/5, 1=4/6, 2=4/7, 3=4/8)(Note:This command will be deleted in the future)
AT+PPL              RW: configure P2P Preamble Length (5-65535)(Note:This command will be deleted in the future)
AT+PTP              RW: configure P2P TX Power(5-22)(Note:This command will be deleted in the future)
AT+PSEND            W: send data in P2P mode
AT+PRECV            RW: enter P2P RX mode for a period of time (ms)
AT+ENCRY            RW: get or set the encryption status of P2P mode
AT+CAD              RW: get or set the Channel Activity Detection status of P2P mode
AT+ENCKEY           RW: get or set the encryption key of P2P mode (16 bytes in hex)
AT+CRYPIV           RW: get or set the encryption IV of P2P mode (16 bytes in hex)
AT+P2P              RW: get or set all P2P parameters
AT+PBR              RW: get or set the P2P FSK modem bitrate (600-300000 b/s)
AT+PFDEV            RW: get or set the P2P FSK modem frequency deviation (600-200000 hz)
AT+IQINVER          RW: get or set P2P IQ inversion (1 = on, 0 = off)
AT+SYNCWORD         RW: get or set P2P syncword (0x0000 - 0xffff)
AT+RFFREQUENCY      RW: get or set P2P Frequency
AT+TXOUTPUTPOWER    RW: get or set P2P Tx Power(5-22)
AT+BANDWIDTH        RW: get or set P2P Bandwidth(LORA: 0 = 125, 1 = 250, 2 = 500, 3 = 7.8, 4 = 10.4, 5 = 15.63, 6 = 20.83,
                        7 = 31.25, 8 = 41.67, 9 = 62.5  FSK:4800-467000)
AT+SPREADINGFACTOR  RW: get or set P2P Spreading Factor (5-12)
AT+CODINGRATE       RW: get or set P2P Code Rate(0=4/5, 1=4/6, 2=4/7, 3=4/8)
AT+PREAMBLELENGTH   RW: get or set P2P Preamble Length (5-65535)
AT+SYMBOLTIMEOUT    RW: get or set P2P symbolTimeout (0-248)
AT+FIXLENGTHPAYLOAD RW: get or set P2P fix length payload on/off ( 1 = on, 0 = off)
AT+ADDMULC          W: add a new multicast group
AT+RMVMULC          W: delete a multicast group
AT+LSTMULC          R: view multicast group information
AT+TRSSI            RW: start RF RSSI tone test
AT+TTONE            RW: start RF tone test
AT+TTX              RW: set number of packets to be sent for PER RF TX test
AT+TRX              RW: set number of packets to be received for PER RF RX test
AT+TCONF            RW: configure LoRa RF test
AT+TTH              RW: start RF TX hopping test from Fstart to Fstop, with Fdelta steps
AT+TRTH             RW: start RF TX hopping test from Fstart to Fstop, with Fdelta interval in random sequence
AT+TOFF             RW: stop ongoing RF test
AT+CERTIF           RW: set the module in LoraWAN certification mode (0 = normal mode, 1 = certification mode)
AT+CW               RW: start continuous wave
```
### Custom AT command
```
ATC+VER         R: VER, Return firmware version

ATC+SHT         R: SHT, Return the status of the SHTC3 sensor. 1 if available.
ATC+TEMP        R: TEMP, Return the temperature value with 0.01° resolution
ATC+HUM         R: HUM, Return the humidity value with 1 % resolution

ATC+KX023       R: KX023, Return the status of the KX023 sensor. 1 if available.
ATC+AX          R: AX, Return the value of X acceleration with 0.01G resolution
ATC+AY          R: AY, Return the value of Y acceleration with 0.01G resolution
ATC+AZ          R: AZ, Return the value of Z acceleration with 0.01G resolution

ATC+LTR         R: LTR, Return the status of the LTR - 303 sensor. 1 if available.
ATC+LUMCH0      R: LUMCH0, Return the CHANNEL0 value of the LTR - 303 sensor
ATC+LUMCH1      R: LUMCH1, Return the CHANNEL1 value of the LTR - 303 sensor
ATC+LUM         R: LUM, Return the CHANNEL1 value of the LTR - 303 sensor

ATC+GPS         R: GPS, Return the status of the GNSS module. 1 if available.
ATC+GPSON       W: GPSON, Return the GNSS module power | = 1 : GNSS ON | = 0 : GNSS OFF
ATC+GPSPWR      W: GPSPWR, Return the GNSS module power | = 1 : GNSS ON | = 0 : GNSS OFF
ATC+GPSSAT      R: GPSSAT, Return the number of available satellite(s)
ATC+GPSTIME     R: GPSTIME, Return GPS time in EPOCH format
ATC+GPSLAT      R: GPSLAT, Return latitude | Return 0 if coordinate is not available
ATC+GPSLON      R: GPSLON, Return longtitude | Return 0 if coordinate is not available
ATC+GPSALT      R: GPSALT, Return altitude | Return 0 if coordinate is not available
ATC+GPSNMEA     W: GPSON, Activate the NMEA log from GNSS module | = 1 : NMEA ON | = 0 : NMEA OFF
ATC+GPSLOG      W: GPSLOG, Activate the results from GNSS module every 5s | = 1 : LOG ON | = 0 : LOG OFF
ATC+GPSDC       W: GPSDC, Set GNSS module in duty cycle mode with 30sec sleep | = 1 : DC ON | = 0 : DC OFF

ATC+BAT         R: BAT, Return battery voltage in mV | Return 0 if not available
ATC+LDO         R: LDO, Return LDO voltage in mV | Return 0 if not available
```
_**New RAK3172 custom firmware version (not available)**_
```
ATC+GPSCONST    R: GPSCONST, Activate Galileo, Beidu, GPS and Glonass constellations
```
### Continue update
