/**
 * @defgroup  grp_uhsddev 音视频SDK功能接口
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uhsd_video.h
 * @author dengtaowen (dengtaowen@haier.com)
 * @brief 音视频插件对外接口
 * @date 2023-03-24
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-03-24   <td>1.0     <td>        <td>
 * </table>
 */

#ifndef __UHSD_VIDEO_H__
#define __UHSD_VIDEO_H__

#include <uhsd_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PATH_SIZE           (128)
#define MAX_DEVID_SIZE          (20)
#define MAX_NETCAR_NAME_SIZE    (20)
#define IV_AVT_MAX_VENC_CHN_NUM (4)
/* CODEC ID */
typedef enum
{
    VIDEO_CODEC_TYPE_UNSUPPORTED = 0,
    VIDEO_CODEC_TYPE_H264 = 1,
    VIDEO_CODEC_TYPE_H265 = 2,
} uhsd_video_codec_type_e;

/* CODEC ID */
typedef enum
{
    VIDEO_ABILITY_UNSUPPORTED = 0,
    VIDEO_ABILITY_MONITOR = 1,                   //实时监控
    VIDEO_ABILITY_RECORDR = 2,                   //本地录像
    VIDEO_ABILITY_MONITORandRECORDR = 3,         //实时监控+本地录像
    VIDEO_ABILITY_CLOUD = 4,                     //云存
    VIDEO_ABILITY_MONITORandCLOUD = 5,           //实时监控+云存
    VIDEO_ABILITY_RECORDRandCLOUD = 6,           //本地录像+云存
    VIDEO_ABILITY_MONITORandRECORDRandCLOUD = 7, //实时监控+本地录像+云存
} uhsd_video_device_ability_e;

/* 音视频清晰度定义 */
typedef enum
{
    VIDEO_DEFINITION_FL = 0,         /*fluency，流畅*/
    VIDEO_DEFINITION_SD = 1,         /*standard definition，标清*/
    VIDEO_DEFINITION_HD = 2,         /*high definition，高清（单清晰度设备默认）*/
    VIDEO_DEFINITION_ONLY_AUDIO = 3, /*onlyaudio，纯音频*/

    VIDEO_DEFINITION_DEFAULT = 10, /*当前默认的清晰度等级，应该是0、1、2中的一种*/
} uhsd_video_definition_e;

/*define the aenc mode  of audio*/
typedef enum
{
    AENC_MODE_MONO = 0,   /*mono*/
    AENC_MODE_STEREO = 1, /*stereo*/

    AENC_MODE_BUTT
} uhsd_audio_aenc_mode_e;

/*define the sample rate  of audio*/
typedef enum
{
    AENC_SAMPLE_RATE_UNSUPPORTED = 0, /* Unsupported*/
    AENC_SAMPLE_RATE_8000 = 8000,     /* 8K samplerate*/
    AENC_SAMPLE_RATE_12000 = 12000,   /* 12K samplerate*/
    AENC_SAMPLE_RATE_11025 = 11025,   /* 11.025K samplerate*/
    AENC_SAMPLE_RATE_16000 = 16000,   /* 16K samplerate*/
    AENC_SAMPLE_RATE_22050 = 22050,   /* 22.050K samplerate*/
    AENC_SAMPLE_RATE_24000 = 24000,   /* 24K samplerate*/
    AENC_SAMPLE_RATE_32000 = 32000,   /* 32K samplerate*/
    AENC_SAMPLE_RATE_44100 = 44100,   /* 44.1K samplerate*/
    AENC_SAMPLE_RATE_48000 = 48000,   /* 48K samplerate*/
    AENC_SAMPLE_RATE_64000 = 64000,   /* 64K samplerate*/
    AENC_SAMPLE_RATE_96000 = 96000,   /* 96K samplerate*/

    AENC_SAMPLE_RATE_BUTT,
} uhsd_audio_sample_rate_e;

