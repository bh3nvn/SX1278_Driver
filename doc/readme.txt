+-----------------------------------------------------------------------------+
|                          SX12xx drivers V2.1.0                             |
|               Last modified by Marc0 Xu on Jan 07 2018                     |
+-----------------------------------------------------------------------------+

This package currently supports the following radio drivers:
    
    - SX1278
        - FSK modem
        - LoRa modem ( Application default )
        
1. Introduction
-------------------------------------------------------------------------------

This platform can use a STM8L  microcontroller .

The project included is a IAR project.

The project contains the low level drivers needed to control the extension
board peripherals as well as the SX1278 RF chip drivers. The demonstration
application is a simple PING-PONG.

By default the PING-PONG app acts as Master. If another Master is already
running the application will switch automatically to Slave and the PING-PONG
starts.


2. Radio drivers description
-------------------------------------------------------------------------------

The radio interface for the application is manged by the SX1278-Driver\radio.c file.
The RadioDriverInit function must be updated with SX1278 functions.

In order to select the LoRa modem driver one must modify SX1278-Driver\radio.h as
follows:

/*!
 * SX1272 and SX1278 General parameters definition
 */
#define LORA                                     1         // [0: OFF, 1: ON]

The drivers can also

2.1 Specific radio drivers description
-------------------------------------------------------------------------------

The specific radio driver is split in several parts in order to ease the
portability.


2.1.1 SX1278 driver version V2.1.0
-------------------------------------------------------------------------------

The SX1278 drivers are very similar to the SX1278 drivers. In fact, most of the 
drivers are identical except for the new feature supported by the SX1278 (new 
frequency band, new Bandwidth of operation in LoRa, ...) and some registers
addresses which are different.

The SX1278 driver is split in 4 parts

1. Generic SX1278 driver.
  ( SX1278-Driver\SX1278.c )
2. SX1272 FSK modem driver.
  ( SX1278-Driver\SX1278-Fsk.c and SX1278-Driver\SX1278-FskMisc.c )
3. SX1272 LoRa modem driver.
  ( SX1278-Driver\SX1278-LoRa.c and SX1278-Driver\SX1278-LoRaMisc.c )
4. SX1278 HAL ( Hardware Abstraction Layer ).
  ( SX1278-Driver\SX1278-Hal.c )

1. The generic SX1278 driver implements at least the functions required by 
   the RadioDriver structure defined in SX1278-Driver\radio.h file. It offers 
   also the same interface for the FSK or the LoRa modem.
 
   In order to choose which modem to use one must modify the src\radio\radio.h
    file as follows:
 
   - For FSK modem
        #define LORA                                        0

   - For LoRa modem
        #define LORA                                        1

2. The FSK modem driver handles the SX1278 as a FSK modem

   In order to change generic FSK modem settings one must modify the following
   parameters in file SX1278-Driver\SX1278-Fsk.c

   tFskSettings FskSettings = 
   {
      870000000,      // RFFrequency
      9600,           // Bitrate
      50000,          // Fdev
      20,             // Power
      100000,         // RxBw
      150000,         // RxBwAfc
      true,           // CrcOn
      true,           // AfcOn    
      255             // PayloadLength (set payload size to the maximum for 
                      // variable mode, else set the exact payload length)
   };


   REMARK: All other parameters can be changed by modifying the SX1278FskInit
          function located in SX1278-Driver\SX1278-Fsk.c file

3. The LoRa modem driver handles the SX1278 as a LoRa modem

   In order to change generic LoRa modem settings one must modify the following
   parameters in file SX1278-Driver\SX1278-LoRa.c


   tLoRaSettings LoRaSettings =
   {
      434000000,      // RFFrequency
      20,             // Power  
      8,              // SignalBw [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                      // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved] 
      7,              // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 
                      // 10: 1024, 11: 2048, 12: 4096  chips]
      2,              // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
      true,           // CrcOn
      false,          // ImplicitHeaderOn
      1,              // RxSingleOn
      0,              // FreqHopOn
      4,              // HopPeriod
      100,            // TxPacketTimeout
      100,            // RxPacketTimeout
      4               // PayloadLength
   };



4. The HAL makes the SX1278 driver platform independent.

    One must modify each function inside this file
    ( SX1278-Driver\SX1272-Hal.c ) according to the platform used.

3. How to use the driver:
-------------------------------------------------------------------------------


This driver has been tested for high speed transmission (up to 100kbps in FSK) 
and long payloads (up to 255 bytes in FSK or LoRa). To set a transmission /
reception, it is necessary to:

   - Change the payload length

The payload length for the system is defined with the parameter BUFFER_SIZE 
located in main.c

#define BUFFER_SIZE                                 128 // Define the payload 
                                                        // size here

The payload length can be configured from 1 up to 255


   - Change the RF Parameters

Depending on which mode you are operating (FSK or LoRa), you should modify the 
parameters in their dedicated structure, either the structure LoRaSettings or 
the structure FskSettings. Care must be taken when changing the parametes so 
that there are no conflict between the parameters. For example, in LoRa, the 
Spreading Factor SF6 only operates in Implicit Header mode.
In case of doubts, please refer to the latest datasheet for your device.

The driver is organised to perform a PING PONG between the transmitter and the 
receiver. Both the transmitter and the receiver will start as masters and will 
therefore start sending packet and waiting to receive one. As soon as one of the
device receive a packet PING, it will set itself as a slave and will then send 
a PONG. Afterward, the master will always send a PING and the slave will answer
with a PONG in an infinite loop. This organisation should provide a good example
of transmission and reception using the devices.





