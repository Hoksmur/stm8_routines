/*
 * Written by Oleg Terntiev, aka Hoksmur
 * 17-10-2014
 * t.oleg@ymail.com
 */

#ifndef _OW_INCLUDED_
#define _OW_INCLUDED_

#define F_CPU 2000000UL

#include <stm8s.h>
#include <util/delay.h>
#include <util/atomic.h>

//1-Wire port registers
#define OW_PORT		GPIOA
#define OW_PIN		3	

//Standard 1-Wire commands
#define OW_READ_ROM	0x33
#define OW_MATCH_ROM	0x55
#define OW_SKIP_ROM	0xCC
#define OW_SRCH_ROM	0xF0

// Save and restore interrupt state
// #define INTDE ( 1)

// Init IO port and send reset to bus
// return 0 if devices not found
unsigned char w1_init(void); 

// Read 1 byte from bus
unsigned char w1_read(void);

// Write 1 byte from bus
unsigned char w1_write(unsigned char data);

// Search next ROM id on bus id-by-id.
// write found id at the same place in a memory
// return 0 if error or found last id
unsigned char w1_search(unsigned char cmd, unsigned char *p);

// 
unsigned char w1_dow_crc8(unsigned char *p,unsigned char n);

// time constants by Maxim IC
#define A   6
#define B  64
#define C  60
#define D  10
#define E   9
#define F  55
#define G   0
#define H 480
#define I  70
#define J 410

#define SFR( a ) (uint8_t)( a )

#endif