/*define the bit width  of audio*/
typedef enum
{
    AENC_BIT_WIDTH_8 = 0,  /* 8bit width */
    AENC_BIT_WIDTH_16 = 1, /* 16bit width*/
    AENC_BIT_WIDTH_24 = 2, /* 24bit width*/

    AENC_BIT_WIDTH_BUTT,
} uhsd_audio_bit_width_e;

/*define the aenc type of audio*/
typedef enum
{
    AENC_TYPE_PCM = 0,    /*pcm stream*/
    AENC_TYPE_G711A = 1,  /*g711a stream*/
    AENC_TYPE_G711U = 2,  /*g711u stream*/
    AENC_TYPE_G726 = 3,   /*g726*/
    AENC_TYPE_AAC = 4,    /*aac stream*/
    AENC_TYPE_AMR = 5,    /*amr stream*/
    AENC_TYPE_ADPCMA = 6, /*adpcm*/

    AENC_TYPE_BUTT
} uhsd_audio_aenc_type_e;

/*define the aac type*/
typedef enum
{
    AENC_AAC_TYPE_NONE = 0,
    AENC_AAC_TYPE_MAIN = 1,
    AENC_AAC_TYPE_LC = 2,
    AENC_AAC_TYPE_SSR = 3,
    AENC_AAC_TYPE_LTP = 4,
    AENC_AAC_TYPE_SBR = 5,
    AENC_AAC_TYPE_SCALCBLE = 6,
    AENC_AAC_TYPE_TWINVQ = 7,
    AENC_AAC_TYPE_CELP = 8,
    AENC_AAC_TYPE_HVXC = 9,

    AENC_AAC_TYPE_BUTT
} uhsd_audio_aenc_aac_type_e;

/*Defines the av infomation of av data*/
typedef struct
{
    uhsd_audio_aenc_type_e eAudioType;            /*audio codec type*/
    uhsd_audio_aenc_aac_type_e eAudioCodecOption; /*audio codec subtype*/
    uhsd_audio_aenc_mode_e eAudioMode;            /*audio mode*/
    uhsd_audio_bit_width_e eAudioBitWidth;        /*audio bit width,just support 16 bit*/
    uhsd_audio_sample_rate_e eAudioSampleRate;    /*audio sample rate*/
    uhsd_u32 u32SampleNumPerFrame;                /*audio sample num per Frame*/

    uhsd_video_codec_type_e eVideoType; /*video type(1:h264, 2:h265)*/
    uhsd_u32 u32VideoWidth;             /*video width*/
    uhsd_u32 u32VideoHight;             /*video hight*/
    uhsd_u32 u32Framerate;              /*frame rate*/

    uhsd_video_definition_e eDefintion; /*current default Defintion*/
    uhsd_u32 u32Reserve[4];             /*reserve*/
} uhsd_avt_data_info_t;

typedef enum
{
    UHSD_MDMAIN_BITRATE = 1, /*主码流*/
    UHSD_MDSUB_BITRATE = 2,  /*子码流*/
    UHSD_THIRD_BITRATE = 3,  /*第三路码流*/
} uhsd_avt_bit_rate_e;

//音视频编码
typedef struct
{
    uhsd_u8 *data;        //音视频帧数据指针
    uhsd_u32 data_size;   //音视频帧数据字节数
    uhsd_u64 frame_index; //音视频帧计数
    uhsd_u64 pts;         //音视频帧显示时间戳 UTC 0时区微秒时间戳
    uhsd_u64 dts;         //音视频帧解码时间戳（如果没有b帧，dts和pts一致）UTC 0时区微秒时间戳
    uhsd_u16
        av_channnel; //流通道标识，用来标识NVR类设备的多路流，对于普通IPC设备，当前值固定为1，如果是多摄像头(如双摄门铃,从1开始,1代表主摄,音频从1开始,代表主摄才有音频)。
    uhsd_video_definition_e definition; //本音视频帧的清晰度规格；
    uhsd_s32 is_key_frame;              //是否是关键帧 0 不是，1：是
    uhsd_u8 *extend_data;               //扩展数据
    uhsd_u32 extend_data_size;          ////扩展数据长度
} uhsd_frame_t;

