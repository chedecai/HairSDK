/**
 * @addtogroup grp_uhostypes
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_shell.h
 * @author  maaiguo@haier.com
 * @brief 不需要适配，做为基础头文件方便第三方适配或者第三方业务组件直接使用
 * @date 2021-10-13
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-13   <td>1.0     <td>maaiguo <td>init version
 * </table>
 */

#ifndef __UHOS_SHELL_H__
#define __UHOS_SHELL_H__

/**************************************************************************************************/
/*                         #include (依次为标准库头文件、非标准库头文件)                          */
/**************************************************************************************************/
#include "uh_types.h"

/**************************************************************************************************/
/*                                        其他条件编译选项                                        */
/**************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/*                                           常量定义                                             */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                          全局宏定义                                            */
/**************************************************************************************************/
#ifndef UHOS_SHELL_SECTION
#if defined(__CC_ARM) || defined(__CLANG_ARM)
#define UHOS_SHELL_SECTION(x) __attribute__((section(x)))
#elif defined(__IAR_SYSTEMS_ICC__)
#define UHOS_SHELL_SECTION(x) @x
#elif defined(__GNUC__)
#define UHOS_SHELL_SECTION(x) __attribute__((section(x)))
#else
#define UHOS_SHELL_SECTION(x)
#endif
#endif

#ifndef UHOS_SHELL_USED
#if defined(__CC_ARM) || defined(__CLANG_ARM)
#define UHOS_SHELL_USED __attribute__((used))
#elif defined(__IAR_SYSTEMS_ICC__)
#define UHOS_SHELL_USED __root
#elif defined(__GNUC__)
#define UHOS_SHELL_USED __attribute__((used))
#else
#define UHOS_SHELL_USED
#endif
#endif

/**
 * @brief shell 命令定义
 *
 * @param _name 命令名
 * @param _func 命令函数
 * @param _desc 命令描述
 */
#define UHOS_SHELL_EXPORT_CMD(_name, _func, _desc)                                 \
    const char shellCmd##_name[] = #_name;                                         \
    const char shellDesc##_name[] = #_desc;                                        \
    UHOS_SHELL_USED const uhos_shell_cmd_t shellCommand##_name UHOS_SHELL_SECTION( \
        "shellCommand") = {.attr.value = 0x00002000, .cmd.name = shellCmd##_name, .cmd.func = _func, .cmd.desc = shellDesc##_name}

/**************************************************************************************************/
/*                                       全局数据类型定义                                         */
/**************************************************************************************************/
/**< 临时屏蔽 strict-prototypes 告警 */
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#endif

/**
 * @struct uhos_shell_cmd_t
 * @brief SHELL命令的描述结构定义
 */
typedef struct uhos_shell_cmd
{
    union
    {
        struct
        {
            uhos_u8 permission   : 8; /**< 命令权限 */
            uhos_u8 type         : 4; /**< 命令类型 */
            uhos_u8 en_unchecked : 1; /**< 在未校验密码的情况下可用 */
            uhos_u8 dis_return   : 1; /**< 禁用返回值输出 */
            uhos_u8 rd_only      : 1; /**< 只读 */
            uhos_u8 reserve      : 1; /**< 保留 */
            uhos_u8 param_num    : 4; /**< 参数数量 */
        } attrs;
        uhos_s32 value;
    } attr; /**< 命令属性 */

    struct
    {
        const char *name;                         /**< 命令名 */
        uhos_s32 (*func)(int argc, char *argv[]); /**< 命令执行函数 */
        const char *desc;                         /**< 命令描述 */
    } cmd;                                        /**< 命令定义 */

} uhos_shell_cmd_t;

/**< 解除屏蔽 strict-prototypes 告警 */
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

/**************************************************************************************************/
/*                                         全局变量声明                                           */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                         全局函数原型                                           */
/**************************************************************************************************/
void uhos_shell_init(void);
void uhos_shell_deinit(void);
void uhos_shell_printf(char *fmt, ...);
void uhos_shell_write_endline(char *buffer, uhos_s32 len);

#ifdef __cplusplus
}
#endif

#endif
/**@}*/