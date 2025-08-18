/**
 * @addtogroup grp_uhosfs
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_dirent.h
 * @author your name (wangshaolin@haier.com)
 * @brief
 * @date 2023-02-24
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-02-24   <td>1.0     <td>        <td>
 * </table>
 */

#ifndef __UH_DIRENT_H__
#define __UH_DIRENT_H__

#include "uh_fs_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 打开一个目录
 * @param[in] path 目录
 * @return uhos_dir_t* 成功则返回目录流指针对象, 失败则返回NULL
 */
uhos_dir_t *uhos_opendir(const uhos_char *path);

/**
 * @brief 关闭目录
 * @param[in] dir 待关闭目录
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_closedir(uhos_dir_t *dir);

/**
 * @brief 读取目录的下一个条目
 * @param[in] dir 待读取的目录
 * @return uhos_dirent_t* 返回一个条目对象, 如果返回NULL表示没有了
 */
uhos_dirent_t *uhos_readdir(uhos_dir_t *dir);

/**
 * @brief 创建目录
 * @param[in] path 目录名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_mkdir(const uhos_char *path);

/**
 * @brief 删除目录
 * @param[in] path 待操作目录名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_rmdir(const uhos_char *path);

/**
 * @brief 将目录流的位置重置为目录的开头
 * @param[in] dir 待操作目录名
 * @return uhos_void
 */
uhos_void uhos_rewinddir(uhos_dir_t *dir);

/**
 * @brief 获取与该目录关联的当前位置
 * @param[in] dir 待操作目录名
 * @return uhos_s32 成功则返回当前位置, 如果失败则返回负值
 */
uhos_s32 uhos_telldir(uhos_dir_t *dir);

/**
 * @brief 将目录位置移动到给定位置
 * @param[in] dir 要查找的目录的指针
 * @param[in] loc 目录的位置
 * @return uhos_void
 */
uhos_void uhos_seekdir(uhos_dir_t *dir, uhos_s32 loc);

/**
 * @brief 设置当前工作目录的路径名
 * @param[in] dir 待操作目录名
 * @return uhos_s32 成功则返回0, 失败返回负值
 */
uhos_s32 uhos_chdir(const uhos_char *path);

/**
 * @brief 获取当前工作目录的路径名
 * @param[out] buf 用于保存当前工作目录的缓冲区
 * @param[in] size 缓冲区的大小
 * @return uhos_char* 成功返回路径名(与buf内容相同), 失败则返回NULL
 */
uhos_char *uhos_getcwd(uhos_char *buf, uhos_u32 size);

#ifdef __cplusplus
}
#endif

#endif // __UH_DIRENT_H__

/**@}*/