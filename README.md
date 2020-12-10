stm8_routines
=============


[ for Cosmic and SDCC compiler ]

delay.h - software delay functions

atomic.h - atomic operations. Based on AVR-LibC idea. Cosmic hasn't opportunity to declare variable in operator 'for'.

owire.h, owire.c - full software 1-wire functions. It has macro for use with interupts together.

# Extremely light sheduler
=============
+ ANSI C
+ less 100 strings
+ two function, two files *els.h*, *els.c*
+ only sizeof(void\*) + your int bytes in RAM per task (for 'char': 2+1);

Write task's function run-to-comlete, it is state-mashine. Task can call each other, from ISR, ...
Tasks have some kind unchanged priority, first in list is highest. See end of *els.c* for example.


SEM - simple encoder (or embedded) menu
=============
 Use only 3 functions. Flexible and small footprint.

 Menu item has a structure:  
 +------------+---------+------+-----------+  
 | ->fwLink   | -> text | type | this item |  
 +------------+---------+------+-----------+  
 
 Example menu structure:  
 
  'Item 1>'  
    'back'  
    'item 4'  
  'item 2'  
  'item 3'  
 
 Event route:
 mode: selected or unselected item  
 -each has: event( em_up, em_down or em_press)  
 -each of menuitem has code block for every value type that this one returns. Example has for submenu, signed char and unsigned char.  


LCD display Nokia 5110 3110 on PCD8544 84x48 
=============
Implemented functions to print char or string to LDC with normal size or double size. Simple KOI8 table for readable RU texts.
Also 'push byte' as a basic for primitive graphics.


