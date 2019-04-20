/**
 * tx - disk-send
 * 
 * XMODEM-512 (512K block) routines
 *
 * Thomas Cherryhomes <thom.cherryhomes@gmail.com>
 *
 * Licensed under GPL Version 3.0
 */

#ifndef XMODEM_H
#define XMODEM_H

typedef enum _state {START, BLOCK, CHECK, REBLOCK, END} ProtocolState;

/**
 * XMODEM-512 send file - main entrypoint.
 */
void xmodem_send(void);

/**
 * Send CRC START (0x43) character and delay for 3 seconds, waiting for SOH.
 */
void xmodem_state_start();

/**
 * Send an XMODEM-512 block with CRC
 */
void xmodem_state_block(void);

/**
 * Wait for ack/nak/cancel from receiver
 */
void xmodem_state_check(void);

/**
 * Set next sector (in response to ACK)
 */
void xmodem_set_next_sector(void);

#endif /* XMODEM_H */
