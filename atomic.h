#ifndef _ATOMIC_H_
#define _ATOMIC_H_ 1

#define ATOMIC_BEGIN() getInterruptState()
#define ATOMIC_END( x ) setInterruptState( x )

uint8_t @inline getInterruptState( void )
{
	return( (uint8_t)_asm( "push cc\n pop a\n rim\n" ) );
}


void @inline setInterruptState ( uint8_t istate)
{
	_asm( "push a\n pop cc\n ", istate );
}

#endif
