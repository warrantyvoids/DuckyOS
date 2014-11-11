#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdint.h>

//Two byte flag
#define KEYBOARD_TBF 0xE0

typedef uint16_t keyboard_keycode_t;

void keyboard_handle( keyboard_keycode_t keycode );

#endif