#include "keyboard.h"

#include <stdbool.h>

#define KEYUP 0x80

static char keymap[] = {
    0, 0x1B, '1', '2',   //0x00
   '3', '4', '5', '6',   //0x04
   '7', '8', '9', '0',   //0x08
   '-', '=', '\b', '\t', //0x0c
   'q', 'w', 'e', 'r',   //0x10
   't', 'y', 'u', 'i',   //0x14
   'o', 'p', '[', ']',   //0x18
   '\n',  0, 'a', 's',   //0x1c
   'd', 'f', 'g', 'h',   //0x20
   'j', 'k', 'l', ';',   //0x24
   '\'', '`', 0, '\\',   //0x28
   'z', 'x', 'c', 'v',   //0x2c
   'b', 'n', 'm', ',',   //0x30
   '.', '/',   0,   0,   //0x34
     0, ' ',   0,   0    //0x38
};

static char keymap_up[] = {
   0, 0x1B, '!', '@',
   '#', '$', '%', '^',
   '&', '*', '(', ')',
   '_', '+', '\b', '\t',
   'Q', 'W', 'E', 'R',
   'T', 'Y', 'U', 'I',
   'O', 'P', '{', '}',
   '\n', 0, 'A', 'S',
   'D', 'F', 'G', 'H',
   'J', 'K', 'L', ':',
   '"', '~', 0, '|',
   'Z', 'X', 'C', 'V',
   'B', 'N', 'M', '<',
   '>', '?',   0,   0,
     0, ' ',   0,   0
};


static bool shiftl_down = false;
static bool shiftr_down = false;
static bool caps_down = false;

void keyboard_handle(keyboard_keycode_t code) {
  if ((code & 0xEF00) == (KEYBOARD_TBF << 8)) {
    //two byte statuscode
  } else {
    if (!(code & KEYUP)) {
//      code ^= KEYUP;
      if (code < (sizeof(keymap) / sizeof(keymap[0]))) {
        char * map;
        if (((shiftl_down || shiftr_down) && !caps_down) || caps_down) {
          map = keymap_up;
        } else {
          map = keymap;
        }
        if (map[code] != 0) {
          terminal_putchar(map[code]);
          return;
        }
      }
      //we're dealing with a special key.
      switch (code) {
        case 0x2A: shiftl_down = true; break;
        case 0x36: shiftr_down = true; break;
        case 0x3A: caps_down = true; break;
      };
    } else {
      code ^= KEYUP;
      switch (code) {
        case 0x2A: shiftl_down = false; break;
        case 0x36: shiftr_down = false; break;
        case 0x3A: caps_down = false; break;
      };
      
    }
  }
}
