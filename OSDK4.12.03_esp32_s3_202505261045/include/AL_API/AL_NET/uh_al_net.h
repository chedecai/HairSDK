/**
 * @defgroup grp_uhosnet 网络适配接口定义
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_al_net.h
 * @author weizhiyuan (weizhiyuan@haier.com)
 * @brief uhos_net网络适配层，提供统一的对外接口，接口不区分linux、rtos
 *      命名规则:
 *      1.api接口: uhos_net_xxx();  xxx:  标准 poxis接口名称;
 *      2.宏定义: UHOS_XXXX 原poxis 定义宏名称;
 * @date 2021-10-23
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-23   <td>1.0     <td>        <td>
 * </table>
 */
#ifndef __UH_AL_NET_H__
#define __UH_AL_NET_H__

#include "uh_types.h"
#include "uh_time.h"

/** @def OP_XXX
 *
 * @brief OP接口操作
 * SET 设置。
 * GET 查询。
 * START 开始。
 * STOP 结束。
 * ADD 增加。
 * DELETE 删除。
 *
 */
#define OP_SET    1
#define OP_GET    2
#define OP_START  1
#define OP_STOP   2
#define OP_ADD    1
#define OP_DELETE 2

#define UHOS_SO_REUSEPORT 15

#define UHOS_PF_INET    2 /* IP protocol family.  */
#define UHOS_PF_NETLINK 16

#define UHOS_AF_NETLINK UHOS_PF_NETLINK
/* RTnetlink multicast groups - backwards compatibility for userspace */
#define UHOS_RTMGRP_LINK   1
#define UHOS_RTMGRP_NOTIFY 2
#define UHOS_RTMGRP_NEIGH  4
#define UHOS_RTMGRP_TC     8

#define UHOS_RTMGRP_IPV4_IFADDR 0x10
#define UHOS_RTMGRP_IPV4_MROUTE 0x20
#define UHOS_RTMGRP_IPV4_ROUTE  0x40
#define UHOS_RTMGRP_IPV4_RULE   0x80

#define UHOS_RTM_NEWLINK 16
#define UHOS_RTM_GETLINK 18
#define UHOS_RTM_NEWADDR 20
#define UHOS_RTM_DELADDR 21

#define UHOS_AF_UNSPEC 0
#define UHOS_AF_UNIX   1  /* Unix domain sockets */
#define UHOS_AF_LOCAL  1  /* POSIX name for AF_UNIX */
#define UHOS_AF_INET   2  /* Internet IP Protocol */
#define UHOS_AF_INET6  10 /* IP version 6 */

#define UHOS_IPPROTO_IP   0  /* Dummy protocol for TCP */
#define UHOS_IPPROTO_ICMP 1  /* Internet Control Message Protocol */
#define UHOS_IPPROTO_IGMP 2  /* Internet Group Management Protocol */
#define UHOS_IPPROTO_TCP  6  /* Transmission Control Protocol */
#define UHOS_IPPROTO_UDP  17 /* User Datagram Protocol */
#define UHOS_IPPROTO_IPV6 41 /* IPv6-in-IPv4 tunnelling */

#define UHOS_IPV6_RECVPKTINFO 49

/** 255.255.255.255 */
#define UHOS_INADDR_NONE ((uhos_u32)0xffffffffUL)
/** 127.0.0.1 */
#define UHOS_INADDR_LOOPBACK ((uhos_u32)0x7f000001UL)
/** 0.0.0.0 */
#define UHOS_INADDR_ANY ((uhos_u32)0x00000000UL)
/** 255.255.255.255 */
#define UHOS_INADDR_BROADCAST ((uhos_u32)0xffffffffUL)

typedef uhos_s32 uhos_socklen_t;

#ifndef uhos_ctx_id
typedef uhos_void *uhos_ctx_id;
#endif

#ifndef TIME_SOCK_RXTX
#define TIME_MILLISECOND  (1)
#define TIME_SECOND       (1000 * (TIME_MILLISECOND))
#define TIME_SOCK_RXTX    (5 * (TIME_SECOND))
#define TIME_SOCK_RXTX_2S (2 * (TIME_SECOND))
#endif

/**
 * @brief getsockopt setsockopt level 定义
 */
#define UHOS_SOL_SOCKET 0
#define UHOS_SOL_IP     1
#define UHOS_SOL_TCP    2
#define UHOS_SOL_UDP    3
/**
 * @brief socket type定义
 */
#define UHOS_SOCK_STREAM 1
#define UHOS_SOCK_DGRAM  2
#define UHOS_SOCK_RAW    3

/* 地址串长度 */
#define UHOS_INET_ADDRSTRLEN  16
#define UHOS_INET6_ADDRSTRLEN 46

#define UHOS_IPPROTO_UDP 17 /* User Datagram Protocol.  */

#define UHOS_NETLINK_ROUTE 0 /* Routing/device hook				*/

/* Routing table calls. */
#define UHOS_SIOCADDRT 0x890B /* add routing table entry	*/
#define UHOS_SIOCDELRT 0x890C /* delete routing table entry	*/
#define UHOS_SIOCRTMSG 0x890D /* call to routing system	*/

