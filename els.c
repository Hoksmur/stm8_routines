#include "els.h"

#define T(name)  FN(name) E(name)

#define FN(name) void name (hwint flags);
#define E(name)
TASKLIST;
#undef FN
#undef E


#define FN(name)
#define E(name) name##_enum, 

enum t_itm{	TASKLIST TASKCOUNT };

#undef E
#define E(name)
#undef FN
#define FN(name) {.func=&name, .rules=0},

task_t task_collection[TASKCOUNT] = { TASKLIST };

void els_chain(void)
{
	unsigned char idx=0;
	while ( idx<TASKCOUNT ) {
		if (task_collection[idx].rules != 0) {
			(*task_collection[idx].func) ( task_collection[idx].rules );
			task_collection[idx].rules = 0;
			// Comment next line if don't want pass table
			// after every task's call.
			idx = 0;
		} else {
			idx++;
		}
	}
}

void els_set_flags( enum t_itm tsk, unsigned int arg)
{
	task_collection[tsk].rules |= arg;
}


/* Small test. Use -DTEST for *gcc
 * and most other compilers. */
#if defined(TEST)
#include <stdio.h>
#define SLEEP() break;

void FooTaskName0( hwint flags) { printf("From 0\n"); }
void FooTaskName1( hwint flags) /*The same declaration*/ { printf("From 1\n"); }
void FooTaskName2( hwint flags) { printf("From 2 call 0\n"); els_set_flags( TASKID(FooTaskName0), 1); }

int main(void){
	els_set_flags( TASKID(FooTaskName1),  12 );
	els_set_flags( TASKID(FooTaskName2), 128 );
	while(1) {
		els_chain();
		SLEEP();
	}
	return 0;
}

#endif