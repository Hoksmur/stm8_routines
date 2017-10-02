#include "sem.h"
#include "lcd.h"
#include "delay.h"

/*
	((signed char (*)(signed char))
	и похожие - это жуткое колдунство стало возможно 
	благодаря советам SciFi с форума http://caxapa.ru
	Это преобразование к указателю на функцию и её вызов
*/



static em_Item_t * p_Itm_entry = NULL_LINK;
static bool selected = FALSE;
static  em_Item_t * p_Act_item = NULL_LINK;

static unsigned char active_pos = 0;


void em_Init( em_Item_t * pMenu)
{
	p_Itm_entry = pMenu;
	//selected = FALSE;
	//p_Act_item = NULL_LINK;
}


// Replacing sprintf (this one don't understand 'hh'
void xformat( char *buf, char type, signed char val)
{
char ibuf[5];
unsigned char tmp, idx=4;

	switch(type) {
		case 0x00: // signed char
			ibuf[idx--] = '\0';
			if ( val<0 ) {
				tmp = (-val);
			} else {
				tmp = val;
			}
			do {
				ibuf[idx--] = (tmp%10)+'0';
				tmp = tmp/10;
			} while( tmp > 0);
			ibuf[idx] = (val<0) ? '-' : ' ';
			break;
		case 0x01: // insigned 
			ibuf[idx--] = '\0';
			do {
				ibuf[idx--] = (tmp%10)+'0';
				tmp = tmp/10;
			} while( tmp > 0);
			break;
	}
	// add spaces before if it's need
	while ((idx)>0) {
		ibuf[--idx] = ' ';
	}

	for ( ; idx<=4; idx++ ) {
		*buf++ = ibuf[idx];
	}
}


void em_Show(void)
{
em_Item_t * em_curItem = p_Itm_entry;
unsigned char m_i;
char mode;

	 m_i = 0;
	 do {
		lcd_print( 1, m_i, em_curItem->text);
		if ( m_i !=  active_pos ) {
			// Not active
			mode = '.';
		} else {
			p_Act_item = em_curItem;
			if ( selected == FALSE) {
				// Active, not selected
				mode = '>';
			} else {
				// Active, selected
				mode = '#';
			}
		}
		lcd_pchar( 0, m_i, mode);
		m_i++;
		em_curItem = em_curItem->fwLink;
	} while ( em_curItem != NULL_LINK );
	
	// clear screen area
	while ( m_i < EM_LINES ) {
		lcd_print( 0, m_i, "            " );
		m_i++;
	}
}

void em_Event( em_event_t evnt )
{
//signed char (*p_func_s)( signed char);
//unsigned char (*p_func_u)( unsigned char);

char v_char;
char vis_buf[5];
	if ( selected == FALSE) {
	// ######## UNSELECTED #################
		switch(evnt) {
		//==================================
			case em_press:
				switch(p_Act_item->type) {
				// -------------------------
					case submenu:
						selected = FALSE;
						active_pos = 0;
						p_Itm_entry = p_Act_item->item; // Entry point to submenu
						p_Act_item =  p_Itm_entry; // place user pointer to first line
						break;
				// -------------------------
					case schar:
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_s)(0);
						v_char = ((signed char (*)(signed char))p_Act_item->item)(0);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						selected = TRUE;
						break;
				// -------------------------
					case uchar:
						lcd_pchar(1, 5, 'U');
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_u)(0);
						v_char = ((unsigned char (*)(signed char))p_Act_item->item)(0);

						xformat( vis_buf, 1, v_char);
						lcd_print( 9, active_pos, vis_buf );
						selected = TRUE;
						break;
				// -------------------------
				//	case str:
				//		break;
				}
				break;
		//==================================
			case em_up:
				active_pos--;
				break;
		//==================================
			case em_down:
				active_pos++;
				break;
		//==================================
		}
	} else {
	// ######## SELECTED ###################
		switch(evnt) {
		//==================================
			case em_press:
				switch(p_Act_item->type) {
				// -------------------------
					case submenu:
						selected = FALSE;
						active_pos = 0;
						p_Itm_entry = p_Act_item->item; // Entry point to submenu
						p_Act_item =  p_Itm_entry; // place user pointer to first line
						break;
				// -------------------------
					case schar:
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_s)(0);
						v_char = ((signed char (*)(signed char))p_Act_item->item)(0);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						selected = FALSE;
						break;
				// -------------------------
					case uchar:
						lcd_pchar(4, 5, '*');
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_u)(0);
						v_char = ((unsigned char (*)(signed char))p_Act_item->item)(0);

						xformat( vis_buf, 1, v_char);
						lcd_print( 9, active_pos, vis_buf );
						lcd_pchar(4, 6, '=');
						selected = FALSE;
						break;
				// -------------------------
				//	case str:
				//		break;
				}
				break;
		//==================================
			case em_up:
				switch(p_Act_item->type) {
				// -------------------------
					case submenu:
						break;
				// -------------------------
					case schar:
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_s)(1);
						v_char = ((signed char (*)(signed char))p_Act_item->item)(1);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						break;
				// -------------------------
					case uchar:
						lcd_pchar(1, 5, '<');
						//p_func_s = p_Act_item->item;
						//v_char = (*p_func_u)(1);
						v_char = ((unsigned char (*)(signed char))p_Act_item->item)(1);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						break;
				}
				break;
		//==================================
			case em_down:
				switch(p_Act_item->type) {
				// -------------------------
					case submenu:
						break;
				// -------------------------
					case schar:
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_s)(-1);
						v_char = ((signed char (*)(signed char))p_Act_item->item)(-1);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						break;
				// -------------------------
					case uchar:
						lcd_pchar(1, 5, '>');
//						p_func_s = p_Act_item->item;
//						v_char = (*p_func_u)(-1);
						v_char = ((unsigned char (*)(signed char))p_Act_item->item)(-1);
						xformat( vis_buf, 0, v_char);
						lcd_print( 9, active_pos, vis_buf );
						break;
				}
				break;
		//==================================
		}
	}
	em_Show();
}