/* Socket configuration controls. */
#define UHOS_SIOCGIFNAME        0x8910 /* get iface name		*/
#define UHOS_SIOCSIFLINK        0x8911 /* set iface channel    */
#define UHOS_SIOCGIFCONF        0x8912 /* get iface list		*/
#define UHOS_SIOCGIFFLAGS       0x8913 /* get flags			*/
#define UHOS_SIOCSIFFLAGS       0x8914 /* set flags			*/
#define UHOS_SIOCGIFADDR        0x8915 /* get PA address		*/
#define UHOS_SIOCSIFADDR        0x8916 /* set PA address		*/
#define UHOS_SIOCGIFDSTADDR     0x8917 /* get remote PA address	*/
#define UHOS_SIOCSIFDSTADDR     0x8918 /* set remote PA address	*/
#define UHOS_SIOCGIFBRDADDR     0x8919 /* get broadcast PA address	*/
#define UHOS_SIOCSIFBRDADDR     0x891a /* set broadcast PA address	*/
#define UHOS_SIOCGIFNETMASK     0x891b /* get network PA mask		*/
#define UHOS_SIOCSIFNETMASK     0x891c /* set network PA mask		*/
#define UHOS_SIOCGIFMETRIC      0x891d /* get metric			*/
#define UHOS_SIOCSIFMETRIC      0x891e /* set metric			*/
#define UHOS_SIOCGIFMEM         0x891f /* get memory address (BSD)	*/
#define UHOS_SIOCSIFMEM         0x8920 /* set memory address (BSD)	*/
#define UHOS_SIOCGIFMTU         0x8921 /* get MTU size			*/
#define UHOS_SIOCSIFMTU         0x8922 /* set MTU size			*/
#define UHOS_SIOCSIFNAME        0x8923 /* set interface name */
#define UHOS_SIOCSIFHWADDR      0x8924 /* set hardware address 	*/
#define UHOS_SIOCGIFENCAP       0x8925 /* get/set encapsulations       */
#define UHOS_SIOCSIFENCAP       0x8926
#define UHOS_SIOCGIFHWADDR      0x8927 /* Get hardware address		*/
#define UHOS_SIOCGIFSLAVE       0x8929 /* Driver slaving support	*/
#define UHOS_SIOCSIFSLAVE       0x8930
#define UHOS_SIOCADDMULTI       0x8931 /* Multicast address lists	*/
#define UHOS_SIOCDELMULTI       0x8932
#define UHOS_SIOCGIFINDEX       0x8933            /* name -> if_index mapping	*/
#define UHOS_SIOGIFINDEX        UHOS_SIOCGIFINDEX /* misprint compatibility :-)	*/
#define UHOS_SIOCSIFPFLAGS      0x8934            /* set/get extended flags set	*/
#define UHOS_SIOCGIFPFLAGS      0x8935
#define UHOS_SIOCDIFADDR        0x8936 /* delete PA address		*/
#define UHOS_SIOCSIFHWBROADCAST 0x8937 /* set hardware broadcast addr	*/
#define UHOS_SIOCGIFCOUNT       0x8938 /* get number of devices */
#define UHOS_SIOCGIFBR          0x8940 /* Bridging support		*/
#define UHOS_SIOCSIFBR          0x8941 /* Set bridging options 	*/
#define UHOS_SIOCGIFTXQLEN      0x8942 /* Get the tx queue length	*/
#define UHOS_SIOCSIFTXQLEN      0x8943 /* Set the tx queue length 	*/
#define UHOS_SIOCETHTOOL        0x8946 /* Ethtool interface		*/
#define UHOS_SIOCGMIIPHY        0x8947 /* Get address of MII PHY in use. */
#define UHOS_SIOCGMIIREG        0x8948 /* Read MII PHY register.	*/
#define UHOS_SIOCSMIIREG        0x8949 /* Write MII PHY register.	*/
#define UHOS_SIOCWANDEV         0x894A /* get/set netdev parameters	*/
#define UHOS_SIOCOUTQNSD        0x894B /* output queue size (not sent only) */
/* ARP cache control calls. */
/*  0x8950 - 0x8952  * obsolete calls, don't re-use */
#define UHOS_SIOCDARP 0x8953 /* delete ARP table entry	*/
#define UHOS_SIOCGARP 0x8954 /* get ARP table entry		*/
#define UHOS_SIOCSARP 0x8955 /* set ARP table entry		*/
/* RARP cache control calls. */
#define UHOS_SIOCDRARP 0x8960 /* delete RARP table entry	*/
#define UHOS_SIOCGRARP 0x8961 /* get RARP table entry		*/
#define UHOS_SIOCSRARP 0x8962 /* set RARP table entry		*/

/* Standard interface flags. */

#define UHOS_IFF_UP          0x1      /* Interface is up.  */
#define UHOS_IFF_RUNNING     (1 << 6) /* volatile */
#define UHOS_IFF_LOOPBACK    0x8      /* Is a loopback net.  */
#define UHOS_IFF_POINTOPOINT 0x10

struct uhos_nlmsghdr
{
    uhos_u32 nlmsg_len;   /* Length of message including header */
    uhos_u16 nlmsg_type;  /* Message content */
    uhos_u16 nlmsg_flags; /* Additional flags */
    uhos_u32 nlmsg_seq;   /* Sequence number */
    uhos_u32 nlmsg_pid;   /* Sending process port ID */
};

#define UHOS_NLMSG_ALIGNTO     4U
#define UHOS_NLMSG_ALIGN(len)  (((len) + UHOS_NLMSG_ALIGNTO - 1) & ~(UHOS_NLMSG_ALIGNTO - 1))
#define UHOS_NLMSG_HDRLEN      ((uhos_s32)UHOS_NLMSG_ALIGN(sizeof(struct uhos_nlmsghdr)))
#define UHOS_NLMSG_LENGTH(len) ((len) + UHOS_NLMSG_HDRLEN)
#define UHOS_NLMSG_DATA(nlh)   ((uhos_void *)(((uhos_char *)nlh) + UHOS_NLMSG_LENGTH(0)))
#define UHOS_NLMSG_NEXT(nlh, len) \
    ((len) -= UHOS_NLMSG_ALIGN((nlh)->nlmsg_len), (struct uhos_nlmsghdr *)(((uhos_char *)(nlh)) + UHOS_NLMSG_ALIGN((nlh)->nlmsg_len)))

#define UHOS_NLMSG_NOOP    0x1 /* Nothing.		*/
#define UHOS_NLMSG_ERROR   0x2 /* Error		*/
#define UHOS_NLMSG_DONE    0x3 /* End of a dump	*/
#define UHOS_NLMSG_OVERRUN 0x4 /* Data lost		*/

#define UHOS_NLMSG_MIN_TYPE 0x10 /* < 0x10: reserved control messages */

/* Flags values */

#define UHOS_NLM_F_REQUEST       0x01 /* It is request message. 	*/
#define UHOS_NLM_F_MULTI         0x02 /* Multipart message, terminated by NLMSG_DONE */
#define UHOS_NLM_F_ACK           0x04 /* Reply with ack, with zero or error code */
#define UHOS_NLM_F_ECHO          0x08 /* Echo this request 		*/
#define UHOS_NLM_F_DUMP_INTR     0x10 /* Dump was inconsistent due to sequence change */
#define UHOS_NLM_F_DUMP_FILTERED 0x20 /* Dump was filtered as requested */

/**
 * @brief LINUX 使用
 */
struct uhos_ifinfomsg
{
    uhos_u8 ifi_family;  /* AF_UNSPEC */
    uhos_u16 ifi_type;   /* Device type */
    uhos_s32 ifi_index;  /* Link index	*/
    uhos_u32 ifi_flags;  /* IFF_* flags	*/
    uhos_u32 ifi_change; /* IFF_* change mask */
};
struct uhos_ifaddrmsg
{
    uhos_u8 ifa_family;    /* Address type */
    uhos_u8 ifa_prefixlen; /* The prefix length of address */
    uhos_u8 ifa_flags;     /* Address flags */
    uhos_u8 ifa_scope;     /* Address scope */
    uhos_u32 ifa_index;    /* Interface index */
};

/* for UHOS_SOL_SOCKET */
#define SO_OPT_KEEPALIVE 1
#define SO_OPT_BROADCAST 2
#define SO_OPT_REUSEADDR 3
#define SO_OPT_SNDTIMEO  4
#define SO_OPT_RCVTIMEO  5
#define SO_OPT_NONBLOCK  6
#define SO_OPT_ERROR     7
#define SO_OPT_REUSEPORT 8

/*for UHOS_SOL_IP*/
#define SO_IP_MULTICAST_TTL   1
#define SO_IP_MULTICAST_IF    2
#define SO_IP_MULTICAST_LOOP  3
#define SO_IP_ADD_MEMBERSHIP  4
#define SO_IP_DROP_MEMBERSHIP 5
#define UHOS_IP_TTL           6 /* IP time to live.  */
#define UHOS_IP_PKTINFO       8 /* bool */

