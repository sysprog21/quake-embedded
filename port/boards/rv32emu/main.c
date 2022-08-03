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
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

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

int qembd_dequeue_key_event(key_event_t *e)
{
	/* Not Implemented */
	return -1;
}

int qembd_get_current_position(mouse_position_t *position)
{
	/* Not Implemented */
	return -1;
}
