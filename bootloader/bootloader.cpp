/*
 Copyright (C) 2022 jobmarley

 This file is part of mips_project_test.

 This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>.
 */


extern "C" void start();

//// Must always be first so it is at ofs 0 in the .text section
[[gnu::naked]]
int main()
{
	asm("li $v0, 0x2000");
	asm("move $sp, $v0");
	asm("j start");
};

typedef unsigned int uint32_t;
typedef unsigned int size_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

struct elf_header
{
	uint32_t	magic;
	uint8_t		file_class;
	uint8_t		encoding;
	uint8_t		version;
	uint8_t		abi;
	uint8_t		abi_version;
	uint8_t		padding[7];
	uint16_t	type;
	uint16_t	machine;
	uint32_t	version2;
	uint32_t	entry_point;
	uint32_t	pht_file_ofs;
	uint32_t	sht_file_ofs;
	uint32_t	processor_flags;
	uint16_t	hdr_size;
	uint16_t	pht_entry_size;
	uint16_t	pht_entry_count;
	uint16_t	sht_entry_size;
	uint16_t	sht_entry_count;
	uint16_t	sht_string_table_index;
};

struct elf_pht_entry
{
	uint32_t	type;
	uint32_t	file_ofs;
	uint32_t	virtual_addr;
	uint32_t	phys_addr;
	uint32_t	file_size;
	uint32_t	memory_size;
	uint32_t	flags;
	uint32_t	alignment;
};
void memcpy(void* dest,
	const void* src,
	size_t count)
{
	uint8_t* u8dest = (uint8_t*)dest;
	uint8_t* u8src = (uint8_t*)src;
	for (size_t i = 0; i < count; ++i)
		*u8dest++ = *u8src++;
}
void load_elf(elf_header* hdr)
{
	uint8_t* base = (uint8_t*)hdr;
	for (uint16_t i = 0; i < hdr->pht_entry_count; ++i)
	{
		elf_pht_entry* entry = (elf_pht_entry*)(base + hdr->pht_file_ofs + (i * hdr->pht_entry_size));
		memcpy((void*)entry->virtual_addr, base + entry->file_ofs, entry->file_size);
	}
}
#define default_kernel_address 0x10000
extern "C" void start()
{
	elf_header* hdr = (elf_header*)default_kernel_address;
	if (hdr->magic != 0x464C457F)
	{
		while (true)
		{

		}
	}
	load_elf(hdr);
	int (*kentry)() = (int (*)())hdr->entry_point;
	kentry();
}