/*for UHOS_SOL_TCP*/
#define SO_TCP_NODELAY 1
#define SO_TCP_MAXSEG  2
#if 0
#define SO_TCP_KEEPALIVE 2 /*may not be supported in some systems*/
#define SO_TCP_KEEPIDLE  3
#define SO_TCP_KEEPINTVL 4
#define SO_TCP_KEEPCNT   5
#endif
/* Standard well-defined IP protocols.  */
// #define UHOS_IPPROTO_IP 0 /* Dummy protocol for TCP.  */

// #define UHOS_IP_MULTICAST_IF   32
// #define UHOS_IP_MULTICAST_TTL  33
// #define UHOS_IP_ADD_MEMBERSHIP 35

// 产品适配时，需要定义宏CONFIG_UHOS_FD_SETSIZE。 在RTOS上建议是256， 在LINUX上是1024
// 如果需要精确确定此值，需要在平台底层获取fd_bits的长度，调整为该长度
// 宏CONFIG_UHOS_FD_SETSIZE 需要大于等于fd_bits的长度

typedef struct uh_fd_set
{
    uhos_u8 fd_bits[(CONFIG_UHOS_FD_SETSIZE + 7) / 8];
} uhos_fd_set;

typedef uhos_u32 uhos_in_addr_t;

struct uhos_in_addr
{
    uhos_in_addr_t s_addr;
};

/* members are in network byte order */
/*
    注： 在lwip中是如下数据结构，在linux中 没有sin_len; sin_family是16位的
    al层实现时，注意转换
*/
struct uhos_sockaddr_in
{
    uhos_u8 sin_len;
    uhos_u8 sin_family;
    uhos_u16 sin_port;
    struct uhos_in_addr sin_addr;
#define SIN_ZERO_LEN 8
    uhos_char sin_zero[SIN_ZERO_LEN];
};

/* IPv6 address */
struct uhos_in6_addr
{
    union
    {
        uhos_u8 __u6_addr8[16];
        uhos_u16 __u6_addr16[8];
        uhos_u32 __u6_addr32[4];
    } __in6_u;
};

/* POSIX.1g specifies this type name for the `sa_family' member.  */
typedef unsigned short int uhos_sa_family_t;

/* This macro is used to declare the initial common members
   of the data types used for socket addresses, `struct sockaddr',
   `struct sockaddr_in', `struct sockaddr_un', etc.  */

#define UHOS_SOCKADDR_COMMON(sa_prefix) uhos_sa_family_t sa_prefix##family

#define UHOS_SOCKADDR_COMMON_SIZE (sizeof(unsigned short int))
/* Ditto, for IPv6.  */
struct uhos_sockaddr_in6
{
    UHOS_SOCKADDR_COMMON(sin6_);
    uhos_u16 sin6_port;             /* Transport layer port # */
    uhos_u32 sin6_flowinfo;         /* IPv6 flow information */
    struct uhos_in6_addr sin6_addr; /* IPv6 address */
    uhos_u32 sin6_scope_id;         /* IPv6 scope-id */
};

/* Size of struct sockaddr_storage.  */
#define UHOS_SS_SIZE 128

/* Structure large enough to hold any socket address (with the historical
   exception of AF_UNIX).  */
#define uhos_ss_aligntype unsigned long int
#define UHOS_SS_PADSIZE   (UHOS_SS_SIZE - UHOS_SOCKADDR_COMMON_SIZE - sizeof(uhos_ss_aligntype))
struct uhos_sockaddr_storage
{
    UHOS_SOCKADDR_COMMON(ss_); /* Address family, etc.  */
    uhos_char __ss_padding[UHOS_SS_PADSIZE];
    uhos_ss_aligntype __ss_align; /* Force desired alignment.  */
};

struct uhos_sockaddr_nl
{
    uhos_u16 nl_family; /* AF_NETLINK	*/
    uhos_u16 nl_pad;    /* zero		*/
    uhos_u32 nl_pid;    /* port ID	*/
    uhos_u32 nl_groups; /* multicast groups mask */
};

/* IPv4 multicast request.  */
struct uhos_ip_mreq
{
    /* IP multicast address of group.  */
    struct uhos_in_addr imr_multiaddr;

    /* Local IP address of interface.  */
    struct uhos_in_addr imr_interface;
};

struct uhos_sockaddr
{
    uhos_u8 sa_len;
    uhos_u8 sa_family;
    uhos_char sa_data[14];
};

/* Structure describing the address of an AF_LOCAL (aka AF_UNIX) socket.  */
struct uhos_sockaddr_un
{
    UHOS_SOCKADDR_COMMON(sun_);
    uhos_char sun_path[108]; /* Path name.  */
};

#ifdef _UHOS_ANDROID_
struct uhos_addrinfo
{
    uhos_s32 ai_flags;             /* Input flags. */
    uhos_s32 ai_family;            /* Address family of socket. */
    uhos_s32 ai_socktype;          /* Socket type. */
    uhos_s32 ai_protocol;          /* Protocol of socket. */
    uhos_socklen_t ai_addrlen;     /* Length of socket address. */
    uhos_char *ai_canonname;       /* Canonical name of service location. */
    struct uhos_sockaddr *ai_addr; /* Socket address of socket. */
    struct uhos_addrinfo *ai_next; /* Pointer to next in list. */
};
#else
struct uhos_addrinfo
{
    uhos_s32 ai_flags;             /* Input flags. */
    uhos_s32 ai_family;            /* Address family of socket. */
    uhos_s32 ai_socktype;          /* Socket type. */
    uhos_s32 ai_protocol;          /* Protocol of socket. */
    uhos_socklen_t ai_addrlen;     /* Length of socket address. */
    struct uhos_sockaddr *ai_addr; /* Socket address of socket. */
    uhos_char *ai_canonname;       /* Canonical name of service location. */
    struct uhos_addrinfo *ai_next; /* Pointer to next in list. */
};
#endif

struct uhos_hostent
{
    uhos_char *h_name;       /* official name of host */
    uhos_char **h_aliases;   /* alias list */
    uhos_s32 h_addrtype;     /* host address type */
    uhos_s32 h_length;       /* length of address */
    uhos_char **h_addr_list; /* list of addresses */
};
#define h_addr h_addr_list[0] /* for backward compatibility */

#define UHOS_IF_NAMESIZE 16
typedef uhos_char *uhos_caddr_t;

/* Device mapping structure. I'd just gone off and designed a
   beautiful scheme using only loadable modules with arguments for
   driver options and along come the PCMCIA people 8)

   Ah well. The get() side of this is good for WDSETUP, and it'll be
   handy for debugging things. The set side is fine for now and being
   very small might be worth keeping for clean configuration.  */

struct uhos_ifmap
{
    uhos_size_t mem_start;
    uhos_size_t mem_end;
    uhos_u16 base_addr;
    uhos_u8 irq;
    uhos_u8 dma;
    uhos_u8 port;
    /* 3 bytes spare */
};

