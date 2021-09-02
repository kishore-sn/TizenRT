/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifdef CONFIG_NETUTILS_DHCPD

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <protocols/dhcpd.h>
#include <netutils/netlib.h>
#include "netcmd_log.h"

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/
#define DHCPD_STACK_SIZE		20480
#define DHCPD_SCHED_PRI			100
#define DHCPD_SCHED_POLICY		SCHED_RR

#define NTAG "[NETCMD]"
/****************************************************************************
* Private Data
****************************************************************************/
/****************************************************************************
* Global Data
****************************************************************************/
/****************************************************************************
* Private Functions
****************************************************************************/
/****************************************************************************
* Name: show_usage
****************************************************************************/

static void show_usage(void)
{
	NETCMD_LOG(NTAG, "\n");
	NETCMD_LOG(NTAG, "Usage: dhcpd <command> [interface name]\n");
	NETCMD_LOG(NTAG, "\nWhere:\n");
	NETCMD_LOG(NTAG, "	<command>		command string (start | stop | status ) \n");
	NETCMD_LOG(NTAG, "					 - start  : start dhcpd daemon \n");
	NETCMD_LOG(NTAG, "					 - stop   : terminate dhcpd daemon \n");
	NETCMD_LOG(NTAG, "					 - status : show dhcpd's status \n");
	NETCMD_LOG(NTAG, "	[interface name] name of network interface used for running dhcpd  \n");
	NETCMD_LOG(NTAG, "\n");

}


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/****************************************************************************
 * Name: cmd_dhcpd
 ****************************************************************************/

int cmd_dhcpd(int argc, char *argv[])
{
	int result = OK;
	uint8_t flags;

	if (argc < 2) {
		show_usage();
		goto done;
	}

	if (!strcmp(argv[1], "start")) {
		if (argc < 3) {
			NETCMD_LOGE(NTAG, "%s insert [interface name]\n", __FUNCTION__);
			show_usage();
			goto done;
		}

		if (dhcp_server_status(argv[2])) {
			NETCMD_LOGE(NTAG, "%s, dhcpd is already running\n", __FUNCTION__);
			result = ERROR;
			goto done;
		}

		if (netlib_getifstatus(argv[2], &flags) == ERROR) {
			NETCMD_LOGE(NTAG, "%s, failed to get interface status\n", __FUNCTION__);
			result = ERROR;
			goto done;
		} else {
			if (flags & IFF_UP) {
				NETCMD_LOGI(NTAG, "%s : dhcp server start on %s\n", __FUNCTION__, argv[2]);
			} else {
				NETCMD_LOGI(NTAG, "%s : interface %s is down, unable to run dhcpd\n", __FUNCTION__, argv[2]);
				result = ERROR;
				goto done;
			}
		}

		if (dhcp_server_start(argv[2], NULL) != 0) {
			NETCMD_LOGE(NTAG, "%s : failed to start dhcp server\n", __FUNCTION__);
			goto done;
		}
	} else if (!strcmp(argv[1], "stop")) {
		NETCMD_LOG(NTAG, "%s : dhcp server stop\n", __FUNCTION__);
		dhcp_server_stop(argv[2]);
	} else if (!strcmp(argv[1], "status")) {
		NETCMD_LOG(NTAG, "\ndhcpd status : ");
		if (dhcp_server_status(argv[2])) {
			NETCMD_LOG(NTAG, "running\n");
		} else {
			NETCMD_LOG(NTAG, "stopped\n");
		}
	} else {
		NETCMD_LOGE(NTAG, "%s invalid <command> : %s\n", __FUNCTION__, argv[1]);
		show_usage();
		goto done;
	}

done:
	return result;
}
#endif
