/*!
 *  \file    lcd.c
 *  \author  Wim Dolman (<a href="email:w.e.dolman@hva.nl">w.e.dolman@hva.nl</a>)
 *  \date    21-10-2013
 *  \version 1.0
 *
 *  \brief   lcd driver for Xmega
 *
 *  \details This file contains the routines for interfacing a HD44780U-based text
 *           lcd display for Atmel Xmega devices.
 *
 *           It is based on the lcd libray from Peter Fleury
 *           (<a href="http://homepage.hispeed.ch/peterfleury/">http://homepage.hispeed.ch/peterfleury/</a>)
 *           for the ATmega devices.
 *
 *           This library is suitable for four different io modes:
 *           - 8 bit mode without busyflag using connections D7..D0, RS and E
 *           - 8 bit mode with busyflag  using connections D7..D0, RS, E and R/W
 *           - 4 bit mode without busyflag using connections D7..D4, RS and E
 *           - 4 bit mode with busyflag using connections D7..D4, RS, E and R/W
 *
 *           There are defines for these four modes:
 *           - LCD_4BIT_MODE     1 is 4 bit mode, 0 is 8 bit mode
 *           - LCD_BUSY_FLAG     1 is with busyflag, 0 is without busyflag
 *
 *           In 8 bit mode all 8 data pins must be connected to one 8-pin port of
 *           the Xmega.
 *           In 4 bit mode all 4 data pins must be connected to one port of Xmega,
 *           but the 4 data lines may be connected to any pin of that port and
 *           in any order. The header file lcd.h contains macro definitions that
 *           can be for that purpose.
 *
 *           The control lines RS, E and R/W can connected to any pin of any port
 *           of the Xmega.
 *
 *           \warning
 *           Be careful using the busyflag. Most alfanumeric displays are 5 Volt devices.
 *           Xmega's are not 5 Volt tolerant.
 *           When using the busyflag, information is also send from the LCD to the
 *           Xmega. So you can damage your Xmega.
 */

#include <avr/io.h>
#include "lcd.h"
#include <util/delay.h>

static inline void set_rs(uint8_t rs)
{
  if (rs) {
    LCD_RS_PORT.OUTSET = LCD_RS_bm;  // RS high (data)
  } else {
    LCD_RS_PORT.OUTCLR = LCD_RS_bm;  // RS low (command)
  }
}

static inline void enable_puls(void)
{
  LCD_E_PORT.OUTSET  = LCD_E_bm;     // make E high
  _delay_us(TPWE_us);
  LCD_E_PORT.OUTCLR  = LCD_E_bm;     // make E low
}

static inline void write_high_nibble(uint8_t b)
{
  LCD_DATA_PORT.OUT    = (LCD_DATA_PORT.OUT & (~(LCD_DATA_PORT_gm))) |
                         (((b >> 7) & (0x01)) << (LCD_D7_bp)) |
                         (((b >> 6) & (0x01)) << (LCD_D6_bp)) |
                         (((b >> 5) & (0x01)) << (LCD_D5_bp)) |
                         (((b >> 4) & (0x01)) << (LCD_D4_bp)) ;
}

static inline void write_low_nibble(uint8_t b)
{
	LCD_DATA_PORT.OUT    = (LCD_DATA_PORT.OUT & (~(LCD_DATA_PORT_gm))) |
	(((b >> 3) & (0x01)) << (LCD_D7_bp)) |
	(((b >> 2) & (0x01)) << (LCD_D6_bp)) |
	(((b >> 1) & (0x01)) << (LCD_D5_bp)) |
	(((b >> 0) & (0x01)) << (LCD_D4_bp)) ;
}

static inline void start_init_byte(void)
{
  _delay_ms(TDELAY1_ms);
  set_rs(0);
  LCD_DATA_PORT.OUT = LCD_FUNCTION_8BIT;
  enable_puls();
  _delay_ms(TDELAY2_ms);
  LCD_DATA_PORT.OUT = LCD_FUNCTION_8BIT;
  enable_puls();
  _delay_us(TDELAY3_us);
  LCD_DATA_PORT.OUT = LCD_FUNCTION_8BIT;
  enable_puls();
  _delay_us(TDELAY_us);
}

static inline void start_init_nibble(void)
{
  _delay_ms(TDELAY1_ms);
  set_rs(0);
  write_high_nibble(LCD_FUNCTION_8BIT);
  enable_puls();
  _delay_ms(TDELAY2_ms);
  write_high_nibble(LCD_FUNCTION_8BIT);
  enable_puls();
  _delay_us(TDELAY3_us);
  write_high_nibble(LCD_FUNCTION_8BIT);
  enable_puls();
  _delay_us(TDELAY_us);
  write_high_nibble(LCD_FUNCTION_4BIT);
  enable_puls();
  _delay_us(TDELAY_us);
}