struct uhos_ifreq
{
#define UHOS_IFHWADDRLEN 6
#define UHOS_IFNAMSIZ    UHOS_IF_NAMESIZE
    union
    {
        uhos_char ifrn_name[UHOS_IFNAMSIZ]; /* Interface name, e.g. "en0".  */
    } ifr_ifrn;

    union
    {
        struct uhos_sockaddr ifru_addr;
        struct uhos_sockaddr ifru_dstaddr;
        struct uhos_sockaddr ifru_broadaddr;
        struct uhos_sockaddr ifru_netmask;
        struct uhos_sockaddr ifru_hwaddr;
        uhos_s16 ifru_flags;
        uhos_s32 ifru_ivalue;
        uhos_s32 ifru_mtu;
        struct uhos_ifmap ifru_map;
        uhos_char ifru_slave[UHOS_IFNAMSIZ]; /* Just fits the size */
        uhos_char ifru_newname[UHOS_IFNAMSIZ];
        uhos_caddr_t ifru_data;
    } ifr_ifru;
};
#define ifr_name      ifr_ifrn.ifrn_name      /* interface name 	*/
#define ifr_hwaddr    ifr_ifru.ifru_hwaddr    /* MAC address 		*/
#define ifr_addr      ifr_ifru.ifru_addr      /* address		*/
#define ifr_flags     ifr_ifru.ifru_flags     /* flags		*/
#define ifr_netmask   ifr_ifru.ifru_netmask   /* interface net mask	*/
#define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast addr	*/

struct uhos_ifconf
{
    uhos_s32 ifc_len; /* Size of buffer.  */
    union
    {
        uhos_caddr_t ifcu_buf;
        struct uhos_ifreq *ifcu_req;
    } ifc_ifcu;
};
#define uhos_ifc_buf ifc_ifcu.ifcu_buf /* Buffer address.  */
#define uhos_ifc_req ifc_ifcu.ifcu_req /* Array of structures.  */
// #define _IOT_ifconf _IOT(_IOTS(struct uhos_ifconf),1,0,0,0,0) /* not right */

/* Structure for scatter/gather I/O.  */
struct uhos_iovec
{
    uhos_void *iov_base; /* Pointer to data. */
    uhos_size_t iov_len; /* Length of data.  */
};

/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct uhos_msghdr
{
    uhos_void *msg_name;        /* Address to send to/receive from.  */
    uhos_socklen_t msg_namelen; /* Length of address data.  */
    struct uhos_iovec *msg_iov; /* Vector of data to send/receive into.  */
    uhos_size_t msg_iovlen;     /* Number of elements in the vector.  */
    uhos_void *msg_control;     /* Ancillary data (eg BSD filedesc passing). */
    uhos_size_t msg_controllen; /* Ancillary data buffer length.!! The type should be socklen_t but the definition of the kernel is incompatible with this */
    uhos_s32 msg_flags;         /* Flags on received message.  */
};

/* Structure used for storage of ancillary data object information.  */
struct uhos_cmsghdr
{
    uhos_size_t cmsg_len; /* Length of data in cmsg_data plus length
             of cmsghdr structure.
             !! The type should be socklen_t but the
             definition of the kernel is incompatible
             with this.  */
    uhos_s32 cmsg_level;  /* Originating protocol.  */
    uhos_s32 cmsg_type;   /* Protocol specific type.  */

#ifdef __glibc_c99_flexarr_available
    __extension__ unsigned char __cmsg_data __flexarr; /* Ancillary data.  */
#endif
};

#define UHOS_MSG_CTRUNC 0x08
/* Ancillary data object manipulation macros.  */

#ifdef __glibc_c99_flexarr_available
#define UHOS_CMSG_DATA(cmsg) ((cmsg)->__cmsg_data)
#else
#define UHOS_CMSG_DATA(cmsg) ((uhos_u8 *)((struct cmsghdr *)(cmsg) + 1))
#endif

#define UHOS_NET_CMSG_NXTHDR(mhdr, cmsg) uhos_net_cmsg_nxthdr(mhdr, cmsg)
#define UHOS_CMSG_FIRSTHDR(mhdr) \
    ((uhos_size_t)(mhdr)->msg_controllen >= sizeof(struct uhos_cmsghdr) ? (struct uhos_cmsghdr *)(mhdr)->msg_control : (struct uhos_cmsghdr *)0)

#define UHOS_CMSG_ALIGN(len) (((len) + sizeof(uhos_size_t) - 1) & (uhos_size_t) ~(sizeof(uhos_size_t) - 1))
#define UHOS_CMSG_SPACE(len) (UHOS_CMSG_ALIGN(len) + UHOS_CMSG_ALIGN(sizeof(struct uhos_cmsghdr)))
#define UHOS_CMSG_LEN(len)   (UHOS_CMSG_ALIGN(sizeof(struct uhos_cmsghdr)) + (len))

#define UHOS_IP_RF      0x8000 /* reserved fragment flag */
#define UHOS_IP_DF      0x4000 /* dont fragment flag */
#define UHOS_IP_MF      0x2000 /* more fragments flag */
#define UHOS_IP_OFFMASK 0x1fff /* mask for fragmenting bits */
/*
 * Structure of an internet header, naked of options.
 */
typedef struct uhos_iphdr
{
#if UHOS_BYTE_ORDER == UHOS_LITTLE_ENDIAN
    uhos_u8 ip_hl : 4; /* header length */
    uhos_u8 ip_v  : 4; /* version */
#endif
#if UHOS_BYTE_ORDER == UHOS_BIG_ENDIAN
    uhos_u8 ip_v  : 4; /* version */
    uhos_u8 ip_hl : 4; /* header length */
#endif
    uhos_u8 ip_tos;             /* type of service */
    uhos_u16 ip_len;            /* total length */
    uhos_u16 ip_id;             /* identification */
    uhos_u16 ip_off;            /* fragment offset field */
    uhos_u8 ip_ttl;             /* time to live */
    uhos_u8 ip_p;               /* protocol */
    uhos_u16 ip_sum;            /* checksum */
    struct uhos_in_addr ip_src; /* source address */
    struct uhos_in_addr ip_dst; /* dest address */
} uhos_iphdr_st;

#define UHOS_AI_NUMERICSERV 0x0400 /* Don't use name resolution.  */

/* Possible values for `ai_flags' field in `addrinfo' structure.  */
#define UHOS_AI_PASSIVE     0x0001 /* Socket address is intended for `bind'.  */
#define UHOS_AI_CANONNAME   0x0002 /* Request for canonical name.  */
#define UHOS_AI_NUMERICHOST 0x0004 /* Don't use name resolution.  */
#define UHOS_AI_V4MAPPED    0x0008 /* IPv4 mapped addresses are acceptable.  */
#define UHOS_AI_ALL         0x0010 /* Return IPv4 mapped and IPv6 addresses.  */
#define UHOS_AI_ADDRCONFIG                             \
    0x0020 /* Use configuration of this host to choose \
returned address type..  */

/* Socket level message types.  This must match the definitions in
   <linux/socket.h>.  */
#define UHOS_SCM_RIGHTS 0x01

