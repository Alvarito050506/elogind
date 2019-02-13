/* SPDX-License-Identifier: LGPL-2.1+ */
#pragma once

/* Missing glibc definitions to access certain kernel APIs */

#include <errno.h>
#include <inttypes.h>
//#include <linux/falloc.h>
#include <sys/syscall.h>

#else

/// Additional includes needed by elogind
#include "musl_missing.h"

#ifndef RLIMIT_RTTIME
#define RLIMIT_RTTIME 15
#endif

/* If RLIMIT_RTTIME is not defined, then we cannot use RLIMIT_NLIMITS as is */
#ifndef FS_NOCOW_FL
#define FS_NOCOW_FL 0x00800000
#endif


#if 0 /// UNNEEDED by elogind (It can not support BTRFS at all)
#endif // 0
#ifndef MS_MOVE
#define MS_MOVE 8192
#endif

#ifndef MS_REC
#define MS_REC 16384
#endif

#ifndef MS_PRIVATE
#define MS_PRIVATE      (1<<18)
#endif

#ifndef MS_REC
#define MS_REC          (1<<19)
#endif

#ifndef MS_SHARED
#define MS_SHARED       (1<<20)
#endif

#ifndef MS_RELATIME
#define MS_RELATIME     (1<<21)
#endif

#ifndef MS_KERNMOUNT
#define MS_KERNMOUNT    (1<<22)
#endif

#ifndef MS_I_VERSION
#define MS_I_VERSION    (1<<23)
#endif

#ifndef MS_STRICTATIME
#define MS_STRICTATIME  (1<<24)
#endif

#ifndef MS_LAZYTIME
#define MS_LAZYTIME     (1<<25)
#endif

#ifndef CIFS_MAGIC_NUMBER
#  define CIFS_MAGIC_NUMBER 0xFF534D42
#endif

#else

#if 0 /// UNNEEDED by elogind
#ifndef RENAME_NOREPLACE
#define RENAME_NOREPLACE (1 << 0)
#endif

#endif // 0

#if 0 /// UNNEEDED by elogind
#endif // 0
#ifndef EXT4_IOC_RESIZE_FS
#  define EXT4_IOC_RESIZE_FS              _IOW('f', 16, __u64)
#endif

#ifndef NS_GET_NSTYPE
#define NS_GET_NSTYPE _IO(0xb7, 0x3)
#endif

#ifndef FALLOC_FL_KEEP_SIZE
#define FALLOC_FL_KEEP_SIZE 0x01
#endif

#ifndef FALLOC_FL_PUNCH_HOLE
#define FALLOC_FL_PUNCH_HOLE 0x02
#endif

#else
#else
//#include "missing_audit.h"
//#include "missing_btrfs_tree.h"
//#include "missing_capability.h"
//#include "missing_fcntl.h"
//#include "missing_input.h"
//#include "missing_magic.h"
//#include "missing_mman.h"
//#include "missing_network.h"
//#include "missing_prctl.h"
//#include "missing_random.h"
//#include "missing_resource.h"
//#include "missing_sched.h"
//#include "missing_socket.h"
//#include "missing_stdlib.h"
//#include "missing_timerfd.h"
//#include "missing_type.h"

#include "missing_syscall.h"
