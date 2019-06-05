/*
 * StdpC is a free dynamic clamp software.
 * Copyright (C) 2019 Thomas Nowotny, Felix Kern
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

#include "PortTalkX_IOCTL.h"

int OpenPortTalk(void);
void ClosePortTalk(void);

void outportc(unsigned short PortAddress, unsigned char byte);
void outports(unsigned short PortAddress, unsigned short dbyte);
unsigned char inportc(unsigned short PortAddress);
unsigned short inports(unsigned short PortAddress);

#define    inp(PortAddress)         inportc(PortAddress)
#define    outp(PortAddress, Value) outportc(PortAddress, Value)
