/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_audio.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2013/10/21
 *
 * Description:
 *          panjun 2015.08.26 Audio API.
 **************************************************************************/

#ifndef __PLATFORM_AUDIO_H__
#define __PLATFORM_AUDIO_H__

#ifdef LUA_AUDIO_LIB

#include "type.h"

typedef enum PlatformAudioFormatTag
{
	/*+\BUG\wangyuan\2020.12.31\BUG_4041:3024LUA�汾¼��demo¼������û������*/
	/*lua¼���ļ����Ͳ��� n: 1:pcm 2:wav 3:amrnb*/
	PLATFORM_AUD_MP3, 
	PLATFORM_AUD_PCM,
	PLATFORM_AUD_WAV,
	PLATFORM_AUD_AMRNB,
	/*-\BUG\wangyuan\2020.12.31\BUG_4041:3024LUA�汾¼��demo¼������û������*/
	/*+\NEW\zhuwangbin\2020.05.15\����speex��ʽ��¼���Ͳ���*/
    PLATFORM_AUD_SPEEX,
	/*-\NEW\zhuwangbin\2020.05.15\����speex��ʽ��¼���Ͳ���*/
	PLATFORM_AUD_AMRWB,
    //PLATFORM_AUD_MIDI,
    NumOfPlatformAudFormats
}PlatformAudioFormat;

/*+\NEW\czm\2020.11.13\bug:3271�ϲ�չ�����㷨��ͨ�ð汾*/
typedef enum PlatformAudioPlayTypeTag
{
    /**
     * Placeholder for not in playing.
     */
    PLATFORM_AUD_PLAY_TYPE_NONE = 0,
    /**
     * ���ű�����Ƶ·����
     */
    PLATFORM_AUD_PLAY_TYPE_LOCAL,
    /**
     * ������ͨ���в��ŵ��Զˡ�
     */
    PLATFORM_AUD_PLAY_TYPE_VOICE,
    /**
     * ��pocģʽ�²��ű�����Ƶ·����poc���벥�š�
     */
    PLATFORM_AUD_PLAY_TYPE_POC,
}PlatformAudioPlayType;


typedef enum PlatformAudioRecordTypeTag
{
    PLATFORM_AUD_RECORD_TYPE_NONE, ///< ����δ֪��ʽ��ռλ��
    PLATFORM_AUD_RECORD_TYPE_MIC,     ///����˷�¼�ơ� 
    PLATFORM_AUD_RECORD_TYPE_VOICE,  ///¼������ͨ����¼�Ƶ�����������ͨ����
    PLATFORM_AUD_RECORD_TYPE_VOICE_DUAL,     //¼������ͨ����¼�Ƶ����Ǵ��з����������·��������·�ŵ���
    PLATFORM_AUD_RECORD_TYPE_DEBUG_DUMP, //PCMת���������ڵ��ԡ�
    PLATFORM_AUD_RECORD_TYPE_POC,//��pocģʽ�´���˷�¼�ơ�
}PlatformAudioRecordType;

/*-\NEW\czm\2020.11.13\bug:3271�ϲ�չ�����㷨��ͨ�ð汾*/

/*+\NEW\zhuth\2014.7.25\����������Ƶͨ����������ͬ���ӿ�*/
typedef enum PlatformAudioChannelTag
{
	/*+\new\wj\2020.4.22\֧����Ƶͨ���л��ӿ�*/
    PLATFORM_AUD_CHANNEL_HANDSET,
    PLATFORM_AUD_CHANNEL_EARPIECE,
    PLATFORM_AUD_CHANNEL_LOUDSPEAKER,
    NumOfPlatformAudChannels
    #if 0
    PLATFORM_AUD_CHANNEL_BLUETOOTH,
    PLATFORM_AUD_CHANNEL_FM,
    PLATFORM_AUD_CHANNEL_FM_LP,
    PLATFORM_AUD_CHANNEL_TV,
    PLATFORM_AUD_CHANNEL_AUX_HANDSET,
    PLATFORM_AUD_CHANNEL_AUX_LOUDSPEAKER,
    PLATFORM_AUD_CHANNEL_AUX_EARPIECE,
    PLATFORM_AUD_CHANNEL_DUMMY_HANDSET,    
    PLATFORM_AUD_CHANNEL_DUMMY_AUX_HANDSET,
    PLATFORM_AUD_CHANNEL_DUMMY_LOUDSPEAKER,
    PLATFORM_AUD_CHANNEL_DUMMY_AUX_LOUDSPEAKER,
	#endif
	/*-\new\wj\2020.4.22\֧����Ƶͨ���л��ӿ�*/
}PlatformAudioChannel;

typedef enum PlatformAudioVolTag
{
    PLATFORM_AUD_VOL0,
    PLATFORM_AUD_VOL1,
    PLATFORM_AUD_VOL2,
    PLATFORM_AUD_VOL3,
    PLATFORM_AUD_VOL4,
    PLATFORM_AUD_VOL5,
    PLATFORM_AUD_VOL6,
    PLATFORM_AUD_VOL7,
    NumOfPlatformAudVols
}PlatformAudioVol;

