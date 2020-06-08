#ifndef __IOT_AUDIO_H__
#define __IOT_AUDIO_H__

#include "iot_os.h"
#include "am_openat.h"



/**
 * @defgroup iot_sdk_audio ��Ƶ�ӿ�
 * @{
 */
/**@example audio/demo_audio.c
* audio�ӿ�ʾ��
*/

/**������
*@note  ��ͨ����ʼʱ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_open_tch(                                        
                        VOID
                );

/**�ر�����
*@note  ͨ������ʱ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_close_tch(                                      
                        VOID
                 );

/**����TONE��
*@param  toneType:      TONE������
*@param  duration:      ����ʱ��
*@param  volume:        ��������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_play_tone(                                        
                        E_AMOPENAT_TONE_TYPE toneType,     
                        UINT16 duration,                   
                        E_AMOPENAT_SPEAKER_GAIN volume     
                 );

/**ֹͣ����TONE��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_tone(                                        
                        VOID
                 );


/**����DTMF��
*@param  dtmfType:      DTMF����
*@param  duration:      ����ʱ��
*@param  volume:        ��������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_play_dtmf(                                        
                        E_AMOPENAT_DTMF_TYPE dtmfType,     
                        UINT16 duration,                   
                        E_AMOPENAT_SPEAKER_GAIN volume     
                 );

/**ֹͣ����DTMF��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_dtmf(                            
                        VOID
                 );

/**������Ƶ
*@param  playParam:     ���Ų���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_play_music(T_AMOPENAT_PLAY_PARAM*  playParam);

/**ֹͣ��Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_music(                                        
                        VOID
                  );

/**��ͣ��Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_pause_music(                                     
                        VOID
                   );

/**�ָ���Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_resume_music(                                       
                        VOID
                    );

/**��������������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_mute_speaker(                                     
                        VOID
                    );

/**�������������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_unmute_speaker(                                   
                        VOID
                      );

/**����������������ֵ
*@param     vol:   ��������������ֵ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_speaker_vol(                                   
                        UINT32 vol 
                        );

/**��ȡ������������
*@return	UINT32: 	 ����������������ֵ
**/
UINT32 iot_audio_get_speaker_vol(                
                        VOID
                                           );

/**������Ƶͨ��
*@param     channel:    ͨ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_channel(                                       
                        E_AMOPENAT_AUDIO_CHANNEL channel    
                   );

/**��ȡ��ǰͨ��
*@return	E_AMOPENAT_AUDIO_CHANNEL: 	  ����ͨ��ֵ
**/
E_AMOPENAT_AUDIO_CHANNEL iot_audio_get_current_channel(            
                        VOID
                                               );

/**��ʼ¼��
*@param     E_AMOPENAT_RECORD_PARAM:   ¼������
*@param     AUD_RECORD_CALLBACK_T:     ��ȡ¼�����ݻص�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_rec_start(
                    			E_AMOPENAT_RECORD_PARAM* param,
								AUD_RECORD_CALLBACK_T cb);

/**ֹͣ¼��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_rec_stop();



/** @}*/


#endif
