/*!
 *  \file    lcd.h
 *  \author  Wim Dolman (<a href="email:w.e.dolman@hva.nl">w.e.dolman@hva.nl</a>)
 *  \date    21-10-2013
 *  \version 1.0
 *
 *  \brief   macro's for lcd driver for Xmega
 *
 *  \details This file contains the macro definitions for interfacing a HD44780U-based
 *           text lcd display for Atmel Xmega devices.
 *
 *           This file and the accompanying file lcd.c are based on the lcd libray
 *           from Peter Fleury (<a href="http://homepage.hispeed.ch/peterfleury/">http://homepage.hispeed.ch/peterfleury/</a>)
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
 *           in any order. This file contains the macro definitions that
 *           for this purposes.
 *
 *           The control lines RS, E and R/W can connected to any pin of any port
 *           of the Xmega.
 *
 *           \warning
 *           Be careful using the busyflag. Most alfanumeric displays are 5 Volt devices.
 *           The Xmega is not 5 Volt tolerant.
 *           When using the busyflag, information is also send from the LCD to the
 *           Xmega. So you can damage your Xmega.
 */
/*!
 *  \brief F_CPU must be declared
 */
#ifndef F_CPU
#define F_CPU             2000000UL
#endif

/*!
 *  \brief Macro defining the 4-bit mode (1) or the 8-bit mode (0)
 */
#define LCD_4BIT_MODE     1
/*!
 *  \brief Macro defining that you want to use the busy flag (1) or not (0)
 */
#define LCD_BUSY_FLAG     0

/*!
 *  \brief Macro's to define the data port
 */
#define LCD_DATA_PORT     PORTA
#define LCD_D0_PORT       LCD_DATA_PORT  //!< Port D0-pin  (currently not used)
#define LCD_D1_PORT       LCD_DATA_PORT  //!< Port D1-pin  (currently not used)
#define LCD_D2_PORT       LCD_DATA_PORT  //!< Port D2-pin  (currently not used)
#define LCD_D3_PORT       LCD_DATA_PORT  //!< Port D3-pin  (currently not used)
#define LCD_D4_PORT       LCD_DATA_PORT  //!< Port D4-pin  (currently not used)
#define LCD_D5_PORT       LCD_DATA_PORT  //!< Port D5-pin  (currently not used)
#define LCD_D6_PORT       LCD_DATA_PORT  //!< Port D6-pin  (currently not used)
#define LCD_D7_PORT       LCD_DATA_PORT  //!< Port D7-pin  (currently not used)
#define LCD_D0_bp         PIN0_bp        //!< Bit position D0-pin (currently not used)
#define LCD_D1_bp         PIN1_bp        //!< Bit position D1-pin (currently not used)
#define LCD_D2_bp         PIN2_bp        //!< Bit position D2-pin (currently not used)
#define LCD_D3_bp         PIN3_bp        //!< Bit position D3-pin (currently not used)
#define LCD_D4_bp         PIN4_bp        //!< Bit position D4-pin
#define LCD_D5_bp         PIN5_bp        //!< Bit position D5-pin
#define LCD_D6_bp         PIN6_bp        //!< Bit position D6-pin
#define LCD_D7_bp         PIN7_bp        //!< Bit position D7-pin

/*!
 *  \brief Macro's to define the communication port
 */
#define LCD_COMM_PORT     PORTD
#define LCD_RS_PORT       LCD_COMM_PORT  //!< Port RS-pin
#define LCD_RW_PORT       LCD_COMM_PORT  //!< Port R/W-pin
#define LCD_E_PORT        LCD_COMM_PORT  //!< Port E-pin
#define LCD_RS_bp         PIN4_bp        //!< Bit position RS-pin
#define LCD_RW_bp         PIN3_bp        //!< Bit position R/W-pin
#define LCD_E_bp          PIN5_bp        //!< Bit position E-pin

