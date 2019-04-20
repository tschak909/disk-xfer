/**
 * tx - disk-send
 * 
 * XMODEM-512 (512K block) routines
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#include "xmodem-send.h"

/**
 * XMODEM-512 send file - main entrypoint.
 */
void xmodem_send(char* data, long len)
{
  ProtocolState state=START;
  
}

/**
 * Send CRC START (0x43) character and delay for 3 seconds, waiting for SOH.
 */
unsigned char xmodem_state_start()
{
}