// #define UHOS_SOL_SOCKET   1
#define UHOS_SO_REUSEADDR 2
#define UHOS_SO_ERROR     4
#define UHOS_SO_RCVTIMEO  -1

/* Type to represent a port.  */
typedef uhos_u16 uhos_in_port_t;

struct uhos_in_pktinfo
{
    uhos_s32 ipi_ifindex;             /* Interface index  */
    struct uhos_in_addr ipi_spec_dst; /* Routing destination address  */
    struct uhos_in_addr ipi_addr;     /* Header destination address  */
};

/**
 * @typedef uh_conn_auth_info_t
 * @brief 云端连接认证证书。
 * cacert ca证书, 设备认证云端只需要一个证书，使用该参数。
 * cacert_len ca证书长度。
 * client_cert 设备证书公钥，双向认证，用于云端认证设备。
 * client_cert_len 设备证书长度。
 * client_privkey 设备私钥，与设备证书中的公钥配对；。
 * client_privkey_len 设备私钥长度。
 * @return N/A。
 */
typedef struct auth_info
{
    uhos_u8 *ca_cert;
    uhos_u32 ca_cert_len;
    uhos_u8 *client_cert;
    uhos_u32 client_cert_len;
    uhos_u8 *client_privkey;
    uhos_u32 client_privkey_len;
} uh_conn_auth_info_t;

/**
 * @brief struct uhos_icmp icmp报文首部定义的宏;
 */
#define UHOS_ICMP_ER   0  /* echo reply */
#define UHOS_ICMP_DUR  3  /* destination unreachable */
#define UHOS_ICMP_SQ   4  /* source quench */
#define UHOS_ICMP_RD   5  /* redirect */
#define UHOS_ICMP_ECHO 8  /* echo */
#define UHOS_ICMP_TE   11 /* time exceeded */
#define UHOS_ICMP_PP   12 /* parameter problem */
#define UHOS_ICMP_TS   13 /* timestamp */
#define UHOS_ICMP_TSR  14 /* timestamp reply */
#define UHOS_ICMP_IRQ  15 /* information request */
#define UHOS_ICMP_IR   16 /* information reply */
#define UHOS_ICMP_AM   17 /* address mask request */
#define UHOS_ICMP_AMR  18 /* address mask reply */
/**
 * @brief struct uhos_icmp icmp报文首部数据结构.
 */
typedef struct uhos_icmphdr
{
    uhos_u8 icmp_type;
    uhos_u8 icmp_code;
    uhos_u16 icmp_cksum;
    uhos_u16 icmp_id;
    uhos_u16 icmp_seq;
    uhos_u8 icmp_data[0]; /* 由具体data长度决定;注意申请内存时 size = sizeof(uhos_icmphdr_st)+dataLen */
} uhos_icmphdr_st;

/**
 * @brief struct uhos_ipconfig IP配置信息数据结构，后期会不断补充.
 */
typedef struct uhos_ipconfig
{
    struct uhos_in_addr ip_addr; /* 4字节的网络字节序int数据 */
    struct uhos_in_addr netmask;
    struct uhos_in_addr gw;
} uhos_ipconfig_st;
typedef void (*uhos_net_ipconfig_notify_cb)(uhos_ipconfig_st ipconfig);

/**
 * @brief Socket库socket create。
 */
extern uhos_s32 uhos_net_socket(uhos_s32 domain, uhos_s32 type, uhos_s32 protocol);

/**
 * @brief Socket库close fd。
 * @param
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_close(uhos_s32 sockfd);

/**
 * @brief Socket库bind。
 */
extern uhos_s32 uhos_net_bind(uhos_s32 sockfd, const struct uhos_sockaddr *addr, uhos_socklen_t addrlen);

/**
 * @brief Socket库listen。
 */
extern uhos_s32 uhos_net_listen(uhos_s32 sockfd, uhos_s32 backlog);

/**
 * @brief Socket库accept。
 */
extern uhos_s32 uhos_net_accept(uhos_s32 sockfd, struct uhos_sockaddr *addr, uhos_socklen_t *addrlen);

/**
 * @brief Socket库connect。
 */
extern uhos_s32 uhos_net_connect(uhos_s32 sockfd, const struct uhos_sockaddr *addr, uhos_socklen_t addrlen);

/**
 * @brief Socket库send。
 */
extern uhos_s32 uhos_net_send(uhos_s32 sockfd, const uhos_void *buf, uhos_size_t len, uhos_s32 flags);

/**
 * @brief Socket库sendto。
 */
extern uhos_s32 uhos_net_sendto(uhos_s32 sockfd,
                                const uhos_void *buf,
                                uhos_size_t len,
                                uhos_s32 flags,
                                const struct uhos_sockaddr *addr,
                                uhos_socklen_t addrlen);

/**
 * @brief Socket库recv。
 */
extern uhos_s32 uhos_net_recv(uhos_s32 sockfd, uhos_void *buf, uhos_size_t len, uhos_s32 flags);

/**
 * @brief           Socket库recvfrom
 *
 * @param[in] sockfd    socket 描述符
 * @param[out] buf      存储接收的数据
 * @param[in] len       最大可接收长度
 * @param[in] flags     接收方式，详情看下方描述
 * @param[out] src_addr 发送方地址与端口
 * @param[in] addrlen   地址长度，可设置为 sizeof(struct uhos_sockaddr)
 * @return uhos_ssize_t 返回实际接收字节数
 * @note    flags:
 *              0 :默认接收方式， MSG_OOB:接收带外数据
 *              MSG_DNOTWAIT: 设置为非阻塞方式
 *              MSG_WAITALL: 强迫接收len大小的数据才返回，除非有信号或错误产生
 *              MSG_PEEK: 查看数据标识，返回的数据并不在系统中删除，如果再次调用recv函数会返回相同的数据
 */
extern uhos_ssize_t uhos_net_recvfrom(uhos_s32 sockfd,
                                      uhos_void *buf,
                                      uhos_size_t len,
                                      uhos_s32 flags,
                                      struct uhos_sockaddr *src_addr,
                                      uhos_socklen_t *addrlen);

/**
 * @brief           设置socket选项。
 * @param sockfd    socket描述符。
 * @param level     SO_LEVEL_XXX。
 * @param optname   SO_OPT_XXX。
 * @param optval    指向设置值的指针。
 * @param optlen    数据长度。
 * @return 成功返回0，不支持返回0，其他错误返回-1。
 *
 * @note    1. optname 是SO_OPT_RCVTIMEO或者SO_OPT_SNDTIMEO时，optval是struct uhos_timeval *转化来的。
 *             uhos_timeval与 timeval类型比较
 *             struct uhos_timeval                          |     struct timeval
 *             {                                            |     {
 *                 uhos_s32 tv_sec;                         |         long tv_sec;
 *                 uhos_s32 tv_usec;                        |         long uhos_s32 tv_usec;
 *             }                                            |     }
 *            64位平台： sizeof(struct uhos_timeval)  == 8   |   sizeof(struct timeval)  == 16
 *          警告：
 *          当平台是64位的情况下, 需要将optval转成struct timeval模式, 如下所示:
 *          struct uhos_timeval *temp = (struct uhos_timeval *)optval;
 *          struct timeval sys_data = {.tv_sec = (long)temp->tv_sec, .tv_usec = (long)temp->tv_usec};
 *
 *          2. 后续如果用其他数据结构，另作描述
 */