typedef enum PlatformMicVolTag
{
    PLATFORM_MIC_VOL0,
    PLATFORM_MIC_VOL1,
    PLATFORM_MIC_VOL2,
    PLATFORM_MIC_VOL3,
    PLATFORM_MIC_VOL4,
    PLATFORM_MIC_VOL5,
    PLATFORM_MIC_VOL6,
    PLATFORM_MIC_VOL7,
    PLATFORM_MIC_VOL8,
    PLATFORM_MIC_VOL9,
    PLATFORM_MIC_VOL10,
    PLATFORM_MIC_VOL11,
    PLATFORM_MIC_VOL12,
    PLATFORM_MIC_VOL13,
    PLATFORM_MIC_VOL14,
    PLATFORM_MIC_VOL15,
    NumOfPlatformMicVols
}PlatformMicVol;

typedef enum PlatformAudioLoopbackTag
{
    PLATFORM_AUD_LOOPBACK_HANDSET,
    PLATFORM_AUD_LOOPBACK_EARPIECE,
    PLATFORM_AUD_LOOPBACK_LOUDSPEAKER,
    PLATFORM_AUD_LOOPBACK_AUX_HANDSET,
    PLATFORM_AUD_LOOPBACK_AUX_LOUDSPEAKER,
    NumOfPlatformAudLoopbacks
}PlatformAudioLoopback;
/*-\NEW\zhuth\2014.7.25\����������Ƶͨ����������ͬ���ӿ�*/

/*+\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/
typedef enum
{
    PLATFORM_SPKPA_TYPE_CLASSAB,
    PLATFORM_INPUT_TYPE_CLASSD,
    PLATFORM_INPUT_TYPE_CLASSK,
    PLATFORM_SPKPA_INPUT_TYPE_QTY = 0xFF000000
} PlatformSpkPaType;
/*-\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/
/*+\BUG\wangyuan\2020.11.27\BUG_3634����Luat�汾�Ͽ���������mic����ͨ�����Ľӿ�*/
typedef enum PlatformMicChannelTag
{
    PLATFORM_AUDEV_INPUT_MAINMIC = 0, ///< main mic
    PLATFORM_AUDEV_INPUT_AUXMIC = 1,  ///< auxilary mic
    PLATFORM_AUDEV_INPUT_DUALMIC = 2, ///< dual mic
    PLATFORM_AUDEV_INPUT_HPMIC_L = 3, ///< headphone mic left
    PLATFORM_AUDEV_INPUT_HPMIC_R = 4, ///< headphone mic right
    NumOfPlatformMicChannels
}PlatformMicChannel;
/*-\BUG\wangyuan\2020.11.27\BUG_3634����Luat�汾�Ͽ���������mic����ͨ�����Ľӿ�*/

typedef struct AudioPlayParamTag
{
    BOOL isBuffer;
    union u_tag
    {
        struct
        {
            const u8 *data;
            u32 len;
            PlatformAudioFormat format;
            BOOL loop;
        }buffer;
        const char *filename;
    }u;
}AudioPlayParam;

int platform_audio_play(AudioPlayParam *param);

int platform_audio_stop(void);

/*+\NEW\zhuth\2014.7.25\����������Ƶͨ����������ͬ���ӿ�*/
/*+\BUG\wangyuan\2020.11.27\BUG_3634����Luat�汾�Ͽ���������mic����ͨ�����Ľӿ�*/
int platform_audio_set_channel(PlatformAudioChannel outputchannel,PlatformMicChannel inputchannel);
/*-\BUG\wangyuan\2020.11.27\BUG_3634����Luat�汾�Ͽ���������mic����ͨ�����Ľӿ�*/

int platform_audiod_set_vol(int vol);

int platform_audio_set_mic_vol(PlatformMicVol vol);

int platform_audio_set_loopback(BOOL flag, PlatformAudioLoopback typ, BOOL setvol, u32 vol);
/*-\NEW\zhuth\2014.7.25\����������Ƶͨ����������ͬ���ӿ�*/
/*+\new\wj\2020.4.26\ʵ��¼���ӿ�*/
/*+\NEW\czm\2020.11.13\bug:3271�ϲ�չ�����㷨��ͨ�ð汾*/
int platform_audio_record(char* file_name, int time_sec, int quality, PlatformAudioRecordType type, PlatformAudioFormat format);
/*-\NEW\czm\2020.11.13\bug:3271�ϲ�չ�����㷨��ͨ�ð汾*/
/*-\new\wj\2020.4.26\ʵ��¼���ӿ�*/
int platform_audio_stop_record(void);

/*+\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/
int platform_setpa(PlatformSpkPaType type);
int platform_getpa(void);
/*-\new\zhuwangbin\2020.6.2\�����Ƶ�����������ýӿ�*/

/*+\bug2767\zhuwangbin\2020.8.5\����ⲿpa���ýӿ�*/
int platform_setexpa(BOOL enable, UINT16 gpio, UINT16 count, 
					UINT16 us,  E_AMOPENAT_AUDIO_CHANNEL outDev);
/*-\bug2767\zhuwangbin\2020.8.5\����ⲿpa���ýӿ�*/

/*+\NEW\zhuwangbin\2020.8.11\��Ӷ����������*/
int platform_headPlug(int type);
/*-\NEW\zhuwangbin\2020.8.11\��Ӷ����������*/

#endif

#endif //__PLATFORM_AUDIO_H__