static void lcd8_write_byte (uint8_t b, uint8_t rs) {
  set_rs(rs);
  LCD_DATA_PORT.OUT  = b;            // assign data
  enable_puls();
  _delay_us(TDELAY_us);
}

static void lcd8bf_write_byte (uint8_t b, uint8_t rs) {
  uint8_t x;

  LCD_DATA_PORT.DIRCLR = LCD_DATA_PORT_gm;  // read data
  LCD_RW_PORT.OUTSET   = LCD_RW_bm;         // R/W high
  LCD_RS_PORT.OUTCLR   = LCD_RS_bm;         // RS low (command)
  do {
    LCD_E_PORT.OUTSET  = LCD_E_bm;          // make E high
    _delay_us(TPWE_us);
    x = LCD_DATA_PORT.IN;
    LCD_E_PORT.OUTCLR  = LCD_E_bm;          // make E low
  } while (x & (1<<LCD_BUSY_bp));

  LCD_DATA_PORT.DIRSET = LCD_DATA_PORT_gm;  // write data
  LCD_RW_PORT.OUTCLR = LCD_RW_bm;           // R/W low
  set_rs(rs);
  LCD_DATA_PORT.OUT = b;         // assign data
  enable_puls();
}

static void lcd4_write_byte (uint8_t b, uint8_t rs) {
  set_rs(rs);
  write_high_nibble(b);
  enable_puls();
  write_low_nibble(b);
  enable_puls();
  _delay_us(TDELAY_us);
}

static void lcd4bf_write_byte (uint8_t b, uint8_t rs) {
  uint8_t x;

  LCD_DATA_PORT.DIRCLR  = LCD_DATA_PORT_gm;   // read data
  LCD_RW_PORT.OUTSET    = LCD_RW_bm;          // R/W high
  LCD_RS_PORT.OUTCLR    = LCD_RS_bm;          // RS low (command)
  do {
    LCD_E_PORT.OUTSET  = LCD_E_bm;            // make E high
    _delay_us(TPWE_us);
    x = (((LCD_DATA_PORT.IN & LCD_D7_bm) >> LCD_D7_bp) << 7) |
        (((LCD_DATA_PORT.IN & LCD_D6_bm) >> LCD_D6_bp) << 6) |
        (((LCD_DATA_PORT.IN & LCD_D5_bm) >> LCD_D5_bp) << 5) |
       (((LCD_DATA_PORT.IN & LCD_D4_bm) >> LCD_D4_bp) << 4) ;

    LCD_E_PORT.OUTCLR  = LCD_E_bm;            // make E low
    _delay_us(TPWE_us);
    LCD_E_PORT.OUTSET  = LCD_E_bm;            // make E high
    _delay_us(TPWE_us);
    x |= (((LCD_DATA_PORT.IN & LCD_D7_bm) >> LCD_D7_bp) << 3) |
         (((LCD_DATA_PORT.IN & LCD_D6_bm) >> LCD_D6_bp) << 2) |
         (((LCD_DATA_PORT.IN & LCD_D5_bm) >> LCD_D5_bp) << 1) |
         (((LCD_DATA_PORT.IN & LCD_D4_bm) >> LCD_D4_bp)) ;

    LCD_E_PORT.OUTCLR  = LCD_E_bm;            // make E low
  } while (x & (1<<LCD_BUSY_bp));

  LCD_DATA_PORT.DIRSET  = LCD_DATA_PORT_gm;   // write data
  LCD_RW_PORT.OUTCLR    = LCD_RW_bm;          // R/W low

  set_rs(rs);
  write_high_nibble(b);
  enable_puls();
  write_low_nibble(b);
  enable_puls();
}

static inline void lcd8_init(void)
{
  LCD_DATA_PORT.DIR    = LCD_DATA_PORT_gm;     // 8-bits data port are outputs
  LCD_COMM_PORT.DIRSET = LCD_RS_bm|LCD_E_bm;   // RS and E are outputs

  start_init_byte();
  lcd8_write_byte(LCD_FUNCTION_8BIT_2LINES,0);
  lcd8_write_byte(LCD_DISP_ON,0);
  lcd8_write_byte(LCD_ENTRY_INC,0);
  lcd_clear();
}

static inline void lcd8bf_init(void)
{
  LCD_DATA_PORT.DIR    = LCD_DATA_PORT_gm;     // 8-bits data port are outputs
  LCD_COMM_PORT.DIRSET = LCD_RS_bm|LCD_E_bm;   // RS and E are outputs
  LCD_COMM_PORT.DIRSET = LCD_RW_bm;            // RW is output
  LCD_COMM_PORT.OUTCLR = LCD_RW_bm;            // RW is low

  start_init_byte();
  lcd8_write_byte(LCD_FUNCTION_8BIT_2LINES,0);
  lcd8bf_write_byte(LCD_DISP_ON,0);
  lcd8bf_write_byte(LCD_ENTRY_INC,0);
  lcd_clear();
}