extern uhos_s32 uhos_net_setsockopt(uhos_s32 sockfd, uhos_s32 level, uhos_s32 optname, const uhos_void *optval, uhos_s32 optlen);

/**
 * @brief 获取socket选项。
 * @param sockfd socket描述符。
 * @param level SO_LEVEL_XXX。
 * @param optname SO_OPT_XXX。
 * @param optval 指向设置值的指针。
 * @param optlen 数据长度。
 * @return 成功返回0，不支持返回0，其他错误返回-1。
 */
extern uhos_s32 uhos_net_getsockopt(uhos_s32 sockfd, uhos_s32 level, uhos_s32 optname, const uhos_void *optval, uhos_u32 *optlen);

extern uhos_s32 uhos_net_getaddrinfo(const uhos_char *nodename, const uhos_char *servname, const struct uhos_addrinfo *hints, struct uhos_addrinfo **res);

/* Convert error return from getaddrinfo() to a string.  */
extern const uhos_char *uhos_net_gai_strerror(uhos_s32 __ecode);

extern uhos_void uhos_net_freeaddrinfo(struct uhos_addrinfo *ai);
/**
 * @brief 字节序转换htonl
 * @param hostlong 主机序4字节整数。
 * @return 网络序4字节整数。
 */
extern uhos_u32 uhos_net_htonl(uhos_u32 hostlong);

/**
 * @brief 字节序转换ntohl
 * @param netlong 网络序4字节整数。
 * @return 主机序4字节整数。
 */
extern uhos_u32 uhos_net_ntohl(uhos_u32 netlong);

/**
 * @brief 字节序转换htons
 * @param hostshort 主机序2字节整数。
 * @return 网络序2字节整数。
 */
extern uhos_u16 uhos_net_htons(uhos_u16 hostshort);

/**
 * @brief 字符串IP转为网络字节序(inet_aton)
 * @param[in]   str_ip  字符串ip
 * @param[out]  inp     网络序2字节整数。
 * @return 网络序4字节整数。
 */
extern uhos_s32 uhos_net_aton(const uhos_char *str_ip, struct uhos_in_addr *inp);

/* Convert a Internet address in binary network format for interface
   type AF in buffer starting at CP to presentation form and place
   result in buffer of length LEN astarting at BUF.  */
/**
 * @brief      二进制地址转化为字符串地址(inet_ntop)
 * @param[in] af    AF 类型
 * @param[in] cp    二进制地址
 * @param[out] buf  存储字符串地址
 * @param[out] len  字符串地址长度
 * @return uhos_char*
 */
extern uhos_char *uhos_net_ntop(uhos_s32 af, const uhos_void *cp, uhos_char *buf, uhos_socklen_t len);

/* Convert from presentation format of an Internet number in buffer
   starting at CP to the binary network format and store result for
   interface type AF in buffer starting at BUF.  */

/**
 * @brief       将字符串转化为二进制网络字节序(inet_pton)
 * @param[in] af    AF类型 AF_INET 或者AF_INET6
 * @param[in] src   源数据
 * @param[out] buf  存储二进制字节序
 * @return uhos_s32
 */
extern uhos_s32 uhos_net_pton(uhos_s32 af, const uhos_char *src, uhos_void *buf);

/**
 * @brief 字节序转换ntohs
 * @param netshort 网络序2字节整数。
 * @return 主机序2字节整数。
 */
extern uhos_u16 uhos_net_ntohs(uhos_u16 netshort);

/**
 * @brief IP地址转换为字符串。
 * @param in 网络字节序的IP地址。
 * @return 点分十进制表示的IP地址字符串。 TODO AL_NET
 */
extern uhos_char *uhos_net_inet_ntoa(struct uhos_in_addr in);

/**
 * @brief 字符串转换为IP地址。
 * @param cp 点分十进制表示的IP地址字符串。
 * @return 网络序4字节整数表示的IP地址。  TODO AL_NETS
 */
extern uhos_u32 uhos_net_inet_addr(const uhos_char *cp);

/**
 * @brief IO复用。
 * @param nfds 描述集合中所有文件描述符，最大值加1。
 * @param readfds 指向fd set的指针。监视文件描述是否可读。NULL表示不关心。
 * @param writefds 指向fd set的指针。监视文件描述是否可写。NULL表示不关心。
 * @param exceptfds 指向fd set的指针。监视文件描述是否异常。NULL表示不关心。
 * @param timeout select超时时间。NULL表示select处于阻塞状态。
 * @return >0，一些文件描述符可读、可写或者异常。<0，错误。=0，超时。
 */
extern uhos_s32 uhos_net_select(uhos_s32 nfds, uhos_void *readfds, uhos_void *writefds, uhos_void *exceptfds, struct uhos_timeval *timeout);

/**
 * @brief 清空FD集合。
 * @param set 指向fd set的指针。
 * @return N/A。
 */
extern uhos_void uhos_net_fd_zero(uhos_void *set);

/**
 * @brief 清除FD集合中指定的文件描述符。
 * @param fd 文件描述符。
 * @param set 指向fd set的指针。
 * @return N/A。
 */
extern uhos_void uhos_net_fd_clr(uhos_s32 fd, uhos_void *set);

/**
 * @brief 设置FD集合中指定的文件描述符。
 * @param fd 文件描述符。
 * @param set 指向fd set的指针。
 * @return N/A。
 */
extern uhos_void uhos_net_fd_set(uhos_s32 fd, uhos_void *set);

/**
 * @brief 判断指定的文件描述符是否就绪。
 * @param fd 文件描述符。
 * @param set 指向fd set的指针。
 * @return 1表示fd已就绪，0表示fd未就绪。
 */
extern uhos_s32 uhos_net_fd_isset(uhos_s32 fd, uhos_void *set);

/**
 * @brief 获取fd set的大小。
 * @return Fd set的大小。
 */
extern uhos_u32 uhos_net_fd_size(uhos_void);

/**
 * @brief 申请可用于select的文件描述符。
 * @return 成功返回FD，失败返回-1。
 */
extern uhos_s32 uhos_net_alloc_fd(uhos_void);

/**
 * @brief 释放文件描述符。
 * @param fd 文件描述符。
 * @return N/A。
 */
extern uhos_void uhos_net_free_fd(uhos_s32 fd);

/**
 * @brief 通知select有数据可读。
 * @param fd 文件描述符。
 * @return N/A。
 */
extern uhos_void uhos_net_fd_rcv_plus(uhos_s32 fd);

/**
 * @brief 数据读取后清除可读标记。
 * @param fd 文件描述符。
 * @return N/A。
 */
extern uhos_void uhos_net_fd_rcv_minus(uhos_s32 fd);

