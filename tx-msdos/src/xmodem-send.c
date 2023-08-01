/**
 * tx - disk-send
 * 
 * XMODEM-512 (512K block) routines
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include "xmodem-send.h"
#include "int14.h"
#include "int13.h"

#define START_DELAY_TIME_MS 3000 // approx 3 secs.
#define BYTE_XMODEM_START   0x43 // C (for CRC)

ProtocolState state=START;
unsigned char block_num=1;
short cylinder=0;
unsigned char head=0;
unsigned char sector=1;
char* buf;
DiskGeometry geometry;

/**
 * Calculate 16-bit CRC
 */
unsigned short xmodem_calc_crc(char* ptr, short count)
{
  unsigned short crc;
  char i;

  crc=0;
  while (--count >= 0)
    {
      crc = crc ^ (unsigned short) *ptr++ << 8;
      i=8;
      do {
        if (crc & 0x8000)
          crc = crc << 1 ^ 0x1021;
        else
          crc = crc << 1;
      } while (--i);
    }
  return crc;
}

/**
 * XMODEM-512 send file - main entrypoint.
 */
void xmodem_send(void)
{
  buf=malloc(512);

  if (int13_disk_geometry(&geometry)==1)
    {
      printf("Could not retrieve disk geometry for device 0x80. Aborting.\n");
      free(buf);
      return;
    }
  
  while (state!=END)
    {
      switch (state)
        {
          case START:
            xmodem_state_start();
            break;
          case BLOCK:
            xmodem_state_block();
            break;
          case CHECK:
            xmodem_state_check();
            break;
          case REBLOCK:
            break;
          case END:
            break;
        }
    }
  
  free(buf);

}

/**
 * Send CRC START (0x43) character and delay for 3 seconds, waiting for SOH.
 */
void xmodem_state_start()
{
  short wait_time=START_DELAY_TIME_MS;
  
  while (wait_time>0)
    {
      delay(1);
      wait_time--;
      if (int14_data_waiting()!=0)
        {
          if (int14_read_byte()=='C')
            {
              state=BLOCK;
              printf("Starting Transfer.\n");
              return;
            }
        }
    }
  printf("Trying Again...\n");
}

/**
 * Send an XMODEM-512 block with CRC
 */
void xmodem_state_block(void)
{
  short i=0;
  unsigned short calced_crc;
  if (int13_read_sector(cylinder,head,sector,buf)==0)
    {
      printf("Sending Cylinder: %4d, Head: %2d, Sector: %2d...",cylinder,head,sector);

      int14_send_byte(0x01);  // SOH
      int14_send_byte(block_num); // block # (mod 256)
      int14_send_byte(0xFF-block_num); // 0xFF - BLOCK # (simple checksum)

      for (i=0;i<512;i++)     // Send the data
        int14_send_byte(buf[i]);

      calced_crc=xmodem_calc_crc(buf,512);
      int14_send_byte((calced_crc>>8));       // CRC Hi
      int14_send_byte(calced_crc&0xFF);       // CRC Lo

      state=CHECK;
    }
  else
    {
      printf("Could not read C: %4d, H: %2d, S: %2d from drive. Cancelling.\n",cylinder,head,sector);
      int14_send_byte(0x18); // CANCEL
      state=END;
    }
}

/**
 * Wait for ack/nak/cancel from receiver
 */
void xmodem_state_check(void)
{
  unsigned char b;
  if (int14_data_waiting()!=0)
    {
      b=int14_read_byte();
      switch (b)
        {
        case 0x06: // ACK
          printf("ACK!\n");
          block_num++;
          block_num&=0xff;
          state=BLOCK;
          xmodem_set_next_sector();  // so if we're at end, it can be overridden.
          break;
        case 0x15: // NAK
          printf("NAK!\n");
          state=BLOCK;  // Resend.
          break;
        case 0x18: // CAN
          printf("CANCEL!\n");
          state=END;   // end.
          break;
        default:
          printf("Unknown Byte: 0x%02d: %c",b,b);
        }
    }
}

/**
 * Set next sector (in response to ACK)
 */
void xmodem_set_next_sector(void)
{
  if (sector>=geometry.s)
    {
      sector=1;
      if (head>=geometry.h)
        {
          head=0;
          if (cylinder>geometry.c)
            state=END;
          else
            cylinder++;
        }
      else
        head++;
    }
  else
    sector++;
}
