/*
 Copyright (C) 2022 jobmarley

 This file is part of mips_project_test.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.
 */


extern "C" int start();

[[gnu::naked]]
extern "C" int main()
{
	asm("li $v0, 0x2000");
	asm("move $sp, $v0");
	asm("j start");
}
unsigned int* leds = (unsigned int*)0x80000004;

#define debugbreak() asm("sdbbp")

[[gnu::naked]]
unsigned int get_cycle_count()
{
	asm("mfc0 $v0, $9, 0b000");
	asm("jr $ra");
}

[[gnu::naked]]
unsigned int get_performance_count()
{
	asm("mfc0 $v0, $9, 0b001");
	asm("jr $ra");
}

char* g_console_buffer = (char*)0x30000004;
unsigned int* g_console_buffer_size = (unsigned int*)0x30000000;


unsigned int strlen(const char* s)
{
	unsigned int l = 0;
	while (s[l] != '\0')
		++l;
	return l;
}
char* strcpy(
	char* strDestination,
	const char* strSource
)
{
	char* result = strDestination;
	while (*strSource != '\0')
		*strDestination++ = *strSource++;
	*strDestination = '\0';
	return result;
}
void init_console()
{
	*g_console_buffer_size = 0;
}
void kprint(const char* s)
{
	unsigned int len = strlen(s);
	char* end = g_console_buffer + *g_console_buffer_size + len;

	if (end >= g_console_buffer + 0xFFC)
		return; // overflow

	strcpy(g_console_buffer + *g_console_buffer_size, s);
	*g_console_buffer_size = *g_console_buffer_size + len;
}

template<typename T>
void swap(T& t1, T& t2)
{
	T tmp = t1;
	t1 = t2;
	t2 = tmp;
}

void utoa(unsigned int i, char* buffer, int base = 10)
{
	int count = 0;
	do
	{
		int m = i % base;
		i = i / base;
		const char table[] = "0123456789ABCDEF";
		buffer[count++] = table[m];
	} while (i > 0);

	for (int j = 0, k = 0; j < count; j += 2, k++)
		swap(buffer[k], buffer[count - k - 1]);

	buffer[count] = '\0';
}
void itoa(int i, char* buffer, int base = 10)
{
	if (i < 0)
	{
		i = -i;
		buffer[0] = '-';
		++buffer;
	}
	utoa(i, buffer, base);
}
extern "C" int start()
{
	unsigned int start_count = get_cycle_count();
	unsigned int start_perf_count = get_performance_count();

	unsigned int toggle = 0;
	*leds = 0;
	init_console();
	kprint("start...\n");


	unsigned int max_count = 50000000; // 0.5s for a toggle, blink every 1 second
	unsigned last_tick = start_count;

	int secondCount = 0;
	while (true)
	{
		unsigned int now_count = get_cycle_count();
		unsigned int perf_count = get_performance_count();
		unsigned int elapsed = now_count - last_tick;
		unsigned int elapsed_perf = perf_count - start_perf_count;

		unsigned total_elapsed = now_count - start_count;
		if (total_elapsed > 1000000000) // 10s at 100Mhz
		{
			debugbreak();
		}

		if (elapsed > max_count)
		{
			if (toggle == 0)
			{
				toggle = 1;
				*leds = 1;
				char s[10] = {};
				itoa(++secondCount, s);
				kprint(s);
				kprint(" seconds elapsed.\n");
			}
			else
			{
				toggle = 0;
				*leds = 0;
			}
			last_tick += max_count; // do not use now_count, so not to drift. Since elapsed wont be exactly max_count
		}
	}
	return 55;
}


