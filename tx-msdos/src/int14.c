/**
 * tx - disk-send
 * 
 * BIOS INT 14H (RS-232C) routines
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <i86.h>
#include "int14.h"

static union REGS regs;

/**
 * Initialize port
 */
unsigned char int14_init(void)
{
  regs.x.dx=0;
  regs.h.ah=0x04; // Extended initialize (for FOSSIL)
  regs.h.cl=8;    // 19200 bps
  regs.h.ch=3;    // 8 bits
  regs.h.bh=0;    // No parity
  regs.h.bl=0;    // 1 stop bit
  regs.h.al=0;    // no break
  int86(0x14,&regs,&regs);

  if (regs.x.ax!=0x1954)
    return 1;

  // Set RTS/CTS flow control
  regs.h.ah = 0x0f;
  regs.h.al = 0x02;
  regs.x.dx = 0;
  int86(0x14,&regs,&regs);

  return 0;
}

/**
 * Send byte
 */
void int14_send_byte(unsigned char b)
{
  regs.x.dx = 0;
  regs.h.al = b;
  regs.h.ah = 0x01;
  int86(0x14,&regs,&regs);
}

/**
 * Get Port Status
 */
short int14_get_status(void)
{
  // Get port status
  regs.x.dx = 0;
  regs.h.al = 0;
  regs.h.ah = 3;
  int86(0x14,&regs,&regs);
  return regs.x.ax;
}

/**
 * Is data waiting?
 * Return 0 if nothing, 1 if data waiting.
 */
unsigned short int14_data_waiting(void)
{
  return (int14_get_status()&0x100);
}

/**
 * Read byte
 */
unsigned char int14_read_byte(void)
{
  regs.x.dx=0;
  regs.h.ah=0x02;
  int86(0x14,&regs,&regs);
  return regs.h.al;
}
