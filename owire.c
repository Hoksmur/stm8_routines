#include <util/owire.h>

/* --- used APPLICATION NOTE 126 ---
   --- Maxim's for time delays.  --- 
   http://www.maximintegrated.com/en/app-notes/index.mvp/id/126
   --- */

uint8_t iStateSave;

/* do CRC8 table */
const unsigned char OW_crc8_table[] = {
	0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
	157,195, 33,127,252,162, 64, 30, 95, 1,227,189, 62, 96,130,220,
	35,125,159,193, 66, 28,254,160,225,191, 93, 3,128,222, 60, 98,
	190,224, 2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
	70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89, 7,
	219,133,103, 57,186,228, 6, 88, 25, 71,165,251,120, 38,196,154,
	101, 59,217,135, 4, 90,184,230,167,249, 27, 69,198,152,122, 36,
	248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91, 5,231,185,
	140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
	17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
	175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
	50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
	202,148,118, 40,171,245, 23, 73, 8, 86,180,234,105, 55,213,139,
	87, 9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
	233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
	116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};


unsigned char OW_rxBit(void)
{
unsigned char out;
	_asm("nop");
	OW_PORT->ODR &= SFR( ~(1<<OW_PIN) ); // low
	_delay_us( A);
	OW_PORT->ODR |= 1<<OW_PIN;
	_delay_us( E);
	out = SFR( ( OW_PORT->IDR & 1<<OW_PIN )  ? 0x01 : 0x00 );
	_delay_us( F);
	return (out);
}


unsigned char OW_txBit( unsigned char bit)
{
        if (bit != 0)
        {
                // Write '1' bit
                OW_PORT->ODR &= SFR( ~(1<<OW_PIN)); // Drives DQ low
				//if (bit_is_set(OW_IN, OW_PIN) ) { return 0;}
                _delay_us(A);
                OW_PORT->ODR |=  1<<OW_PIN; // Releases the bus
                _delay_us(B); // Complete the time slot and 10us recovery
        } else {
                // Write '0' bit
                OW_PORT->ODR &= SFR(~(1<<OW_PIN)); // Drives DQ low
				//if (bit_is_set(OW_IN, OW_PIN) ) { return 0;}
                _delay_us(C);
                OW_PORT->ODR |=  1<<OW_PIN; // Releases the bus
                _delay_us(D);
        }
	return ( 1 ); // compatability CodeVision
}
 

unsigned char w1_init(void)
{
unsigned char out;

#if INTDE
iStateSave = ATOMIC_BEGIN();
#endif	
/* Init IO and reset presence pulse */
	// select Open drain output, fast mode
	OW_PORT->DDR |= SFR(  1<<OW_PIN);
//	OW_PORT->CR1 |= SFR(1<<OW_PIN);
	OW_PORT->CR1 &= SFR(~(1<<OW_PIN));
	OW_PORT->CR2 |= SFR(  1<<OW_PIN);

	OW_PORT->ODR &= SFR(~(1<<OW_PIN));
	_delay_us(H);

	OW_PORT->ODR |=   1<<OW_PIN;
	_delay_us(I);
//	out = bit_is_clean( (OW_PORT->IDR), OW_PIN) ? 0x01 : 0x00 ;
	out = (uint8_t)( !( OW_PORT->IDR & 1<<OW_PIN)  ? 0x01 : 0x00 );
	_delay_us(J);
#if INTDE
ATOMIC_END( iStateSave );
#endif
	return out;
}


unsigned char w1_read(void)
{
// 1-wire read byte
uint8_t out = 0x00;
uint8_t j;

#if INTDE
iStateSave = ATOMIC_BEGIN();
#endif
	_delay_us(60);
	for (j = 0;j<8;j++) {
		out >>= 1;
		out |= SFR( OW_rxBit() ? 0x80 : 0 );
	}
#if INTDE
ATOMIC_END( iStateSave );
#endif
	return (out);
}


unsigned char w1_write(unsigned char data)
{
// 1-Wire write byte
uint8_t i;

#if INTDE
iStateSave = ATOMIC_BEGIN();
#endif
	for(i=0; i<8;i++) {
		if ( (data & (0x01<<i)) != 0 ) {
			OW_txBit(1);
		}
		else {
			OW_txBit(0);
		}
//		_asm("nop");
	}
#if INTDE
ATOMIC_END( iStateSave );
#endif
	return ( 1); // for compatibility CodeVision
}

void storeBit( unsigned char  bitIdx, unsigned char bitVal, unsigned char *p)
{
unsigned char ch;
ch = *(p + (bitIdx>>3));	// byte address
if (bitVal) {
	ch |= SFR( 1<<(bitIdx & 0x07)) ;
} else {
	ch &= SFR(~(1<<(bitIdx & 0x07)));
}
*(p + (bitIdx>>3)) = ch;	// save actual byte
}

unsigned char w1_search(unsigned char cmd, unsigned char *p)
// using application note AN187 
// http://pdfserv.maximintegrated.com/en/an/AN187.pdf
{
static unsigned char lastZero;
static unsigned char lastDevFlg = 0;
static unsigned char lastConflict = 0;
unsigned char id_bit_num;
unsigned char id_bit, id_cmp_bit;
unsigned char bit_to_write;

	if ( !w1_init() || (lastDevFlg != 0) ) {	// no devices // previous ID was the last
		lastConflict = 0;
		lastDevFlg = 0;
		return( 0);
	}
	id_bit_num = 1;
	lastZero = 0;
	w1_write( cmd); // search(cmd)

	do {
#if INTDE
iStateSave = ATOMIC_BEGIN();
#endif
		id_bit     = OW_rxBit();
		id_cmp_bit = OW_rxBit();
#if INTDE
ATOMIC_END( iStateSave );
#endif
		if ( (id_bit != 0) && (id_cmp_bit != 0) ) {	// connection fault!
			lastConflict = 0;
			lastDevFlg = 0;
			return( 0);
		} 
		if ( (id_bit==0) && (id_cmp_bit==0) ) {	// 00: different bits in devices
			if ( id_bit_num == lastConflict) {
				bit_to_write = 1;
			} else if ( id_bit_num > lastConflict) {
				bit_to_write =  0;
			} else {	// #13
				bit_to_write = SFR(( *(p + ((id_bit_num - 1) >>3)) >> ((id_bit_num - 1) & 0x07) ) & 0x01);	// get bit ROM[idx]
			}
			if ( !bit_to_write ) {	// bit_to_write==0 #15
				lastZero = id_bit_num;
			}
		} else {
			// all devices have the same bit
			bit_to_write = id_bit;
		}

		// write, send
		storeBit( SFR(id_bit_num-1), bit_to_write, (unsigned char *)p);
#if INTDE
iStateSave = ATOMIC_BEGIN();
#endif
		OW_txBit(bit_to_write);
#if INTDE
ATOMIC_END( iStateSave );
#endif
		id_bit_num++;
	} while (id_bit_num <= 64);

	lastConflict = lastZero;
	if ( lastConflict == 0) {
		lastDevFlg = 1;
	}
	return( 1);
}


unsigned char w1_dow_crc8(unsigned char *p, unsigned char n)
{
//unsigned char *ptr=(unsigned char *)p;
unsigned char i;
unsigned char crc = 0;

	i=0;
	do {
		crc = OW_crc8_table[ crc ^ *(p +i) ];
	} while ( ++i < n );
	return (crc);
}

