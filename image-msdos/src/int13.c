/**
 * image - disk-send
 * 
 * BIOS INT 13H (Disk) routines
 *
 * Sergey Kiselev <skiselev@gmail.com>
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <i86.h>
#include "int13.h"

union REGS regs;

/**
 * Get disk Geometry
 */
unsigned char int13_disk_geometry(DiskGeometry* geometry)
{
  // BIOS call to get disk geometry.
  regs.h.ah=AH_GET_DRIVE_PARAMETERS;
  regs.h.dl=0x80;
  int86(0x13,&regs,&regs);

  // Unpack disk geometry.
  geometry->c=regs.h.ch;              // Get lower 8 bits of cylinder count.
  geometry->c|=((regs.h.cl)&0xC0)<<2; // Get upper two bits of cylinder count.
  geometry->h=regs.h.dh;
  geometry->s=regs.h.cl&0x3F;         // mask off high order bits of cylinder count (upper 2-bits)

  // 0 if successful, 1 if not.
  return regs.x.cflag;
}

/**
 * Read sectors given CHS
 */
unsigned char int13_read_sectors(short c, unsigned char h, unsigned char s, unsigned char n, char* buf)
{
  // Perform the read.
  regs.h.ah=AH_READ_DISK_SECTORS;
  regs.h.al=n;                       // n sectors.
  regs.h.dh=h;
  regs.h.dl=0x80;                    // first hd
  regs.x.bx=(short)buf;
  regs.h.ch=c&0xFF;                  // cyl low
  regs.h.cl=s;
  regs.h.cl|=((c >> 2)&0xC0);        // sector / cyl high */
  int86(0x13,&regs,&regs);

  // 0 if successful, 1 if not.
  return regs.x.cflag;
}
