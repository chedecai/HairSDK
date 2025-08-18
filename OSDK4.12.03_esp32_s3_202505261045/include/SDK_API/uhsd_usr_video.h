#ifndef __UHSD_USR_VIDEO_H__
#define __UHSD_USR_VIDEO_H__

#include "uhsd_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UH_USR_VIDEO_DEFINITION_UNKONOW = -1,    /*未知清晰度*/
    UH_USR_VIDEO_DEFINITION_FL = 0,          /*fluency，流畅*/
    UH_USR_VIDEO_DEFINITION_SD = 1,          /*standard definition，标清*/
    UH_USR_VIDEO_DEFINITION_HD = 2,          /*high definition，高清（单清晰度设备默认）*/
    UH_USR_VIDEO__DEFINITION_ONLY_AUDIO = 3, /*onlyaudio，纯音频*/
} uhsd_usr_video_definiton_e;

typedef enum
{
    UH_AUDIO_SAMPLE_RATE_UNSUPPORTED = 0, /* Unsupported*/
    UH_AUDIO_SAMPLE_RATE_8000 = 8000,     /* 8K samplerate*/
    UH_AUDIO_SAMPLE_RATE_12000 = 12000,   /* 12K samplerate*/
    UH_AUDIO_SAMPLE_RATE_11025 = 11025,   /* 11.025K samplerate*/
    UH_AUDIO_SAMPLE_RATE_16000 = 16000,   /* 16K samplerate*/
    UH_AUDIO_SAMPLE_RATE_22050 = 22050,   /* 22.050K samplerate*/
    UH_AUDIO_SAMPLE_RATE_24000 = 24000,   /* 24K samplerate*/
    UH_AUDIO_SAMPLE_RATE_32000 = 32000,   /* 32K samplerate*/
    UH_AUDIO_SAMPLE_RATE_44100 = 44100,   /* 44.1K samplerate*/
    UH_AUDIO_SAMPLE_RATE_48000 = 48000,   /* 48K samplerate*/
    UH_AUDIO_SAMPLE_RATE_64000 = 64000,   /* 64K samplerate*/
    UH_AUDIO_SAMPLE_RATE_96000 = 96000,   /* 96K samplerate*/

    UH_AUDIO_SAMPLE_RATE_BUTT,
} uhsd_audio_sample_rate_e;

typedef enum
{
    UH_AV_STREAM_TYPR_NONE = 0,
    UH_AV_STREAM_TYPR_H264 = 1, // h264 video
    UH_AV_STREAM_TYPR_H265,     // h265 video
    UH_AV_STREAM_TYPR_AAC,
    UH_AV_STREAM_TYPR_OPUS, // 360 dishi-no_support
    UH_AV_STREAM_TYPR_PCM,
    UH_AV_STREAM_TYPR_G711_U,
    UH_AV_STREAM_TYPR_G711_A,
    UH_AV_STREAM_TYPR_G726,  // 360 dishi-no_support
    UH_AV_STREAM_TYPR_MJPEG, // mjpeg video
} uhsd_usr_video_stream_type_e;

typedef struct
{
    uhsd_usr_video_stream_type_e stream_type; //音视频流类型
    uhsd_u8 *frame_data;                      //音视频数据
    uhsd_s32 frame_data_len;                  //数据长度
    uhsd_u64 pts;                             // 0时区秒数，单位ms
    uhsd_u64 dts;                             // 0时区秒数，单位ms，同pts
    uhsd_s32 is_key_frame;                    //是否是关键帧
    uhsd_s32 channel_id;                      //流通道标识。默认从1开始
} uhsd_usr_video_frame_info_t;

typedef struct
{
    // audio
    uhsd_usr_video_stream_type_e stream_type;
    uhsd_audio_sample_rate_e sample_rate;
    uhsd_s32 bit_width;      //位宽 8bit, 16bit, 24bit
    uhsd_s32 audio_channels; // 1 单通道 2 双通道
} uhsd_usr_audio_frame_info_t;

typedef enum
{
    UH_PLAY_TYPE_MONITOR = 0, // 实时监控
    UH_PLAY_TYPE_RECORD,      // 本地录像
    UH_PLAY_TYPE_CLOUD,       // 云存
} uhsd_usr_video_paly_type_e;

typedef enum
{
    PLAY_ERR_GET_AURH_INFO_ERR = -100,       // 获取设备认证信息错误
    PLAY_ERR_GET_SECRET_KEY_INFO_ERR,        // 获取设备加密key错误
    PLAY_ERR_360_LSNVD_INIT_ERR,             // 360_LSNVD_INIT_ERR
    PLAY_ERR_360_SET_LLTYPE_ERR,             //设置设备sn使用的长连接类型错误
    PLAY_ERR_360_LSNVD_PRECONNECT_ERR,       // p2p预连接错误
    PLAY_ERR_360_LSNVD_HANDSHAKE_ERR,        // p2p连接握手错误
    PLAY_ERR_360_LSNVD_UPDATE_SECRETKEY_ERR, //设置加密key错误
    PLAY_ERR_360_LSNVD_CREATE_SESSION_ERR,   //创建播放实例错误
    PLAY_ERR_360_DATA_CALLBACK_SWITCH_ERR,   //开启码流数据回调错误
    PLAY_ERR_360_TRANSFER_TOKEN_ERR,         //设置验证设备token
    PLAY_ERR_360_CONNECT_ERR,                //连接错误
} uhsd_usr_video_paly_opt_e;
typedef struct
{
    uhsd_char deviceId[32];
    uhsd_usr_video_paly_type_e paly_type; //操作视频类型
    uhsd_s32 status;                      // 0:停止、暂停 1:播放/恢复播放
    uhsd_s32 channel_id;                  //视频流通道号，默认从1开始
} uhsd_usr_video_paly_opt_t;

