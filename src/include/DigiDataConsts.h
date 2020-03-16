/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2020 Thomas Nowotny, Felix Kern
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


//-------------- Constants related to Digidata 1200A board ------------------
//  Addresses:
#define BASEADDRESS             0x320
//#define BASEADDRESS             0x340 (Attila)
#define DACDATA                 0x00
#define ADCDATA                 0x00
#define BOARDID                 0x02
#define TIMERDATA               0x04
#define TIMERCONTROL            0x06
#define TIMERSTATUS             0x06
#define ADCDACCONTROL           0x08
#define INTDMACONTROL           0x0A
#define DIGITALIO               0x0C
#define CHANNELSCANLIST         0x0E
#define REALTIMEDATA0           0x10
#define REALTIMEDATA1           0x12
#define REALTIMEDATA2           0x14
#define REALTIMECONTROL         0x16
#define ADCDACSTATUS            0x18
#define RESETCONTROL            0x1A
//   Board IDs:
#define DIGIDATA1200ID          2
#define DIGIDATA1200AID         3
//   ADCDAC Control
#define DD1200AMODE             0x0040
#define ENABLEDIGITALIO         0x0008
#define ADCASYNCDAC             0x0004
#define ADCGEARENABLE           0x0060
#define ADCSCANLISTENABLE       0x0050
#define ADCSCANLISTDISABLE      0x0040
#define DACCHANNEL0ENABLE       0x0001
#define DACCHANNEL1ENABLE       0x0002
//   ADC/DAC Status
#define ADCDATAREADY            0x0040
//   INT/DMA Control
#define ZEROWORD                0x0000
//   Reset Control
#define RESETWHOLEBOARD         0x007F
#define RESETSCANLIST           0x10
#define ADCSTARTCONVERT         0x0080
#define RESETADCFLAGS           0x0010
//   Real Time Clock
#define C0MODE2                 0x34
#define C1MODE2                 0x74
#define C2MODE2                 0xB4
#define ZEROBYTE                0x00
#define FASTBYTE                0x20
#define READCOUNT0              0x00
#define READCOUNT1              0x40
#define READCOUNT2              0x80
#define READCOUNT0AND1          0xD6
#define READALLCOUNTERS         0xDE

//   Channel Scan List
#define CHANNELGAIN1            0x0000
#define CHANNELGAIN2            0x2000
#define CHANNELGAIN4            0x4000
#define CHANNELGAIN8            0x6000
#define CHANNELSHIFT            0x0100
#define LASTCHANNELFLAG         0x8000
