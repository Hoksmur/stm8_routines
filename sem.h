#ifndef __SEM_H__
#define __SEM_H__

// Simple Encoder-like menu

// Menu item has a structure:
// +------------+---------+------+-----------+
// | ->fwLink   | -> text | type | item this |
// +------------+---------+------+-----------+
// fwLink - pointer no a next line for this level menu or NULL_LINK for last on this level
// text   - pointer to a string with showed name
// type   - type of this item; 'submenu' for next level menu
// item   - pointer to a function or link to other menuitem


// Button or encoder event
typedef enum {
	em_press,
	em_up,
	em_down
} em_event_t ;

// type of contest item
typedef enum {
	submenu,
	schar,
	uchar,
	str,
	dummy
} em_ItmType_t;


// union of pointers, strongly recommended SciFi from caxapa.ru

// menu item structure
struct em_Item_s {
	struct em_Item_s * fwLink;
	char * text;
	em_ItmType_t type;
	void * item;
};

// Declare menu type
typedef const struct em_Item_s  em_Item_t;

// Set start values for selected menu
void em_Init( em_Item_t * pMenu);

// pritn text menu
void em_Show(void);

// Call when event 
// Use  em_Show() to update menu on screen
void em_Event( em_event_t evnt );

#define NULL_LINK ((em_Item_t *)0)
#define EM_LINES 4 // number of lines on screen 



#endif // __SEM_H__ //