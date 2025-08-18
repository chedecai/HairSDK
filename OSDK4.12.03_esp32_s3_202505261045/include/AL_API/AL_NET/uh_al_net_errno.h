/**
 * @addtogroup grp_uhosnet
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_al_net_errno.h
 * @author your name (you@haier.com)
 * @brief
 * @date 2023-01-12
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-01-12   <td>1.0     <td>        <td>
 * </table>
 */
#ifndef __UH_AL_NET_ERRNO_H__
#define __UH_AL_NET_ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif

#define UH_EINTR        4         /* Interrupted system call */
#define UH_EAGAIN       11        /* Try again */
#define UH_EINVAL       22        /* Invalid argument */
#define UH_EPIPE        32        /* Broken pipe */
#define UH_ENAMETOOLONG 36        /* File name too long */
#define UH_EWOULDBLOCK  UH_EAGAIN /* Operation would block */
#define UH_EPROTOTYPE   91        /* Protocol wrong type for socket */
#define UH_ECONNRESET   104       /* Connection reset by peer */
#define UH_ENOBUFS      105       /* No buffer space available */
#define UH_ETOOMANYREFS 109       /* Too many references: cannot splice */
#define UH_EALREADY     114       /* Operation already in progress */
#define UH_EINPROGRESS  115       /* Operation now in progress */

#ifdef __cplusplus
}
#endif

#endif
/**@}*/