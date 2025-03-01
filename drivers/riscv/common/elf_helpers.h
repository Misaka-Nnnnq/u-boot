/* SPDX-License-Identifier: GPL-2.0 */
/*
 * elf helpers defines
 * reference to kernel/linux-5.15/drivers/remoteproc/remoteproc_elf_helpers.h
 * Copyright (C) 2020 Kalray, Inc.
 */

#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include <elf.h>
static inline u8 fw_elf_get_class(u32 elf_fw_addr)
{
	struct elf32_hdr *ehdr = (struct elf32_hdr *)elf_fw_addr;

	return ehdr->e_ident[EI_CLASS];
}

static inline void elf_hdr_init_ident(struct elf32_hdr *hdr, u8 class)
{
	memcpy(hdr->e_ident, ELFMAG, SELFMAG);
	hdr->e_ident[EI_CLASS] = class;
	hdr->e_ident[EI_DATA] = ELFDATA2LSB;
	hdr->e_ident[EI_VERSION] = EV_CURRENT;
	hdr->e_ident[EI_OSABI] = ELFOSABI_NONE;
}

/* Generate getter and setter for a specific elf struct/field */
#define ELF_GEN_FIELD_GET_SET(__s, __field, __type) \
static inline __type elf_##__s##_get_##__field(u8 class, const void *arg) \
{ \
	if (class == ELFCLASS32) \
		return (__type) ((const struct elf32_##__s *) arg)->__field; \
	else \
		return (__type) ((const struct elf64_##__s *) arg)->__field; \
} \
static inline void elf_##__s##_set_##__field(u8 class, void *arg, \
					     __type value) \
{ \
	if (class == ELFCLASS32) \
		((struct elf32_##__s *) arg)->__field = (__type) value; \
	else \
		((struct elf64_##__s *) arg)->__field = (__type) value; \
}

ELF_GEN_FIELD_GET_SET(hdr, e_entry, u64)
ELF_GEN_FIELD_GET_SET(hdr, e_phnum, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_shnum, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_phoff, u64)
ELF_GEN_FIELD_GET_SET(hdr, e_shoff, u64)
ELF_GEN_FIELD_GET_SET(hdr, e_shstrndx, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_machine, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_type, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_version, u32)
ELF_GEN_FIELD_GET_SET(hdr, e_ehsize, u32)
ELF_GEN_FIELD_GET_SET(hdr, e_phentsize, u16)
ELF_GEN_FIELD_GET_SET(hdr, e_shentsize, u16)

ELF_GEN_FIELD_GET_SET(phdr, p_paddr, u64)
ELF_GEN_FIELD_GET_SET(phdr, p_vaddr, u64)
ELF_GEN_FIELD_GET_SET(phdr, p_filesz, u64)
ELF_GEN_FIELD_GET_SET(phdr, p_memsz, u64)
ELF_GEN_FIELD_GET_SET(phdr, p_type, u32)
ELF_GEN_FIELD_GET_SET(phdr, p_offset, u64)
ELF_GEN_FIELD_GET_SET(phdr, p_flags, u32)
ELF_GEN_FIELD_GET_SET(phdr, p_align, u64)

ELF_GEN_FIELD_GET_SET(shdr, sh_type, u32)
ELF_GEN_FIELD_GET_SET(shdr, sh_flags, u32)
ELF_GEN_FIELD_GET_SET(shdr, sh_entsize, u16)
ELF_GEN_FIELD_GET_SET(shdr, sh_size, u64)
ELF_GEN_FIELD_GET_SET(shdr, sh_offset, u64)
ELF_GEN_FIELD_GET_SET(shdr, sh_name, u32)
ELF_GEN_FIELD_GET_SET(shdr, sh_addr, u64)

#define ELF_STRUCT_SIZE(__s) \
static inline unsigned long elf_size_of_##__s(u8 class) \
{ \
	if (class == ELFCLASS32)\
		return sizeof(struct elf32_##__s); \
	else \
		return sizeof(struct elf64_##__s); \
}

ELF_STRUCT_SIZE(shdr)
ELF_STRUCT_SIZE(phdr)
ELF_STRUCT_SIZE(hdr)

static inline unsigned int elf_strtbl_add(const char *name, void *ehdr, u8 class, size_t *index)
{
	u16 shstrndx = elf_hdr_get_e_shstrndx(class, ehdr);
	void *shdr;
	char *strtab;
	size_t idx, ret;

	shdr = ehdr + elf_size_of_hdr(class) + shstrndx * elf_size_of_shdr(class);
	strtab = ehdr + elf_shdr_get_sh_offset(class, shdr);
	idx = index ? *index : 0;
	if (!strtab || !name)
		return 0;

	ret = idx;
	strcpy((strtab + idx), name);
	idx += strlen(name) + 1;
	if (index)
		*index = idx;

	return ret;
}

#endif /* ELF_LOADER_H */
