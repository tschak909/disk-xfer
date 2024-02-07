/**
 * tx - disk-send
 * 
 * BIOS INT 13H (Disk) routines
 *
 * Sergey Kiselev <skiselev@gmail.com>
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#ifndef INT13_H
#define INT13_H

typedef struct {
  short c; /* Cylinders */
  short h; /* Heads */
  short s; /* Sectors per Track */
} DiskGeometry;

#define AH_READ_DISK_SECTORS    0x02
#define AH_GET_DRIVE_PARAMETERS 0x08

/**
 * Get disk Geometry
 */
unsigned char int13_disk_geometry(DiskGeometry* geometry);

/**
 * Read sectors given CHS
 */
unsigned char int13_read_sectors(short c, unsigned char h, unsigned char s, unsigned char n, char* buf);

#endif /* INT13_H */
