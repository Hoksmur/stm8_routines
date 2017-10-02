#ifndef __LCD_H__
#define __LCD_H__

#include "stm8s.h"
#include "main.h"

//extern const char * arr_token;

void lcd_reset(void);
void lcd_preset(void);
void lcd_init(void);
void lcd_write(uint8_t x, uint8_t y, uint8_t *buf, uint16_t cnt);
void lcd_setXY( uint8_t x, uint8_t y);
void lcd_push_byte( uint8_t bv);
void lcd_bigchar(uint8_t x, uint8_t y, uint8_t *ch);

void lcd_pchar(uint8_t x, uint8_t y, uint8_t tkn);
void lcd_pchar_2x2(uint8_t x, uint8_t y, uint8_t tkn);

void lcd_print(uint8_t x, uint8_t y, uint8_t *p_tkn);
void lcd_print_2x2(uint8_t x, uint8_t y, uint8_t *p_tkn);


#define  RUKOI_COMACT
#define  RU_EXTENSION
#endif /* __LCD_H__ */