typedef struct
{
    uhsd_u32 width;
    uhsd_u32 height;
    uhsd_u32 format;

    uhsd_u32 pitchs[4];
    uhsd_u32 offsets[4];
    uhsd_u64 modifiers[4];
    uhsd_s32 prime_fds[4];

    uhsd_u32 nb_planes;
} uhsd_usr_video_raw_avFrame;

/**
 * @brief 用户侧video组件视频流解码数据回调接口
 * @param video_info 视频参数
 */
typedef uhsd_s32 (*uhsd_usr_video_decode_stream_cb)(uhsd_usr_video_raw_avFrame *raw_video_data);

/**
 * @brief 用户侧video组件视频流(未解码)回调接口
 * @param video_info 视频参数
 */
typedef uhsd_s32 (*uhsd_usr_video_stream_cb)(uhsd_usr_video_frame_info_t *video_info);

/**
 * @brief 用户侧video组件音频流回调接口
 * @param video_info 视频参数
 */
typedef uhsd_s32 (*uhsd_usr_audio_stream_cb)(uhsd_usr_video_frame_info_t *audio_info, uhsd_usr_audio_frame_info_t *audio_code_info);

/**
 * @brief 用户侧video组件开始/停止播放操作回调接口
 * @param paly_type 要播放视频的类型（实时监控/录像/云存）
 * @param status  操作类型（开始/停止)
 * @param channel_id  视频流通道号，默认从1开始
 */
typedef uhsd_s32 (*uhsd_usr_video_play_operation_cb)(uhsd_usr_video_paly_opt_t *play_info, uhsd_usr_video_paly_opt_e err);

typedef struct
{
    uhsd_char appId[65];
    uhsd_char appKey[65];
    uhsd_char appVersion[32];
    uhsd_char clientId[65];
    uhsd_char access_token[65];
    uhsd_char ca_path[128]; // CA证书路径
    uhsd_char cache_path[128];

    uhsd_usr_video_stream_cb video_stream_cb;           //未解码数据（h264/h265）视频数据回调
    uhsd_usr_video_decode_stream_cb decode_stream_cb;   //解码后原生数据回调
    uhsd_usr_audio_stream_cb audio_stream_cb;           // pcm音频数据回调
    uhsd_usr_video_play_operation_cb play_operation_cb; //播放停止操作结果回调
} uhsd_usr_video_params_t;

/**
 * @brief 用户侧video组件初始化接口
 * @param uhsd_usr_video_params_t 初始化参数，使用前必须memset为0
 */
UHSD_API uhsd_s32 uhsd_usr_video_init(uhsd_usr_video_params_t param);

/**
 * @brief 用户侧video组件更新token接口
 * @param token 长度小于65字节
 */
UHSD_API uhsd_s32 uhsd_usr_video_update_token(uhsd_char *token);
/**
 * @brief 用户侧video组件开始/停止播放操作接口
 * @param play_info:paly_type 要播放视频的类型（实时监控/录像/云存）
 * @param play_info:status  操作类型（开始/停止)
 * @param play_info:channel_id  视频流通道号，默认从1开始
 */
UHSD_API uhsd_s32 uhsd_usr_video_play_operation(uhsd_usr_video_paly_opt_t *play_info);
#if 0
/**
 * @brief 用户侧video组件暂停/播放播放操作接口
 * @param play_info:paly_type 要播放视频的类型（实时监控/录像/云存）
 * @param play_info:status  操作类型（0暂停/1播放）
 * @param play_info:channel_id  视频流通道号，默认从1开始
 */
UHSD_API uhsd_s32 uhsd_usr_video_pause_operation(uhsd_usr_video_paly_opt_t *play_info);

/**
 * @brief 用户侧video组件切换清晰度操作接口
 * @param definition 要切换的视频清晰度类型（FL/SD/HD）
 * @param channel_id  视频流通道号，默认从1开始
 */
UHSD_API uhsd_s32 uhsd_usr_video_change_definition(uhsd_char *deviceId, uhsd_usr_video_definiton_e definition, uhsd_s32 channel_id);
#endif
/**
 * @brief 用户侧video组件开始对讲接口
 */
UHSD_API uhsd_s32 uhsd_usr_video_start_rtc(uhsd_usr_audio_frame_info_t *audio_config_info);

/**
 * @brief 用户侧video组件停止对讲接口
 */
UHSD_API uhsd_s32 uhsd_usr_video_stop_rtc(uhsd_void);

/**
 * @brief 用户侧video组件对讲发送音频数据接口
 * @param audio_info 音频数据
 * @param audio_code_info 音频参数信息
 */
UHSD_API uhsd_s32 uhsd_usr_video_send_rtc_data(uhsd_usr_video_frame_info_t *audio_info, uhsd_usr_audio_frame_info_t *audio_code_info);

#ifdef __cplusplus
}
#endif
#endif