/**
 * @addtogroup grp_uhosfs
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_fs_types.h
 * @author your name (wangshaolin@haier.com)
 * @brief
 * @date 2023-03-01
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-03-01   <td>1.0     <td>        <td>
 * </table>
 */

#ifndef __UH_FS_TYPES_H__
#define __UH_FS_TYPES_H__

#include "uh_time.h"

#ifdef __cplusplus
extern "C" {
#endif

// flags for uhos_open(), 参考linux posix
#define UHOS_O_RDONLY  00
#define UHOS_O_WRONLY  01
#define UHOS_O_RDWR    02
#define UHOS_O_ACCMODE 00000003

#define UHOS_O_CREAT     0100
#define UHOS_O_EXCL      0200
#define UHOS_O_NOCTTY    0400
#define UHOS_O_TRUNC     01000
#define UHOS_O_APPEND    02000
#define UHOS_O_NONBLOCK  04000
#define UHOS_O_NDELAY    UHOS_O_NONBLOCK
#define UHOS_O_DSYNC     010000
#define UHOS_O_ASYNC     020000
#define UHOS_O_DIRECT    040000
#define UHOS_O_LARGEFILE 0100000
#define UHOS_O_DIRECTORY 0200000
#define UHOS_O_NOFOLLOW  0400000
#define UHOS_O_NOATIME   01000000
#define UHOS_O_CLOEXEC   02000000
#define UHOS_O_SYNC      04010000
#define UHOS_O_FSYNC     UHOS_O_SYNC
#define UHOS_O_PATH      010000000

// whence for uhos_lseek()
#define UHOS_SEEK_SET 0 /* Seek from beginning of file.  */
#define UHOS_SEEK_CUR 1 /* Seek from current position.  */
#define UHOS_SEEK_END 2 /* Seek from end of file.  */

// amode for uhos_access()
#define UHOS_R_OK 4 /* Test for read permission.  */
#define UHOS_W_OK 2 /* Test for write permission.  */
#define UHOS_X_OK 1 /* Test for execute permission.  */
#define UHOS_F_OK 0 /* Test for existence.  */

/* 文件类型 */
#define UHOS_S_IFMT   0170000 /* These bits determine file type.  */
#define UHOS_S_IFSOCK 0140000 /* Socket.  */
#define UHOS_S_IFLNK  0120000 /* Symbolic link.  */
#define UHOS_S_IFREG  0100000 /* Regular file.  */
#define UHOS_S_IFBLK  0060000 /* Block device.  */
#define UHOS_S_IFDIR  0040000 /* Directory.  */
#define UHOS_S_IFCHR  0020000 /* Character device.  */
#define UHOS_S_IFIFO  0010000 /* FIFO.  */

#define UHOS_S_ISTYPE(mode, mask) (((mode)&UHOS_S_IFMT) == (mask))
#define UHOS_S_ISDIR(mode)        UHOS_S_ISTYPE((mode), UHOS_S_IFDIR)
#define UHOS_S_ISCHR(mode)        UHOS_S_ISTYPE((mode), UHOS_S_IFCHR)
#define UHOS_S_ISBLK(mode)        UHOS_S_ISTYPE((mode), UHOS_S_IFBLK)
#define UHOS_S_ISREG(mode)        UHOS_S_ISTYPE((mode), UHOS_S_IFREG)
#define UHOS_S_ISFIFO(mode)       UHOS_S_ISTYPE((mode), UHOS_S_IFIFO)
#define UHOS_S_ISLNK(mode)        UHOS_S_ISTYPE((mode), UHOS_S_IFLNK)

// cmd for uhos_fcntl(), 参考linux posix
#define UHOS_F_GETFL 3
#define UHOS_F_SETFL 4

typedef struct
{
    uhos_u16 st_mode;       /* 文件模式, 现在只支持文件类型 */
    uhos_u32 st_size;       /* 文件字节数 */
    uhos_time_t st_actime;  /* 最后访问时间 */
    uhos_time_t st_modtime; /* 最后修改时间 */
    uhos_size_t st_blksize; /* 文件系统的块大小 */
    uhos_u64 st_blocks;     /* 文件占用的块数 */
} uhos_stat_t;

typedef struct
{
    uhos_time_t actime;  /* 访问时间 */
    uhos_time_t modtime; /* 修改时间 */
} uhos_utimbuf_t;

typedef struct
{
    uhos_s32 d_ino;     /* 文件编号 */
    uhos_u8 d_type;     /* 文件类型 */
    uhos_char d_name[]; /* 文件名字 */
} uhos_dirent_t;

struct uhos_dir_s;
typedef struct uhos_dir_s *uhos_dir_t;

#ifdef __cplusplus
}
#endif

#endif // __UH_FS_TYPES_H__

/**@}*/