typedef enum
{
    STREAM_TYPE_AUDIO = 0,      /*audio*/
    STREAM_TYPE_VIDEO = 1,      /*video*/
    STREAM_TYPE_AV = 2,         /*audio+video*/
    STREAM_TYPE_ONLY_AUDIO = 3, /*only audio*/
    STREAM_TYPE_ONLY_VIDEO = 4, /*only video*/
    STREAM_TYPE_BUTT
} uhsd_stream_type_e;

typedef enum
{
    UHSD_SEND_CHANNEL_MONITOR = 0, //实时监控
    UHSD_SEND_CHANNEL_RECORD,      //本地录像
    UHSD_SEND_CHANNEL_CLOUD,       //云存
} uhsd_send_channel_e;

//云存推送事件类型
typedef enum
{
    CS_EVENT_TYPE_MOTION = 0,       //移动侦测
    CS_EVENT_TYPE_PERSON = 1,       //人形识别
    CS_EVENT_TYPE_SOUND = 2,        //声音检测
    CS_EVENT_TYPE_LINKAGE = 3,      // data model Linkage Detection
    CS_EVENT_TYPE_AIFACE = 4,       //人脸识别
    CS_EVENT_TYPE_MANUAL = 5,       //主动报警
    CS_EVENT_TYPE_CRY_VOICE = 6,    //哭声侦测
    CS_EVENT_TYPE_PERSON_TRACE = 7, //人形跟踪
    CS_EVENT_TYPE_CAR_NUMBER = 8,   //车牌识别
    CS_EVENT_TYPE_CAR_SHAPE = 9,    //车型识别
    CS_EVENT_TYPE_OUT_AREA = 10,    //越界报警
    CS_EVENT_TYPE_AREA = 11,        //区域报警
    CS_EVENT_TYPE_PIR = 12,         // PIR报警
    CS_EVENT_TYPE_STAY = 13,        //逗留报警
    CS_EVENT_TYPE_DOORBELL = 14,    //门铃呼叫
    CS_EVENT_TYPE_PROTECE = 15,     //防拆报警
    CS_EVENT_TYPE_LOW_BATTERY = 16, //低电量报警
    CS_EVENT_TYPE_MAX
} uhsd_cs_evevt_type_e;

/**
 * @brief 接受多路音视频回调函数
 * @param cap_chn 流通道标识，用来标识NVR类设备的多路流
 * @param user_index 与设备建立P2P通道的用户序号，可取值[0~3]，特别注意，此标识仅为当前连接用户的P2P通道序号，非真实用户标识；
 * @param eAvtStreamType 帧类型
 * @param frame  帧数据
 */
typedef uhsd_s32 (*uhsd_recv_stream_cb)(uhsd_u16 cap_chn, uhsd_u16 user_index, uhsd_stream_type_e eAvtStreamType, uhsd_frame_t frame);

/**
 * @brief 开始接收音频对讲状态通知回调函数
 * @param cap_chn 流通道标识
 * @param avenc_info 接收对讲的音视频数据信息
 */
typedef uhsd_s32 (*uhsd_start_rcv_audio_stream_cb)(uhsd_u32 avt_channel, uhsd_avt_data_info_t *avenc_info);

/**
 * @brief 停止接收音频对讲状态通知回调函数
 * @param cap_chn 流通道标识
 * @param stream_typer 接收对讲的音视频数据信息
 */
typedef uhsd_s32 (*uhsd_stop_rcv_audio_stream_cb)(uhsd_u32 avt_channel, uhsd_stream_type_e stream_typer);

/**
 * @brief 云存服务状态通知回调函数
 * @param ugw_iot_cs_service_info 云存服务信息
 */
typedef struct
{
    uhsd_char status; //服务开启状态 0：未开启 1：开启
    uhsd_char type;   //服务类型 0：无效值 1：全时云存 2：事件云存
} uhsd_notify_cs_service_info;
typedef uhsd_void (*uhsd_notify_cs_service_cb)(uhsd_notify_cs_service_info *service_info);

/**
 * @brief IOT视频连接状态回调通知
 * @param status 状态  0——离线 1——上线
 */
