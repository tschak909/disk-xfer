/**
 * rx - Receive image from tx
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Released under GPL version 3.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset

typedef enum _state {START, BLOCK, CHECK, REBLOCK, END} ProtocolState;

#define BUFFER_SIZE 517 // 512 + 5 XMODEM-CRC block overhead.

ProtocolState state=START;
int serial_fd;
unsigned char b;
char* filename;
long block_num=1;

unsigned char* buf;

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
      do
	{
	  if (crc & 0x8000)
	    crc=crc<<1^0x1021;
	  else
	    crc=crc<<1;
	}
      while(--i);
    }
  return crc;
}

/**
 * Send byte
 */
void xmodem_send_byte(unsigned char b)
{
  write(serial_fd,&b,1);
}

/**
 * xmodem start, send C, switch to BLOCK state
 */
void xmodem_state_start(void)
{
  printf("Sending Ready to Start.");
  b='C'; // Xmodem CRC start byte.
  write(serial_fd,&b,1); // Write to serial port.
  state=BLOCK; // Ready to start feeding blocks.
}

/**
 * xmodem block - Get XMODEM block into buffer.
 */
void xmodem_state_block(void)
{
  int i=0;
  printf("Receiving next block # %d...",block_num);
  for (i=0;i<BUFFER_SIZE;i++)
    {
      read(serial_fd,&buf[i],1);
    }
  state=CHECK;
}

/**
 * xmodem check crc - given buffer, calculate CRC, and compare against
 * stored value in block.
 */
unsigned char xmodem_check_crc(void)
{
  char* data_ptr=&buf[3];
  unsigned short crc1;
  unsigned short crc2=xmodem_calc_crc(data_ptr,512);

  crc1=buf[515]<<8;
  crc1|=buf[516];
  
  if (crc1==crc2)
    return 1;
  else
    return 0;
}

/**
 * Block is ok, write it to disk.
 */
void xmodem_write_block_to_disk(void)
{
  char* data_ptr=&buf[3];
  int fd;

  fd=open(filename,O_CREAT|O_APPEND|O_WRONLY);
  write(fd,data_ptr,512);
  close(fd);
}

/**
 * xmodem check - Check buffer for well formedness
 * and either send ACK or NAK, return to BLOCK or END.
 */
void xmodem_state_check(void)
{
  unsigned char block_checksum=buf[1]+buf[2];
  if (buf[0]!=0x01) // Check for SOH
    {
      printf("Bad SOH!\n");
      xmodem_send_byte(0x15); // Send NAK
      state=BLOCK;
      return;
    }
  else if (block_checksum!=0xFF) // Check the checksum
    {
      printf("Bad Block Checksum! %u + %u = %u\n",buf[1],buf[2],buf[1]+buf[2]);
      xmodem_send_byte(0x15); // Send NAK
      state=BLOCK;
      return;
    }
  else if (xmodem_check_crc()==0) // Check the CRC.
    {
      printf("Bad CRC!\n");
      xmodem_send_byte(0x15); // Send NAK
      state=BLOCK;
      return;
    }

  // If we get here, block is reasonably ok, write out data payload.
  // And send an ACK.
  xmodem_write_block_to_disk();
    
  xmodem_send_byte(0x06);
  printf("ACK!\n");
  block_num++;
  state=BLOCK;
}

/**
 * Main protocol entry point
 */
int xmodem_receive(void)
{
  buf=malloc(BUFFER_SIZE);
  
  while (state!=END)
    {
      switch(state)
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
  
  // We're done.
  free(buf);
  return 0;
}

/**
 * Initialize serial port
 */
int termio_init(char* serial_filename)
{
  struct termios tio;

  memset(&tio,0,sizeof(tio));
  tio.c_iflag=0;
  tio.c_oflag=0;
  tio.c_cflag=CS8|CREAD|CLOCAL;
  tio.c_lflag=0;
  tio.c_cc[VMIN]=1;
  tio.c_cc[VTIME]=5;
  
  serial_fd=open(serial_filename, O_RDWR);
  cfsetospeed(&tio,B19200);
  cfsetispeed(&tio,B19200);
  
  tcsetattr(serial_fd,TCSANOW,&tio);
  printf("Serial port initialized.");
}

/**
 * print args if less than two args on cmd line.
 */
void print_args(void)
{
  printf("rx /dev/ttySx destination_image_name.img\n");
}

int main(int argc, char* argv[])
{
  // Display args if not provided.
  if (argc!=3)
    {
      print_args();
      exit(1);
    }
  else
    {
      termio_init(argv[1]);
      filename=argv[2];
      return xmodem_receive();
    }
}
