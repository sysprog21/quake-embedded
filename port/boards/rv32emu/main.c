/*
 * Copyright (C) 2022 National Cheng Kung University, Taiwan.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <quakembd.h>
#include <quakedef.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_RIGHT 3

enum {
	KEY_EVENT = 0,
	MOUSE_MOTION_EVENT = 1,
	MOUSE_BUTTON_EVENT = 2,
};

typedef struct {
	uint8_t button;
	uint8_t state;
} mouse_button_t;

typedef struct {
	uint32_t type;
	union {
		key_event_t key_event;
		union {
			mouse_motion_t motion;
			mouse_button_t button;
		} mouse;
	};
} event_t;

static event_t event;
static mouse_movement_t mouse_movement;

uint64_t qembd_get_us_time()
{
	uint64_t cur_time = (uint64_t) clock();
	static int secbase;

	if (!secbase) {
		secbase = cur_time / 1000000;
		return (uint64_t) secbase;
	}

	return cur_time;
}

void qembd_udelay(uint32_t us)
{
	uint64_t start = qembd_get_us_time(), end;
	end = start;
	while (end - start < 1)
		end = qembd_get_us_time();
}

int main(int c, char **v)
{
	return qembd_main(c, v);
}

void *qembd_allocmain(size_t size)
{
	return malloc(size);
}

static int poll_event()
{
	register int a0 asm("a0") = (uintptr_t) &event;
	register int a7 asm("a7") = 0xc0de;
	asm volatile("scall" : "+r"(a0) : "r"(a7));
	if (event.type == MOUSE_MOTION_EVENT) {
		mouse_movement.x += event.mouse.motion.xrel;
		mouse_movement.y += event.mouse.motion.yrel;
	}
	return a0;
}

int qembd_dequeue_key_event(key_event_t *e)
{
	while (poll_event()) {
		if (event.type == KEY_EVENT) {
			*e = event.key_event;
			/* remap keyode */
			switch (e->keycode) {
			case 0x08:
				e->keycode = K_BACKSPACE;
				break;
			case 0x40000052:
				e->keycode = K_UPARROW;
				break;
			case 0x40000051:
				e->keycode = K_DOWNARROW;
				break;
			case 0x40000050:
				e->keycode = K_LEFTARROW;
				break;
			case 0x4000004F:
				e->keycode = K_RIGHTARROW;
				break;
			case 0x400000E2:
				e->keycode = K_ALT;
				break;
			case 0x400000E0:
				e->keycode = K_CTRL;
				break;
			case 0x400000E1:
				e->keycode = K_SHIFT;
				break;
			case 0x4000003A:
				e->keycode = K_F1;
				break;
			case 0x4000003B:
				e->keycode = K_F2;
				break;
			case 0x4000003C:
				e->keycode = K_F3;
				break;
			case 0x4000003D:
				e->keycode = K_F4;
				break;
			case 0x4000003E:
				e->keycode = K_F5;
				break;
			case 0x4000003F:
				e->keycode = K_F6;
				break;
			case 0x40000040:
				e->keycode = K_F7;
				break;
			case 0x40000041:
				e->keycode = K_F8;
				break;
			case 0x40000042:
				e->keycode = K_F9;
				break;
			case 0x40000043:
				e->keycode = K_F10;
				break;
			case 0x40000044:
				e->keycode = K_F11;
				break;
			case 0x40000045:
				e->keycode = K_F12;
				break;
			case 0x40000049:
				e->keycode = K_INS;
				break;
			case 0x7f:
				e->keycode = K_DEL;
				break;
			case 0x4000004E:
				e->keycode = K_PGDN;
				break;
			case 0x4000004B:
				e->keycode = K_PGUP;
				break;
			case 0x4000004A:
				e->keycode = K_HOME;
				break;
			case 0x4000004D:
				e->keycode = K_END;
				break;
			case 0x40000048:
				e->keycode = K_PAUSE;
				break;
			}
			return 0;
		}
		if (event.type == MOUSE_BUTTON_EVENT) {
			e->state = event.mouse.button.state;
			switch (event.mouse.button.button) {
				case MOUSE_BUTTON_LEFT:
					e->keycode = K_MOUSE1;
					break;
				case MOUSE_BUTTON_RIGHT:
					e->keycode = K_MOUSE2;
					break;
				case MOUSE_BUTTON_MIDDLE:
					e->keycode = K_MOUSE3;
					break;
			}
			return 0;
		}
	}
	return -1;
}

int qembd_get_mouse_movement(mouse_movement_t *movement)
{
	*movement = mouse_movement;
	mouse_movement.x = 0;
	mouse_movement.y = 0;
	return 0;
}