typedef uhsd_void (*uhsd_notify_status_cb)(uhsd_s32 status);

/**
 * @brief 获取多路音视频编码信息
 * @param av_channnel 流通道标识，用来标识NVR类设备的多路流
 * @param definition 视频编码清晰度等级标识
 */
typedef uhsd_avt_data_info_t (*uhsd_get_av_enc_info_cb)(uhsd_u32 av_channnel, uhsd_video_definition_e definition);

/**
 * @brief 多路音视频同步关键帧
 * @param av_channnel 流通道标识，用来标识NVR类设备的多路流
 * @param definition 视频编码清晰度等级标识
 */
typedef uhsd_void (*uhsd_request_key_frame_cb)(uhsd_u32 av_channnel, uhsd_video_definition_e definition);

/**
 * @brief 多路音视频开关直播推流通知
 * @param av_channnel 流通道标识，用来标识NVR类设备的多路流
 * @param definition 视频编码清晰度等级标识
 * @param status 0:关闭直播推流
 * 		        1:开启直播推流
 */
typedef uhsd_void (*uhsd_control_live_stream_cb)(uhsd_u32 av_channnel, uhsd_video_definition_e definition, uhsd_s32 status);

/**
 * @brief 音视频直播观看人数通知
 * @param num 0:无人观看 >0:有人观看
 *
 */
typedef uhsd_void (*uhsd_live_stream_play_num_cb)(uhsd_s32 num);

typedef struct
{
    uhsd_char startflag;   //开启状态 0:关闭 1:开启
    uhsd_char pkgtype[64]; //套餐类型
    uhsd_u64 starttime;    //服务开始时间
    uhsd_u64 endtime;      //服务结束时间
} uhsd_video_cs_service_info;

typedef struct
{
    /* 基本功能初始化参数（必填） */
    uhsd_char devid[MAX_DEVID_SIZE];              //当前使用音视频功能的设备ID，通过设备注册取得
    uhsd_char path[MAX_PATH_SIZE];                //相关证书路径,SDK会往里面存放初始化证书等文件,所以需保证该路径可写
    uhsd_char netcard_name[MAX_NETCAR_NAME_SIZE]; //设备网卡名称，例如eth0
    uhsd_u32 channel_count;                       //设备通道数,普通ipc摄像头值是1，双摄是2
    /* 本地录像和云存功能相关参数（仅设备支持本地录像或本地图片功能时才需要填写） */
    uhsd_video_codec_type_e codec_type; //本地录像视频编码类型(H264 or H265)，默认MEDIA_CODEC_VIDEO_UNSUPPORTED，表示不启用本地录像功能
    uhsd_audio_sample_rate_e sample_rate; //本地录像的音频采样率
    uhsd_char store_path[MAX_PATH_SIZE];  //本地录像存储路径,SDK会往里面存放音视频文件,所以需保证该路径可写
    uhsd_u64 min_free_space_del; //触发删除本地录像的可用空间大小，单位M，eg：设置300m表示磁盘可用空间小于300m时开始删除录像
    uhsd_u64 max_size_store_pic;                 //本地图片可使用的最大存储空间(单位为M),不使用此功能赋值为0
    uhsd_char cloud_storage_path[MAX_PATH_SIZE]; //云存缓存视频存储路径,需保证该路径可写
    uhsd_u64 max_size_cloud_store;               //本地云存可使用的最大存储空间(不能小于512k,单位为字节)
    uhsd_u32 enent_clound_video_time;            //事件云存视频的持续时间，建议gop的2或3倍值，单位为s

    /* ugw_iot_video_send_video与ugw_iot_video_send_audio通用音视频发送接口能力设置 */
    uhsd_video_device_ability_e
        enable_ability; //实时监控(1)、本地录像(2)、云存(4)、实时监控+本地录像(3)、实时监控+云存(5)、本地录像+云存(6)、实时监控+本地录像+云存(7)
    uhsd_video_definition_e
        record_definition; //本地录像保存的视频规格，即通用音视频发送接口发送的视频中，哪种规格（HD|SD|FL）的音视频进行本地录像；如果是多摄像头(如双摄门铃,该参数只针对主摄)
    uhsd_video_definition_e
        cloud_definition; //云存保存的视频规格，即通用音视频发送接口发送的视频中，哪种规格（HD|SD|FL）的音视频上传至云存；如果是多摄像头(如双摄门铃,该参数只针对主摄)

    uhsd_notify_status_cb notify_status; //音视频设备认证通知回调函数

    // 多清晰度设备实现实时监控回调；
    uhsd_get_av_enc_info_cb get_multiple_av_enc_info;     //获取音视频编码信息
    uhsd_request_key_frame_cb request_multiple_key_frame; //向设备端请求关键帧
    uhsd_control_live_stream_cb
        control_multiple_live_stream; //开始或停止实时监控，如果是多摄像头(如双摄门铃,由于辅摄不需要切换清晰度,故该参数只针对主摄,channel_no从1开始)
    uhsd_recv_stream_cb recv_multiple_stream;                       //接收手机发出的音频数据
    uhsd_start_rcv_audio_stream_cb start_rcv_multiple_audio_stream; //开始接收音频对讲通知
    uhsd_stop_rcv_audio_stream_cb stop_rcv_multiple_audio_stream;   //停止接收音频对讲通知
    uhsd_live_stream_play_num_cb live_paly_num_notify;              // 实时监控观看人数通知
    uhsd_notify_cs_service_cb notify_cs_service;                    //云存服务状态变化通知
} uhsd_video_params_t;

