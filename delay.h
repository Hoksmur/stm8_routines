/* 
 * delay utilite for STM8 family
 * Terentiev Oleg
 * t.oleg@ymail.com
 */

#ifndef _UTIL_DELAY_H_
#define _UTIL_DELAY_H_ 1

#ifndef F_CPU
#warning F_CPU is not defined!
#endif

/* 
 * Func delayed N cycles, where N = 3 + ( ticks * 3 )
 * so, ticks = ( N - 3 ) / 3, minimum delay is 6 CLK
 * when tick = 1, because 0 equels 65535
 */

static inline void _delay_cycl( const unsigned short __ticks )
{
	// ldw X, __ticks ; insert automaticaly
	_asm("nop\n $N:\n decw X\n jrne $L\n nop\n ", __ticks);
}

static inline void _delay_us( const unsigned short __us )
{
	_delay_cycl( (unsigned short)(( F_CPU * __us / 1000000UL )-3)/3 );
}

static inline void _delay_ms( unsigned short __ms )
{
	while ( __ms-- )
	{
		_delay_us( 1000 );
	}
}

#endif

