#include "iot_tts.h"


/**��ʼ��tts����
*@param		cb:		TTS���Ž���ص�����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_tts_init(
                    TTS_PLAY_CB cb         
              )
{
    return OPENAT_tts_init(cb);
}

/**tts�����ı�
*@param		text:		�������ı�
*@param		len:		�ı����ȣ��ֽڣ�
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_play(                                   
                    char *text,u32 len                    
              )
{
    return OPENAT_tts_play(text, len);
}

/**ttsֹͣ����
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_stop(      )
{
    return OPENAT_tts_stop();
}

/**����tts���ò���
*@param		flag:		������־
*@param		value:		����ֵ
*@return	TRUE: 	    �ɹ�
			FALSE:      ʧ��
**/
BOOL iot_tts_set_param(
		OPENAT_TTS_PARAM_FLAG flag,u32 value
		)
{
	return OPENAT_tts_set_param(flag,value);
}


