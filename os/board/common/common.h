/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#ifndef __BOARD_COMMON_H__
#define __BOARD_COMMON_H__

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/fs/mtd.h>

#if defined(CONFIG_FLASH_PARTITION) || defined(CONFIG_SECOND_FLASH_PARTITION)
struct partition_data_s {
	char *types;
	char *sizes;
#ifdef CONFIG_MTD_PARTITION_NAMES
	char *names;
#endif
};

#define FLASH_MINOR 0 

struct partition_info_s {
	int smartfs_partno;
	int romfs_partno;
	int timezone_partno;
};
typedef struct partition_info_s partition_info_t;

#ifdef CONFIG_FLASH_PARTITION
extern struct partition_data_s g_flash_part_data;
#endif

#ifdef CONFIG_SECOND_FLASH_PARTITION
extern struct partition_data_s g_second_flash_part_data;
#endif

int configure_mtd_partitions(struct mtd_dev_s *mtd, struct partition_data_s *part_data, partition_info_t *partinfo);
FAR struct mtd_dev_s *mtd_initialize(void);
void automount_fs_partition(partition_info_t *partinfo);
int get_partition_num(char *part);
#endif

 #endif /*__BOARD_COMMON_H__ */
