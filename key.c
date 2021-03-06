/* hid_gadget_test */
#include <regex.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 512

struct options {
	const char    *opt;
	unsigned char val;
	unsigned char caractere;
 	unsigned char report[8];
};

static struct options kmod[] = {
	{.opt = "left-ctrl",		.val = 0x01},
	{.opt = "right-ctrl",		.val = 0x10},
	{.opt = "left-shift",		.val = 0x02},
	{.opt = "right-shift",		.val = 0x20},
	{.opt = "left-alt",		.val = 0x04},
	{.opt = "right-alt",		.val = 0x40},
	{.opt = "left-meta",		.val = 0x08},
	{.opt = "right-meta",		.val = 0x80},
	{.opt = NULL}
};


//USB-HID Belgian fr Keyboard keycodes
//
static struct options kval[] = {
	{.opt = "a",			.val = 0x14, .caractere = 'a',.report= {0x00,0x00,0x14,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "A",			.val = 0x14, .caractere = 'A',.report= {0x02,0x00,0x14,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "b",			.val = 0x05, .caractere = 'b',.report= {0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "B",			.val = 0x05, .caractere = 'B',.report= {0x02,0x00,0x05,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "c",			.val = 0x06, .caractere = 'c',.report= {0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "C",			.val = 0x06, .caractere = 'C',.report= {0x02,0x00,0x06,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "d",			.val = 0x07, .caractere = 'd',.report= {0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "D",			.val = 0x07, .caractere = 'D',.report= {0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "e",			.val = 0x08, .caractere = 'e',.report= {0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "E",			.val = 0x08, .caractere = 'E',.report= {0x02,0x00,0x08,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "f",			.val = 0x09, .caractere = 'f',.report= {0x00,0x00,0x09,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "F",			.val = 0x09, .caractere = 'F',.report= {0x02,0x00,0x09,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "g",			.val = 0x0a, .caractere = 'g',.report= {0x00,0x00,0x0a,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "G",			.val = 0x0a, .caractere = 'G',.report= {0x02,0x00,0x0a,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "h",			.val = 0x0b, .caractere = 'h',.report= {0x00,0x00,0x0b,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "H",			.val = 0x0b, .caractere = 'H',.report= {0x02,0x00,0x0b,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "i",			.val = 0x0c, .caractere = 'i',.report= {0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "I",			.val = 0x0c, .caractere = 'I',.report= {0x02,0x00,0x0c,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "j",			.val = 0x0d, .caractere = 'j',.report= {0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "J",			.val = 0x0d, .caractere = 'J',.report= {0x02,0x00,0x0d,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "k",			.val = 0x0e, .caractere = 'k',.report= {0x00,0x00,0x0e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "K",			.val = 0x0e, .caractere = 'K',.report= {0x02,0x00,0x0e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "l",			.val = 0x0f, .caractere = 'l',.report= {0x00,0x00,0x0f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "L",			.val = 0x0f, .caractere = 'L',.report= {0x02,0x00,0x0f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "m",			.val = 0x33, .caractere = 'm',.report= {0x00,0x00,0x33,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "M",			.val = 0x33, .caractere = 'M',.report= {0x02,0x00,0x33,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "n",			.val = 0x11, .caractere = 'n',.report= {0x00,0x00,0x11,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "N",			.val = 0x11, .caractere = 'N',.report= {0x02,0x00,0x11,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "o",			.val = 0x12, .caractere = 'o',.report= {0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "O",			.val = 0x12, .caractere = 'O',.report= {0x02,0x00,0x12,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "q",			.val = 0x04, .caractere = 'q',.report= {0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "Q",			.val = 0x04, .caractere = 'q',.report= {0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "p",			.val = 0x13, .caractere = 'p',.report= {0x00,0x00,0x13,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "P",			.val = 0x13, .caractere = 'P',.report= {0x02,0x00,0x13,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "r",			.val = 0x15, .caractere = 'r',.report= {0x00,0x00,0x15,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "R",			.val = 0x15, .caractere = 'R',.report= {0x02,0x00,0x15,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "s",			.val = 0x16, .caractere = 's',.report= {0x00,0x00,0x16,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "S",			.val = 0x16, .caractere = 'S',.report= {0x02,0x00,0x16,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "t",			.val = 0x17, .caractere = 't',.report= {0x00,0x00,0x17,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "T",			.val = 0x17, .caractere = 'T',.report= {0x02,0x00,0x17,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "u",			.val = 0x18, .caractere = 'u',.report= {0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "U",			.val = 0x18, .caractere = 'U',.report= {0x02,0x00,0x18,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "v",			.val = 0x19, .caractere = 'v',.report= {0x00,0x00,0x19,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "V",			.val = 0x19, .caractere = 'V',.report= {0x02,0x00,0x19,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "w",			.val = 0x1d, .caractere = 'w',.report= {0x00,0x00,0x1d,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "W",			.val = 0x1d, .caractere = 'W',.report= {0x02,0x00,0x1d,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "x",			.val = 0x1b, .caractere = 'x',.report= {0x00,0x00,0x1b,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "X",			.val = 0x1b, .caractere = 'X',.report= {0x02,0x00,0x1b,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "y",			.val = 0x1c, .caractere = 'y',.report= {0x00,0x00,0x1c,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "Y",			.val = 0x1c, .caractere = 'Y',.report= {0x02,0x00,0x1c,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "z",			.val = 0x1a, .caractere = 'z',.report= {0x00,0x00,0x1a,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "Z",			.val = 0x1a, .caractere = 'Z',.report= {0x02,0x00,0x1a,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "1",			.val = 0x1e, .caractere = '1',.report= {0x02,0x00,0x1e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "2",			.val = 0x1f, .caractere = '2',.report= {0x02,0x00,0x1f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "3",			.val = 0x20, .caractere = '3',.report= {0x02,0x00,0x20,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "4",			.val = 0x21, .caractere = '4',.report= {0x02,0x00,0x21,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "5",			.val = 0x22, .caractere = '5',.report= {0x02,0x00,0x22,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "6",			.val = 0x23, .caractere = '6',.report= {0x02,0x00,0x23,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "7",			.val = 0x24, .caractere = '7',.report= {0x02,0x00,0x24,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "8",			.val = 0x25, .caractere = '8',.report= {0x02,0x00,0x25,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "9",			.val = 0x26, .caractere = '9',.report= {0x02,0x00,0x26,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "0",			.val = 0x27, .caractere = '0',.report= {0x02,0x00,0x27,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "space",		.val = 0x2c, .caractere = ' ',.report= {0x00,0x00,0x2c,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "pipe",			.val = 0x1e, .caractere = '|',.report= {0x40,0x00,0x1e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "ampersand",		.val = 0x1e, .caractere = '&',.report= {0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "bang",			.val = 0x25, .caractere = '!',.report= {0x00,0x00,0x25,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "ccd",			.val = 0x26, .caractere = ' ',.report= {0x00,0x00,0x26,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "aac",			.val = 0x27, .caractere = ' ',.report= {0x00,0x00,0x27,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "eaa",			.val = 0x1f, .caractere = ' ',.report= {0x00,0x00,0x1f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "eag",			.val = 0x24, .caractere = ' ',.report= {0x00,0x00,0x24,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "paragraph",		.val = 0x23, .caractere = ' ',.report= {0x00,0x00,0x23,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "plus",	 		.val = 0x38, .caractere = '+',.report= {0x20,0x00,0x38,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "minus",		.val = 0x2d, .caractere = '-',.report= {0x00,0x00,0x2e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "underscore",		.val = 0x2d, .caractere = '-',.report= {0x20,0x00,0x2e,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "equals",		.val = 0x38, .caractere = '=',.report={0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "lbracket",		.val = 0x2e, .caractere = '(',.report={0x00,0x00,0x22,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "rbracket",		.val = 0x30, .caractere = '(',.report={0x00,0x00,0x2d,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "lsbracket",		.val = 0x2f, .caractere = '[',.report={0x40,0x00,0x2f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "rsbracket",		.val = 0x30, .caractere = ']',.report={0x40,0x00,0x30,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "dollars",		.val = 0x30, .caractere = '$',.report={0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "asterisk",		.val = 0x30, .caractere = '*',.report={0x02,0x00,0x30,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "euro",			.val = 0x08, .caractere = ' ',.report={0x40,0x00,0x08,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "lcbracket",		.val = 0x26, .caractere = '{',.report={0x40,0x00,0x26,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "rcbracket",		.val = 0x27, .caractere = '}',.report={0x40,0x00,0x27,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "backslash",		.val = 0x32, .caractere = '\\',.report={0x00,0x00,0x39,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "lt",		 	.val = 0x64, .caractere = '<',.report={0x00,0x00,0x64,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "gt",			.val = 0x64, .caractere = '>',.report={0x20,0x00,0x64,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "hash",			.val = 0x64, .caractere = '#',.report={0x40,0x00,0x64,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "degree",		.val = 0x2d, .caractere = ' ',.report={0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "comma",		.val = 0x10, .caractere = ',',.report={0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "question",		.val = 0x10, .caractere = '?',.report= {0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "quote",		.val = 0x21, .caractere = '\'',.report={0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "doublequote",		.val = 0x20, .caractere = '"',.report={0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "backquote",		.val = 0x31, .caractere = '`',.report={0x40,0x00,0x31,0x2c,0x00,0x00,0x00,0x00} },
	{.opt = "tilde",		.val = 0x38, .caractere = '~',.report={0x40,0x00,0x38,0x2c,0x00,0x00,0x00,0x00} },
	{.opt = "percent",		.val = 0x34, .caractere = '%',.report={0x20,0x00,0x34,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "uac",			.val = 0x34, .caractere = ' ',.report={0x00,0x00,0x34,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "mu",			.val = 0x31, .caractere = ' ',.report={0x00,0x00,0x31,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "semicolon",		.val = 0x36, .caractere = ';',.report={0x00,0x00,0x36,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "period",		.val = 0x36, .caractere = ';',.report={0x20,0x00,0x36,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "colon",		.val = 0x37, .caractere = ':',.report={0x00,0x00,0x37,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "slash",		.val = 0x37, .caractere = ':',.report={0x20,0x00,0x37,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "at",			.val = 0x1f, .caractere = '@',.report={0x40,0x00,0x1f,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "return",		.val = 0x28, .caractere = '\n',.report= {0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "tab",			.val = 0x2b, .caractere = '\t',.report= {0x00,0x00,0x2b,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "backspace",		.val = 0x2a, .caractere = ' ',.report= {0x00,0x00,0x2a,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "esc",		        .val = 0x29, .caractere = ' ',.report= {0x00,0x00,0x29,0x00,0x00,0x00,0x00,0x00} },
	{.opt = "caps-lock",		.val = 0x39, .caractere = ' '},
	{.opt = "capslock",		.val = 0x39, .caractere = ' '},
	{.opt = "f1",			.val = 0x3a, .caractere = ' '},
	{.opt = "f2",			.val = 0x3b, .caractere = ' '},
	{.opt = "f3",			.val = 0x3c, .caractere = ' '},
	{.opt = "f4",			.val = 0x3d, .caractere = ' '},
	{.opt = "f5",			.val = 0x3e, .caractere = ' '},
	{.opt = "f6",			.val = 0x3f, .caractere = ' '},
	{.opt = "f7",			.val = 0x40, .caractere = ' '},
	{.opt = "f8",			.val = 0x41, .caractere = ' '},
	{.opt = "f9",			.val = 0x42, .caractere = ' '},
	{.opt = "f10",			.val = 0x43, .caractere = ' '},
	{.opt = "f11",			.val = 0x44, .caractere = ' '},
	{.opt = "f12",			.val = 0x45, .caractere = ' '},
	{.opt = "print",		.val = 0x46, .caractere = ' '},
	{.opt = "scroll-lock",		.val = 0x47, .caractere = ' '},
	{.opt = "scrolllock",		.val = 0x47, .caractere = ' '},
	{.opt = "pause",		.val = 0x48, .caractere = ' '},
	{.opt = "insert",		.val = 0x49, .caractere = ' '},
	{.opt = "home",			.val = 0x4a, .caractere = ' '},
	{.opt = "pageup",		.val = 0x4b, .caractere = ' '},
	{.opt = "pgup",			.val = 0x4b, .caractere = ' '},
	{.opt = "del",			.val = 0x4c, .caractere = ' '},
	{.opt = "delete",		.val = 0x4c, .caractere = ' '},
	{.opt = "end",			.val = 0x4d, .caractere = ' '},
	{.opt = "pagedown",		.val = 0x4e, .caractere = ' '},
	{.opt = "pgdown",		.val = 0x4e, .caractere = ' '},
	{.opt = "right",		.val = 0x4f, .caractere = ' '},
	{.opt = "left",			.val = 0x50, .caractere = ' '},
	{.opt = "down",			.val = 0x51, .caractere = ' '},
	{.opt = "up",			.val = 0x52, .caractere = ' '},
	{.opt = "num-lock",		.val = 0x53, .caractere = ' '},
	{.opt = "numlock",		.val = 0x53, .caractere = ' '},
	{.opt = "kp-divide",		.val = 0x54, .caractere = '/'},
	{.opt = "kp-multiply",		.val = 0x55, .caractere = '*'},
	{.opt = "kp-minus",		.val = 0x56, .caractere = '-'},
	{.opt = "kp-plus",		.val = 0x57, .caractere = '+'},
	{.opt = "kp-enter",		.val = 0x58, .caractere = ' '},
	{.opt = "kp-return",		.val = 0x58, .caractere = ' '},
	{.opt = "kp-1",			.val = 0x59, .caractere = '1'},
	{.opt = "kp-2",			.val = 0x5a, .caractere = '2'},
	{.opt = "kp-3",			.val = 0x5b, .caractere = '3'},
	{.opt = "kp-4",			.val = 0x5c, .caractere = '4'},
	{.opt = "kp-5",			.val = 0x5d, .caractere = '5'},
	{.opt = "kp-6",			.val = 0x5e, .caractere = '6'},
	{.opt = "kp-7",			.val = 0x5f, .caractere = '7'},
	{.opt = "kp-8",			.val = 0x60, .caractere = '8'},
	{.opt = "kp-9",			.val = 0x61, .caractere = '9'},
	{.opt = "kp-0",			.val = 0x62, .caractere = '0'},
	{.opt = "kp-period",		.val = 0x63, .caractere = '.'},
	{.opt = "kp-stop",		.val = 0x63, .caractere = ' '},
	{.opt = "application",		.val = 0x65, .caractere = '.'},
	{.opt = "power",		.val = 0x66, .caractere = '.'},
	{.opt = "kp-equals",		.val = 0x67, .caractere = '='},
	{.opt = "kp-equal",		.val = 0x67, .caractere = '='},
	{.opt = "f13",			.val = 0x68, .caractere = '.'},
	{.opt = "f14",			.val = 0x69, .caractere = '.'},
	{.opt = "f15",			.val = 0x6a, .caractere = '.'},
	{.opt = "f16",			.val = 0x6b, .caractere = '.'},
	{.opt = "f17",			.val = 0x6c, .caractere = '.'},
	{.opt = "f18",			.val = 0x6d, .caractere = '.'},
	{.opt = "f19",			.val = 0x6e, .caractere = '.'},
	{.opt = "f20",			.val = 0x6f, .caractere = '.'},
	{.opt = "f21",			.val = 0x70, .caractere = '.'},
	{.opt = "f22",			.val = 0x71, .caractere = '.'},
	{.opt = "f23",			.val = 0x72, .caractere = '.'},
	{.opt = "f24",			.val = 0x73, .caractere = '.'},
	{.opt = "execute",		.val = 0x74, .caractere = '.'},
	{.opt = "help",			.val = 0x75, .caractere = '.'},
	{.opt = "menu",			.val = 0x76, .caractere = '.'},
	{.opt = "select",		.val = 0x77, .caractere = '.'},
	{.opt = "cancel",		.val = 0x78, .caractere = '.'},
	{.opt = "redo",			.val = 0x79, .caractere = '.'},
	{.opt = "undo",			.val = 0x7a, .caractere = '.'},
	{.opt = "cut",			.val = 0x7b, .caractere = '.'},
	{.opt = "copy",			.val = 0x7c, .caractere = '.'},
	{.opt = "paste",		.val = 0x7d, .caractere = '.'},
	{.opt = "find",			.val = 0x7e, .caractere = '.'},
	{.opt = "mute",			.val = 0x7f, .caractere = '.'},
	{.opt = "volume-up",		.val = 0x80, .caractere = '.'}, // These are multimedia keys, they will not work on standard keyboard, they need a different USB descriptor
	{.opt = "volume-down",		.val = 0x81, .caractere = '.'},
	{.opt = NULL}
};




int main(int argc, const char *argv[])
{
	char filename[] = "/dev/hidg0\0";
	const char off[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	int fd = 0;
	int i;
 	
	fd=open(filename,O_RDWR,0666);	
	while ( strcmp(kval[i].opt,"return") != 0) {
			write(fd,kval[i].report,8);
		        usleep(5000);
		        write(fd,off,8);
		        usleep(5000);
		i++;
		}
       
}
