#ifndef __DEBUG_H__
#define __DEBUG_H__

/** @file
 * This file defines debug levels.
 * Debugging support vanishes in non-debug builds.
 */

/**
 * The Debug Level bit-mask variable.
 */
extern unsigned int g_dbg_lvl;

/** Display error messages */
#define DL_ERR		(0x00000001)
/** Display warning messages */
#define DL_WARN	(0x00000010)
/** Display messages about test sequence */
#define DL_TEST	(0x00000100)
/** Display messages about parser module */
#define DL_PARSER	(0x00001000)
/** Display messages about vioc module */
#define DL_VIOC	(0x00010000)
/** Display messages about verification module */
#define DL_VERIFY	(0x00100000)
///** Display messages */
//#define DL_TEST	(0x01000000)
///** Display messages */
//#define DL_TEST	(0x10000000)

/** All debug messages off */
#define DBG_OFF		(0)

/**
 * Set the Debug Level variable.
 */
static inline unsigned int SET_DEBUG_LEVEL(const unsigned int new)
{
    unsigned int old = g_dbg_lvl;
    g_dbg_lvl = new;
    return old;
}

/*
The Color Code:     <ESC>[{attr};{fg};{bg}m

{attr} is one of following
    0   Reset All Attributes (return to normal mode)
    1   Bright (Usually turns on BOLD)
    2   Dim
    3   Underline
    5   Blink
    7   Reverse
    8   Hidden

{fg} is one of the following
    30  Black
    31  Red
    32  Green
    33  Yellow
    34  Blue
    35  Magenta
    36  Cyan
    37  White

{bg} is one of the following
    40  Black
    41  Red
    42  Green
    43  Yellow
    44  Blue
    45  Magenta
    46  Cyan
    47  White
*/

/* DEBUG COLOR */
#define C_GREY          30
#define C_RED           31
#define C_GREEN         32
#define C_YELLOW        33
#define C_BLUE          34
#define C_MAGENTA       35
#define C_LBLUE		    36
#define C_WHITE         37

#define __DBG_PRINT(fmt, args...) printf("\e[%dm[%s:%d]\e[0m " fmt, C_YELLOW, __func__, __LINE__, ## args)

#define __DBG(fmt, args...) \
		do { \
			printf("\e[%dm[%s:%d]\e[0m " fmt, C_GREEN, __func__, __LINE__, ## args); \
		} while(0)

/**
 * Debugging functions
 */
#define DBG_ERR(fmt, args...) \
		do { \
			printf("\e[%dm[%s:%d] error:\e[0m " fmt, C_RED, __func__, __LINE__, ## args); \
		} while(0)

#define DBG_WARN(lvl, fmt, args...) \
		do { if ((lvl) & g_dbg_lvl) \
			printf("\e[%dm[%s:%d] warning:\e[0m " fmt, C_RED, __func__, __LINE__, ## args); \
		} while(0)

#define DBG(lvl, fmt, args...) \
		do { \
			if ((lvl) & g_dbg_lvl) \
				__DBG_PRINT(fmt, ## args); \
		} while(0)

#endif /* __DEBUG_H__ */
