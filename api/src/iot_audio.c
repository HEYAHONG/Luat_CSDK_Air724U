#include "iot_audio.h"

/****************************** AUDIO ******************************/
/**������
*@note  ��ͨ����ʼʱ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_auido_open_tch(                                           
                        VOID
                )
{
    return IVTBL(open_tch)();
}

/**�ر�����
*@note  ͨ������ʱ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_close_tch(                                          
                        VOID
                 )
{
    return IVTBL(close_tch)();
}

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
                 )
{
    return IVTBL(play_tone)(toneType, duration, volume);
}

/**ֹͣ����TONE��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_tone(                                         
                        VOID
                 )
{
    return IVTBL(stop_tone)();
}


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
                 )
{
    return IVTBL(play_dtmf)(dtmfType, duration, volume);
}

/**ֹͣ����DTMF��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_dtmf(                                          
                        VOID
                 )
{
    return IVTBL(stop_dtmf)();
}

/**������Ƶ
*@param  playParam:     ���Ų���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_play_music(T_AMOPENAT_PLAY_PARAM*  playParam)
{
    return IVTBL(play_music)(playParam);
}

/**ֹͣ��Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_stop_music(                                       
                        VOID
                  )
{
    return IVTBL(stop_music)();
}

/**��ͣ��Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_pause_music(                                       
                        VOID
                   )
{
    return IVTBL(pause_music)();
}

/**�ָ���Ƶ����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_resume_music(                                     
                        VOID
                    )
{
    return IVTBL(resume_music)();
}

/**����MP3������Ч
*@param  setEQ:        ����MP3��Ч
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_eq(                                      
                        E_AMOPENAT_AUDIO_SET_EQ setEQ
                    )
{
    //return IVTBL(set_eq)(setEQ);
}

/**����MIC
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_open_mic(                                          
                        VOID
                )
{
    //return IVTBL(open_mic)();
}

/**�ر�MIC
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_close_mic(                                          
                        VOID
                 )
{
    //return IVTBL(close_mic)();
}

/**����MIC����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_mute_mic(                                          
                        VOID
                )
{
    //return IVTBL(close_mic)();
}

/**���MIC����
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_unmute_mic(                                        
                        VOID
                  )
{
    //return IVTBL(unmute_mic)();
}

/**����MIC������
*@note  micGainֵ���Ϊ20
*@param  micGain:       ����MIC������ֵ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_mic_gain(                                      
                        UINT16 micGain                     
                    )
{
    //return IVTBL(unmute_mic)();
}

/**��������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_open_speaker(                                      
                        VOID
                    )
{
    //return IVTBL(open_speaker)();
}

/**�ر�������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_close_speaker(                                    
                        VOID
                     )
{
    //return IVTBL(close_speaker)();
}

/**��������������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_mute_speaker(                                       
                        VOID
                    )
{
    return IVTBL(mute_speaker)();
}

/**�������������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_unmute_speaker(                                    
                        VOID
                      )
{
    return IVTBL(unmute_speaker)();
}

/**����������������ֵ
*@param     vol:   ��������������ֵ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_speaker_vol(                                   
                        UINT32 vol 
                        )
{
    return IVTBL(set_music_volume)(vol);
}

/**��ȡ������������ֵ
*@return	UINT32: 	 ����������������ֵ
**/
UINT32 iot_audio_get_speaker_vol(                
                        VOID
                                           )
{
    return IVTBL(get_music_volume)();
}


/**������Ƶͨ��
*@param     channel:    ͨ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_channel(                                        
                        E_AMOPENAT_AUDIO_CHANNEL channel   
                   )
{
    return IVTBL(set_channel)(channel);
}

/**���ù���ͬһ��MIC��Ƶͨ��
*@param     channel_1:    ͨ��1
*@param     channel_2:    ͨ��2
**/
VOID iot_audio_set_channel_with_same_mic(                          
                    E_AMOPENAT_AUDIO_CHANNEL channel_1,     
                    E_AMOPENAT_AUDIO_CHANNEL channel_2      
               )
{
    //return IVTBL(set_channel_with_same_mic)(channel_1, channel_2);
}

/**����MIC��Ƶͨ������
*@param     hfChanne:    �ֱ�ͨ��
*@param     erChanne:    ����ͨ��
*@param     ldChanne:    ����ͨ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_hw_channel(
                      E_AMOPENAT_AUDIO_CHANNEL hfChanne,   
                      E_AMOPENAT_AUDIO_CHANNEL erChanne,   
                      E_AMOPENAT_AUDIO_CHANNEL ldChanne    
                     )
{
    //return IVTBL(set_hw_channel)(hfChanne, erChanne, ldChanne);
}

/**��ȡ��ǰͨ��
*@return	E_AMOPENAT_AUDIO_CHANNEL: 	  ����ͨ��ֵ
**/
E_AMOPENAT_AUDIO_CHANNEL iot_audio_get_current_channel(            
                        VOID
                                               )

{
    return IVTBL(get_current_channel)();
}

/**��ʼ¼��
*@param     param:   ¼������
*@param     cb:     ��ȡ¼�����ݻص�
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_rec_start(
                    			E_AMOPENAT_RECORD_PARAM* param,
								AUD_RECORD_CALLBACK_T cb)
{
    return IVTBL(audio_record)(param, cb);
}

/**ֹͣ¼��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_rec_stop()
{
    return IVTBL(audio_stop_record)();
}