/** @def NETIF_TYPE_STA/NETIF_TYPE_AP
 *
 * @brief 网络接口类型，在不同的系统上，WIFI STA和WIFI
 * AP可能是同一个网络接口，也有可能是不同的网络接口。 NETIF_TYPE_STA WIFI
 * STA对应的网络接口。 NETIF_TYPE_AP WIFI AP对应的网络接口。
 *
 */
#define NETIF_TYPE_STA 0
#define NETIF_TYPE_AP  1

/**
 * @brief DNS服务器设置与查询。
 * @param op 操作方式，OP_SET/OP_GET。
 * @param
 * dns_server，域名服务器，点分十进制的IP地址。当操作方式是查询时，如果成功填写域名服务器；否则不填写。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_dns_config(uhos_u8 op, uhos_char *dns_server);

#define UHOS_DHCPC_EVENT_RENEW_OK   0
#define UHOS_DHCPC_EVENT_RENEW_FAIL 1

#define UHOS_DHCP_ERROR_NONE                       0
#define UHOS_DHCPC_RENEW_FAIL_NAK                  -1
#define UHOS_DHCPC_RENEW_FAIL_TIMEOUT              -2
#define UHOS_DHCPC_RENEW_FAIL_HAIER_CNDI_OPT_ERROR -3

/**
 * @brief DHCP状态变化通知。
 * @note 当DHCP状态发生变化（DHCPC RENEW OK/DHCPC RENEW FAIL）时，通过此回调接口通知调用者。
 * @param event DHCP事件。
 * @param extra_error  事件原因。
 * @return 成功返回0，失败返回-1。
 */
typedef uhos_s32 (*uhos_net_dhcp_status_callback)(uhos_u8 event, uhos_s32 extra_error);

/**
 * @brief 设置 DHCP 状态变化通知回调。
 * @param callback 回调函数，设置为 NULL 时将清除回调。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API extern uhos_s32 uhos_net_dhcp_set_status_callback(uhos_net_dhcp_status_callback callback);

/**
 * @brief 提供给DHCP的IP地址。
 * @param
 * 在通常情况下，同一设备会连接相同的路由器，通过DHCP获取到的IP地址也是相同的。提供上次获得的IP地址，可以加快DHCP的通信流程。
 * @param offered_ip 上次获得的IP地址，点分十进制表示。
 * @return 成功返回0，失败返回-n。
 */
extern uhos_s32 uhos_net_dhcp_offer_ip(uhos_char *offered_ip);

/** @def DHCP_MODE_XXX
 *
 * @brief DHCP模式。
 CLIENT DHCP客户端
 SERVER DHCP服务端
 */
#define DHCP_MODE_CLIENT 1
#define DHCP_MODE_SERVER 2

/**
 * @brief DHCP模式配置。
 * @param op 操作方式，OP_START/OP_STOP。
 * @param mode DHCP模式，DHCP_MODE_XXX。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_dhcp_config(uhos_u8 op, uhos_u8 mode);

/**
 * @brief DHCP地址池设置。
 * @param op 操作方式，OP_ADD/OP_DELETE。
 * @param address_pool_start DHCP地址池起始地址，点分十进制表示的IP地址。
 * @param address_pool_end DHCP地址池结束地址，点分十进制表示的IP地址。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_dhcp_pool_set(uhos_u8 op, uhos_char *address_pool_start, uhos_char *address_pool_end);

/*error*/
#define ERR_OTHERS 1
#define ERR_EAGAIN 2
/**
 * @brief 获取socket接口或者ssl接口调用返回的错误码。
 * @param fd socket描述符，SSL会话标识为NULL时有效。
 * @param ssl_id SSL会话标识。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_errno(uhos_s32 fd, uhos_ctx_id ssl_id);

#define MAX_ERRNO_NUM 139
/**
 * @brief 获取最近设置的ERRNO。
 * @note 如果不支持，返回0。
 * @return
 * errno，如果大于0，表示有错误；如果等于0，表示未检测到连接错误；如果小于0，表示链路层错误。
 */
extern uhos_s32 uhos_net_errno_get(uhos_void);

/**
 * @brief 设置主机名称。
 * @param hostname 以’\0’结尾的字符串。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_net_sethostname(const uhos_char *hostname);

/**
 * @brief 使能/关闭发送ICMP不可达功能。
 * @param enable 0-关闭，1-使能。
 * @return N/A。
 */
extern uhos_void uhos_net_enable_icmp_unreach(uhos_s32 enable);

/**
 * @brief 在指定接口上发送免费ARP。
 * @param netif_type 网络接口类型，NETIF_TYPE_XXX。
 * @param fn_ip_conflict_cb 可以填null,如果为null,需要uhos_net_set_ip_conflict_callback设置回调;建议:调用此接口时直接设置好回调;
 * @return N/A。
 */
typedef uhos_void (*uhos_ip_conflict_cb_fn)(uhos_void);
extern uhos_void uhos_net_tx_gratuitous_arp(uhos_u8 netif_type, uhos_ip_conflict_cb_fn fn_ip_conflict_cb);

/**
 * @brief 设置IP冲突检测回调。调用此接口会设置一个全局的cb;此时uhos_net_tx_gratuitous_arp不设置回调时,会调用此全局回调;
 * @param fn_ip_conflict_cb 回调接口。
 * @return N/A。
 */
extern uhos_void uhos_net_set_ip_conflict_callback(uhos_ip_conflict_cb_fn fn_ip_conflict_cb);

/**
 * @brief 查找系统ARP表项;
 * @param [in] netif_type 网络接口类型,NETIF_TYPE_XXX;
 * @param [in] ip_addr 待查找的IPv4地址,网络字节序;
 * @param [out] eth_addr ARP地址;
 * @return uhos_s32 成功返回0，失败返回-1;
 */
extern uhos_s32 uhos_net_find_arp(uhos_u8 netif_type, uhos_u32 ip_addr, uhos_u8 *eth_addr);

/* Put the local address of FD into *ADDR and its length in *LEN.  */
extern uhos_s32 uhos_net_getsockname(uhos_s32 fd, struct uhos_sockaddr *addr, uhos_socklen_t *len);

extern struct uhos_cmsghdr *uhos_net_cmsg_nxthdr(struct uhos_msghdr *mhdr, struct uhos_cmsghdr *cmsg);

/* Receive a message as described by MESSAGE from socket FD.
   Returns the number of bytes read or -1 for errors.
*/
extern uhos_ssize_t uhos_net_recvmsg(uhos_s32 fd, struct uhos_msghdr *message, uhos_s32 flags);

#define UHOS_IN6_IS_ADDR_LINKLOCAL(a)                                                             \
    ({                                                                                            \
        const struct uhos_in6_addr *__a = (const struct uhos_in6_addr *)(a);                      \
        (__a->__in6_u.__u6_addr32[0] & uhos_net_htonl(0xffc00000)) == uhos_net_htonl(0xfe800000); \
    })

/* Put the address of the peer connected to socket FD into *ADDR
   (which is *LEN bytes long), and its actual length into *LEN.  */
uhos_s32 uhos_net_getpeername(uhos_s32 fd, struct uhos_sockaddr *addr, uhos_socklen_t *len);