static inline void lcd4_init(void)
{
  LCD_DATA_PORT.DIRSET  = LCD_DATA_PORT_gm;     // 4-bits data port are outputs
  LCD_COMM_PORT.DIRSET  = LCD_RS_bm|LCD_E_bm;   // RS and E are outputs

  start_init_nibble();

  lcd4_write_byte(LCD_FUNCTION_4BIT_2LINES,0);
  lcd4_write_byte(LCD_DISP_ON,0);
  lcd4_write_byte(LCD_ENTRY_INC,0);
  lcd_clear();
}

static inline void lcd4bf_init(void)
{
  LCD_DATA_PORT.DIRSET = LCD_DATA_PORT_gm;     // 4-bits data port are outputs
  LCD_COMM_PORT.DIRSET = LCD_RS_bm|LCD_E_bm;   // RS and E are outputs
  LCD_COMM_PORT.DIRSET = LCD_RW_bm;            // RW is output
  LCD_COMM_PORT.OUTCLR = LCD_RW_bm;            // RW is output

  start_init_nibble();

  lcd4bf_write_byte(LCD_FUNCTION_4BIT_2LINES,0);
  lcd4bf_write_byte(LCD_DISP_ON,0);
  lcd4bf_write_byte(LCD_ENTRY_INC,0);
  lcd_clear();
}

volatile uint8_t lcd_line = 0;   //!< Current line number (0 is first line)

/*! \brief Initialize the lcd.
 *
 *  This function initializes the LCD in one of the four modes depending
 *  on de values of LCD_4BIT_MODE and LCD_BUSY_FLAG in the header file.
 *
 *  \return           none
 */
void lcd_init(void)
{
  LCD_INIT();
}

/*! \brief Writes a character to the LCD.
 *
 *  This function writes a character to the LCD.
 *  The characters '\\n' and '\\f' have a special meaning
 *  - '\\n' (new line) :  go to the start of the next line
 *  - '\\f' (formfeed) :  clears display and start at the home position
 *
 *  \param  c         the character to be written
 *
 *  \return           none
 */
void lcd_putc(char c)
{
  switch (c) {
    case '\f':
      lcd_clear();
      lcd_line = 0;
      break;
    case '\n':
      if (++lcd_line==LCD_LINES) lcd_line = 0;
      lcd_gotoxy(0, lcd_line);
      break;
    default:
      LCD_WRITE_BYTE(c, 1);
      break;
  }
}

/*! \brief Writes a command char to the LCD.
 *
 *  This function writes a command char to the LCD.
 *
 *  \param  cmd       command character
 *
 *  \return           none
 */
void lcd_cmd(uint8_t cmd)
{
  LCD_WRITE_BYTE(cmd, 0);
}

/*! \brief Writes a data byte to the LCD.
 *
 *  This function writes a data byte to the LCD.
 *
 *  \param  b         data byte
 *
 *  \return           none
 */
void lcd_data(uint8_t b)
{
  LCD_WRITE_BYTE(b, 1);
}

/*! \brief Writes a string to the LCD.
 *
 *  This function writes a character string to the LCD.
 *
 *  \param  s         pointer to the character string
 *
 *  \return           none
 */
void lcd_puts(char *s)
{
  char c;

  while ( (c = *s++) ) {
    lcd_putc(c);
  }
}

/*! \brief Set cursor to specified position.
 *
 *  This function sets the cursor to the specified position.
 *
 *  \param  x         horizontal position (0: left most position)
 *  \param  y         vertical   position (0: first line)
 *
 *  \return           none
 */
void lcd_gotoxy(uint8_t x, uint8_t y)
{
  uint8_t address;

#if LCD_LINES==1
  address = LCD_START_LINE1;
#elif LCD_LINES==2
  if ( y==0 ) {
    address = LCD_START_LINE1;
  } else {
    address = LCD_START_LINE2;
  }
#else
  if ( y==0 )
    address = LCD_START_LINE1;
  } else if ( y==1)
    address = LCD_START_LINE2;
  } else if ( y==2)
    address = LCD_START_LINE3;
  } else {
    address = LCD_START_LINE4;
  }
#endif

  LCD_WRITE_BYTE((1<<LCD_DDRAM_bp) | (address + x), 0);
}

/*! \brief Clear lcd.
 *
 *  This function clears the LCD and sets cursor to home position.
 *
 *  \return           none
 */
void lcd_clear(void)
{
  lcd_cmd(1<<LCD_CLR_bp);
  lcd_line = 0;
  _delay_us(T_CLEARDISPLAY_us);
}

/*! \brief Cursor to home position.
 *
 *  This function sets cursor to home position.
 *
 *  \return           none
 */
void lcd_home(void)
{
  lcd_cmd(1<<LCD_HOME_bp);
  lcd_line = 0;
  _delay_us(T_CLEARDISPLAY_us);
}