#define TDELAY1_ms        50             //!< Time Delay 1st initialization
#define TDELAY2_ms        5              //!< Time Delay 2nd initialization
#define TDELAY3_us        100            //!< Time Delay 3rd initialization
#define TPWE_us           0.5            //!< Time Period Width Enable (TpwE)
#define TDELAY_us         50             //!< Time Delay commands and data
#define T_CLEARDISPLAY_us 1600           //!< Time Delay clear display

#define LCD_LINES          2             //!< Number of visible lines of the display
#define LCD_DISP_LENGTH    16            //!< Visible characters per line of the display

#if LCD_DISP_LENGTH==16
#define LCD_START_LINE1    0x00          //!< DDRAM address of first char of line 1
#define LCD_START_LINE2    0x40          //!< DDRAM address of first char of line 2
#define LCD_START_LINE3    0x10          //!< DDRAM address of first char of line 3
#define LCD_START_LINE4    0x50          //!< DDRAM address of first char of line 4
#else
#define LCD_START_LINE1    0x00          //!< DDRAM address of first char of line 1
#define LCD_START_LINE2    0x40          //!< DDRAM address of first char of line 2
#define LCD_START_LINE3    0x14          //!< DDRAM address of first char of line 3
#define LCD_START_LINE4    0x54          //!< DDRAM address of first char of line 4
#endif

#define LCD_CLR_bp               0      //!< DB0: clear display
#define LCD_HOME_bp              1      //!< DB1: return to home position
#define LCD_ENTRY_MODE_bp        2      //!< DB2: set entry mode
#define LCD_ENTRY_INC_bp         1      //!<   DB1: 1=increment, 0=decrement
#define LCD_ENTRY_SHIFT_bp       0      //!<   DB2: 1=display shift on
#define LCD_ON_bp                3      //!< DB3: turn lcd/cursor on
#define LCD_ON_DISPLAY_bp        2      //!<   DB2: turn display on
#define LCD_ON_CURSOR_bp         1      //!<   DB1: turn cursor on
#define LCD_ON_BLINK_bp          0      //!<     DB0: blinking cursor ?
#define LCD_MOVE_bp              4      //!< DB4: move cursor/display
#define LCD_MOVE_DISP_bp         3      //!<   DB3: move display (0-> cursor) ?
#define LCD_MOVE_RIGHT_bp        2      //!<   DB2: move right (0-> left) ?
#define LCD_FUNCTION_bp          5      //!< DB5: function set
#define LCD_FUNCTION_8BIT_bp     4      //!<   DB4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES_bp   3      //!<   DB3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS_bp   2      //!<   DB2: 5x10 font (0->5x7 font)
#define LCD_CGRAM_bp             6      //!< DB6: set CG RAM address
#define LCD_DDRAM_bp             7      //!< DB7: set DD RAM address
#define LCD_BUSY_bp              7      //!< DB7: LCD is busy

#define LCD_ENTRY_DEC            0x04   //!< display shift off, dec cursor move dir
#define LCD_ENTRY_DEC_SHIFT      0x05   //!< display shift on,  dec cursor move dir
#define LCD_ENTRY_INC            0x06   //!< display shift off, inc cursor move dir
#define LCD_ENTRY_INC_SHIFT      0x07   //!< display shift on,  inc cursor move dir

#define LCD_DISP_OFF             0x08   //!< display off
#define LCD_DISP_ON              0x0C   //!< display on, cursor off
#define LCD_DISP_ON_BLINK        0x0D   //!< display on, cursor off, blink char
#define LCD_DISP_ON_CURSOR       0x0E   //!< display on, cursor on
#define LCD_DISP_ON_CURSOR_BLINK 0x0F   //!< display on, cursor on, blink char

#define LCD_MOVE_CURSOR_LEFT     0x10   //!< move cursor left  (decrement)
#define LCD_MOVE_CURSOR_RIGHT    0x14   //!< move cursor right (increment)
#define LCD_MOVE_DISP_LEFT       0x18   //!< shift display left
#define LCD_MOVE_DISP_RIGHT      0x1C   //!< shift display right