uhos_s32 uhos_net_gethostname(uhos_char *name, uhos_s32 namelen);

// uhos_s32 uhos_net_daemon(uhos_s32 nochdir, uhos_s32 noclose);

/**
 * @brief 获取底层ip信息接口;只有sdl需要适配; sdr中空函数即可;
 */
/**
 * @brief 获取通讯网卡的设备名称 [只有sdl需要适配; sdr中空函数即可]
 * @param [out] pc_name 网卡设备名称
 * @param [in][out] ps32_name_len 输入: 调用者缓存空间pc_name 的buf长度    输出: pc_name 的实际长度;
 * @return uhos_s32 -1: 获取失败;  成功返回>0的 名称长度;
 */
uhos_s32 uhos_net_get_eth_name(uhos_char *pc_name, uhos_s32 *ps32_name_len);

/**
 * @brief 获取ip; [只有sdl需要适配; sdr中空函数即可]
 * @param [OUT] pc_ip 点分式字符串格式
 * @return uhos_s32 UHOS_SUCCESS: 成功; UHOS_FAILURE:失败;
 */
uhos_s32 uhos_net_get_ip(uhos_char *pc_ip);

/**
 * @brief 获取netmask; [只有sdl需要适配; sdr中空函数即可]
 * @param [OUT] pc_netmask 点分式字符串格式
 * @return uhos_s32 UHOS_SUCCESS: 成功; UHOS_FAILURE:失败;
 */
uhos_s32 uhos_net_get_netmask(uhos_char *pc_netmask);

/**
 * @brief 获取 broadcast; [只有sdl需要适配; sdr中空函数即可]
 * @param [OUT] pc_broadcast 点分式字符串格式
 * @return uhos_s32 UHOS_SUCCESS: 成功; UHOS_FAILURE:失败;
 */
uhos_s32 uhos_net_get_broadcast(uhos_char *pc_broadcast);

/**
 * @brief 获取 mac; [只有sdl需要适配; sdr中空函数即可]
 * @param [IN] pc_devname : 指定网卡名称,如果不指定(即填NULL),默认获取无线网卡的mac地址;
 * @param [OUT] pu8_mac
 * @return uhos_s32
 */
uhos_s32 uhos_net_get_mac(uhos_char *pc_devname, uhos_u8 *pu8_mac);

/**
 * @brief 获取 gateway; [只有sdl需要适配; sdr中空函数即可]
 * @param [OUT] pc_gateway 点分式字符串格式
 * @return uhos_s32 UHOS_SUCCESS: 成功; UHOS_FAILURE:失败;
 */
uhos_s32 uhos_net_get_gateway(uhos_char *pc_gateway);

/**
 * @brief 用于缓存ip信息;
 */
typedef struct
{
    uhos_char m_ip_str[16];
} uhos_ip_str_st;

/**
 * @brief 获取系统当前的dns信息; pc_dns = uhos_libc_zalloc(sizeof(uhos_ip_str_st) * ps32_cnt);
 *        接口内部最多支持10个; [只有sdl需要适配; sdr中空函数即可]
 * @param [OUT] pc_dns
 * @param [IN] [OUT] ps32_cnt
 * @return uhos_s32
 */
uhos_s32 uhos_net_get_dns(uhos_ip_str_st *pc_dns, uhos_s32 *ps32_cnt);

uhos_s32 uhos_net_ip_config(uhos_u8 netif_type, uhos_u8 op, uhos_char *local_ip_addr, uhos_char *net_mask, uhos_char *gateway_ip_addr);

/**
 * @brief ip信息变化通知;
 * @param [in] fn 类型参考:@typedef void (*uhos_net_ipconfig_notify_cb)(uhos_ipconfig_st ipconfig);
 * @return uhos_s32 成功返回 0; 失败返回 -1
 */
extern uhos_s32 uhos_net_regist_ip_config_notify(uhos_net_ipconfig_notify_cb fn);

#define IF_NAME_STR_LEN 16
#define IP_STR_MAX_LEN  16

typedef struct
{
    uhos_char if_name[IF_NAME_STR_LEN];
    uhos_u32 if_status;
} uhos_mon_ifstatus_st;
typedef void (*uhos_net_ifstatus_callback)(uhos_mon_ifstatus_st *status_info);

typedef struct
{
    uhos_u8 is_newaddr;
    uhos_u8 rsv[3];
    uhos_char if_name[IF_NAME_STR_LEN];
    uhos_char src_ip[IP_STR_MAX_LEN];
} uhos_mon_addr_st;
typedef void (*uhos_net_ifaddr_callback)(uhos_mon_addr_st *addr_info);

typedef struct
{
    uhos_u8 is_newroute;
    uhos_u8 rsv[3];
    uhos_u32 rt_metric;
    uhos_char if_name[IF_NAME_STR_LEN];
    uhos_char gateway[IP_STR_MAX_LEN];
} uhos_mon_route_st;
typedef void (*uhos_net_ifroute_callback)(uhos_mon_route_st *route_info);
typedef struct
{
    uhos_net_ifstatus_callback status_cb;
    uhos_net_ifaddr_callback ifaddr_cb;
    uhos_net_ifroute_callback ifroute_cb;
} uhos_net_mon_param_st;

/**
 * @brief 创建网络监听线程
 * @param param 用户传入监听回调函数
 * @ 返回值 成功返回0 失败返回-n
 */
uhos_s32 uhos_net_create_monitor_thread(uhos_net_mon_param_st *param);

#define UHOS_IF_UP_MASK        0x01
#define UHOS_IF_RUNNING_MASK   0X02
#define UHOS_IF_STATUS_INVALID 0xFFFF
/**
 * @brief 获取制定网卡名的状态信息
 * @param ifname 网卡名
 * @return 成功返回实际状态信息，失败返回0xFFFF。
 */
uhos_u32 uhos_net_get_if_status(uhos_char *ifname);

/**
 * @brief 通过dhcp方式获取ip地址
 * @param ifname 网卡名
 * @return 成功返回0，失败返回-n。
 */
uhos_s32 uhos_net_dhcp_obtain_ip(uhos_char *ifname);

/**
 * @brief 设置网卡的路由优先级
 * @param ifname 网卡名
 * @param gateway 点分十进制网关信息
 * @param metric 优先级，数值越低优先级越高
 * @return 成功返回0，失败返回-n。
 */
uhos_s32 uhos_net_add_metric(uhos_char *ifname, uhos_char *gateway, uhos_u32 metric);

/**
 * @brief 删除网卡的路由优先级设置
 * @param ifname 网卡名
 * @param gateway 点分十进制网关信息
 * @return 成功返回0，失败返回-n。
 */
uhos_s32 uhos_net_del_metric(uhos_char *ifname, uhos_char *gateway);

/**
 * @brief 删除网卡ip地址信息
 * @param ifname 网卡名
 * @return 成功返回0，失败返回-n。
 */
uhos_s32 uhos_net_flush_ip(uhos_char *ifname);

#endif
/**@} grp_uhosnet end */