/* ======================= 初始化与反初始化接口 ======================= */

/**
 * @brief 初始化IOT设备运行环境
 * @param ugw_iotvideo_params_t 初始化参数，使用前必须memset为0
 */
UHSD_API uhsd_s32 uhsd_video_init(uhsd_video_params_t param);

/**
 * @brief 释放运行资源
 */
UHSD_API uhsd_s32 uhsd_video_destory(uhsd_void);

/* ======================= 通用音视频发送接口 ======================= */
// 使用说明：
// uhsd_video_send_video与uhsd_video_send_audio为通用音视频发送接口；
// 通用音视频发送接口发送音视频的能力，通过接口uhsd_video_enable_ability、uhsd_video_disable_ability进行设置；
// 这两个接口支持的功能：
//     实时监控功能，支持发送不同清晰度（HD|SD|FL）规格的实时监控音视频；
//     本地录像功能，支持将init参数record_definition设置的清晰度音视频进行本地录像；
//     云存功能，支持将init参数cloud_definition设置的清晰度音视频上传至云存；

/**
 * @brief 发送视频帧；
 * @param frame 帧内容
 */
UHSD_API uhsd_s32 uhsd_video_send_video(uhsd_frame_t *frame);

/**
 * @brief 发送音频帧；
 * @param frame 帧内容
 */
UHSD_API uhsd_s32 uhsd_video_send_audio(uhsd_frame_t *frame);

/**
 * @brief 使能能力集,实时监控(1)、本地录像(2)、云存(4)、
 * 		  实时监控+本地录像(3)、实时监控+云存(5)、本地录像+云存(6)、实时监控+本地录像+云存(7)
 *        注意：开启云存功能的时候，要调用uhsd_video_get_cs_service函数查询是否存在云存套餐
 */
UHSD_API uhsd_s32 uhsd_video_enable_ability(uhsd_s32 ability);

/**
 * @brief 去使能能力集,实时监控(1)、本地录像(2)、云存(4)、
 * 		  实时监控+本地录像(3)、实时监控+云存(5)、本地录像+云存(6)、实时监控+本地录像+云存(7)
 */
UHSD_API uhsd_s32 uhsd_video_disable_ability(uhsd_s32 ability);

/**
 * @brief 设置当前本地录像片段的标签
 * @param recordType 自定义的标签字符串，长度不能超过16
 */
UHSD_API uhsd_s32 uhsd_video_setRecordType(uhsd_char *recordType);

/**
 * @brief 通知云存事件发生
 * @param type 事件类型
 * @param startTimeMs 事件开始时间，单位ms
 */
