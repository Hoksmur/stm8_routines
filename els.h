#pragma once
#ifndef __ELS_H__
#define __ELS_H__

/* Using:
  els_set_flags( TASK(FooTaskName0), 12 );
  els_set_flags( TASK(FooTaskName0), 128 );
  els_chain();
  // See end of *.c - file for small test.
*/

/* Hardware int for your MCU/CPU. */
#define hwint  unsigned int 

#define TASKID(task) task##_enum
#define TASK(task)   void task(hwint flags)

#define TASKLIST \
	T(FooTaskName0) \
	T(FooTaskName1) \
	T(FooTaskName2) 

typedef void (*task_fun_ptr)(hwint flags);
typedef struct task_t{
  task_fun_ptr func;
  int rules;
}task_t;

#endif /* __ELS_H__ */
