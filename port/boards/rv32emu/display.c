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

#define	DISPLAY_WIDTH 640
#define	DISPLAY_HEIGHT 384

static uint32_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

enum {
	syscall_draw_frame = 0xbeef,
};

#define syscall_draw_frame 0xbeef

int qembd_get_width()
{
	return DISPLAY_WIDTH;
}

int qembd_get_height()
{
	return DISPLAY_HEIGHT;
}

void qembd_vidinit()
{
	register int a0 asm("a0") = (uintptr_t) buffer;
	register int a1 asm("a1") = DISPLAY_WIDTH;
	register int a2 asm("a2") = DISPLAY_HEIGHT;
	register int a7 asm("a7") = syscall_draw_frame;

	asm volatile("scall"
	             : "r+"(a0) : "r"(a1), "r"(a2), "r"(a7));
}

void qembd_fillrect(uint8_t *src, uint32_t *clut,
                    uint16_t x, uint16_t y, uint16_t xsize, uint16_t ysize)
{
	for (int py = 0; py < ysize; py++) {
		int offset = (y + py) * DISPLAY_WIDTH + x;
		for (int px = 0; px < xsize; px++)
			buffer[offset + px] = clut[src[offset + px]];
	}
}

void qembd_refresh()
{
	register int a0 asm("a0") = (uintptr_t) buffer;
	register int a1 asm("a1") = DISPLAY_WIDTH;
	register int a2 asm("a2") = DISPLAY_HEIGHT;
	register int a7 asm("a7") = syscall_draw_frame;

	asm volatile("scall"
	             : "r+"(a0) : "r"(a1), "r"(a2), "r"(a7));
}
