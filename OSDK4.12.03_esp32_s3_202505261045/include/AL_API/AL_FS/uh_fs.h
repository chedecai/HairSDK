/**
 * @addtogroup grp_uhosfs
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_fs.h
 * @author your name (wangshaolin@haier.com)
 * @brief 本文件接口是参考linux的posix接口, 每个函数的行为、入参、出参、返回值等都和posix保持一致，只是都加了"UHOS_"或者"uhos_"前缀
 * @date 2023-02-23
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-02-23   <td>1.0     <td>        <td>
 * </table>
 */

#ifndef __UH_FS_H__
#define __UH_FS_H__

#include "uh_fs_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uhos_void (*uhos_poll_notify_t)(uhos_void *fds, uhos_void *arg);

/**
 * @brief 初始化文件系统
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_fs_init(uhos_void);

/**
 * @brief 打开一个文件或者设备
 * @param[in] path 待打开对象
 * @param[in] flags 打开方式, 比如UHOS_O_RDWR、UHOS_O_CREAT等, 参考uh_fs_types.h里的相关定义
 * @return uhos_s32 返回文件描述符, >=0表示成功, <0表示失败
 */
uhos_s32 uhos_open(const uhos_char *path, uhos_s32 flags);

/**
 * @brief 关闭文件或者设备
 * @param[in] fd 待操作对象的描述符
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_close(uhos_s32 fd);

/**
 * @brief 从fd指向的文件或者设备中读取内容到buf中
 * @param[in] fd 描述符
 * @param[out] buf 存放内容的缓冲区
 * @param[in] nbytes 计划读取的字节数
 * @return uhos_s32 >0表示读取的字节数, =0表示已经到了文件末尾, <0表示错误
 */
uhos_s32 uhos_read(uhos_s32 fd, uhos_void *buf, uhos_u32 nbytes);

/**
 * @brief 向fd指向的文件或者设备写入内容
 * @param[in] fd 描述符
 * @param[in] buf 待写入内容
 * @param[in] nbytes 计划写入的字节数
 * @return uhos_s32 成功写入的字节数, 负值表示失败
 */
uhos_s32 uhos_write(uhos_s32 fd, const uhos_void *buf, uhos_u32 nbytes);

/**
 * @brief IO操作, 行为参考posix的ioctl
 * @param[in] fd 描述符
 * @param[in] cmd 命令
 * @param[in] arg cmd的参数, 不同的cmd不同的含义
 * @return uhos_s32 不同的cmd不同的含义
 */
uhos_s32 uhos_ioctl(uhos_s32 fd, uhos_s32 cmd, ...);

/**
 * @brief 通配符API，用于通过fd执行特定的轮询
 * @param[in] fd 描述符
 * @param[in] flag 轮询的标志
 * @param[in] notify 通知回调函数
 * @param[in] fds 指向pollfd数组的指针
 * @param[in] arg 轮询的参数
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_do_pollfd(uhos_s32 fd, uhos_s32 flag, uhos_poll_notify_t notify, uhos_void *fds, uhos_void *arg);

/**
 * @brief 将文件位置移动到给定位置的给定偏移量
 * @param[in] fd 描述符
 * @param[in] offset 目标偏移量
 * @param[in] whence 给定位置, UHOS_SEEK_XXX, 参考uh_fs_types.h中相关定义
 * @return uhos_s32 成功则返回新的偏移量, 失败则返回负值
 */
uhos_s32 uhos_lseek(uhos_s32 fd, uhos_s32 offset, uhos_s32 whence);

/**
 * @brief 刷新与该文件关联的所有缓冲区
 * @param[in] fd 描述符
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_sync(uhos_s32 fd);

/**
 * @brief 刷新内存中更新要写入文件系统的文件系统的所有信息
 * @return uhos_void
 */
uhos_void uhos_allsync(uhos_void);

/**
 * @brief 获取文件信息存储在st中
 * @param[in] path 文件名
 * @param[out] st 待存储信息
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_stat(const uhos_char *path, uhos_stat_t *st);

/**
 * @brief 获取文件信息存储在st中
 * @param[in] fd 描述符
 * @param[out] st 待存储信息
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_fstat(uhos_s32 fd, uhos_stat_t *st);

/**
 * @brief 为一个文件创建一个新的链接
 * @param[in] oldpath 旧文件名
 * @param[in] newpath 新文件名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_link(const uhos_char *oldpath, const uhos_char *newpath);

/**
 * @brief 删除一个文件
 * @param[in] path 文件名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_unlink(const uhos_char *path);

/**
 * @brief 删除一个文件或目录
 * @param[in] path 文件名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_remove(const uhos_char *path);

/**
 * @brief 重命名文件
 * @param[in] oldpath 旧名字
 * @param[in] newpath 新名字
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_rename(const uhos_char *oldpath, const uhos_char *newpath);

/**
 * @brief 获取访问信息
 * @param[in] path 文件名
 * @param[in] amode 方式, 参考uh_fs_types.h中相关定义
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_access(const uhos_char *path, uhos_s32 amode);

/**
 * @brief 操作文件描述符
 * @param[in] fd 描述符
 * @param[in] cmd 命令, 参考uh_fs_types.h中相关定义
 * @param[in] args cmd的参数
 * @return uhos_s32 失败返回负值; 当cmd=UHOS_F_GETFL时, 成功则返回一个>=0的文件状态标记, 其他cmd若成功返回0
 */
uhos_s32 uhos_fcntl(uhos_s32 fd, uhos_s32 cmd, ...);

/**
 * @brief 设置文件的访问和修改时间
 * @param[in] path 文件名
 * @param[in] times 时间
 * @return uhos_s32
 */
uhos_s32 uhos_utime(const uhos_char *path, const uhos_utimbuf_t *times);

#ifdef __cplusplus
}
#endif

#endif // __UH_FS_H__

/**@}*/