#ifndef __IOT_TTS_H__
#define __IOT_TTS_H__

#include "iot_os.h"

/**
 * @defgroup iot_sdk_audio ��Ƶ�ӿ�
 * @{
 */

	/**@example tts/demo_tts.c
	* tts�ӿ�ʾ��
	*/ 

/**��ʼ��tts����
*@param		cb:		TTS���Ž���ص�����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_tts_init(
                    TTS_PLAY_CB cb         
              );

/**tts�����ı�
*@param		text:		�������ı�
*@param		len:		�ı����ȣ��ֽڣ�
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_play(                                   
                    char *text,u32 len                    
              );


/**ttsֹͣ����
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_stop(      );


/**����tts���ò���
*@param		flag:		������־
*@param		value:		����ֵ
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_set_param(
		OPENAT_TTS_PARAM_FLAG flag,u32 value
		);


/** @}*/
/** @}*/

#endif
