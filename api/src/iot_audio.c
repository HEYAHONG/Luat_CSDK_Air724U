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

/**��ͨ��������ֵ
*@param     vol:   ����ͨ������ֵ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_set_sph_vol(                                   
                        UINT32 vol 
                        )
{
    return IVTBL(set_sph_vol)(vol);
}

/**��ȡͨ��������ֵ
*@return	UINT32: 	 ����ͨ��������ֵ
**/
UINT32 iot_audio_get_sph_vol(                
                        VOID
                        )
{
    return IVTBL(get_sph_vol)();
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
    return ((IVTBL(audio_record)(param, cb) == 0) ? 1 : 0);
}

/**ֹͣ¼��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_audio_rec_stop()
{
    return ((IVTBL(audio_stop_record)() == 0) ? 1 : 0);
}

/**������
*@param  playformat:    ����������
*@param  cb:    		�������ص�����
*@param  data:    		������
*@param  len:  		  	����������
*@return	>0: 	    ���ų���
*           -1:      	����ʧ��
**/
int iot_audio_streamplay(E_AMOPENAT_AUD_FORMAT playformat,AUD_PLAY_CALLBACK_T cb,char* data,int len)
{
	return IVTBL(streamplay)(playformat,cb,data,len);
}

