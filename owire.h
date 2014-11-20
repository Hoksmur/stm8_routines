/*
 * Written by Oleg Terntiev, aka Hoksmur
 * 17-10-2014
 * t.oleg@ymail.com
 *
 * 20.nov.2014 changed:
 * - w1_search return status, variable lastDevFlg deleted
 * - timing now is calculate
 */

/* --- used APPLICATION NOTE 126 ---
   --- Maxim's for time delays.  ---
   http://www.maximintegrated.com/en/app-notes/index.mvp/id/126
   --- */

#ifndef _OW_INCLUDED_
#define _OW_INCLUDED_

#define F_CPU 2000000UL

#include <stm8s.h>
#include <util/delay.h>
#include <util/atomic.h>

// Save and restore interrupt state
#define INTDE ( 1)



typedef enum {
	CONN_FAULT = 1,
	W1_OK = 2,
	NODEV_PRESENT = 3,
	LAST_DEV = 4,
} w1_error_t;

// Init IO port and send reset to bus
// return 0 if devices not found
unsigned char w1_init(void); 

// Read 1 byte from bus
unsigned char w1_read(void);

// Write 1 byte from bus
unsigned char w1_write(unsigned char data);

w1_error_t w1_search(unsigned char cmd, unsigned char *p);
// Search next ROM id on bus id-by-id.
// write found id at the same place in a memory
// return 0 if error or found last id

// 
unsigned char w1_dow_crc8(unsigned char *p,unsigned char count);


//1-Wire port registers
#define W1_PORT		GPIOA
#define W1_PIN		3

//Standard 1-Wire commands
#define W1_READ_ROM	0x33
#define W1_MATCH_ROM 0x55
#define W1_SKIP_ROM	0xCC
#define W1_SRCH_ROM	0xF0
#define W1_STRT_CONV 0x44
#define W1_RD_SCRTPD 0xBE
#define W1_SRCH_ALRM 0xEC

/* time constants by Maxim IC
 * A - write '0'
 * C - write '1'
 * A+E - read sample
*/
#define W1SLOT (70) /* A + B */
#define A  6
#define B  (W1SLOT-A)
#define C  60
#define D  (W1SLOT-C)
//#define E   9
#define E   7
#define F  (W1SLOT-E-A)
#define G   0
#define H 480
#define I  70
#define J 410

#define SFR( a ) (uint8_t)( a )

#endif

