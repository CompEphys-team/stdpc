#include "PortTalkX_IOCTL.h"

int OpenPortTalk(void);
void ClosePortTalk(void);

void outportc(unsigned short PortAddress, unsigned char byte);
void outports(unsigned short PortAddress, unsigned short dbyte);
unsigned char inportc(unsigned short PortAddress);
unsigned short inports(unsigned short PortAddress);

#define    inp(PortAddress)         inportc(PortAddress)
#define    outp(PortAddress, Value) outportc(PortAddress, Value)
