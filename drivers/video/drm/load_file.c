/*
 * load_file/load_file.c
 *
 * Copyright (c) 2007-2021 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <common.h>
#include <malloc.h>
#include <asm/cache.h>
#include <sys_partition.h>
#include "load_file.h"
#include <command.h>
#include <linux/string.h>
#include <fs.h>

extern int sunxi_partition_get_partno_byname(const char *part_name);
extern int disp_fat_load(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
extern int do_fat_size(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

static int __unload_file(struct file_info_t *file)
{
	if (file) {
		free(file->name);
		free(file->path);
		free(file->file_addr);
		free(file);
		return 0;
	}
	return -1;
}

static int __print_file_info(struct file_info_t *file)
{
	pr_err("File name:%s\n", file->name);
	pr_err("Partition name:%s\n", file->path);
	pr_err("File size:%u\n", file->file_size);
	pr_err("File addr:%p\n", file->file_addr);
	return 0;
}

struct file_info_t *load_file(char *name, char *part_name)
{
	int partno = -1;
	char *argv[6], file_addr[32];
	char part_info[16] = { 0 }, size[32] = { 0 };
	struct file_info_t *file = NULL;

	if (!name || !part_name) {
		pr_err("NULL pointer! name:%p, part_name:%p\n", name,
		       part_name);
		goto OUT;
	}

	partno = sunxi_partition_get_partno_byname(part_name);
	if (partno < 0) {
		pr_err("%s is not found!\n", part_name);
		goto OUT;
	}
	snprintf(part_info, 16, "0:%x", partno);

	argv[0] = "fatsize";
	argv[1] = "sunxi_flash";
	argv[2] = part_info;
	argv[3] = name;
	argv[4] = NULL;
	argv[5] = NULL;

	if (!do_fat_size(0, 0, 4, argv)) {
		file = (struct file_info_t *)malloc(sizeof(struct file_info_t));
		memset(file, 0, sizeof(struct file_info_t));
		file->file_size = env_get_hex("filesize", 0);
	} else {
		pr_err("get file(%s) size from %s error\n", name, part_name);
		goto OUT;
	}

	file->name = (char *)malloc(strlen(name) + 1);
	strncpy(file->name, name, strlen(name) + 1);
	file->path = (char *)malloc(strlen(part_name) + 1);
	strncpy(file->path, part_name, strlen(part_name) + 1);
	file->file_addr =
		memalign(4096, file->file_size);

	sprintf(file_addr, "%lx", (unsigned long)file->file_addr);
	snprintf(size, 16, "%lx", (unsigned long)file->file_size);

	argv[0] = "fatload";
	argv[1] = "sunxi_flash";
	argv[2] = part_info;
	argv[3] = file_addr;
	argv[4] = name;
	argv[5] = size;

	if (disp_fat_load(0, 0, 6, argv)) {
		pr_err("Unable to open file %s from %s\n", name, part_name);
		goto FREE_FILE;
	}

#if !CONFIG_IS_ENABLED(SYS_DCACHE_OFF)
	flush_dcache_range((ulong)file->file_addr,
			   ALIGN((ulong)(file->file_addr + file->file_size),
				 CONFIG_SYS_CACHELINE_SIZE));
#endif

	file->unload_file = __unload_file;
	file->print_file_info = __print_file_info;

	return file;

FREE_FILE:
	free(file->name);
	free(file->path);
	free(file->file_addr);
	free(file);
OUT:
	return NULL;
}


int write_file(char *name, char *part_name, void *buf_addr, unsigned int buf_size)
{
	int partno = -1;
	char cmd[100] = {0};

	if (!name || !part_name || !buf_addr || !buf_size) {
		return -1;
	}

	partno = sunxi_partition_get_partno_byname(part_name);
	if (partno < 0) {
		pr_err("%s is not found!\n", part_name);
		goto OUT;
	}

	snprintf(cmd, 100, "fatwrite mmc 0:%x 0x%lx %s 0x%lx 0", partno, (unsigned long)buf_addr, name, (unsigned long)buf_size);

#ifdef CONFIG_FAT_WRITE
	if (run_command(cmd, 0))
		goto OUT;
#else
	pr_err("Please enable CONFIG_FAT_WRITE\n");
	goto OUT;

#endif
	return 0;
OUT:
	return -1;
}