UHSD_API uhsd_s32 uhsd_video_notifyCsEvent(uhsd_cs_evevt_type_e type, uhsd_u64 startTimeMs);

/**
 * @brief   云存服务结果查询回调
 *
 * @param   err_num  结果码
 * @param   result  查询结果
 * @param   cb_param  自定义参数
 *
 */
typedef uhsd_s32 (*uhsd_dev_get_cs_service_result_cb)(uhsd_s32 err_num, uhsd_video_cs_service_info *result, uhsd_void *param);

/**
 * @brief   获取云存服务状态
 *
 * @param   devid  设备ID
 * @param   cb  查询结果回调
 * @param   cb_param  自定义参数
 *
 */
UHSD_API uhsd_s32 uhsd_video_get_cs_service(const uhsd_char *devid, uhsd_dev_get_cs_service_result_cb cb, uhsd_void *cb_param);

/**
 * @brief   自定义语音通知回调
 *
 * @param   url  自定义语料播放url
 * @param   cb_param  回传参数
 *
 */
typedef uhsd_void (*uhsd_video_user_audio_notify_cb)(const uhsd_char *url, uhsd_void *cb_param);

/**
 * @brief   设置自定义语音通知回调
 *
 * @param   devid  设备ID
 * @param   cb  通知回调
 * @param   cb_param  自定义参数
 *
 */
UHSD_API uhsd_s32 uhsd_video_set_user_audio_notify_cb(const uhsd_char *devid, uhsd_video_user_audio_notify_cb cb, uhsd_void *cb_param);

/**
 * @brief   黑名单查询结果回调
 *
 * @param   err_num  错误码
 * @param   result   0-设备不在黑名单中；1-设备在黑名单中
 * @param   cb_param 自定义参数
 *
 */
typedef uhsd_s32 (*uhsd_video_blacklist_query_cb)(uhsd_s32 err_num, uhsd_s32 result, uhsd_void *param);

/**
 * @brief   黑名单查询功能
 *
 * @param   devid  设备ID
 * @param   cb  通知回调
 * @param   cb_param  自定义参数
 *
 */
UHSD_API uhsd_s32 uhsd_video_blacklist_query(const uhsd_char *devid, uhsd_video_blacklist_query_cb cb, uhsd_void *cb_param);
/**
 * @brief   黑名单上报结果回调
 *
 * @param   err_num  错误码
 * @param   cs_result  云端当前黑名单状态 0-设备不在黑名单中；1-设备在黑名单中
 * @param   cb_param 自定义参数
 *
 */
typedef uhsd_s32 (*uhsd_video_blacklist_report_cb)(uhsd_s32 err_num, uhsd_s32 cs_result, uhsd_void *param);
/**
 * @brief   黑名单处理结果上报
 *
 * @param   devid  设备ID
 * @param   flag  // 0-设备功能启用；1-设备拉黑
 * @param   cb_param  自定义参数
 *
 */
UHSD_API uhsd_s32 uhsd_video_blacklist_report(const uhsd_char *devid, uhsd_s32 flag, uhsd_video_blacklist_report_cb cb, uhsd_void *cb_param);

/**
 * @brief   上传日志通知回调
 * @param   cb_param 自定义参数
 *
 */
typedef uhsd_s32 (*uhsd_video_upload_log_notify_cb)(uhsd_void *param);

/**
 * @brief   设置上传日志通知回调
 * @param   cb  通知回调
 * @param   cb_param  自定义参数
 *
 */
UHSD_API uhsd_s32 uhsd_video_set_upload_log_notify_cb(uhsd_video_upload_log_notify_cb cb, uhsd_void *cb_param);

/**
 * @brief   解除程序占用sd卡，此函数会关闭录像功能，
 * 			如需录像功能，插入sd卡后需要重新使能录像功能
 * @param
 *
 */
UHSD_API uhsd_s32 uhsd_video_remove_sd_occupation(uhsd_void);
#ifdef __cplusplus
}
#endif

#endif /* __UHOS_VIDEO_INIT_H__ */

/**@} grp_uhsddev end*/