#define LCD_FUNCTION_4BIT        0x20   //!< 4-bit, single line, 5x8 dots
#define LCD_FUNCTION_4BIT_1LINE  0x20   //!< 4-bit, single line, 5x8 dots
#define LCD_FUNCTION_4BIT_2LINES 0x28   //!< 4-bit, dual line,   5x8 dots
#define LCD_FUNCTION_8BIT        0x30   //!< 8-bit, single line, 5x8 dots
#define LCD_FUNCTION_8BIT_1LINE  0x30   //!< 8-bit, single line, 5x8 dots
#define LCD_FUNCTION_8BIT_2LINES 0x38   //!< 8-bit, dual line,   5x8 dots

void lcd_init(void);
void lcd_clear(void);
void lcd_home(void);
void lcd_gotoxy(uint8_t x, uint8_t y);
void lcd_putc(char c);
void lcd_puts(char *s);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t b);

#define LCD_D0_bm   (1  << (LCD_D0_bp))   //!< Bit mask D0-pin
#define LCD_D1_bm   (1  << (LCD_D1_bp))   //!< Bbit mask D1-pin
#define LCD_D2_bm   (1  << (LCD_D2_bp))   //!< Bit mask D2-pin
#define LCD_D3_bm   (1  << (LCD_D3_bp))   //!< Bit mask D3-pin
#define LCD_D4_bm   (1  << (LCD_D4_bp))   //!< Bit mask D4-pin
#define LCD_D5_bm   (1  << (LCD_D5_bp))   //!< Bit mask D5-pin
#define LCD_D6_bm   (1  << (LCD_D6_bp))   //!< Bit mask D6-pin
#define LCD_D7_bm   (1  << (LCD_D7_bp))   //!< Bit mask D7-pin
#define LCD_RS_bm   (1  << (LCD_RS_bp))   //!< Bit mask RS-pin
#define LCD_RW_bm   (1  << (LCD_RW_bp))   //!< Bit mask R/W-pin
#define LCD_E_bm    (1  << (LCD_E_bp))    //!< Bit mask E-pin

/*! \def LCD_DATA_PORT_gm
 *  \brief Group mask for data port
*/
/*! \def LCD_WRITE_BYTE(b,rs)
 *  \brief Writes byte tot LCD
 *
 *  This function writes a byte to the LCD.
 *
 *  \param  b       the byte
 *  \param  rs      register select (0 is coomand, 1 is data)
 *
 */
/*! \def LCD_INIT
 *  \brief Initalizes LCD
 */

#if LCD_BUSY_FLAG==1
#if LCD_4BIT_MODE==1
#define LCD_DATA_PORT_gm         ((LCD_D7_bm)|(LCD_D6_bm)|(LCD_D5_bm)|(LCD_D4_bm))
#define LCD_WRITE_BYTE(b,rs)     (lcd4bf_write_byte((b),(rs)))
#define LCD_INIT                 lcd4bf_init
#else
#define LCD_DATA_PORT_gm         (0xFF)
#define LCD_WRITE_BYTE(b,rs)     (lcd8bf_write_byte((b),(rs)))
#define LCD_INIT                 lcd8bf_init
#endif
#else
#if LCD_4BIT_MODE==1
#define LCD_DATA_PORT_gm         ((LCD_D7_bm)|(LCD_D6_bm)|(LCD_D5_bm)|(LCD_D4_bm))
#define LCD_WRITE_BYTE(b,rs)     (lcd4_write_byte((b),(rs)))
#define LCD_INIT                 lcd4_init
#else
#define LCD_DATA_PORT_gm         (0xFF)
#define LCD_WRITE_BYTE(b,rs)     (lcd8_write_byte((b),(rs)))
#define LCD_INIT                 lcd8_init
#endif
#endif

