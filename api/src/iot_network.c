#include "string.h"
#include <stdio.h>
#include "at_process.h"
#include "at_tok.h"
#include "iot_debug.h"
#include "iot_network.h"

static F_OPENAT_NETWORK_IND_CB g_s_gsmStatusCb = NULL;
static unsigned char g_s_nwMode = 0;
static E_OPENAT_NETWORK_STATE nw_old_state = OPENAT_NETWORK_DISCONNECT;
static unsigned char at_init_flag=0;

static void gsmStatusCb( int status);




static BOOL iot_network_get_SYSINFO(int* srv_status,int*sys_mode,int*sim_state)
{
    ATResponse *p_response = NULL;
    bool result = FALSE;  
    int err = at_send_command_singleline("AT^SYSINFO", "^SYSINFO:", &p_response);
    if (err < 0 || p_response->success == 0)
    {
        iot_debug_print("[iot_network] at_send_command_singleline error %d",__LINE__);
        goto end;
    }
    char* line = p_response->p_intermediates->line;  
    err = at_tok_start(&line);
    if (err < 0)
        goto end;
    err = at_tok_nextint(&line,srv_status);
    if (err < 0)
        goto end;
    int temp=0;
    err = at_tok_nextint(&line,&temp);
    if (err < 0)
        goto end;
    err = at_tok_nextint(&line,&temp);
    if (err < 0)
        goto end;
    err = at_tok_nextint(&line,sys_mode);
    if (err < 0)
        goto end;
    err = at_tok_nextint(&line,sim_state);
    if (err < 0)
        goto end;
    result = TRUE;
end:              
    if(p_response!=NULL)
    {
        at_response_free(p_response);
        p_response=NULL;
    }  
    return result;
}


static BOOL iot_network_get_CSQ(int* rssi)
{

    ATResponse *p_response = NULL;
    bool result = FALSE;  
    int err = at_send_command_singleline("AT+CSQ", "+CSQ:", &p_response);
    if (err < 0 || p_response->success == 0)
        goto end;
    char* line = p_response->p_intermediates->line;  
    err = at_tok_start(&line);
    if (err < 0)
        goto end;
    err = at_tok_nextint(&line,rssi);
    if (err < 0)
        goto end; 
    result = TRUE;  
end:              
    if(p_response!=NULL)
    {
        at_response_free(p_response);
        p_response=NULL;
    }  
    return result;
}

/**��ȡ����״̬
*@param     status:   ��������״̬
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��            
**/                                
BOOL iot_network_get_status(T_OPENAT_NETWORK_STATUS* status)
{
    if(at_init_flag==0)
    {
        at_init();
        at_init_flag=1;
    }


    int rssi=0;
    if(iot_network_get_CSQ(&rssi)==FALSE)
    {
        iot_debug_print("[iot_network] iot_network_get_CSQ error %d",__LINE__);
        return FALSE;
    }
    status->csq=rssi;
    //iot_debug_print("[iot_network] rssi:%d",rssi);   


    int srv_status=0,sys_mode=0,sim_state=0;
    if(iot_network_get_SYSINFO(&srv_status,&sys_mode,&sim_state)==FALSE)
    {
        iot_debug_print("[iot_network] iot_network_get_SYSINFO error %d",__LINE__);
        return FALSE;
    }
    if(sim_state!=1)
    {
        status->state=OPENAT_NETWORK_DISCONNECT;
        goto end;
    }
    if(srv_status!=2)
    {
        status->state=OPENAT_NETWORK_DISCONNECT;
        goto end;
    }
    if(sys_mode!=17)
    {
        status->state=OPENAT_NETWORK_READY;
        goto end;
    }

    status->state=OPENAT_NETWORK_LINKED;

end:
    status->simpresent=sim_state;
    //iot_debug_print("[iot_network] srv_status:%d,sys_mode:%d,sim_state:%d",srv_status, sys_mode, sim_state);
    return TRUE;  
}  

static void gsmStatusCb( int status)
{
  E_OPENAT_NETWORK_STATE nw_state;

  if(status >= 0x80 && status < 0x90)
  {
      //LTE 
      status = status - 0x80;
      if(status == 1 || status == 5)
      {
          nw_state = OPENAT_NETWORK_READY;

          g_s_nwMode = 4;
      }
      else
      {
          nw_state = OPENAT_NETWORK_DISCONNECT;

          g_s_nwMode = 0;
      }
  }
  else if(status >= 0x90)
  {
      status = status - 0x90;
      if(status == 1)
      {
          nw_state = OPENAT_NETWORK_LINKED;
      }
      else
      {
          nw_state = OPENAT_NETWORK_GOING_DOWN;
      }
  }
  else
  {
      if(status == 1 || status == 5)
      {
          nw_state = OPENAT_NETWORK_READY;
          g_s_nwMode = 2;
      }
      else
      {
          nw_state = OPENAT_NETWORK_DISCONNECT;
          g_s_nwMode = 0;  
      }
  }

  iot_debug_print("[iot_network] %s old %d new %d", __FUNCTION__, nw_old_state, nw_state);

  if(g_s_gsmStatusCb && nw_old_state != nw_state)
  {
    g_s_gsmStatusCb(nw_state);
    nw_old_state = nw_state;
  }
  
}
                           
/**��������״̬�ص�����
*@param     indCb:   �ص�����
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
**/                            
BOOL iot_network_set_cb(F_OPENAT_NETWORK_IND_CB indCb)
{
    if(at_init_flag==0)
    {    
        at_init();
        at_init_flag=1;
    }
    at_regNetStatusCb(gsmStatusCb);
    g_s_gsmStatusCb = indCb;
}        


static bool gsmGprsPDPActive(const char* apn, const char* user, const char* pwd )
{
  int err;
  ATResponse *p_response = NULL;
  char cmd[64] = {0};

  memset(cmd, 0, 64);

  sprintf(cmd, "AT+CGDCONT=5,IP,%s\r\n",apn);
            
  err = at_send_command(cmd, &p_response);
  iot_debug_print("[iot_network] CGDCONT error %d, success %d", err,(p_response?p_response->success:-1));
  if (err < 0 || p_response->success == 0){
    goto error;
  }

  at_response_free(p_response);
  
  memset(cmd, 0, 64);
  sprintf(cmd, "AT+CGACT=1,5");
            
  err = at_send_command(cmd, &p_response);
  iot_debug_print("[iot_network] CGACT error %d, success %d", err,(p_response?p_response->success:-1));
  if (err < 0 || p_response->success == 0){
    goto error;
  }

  at_response_free(p_response);
  return TRUE;

error:
  at_response_free(p_response);
  return FALSE;
}

/**�����������ӣ�ʵ��Ϊpdp��������
*@param     connectParam:  �������Ӳ�������Ҫ����APN��username��passwrd��Ϣ
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������Ӿͳɹ��ˣ�indCb��֪ͨ�ϲ�Ӧ�����������Ƿ�ɹ������ӳɹ�������OPENAT_NETWORK_LINKED״̬
           ����socket����֮ǰ����Ҫ������������
           ��������֮ǰ��״̬��ҪΪOPENAT_NETWORK_READY״̬�����������ʧ��
**/                          
BOOL iot_network_connect(T_OPENAT_NETWORK_CONNECT* connectParam)
{
    //1. �ȴ�4G����׼����
    if(g_s_nwMode == 4) //4Gֱ���á�Ĭ�ϼ���pdp
    {
        return TRUE;
        
    }
    else if(g_s_nwMode == 2)//�����2g���ͻ�ȥ���¼���һ·pdp
    {
        if(gsmGprsPDPActive(connectParam->apn, connectParam->username, connectParam->password))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

}      



static bool gsmGprsPDPDeactive(void)
{
  int err;
  ATResponse *p_response = NULL;
  char cmd[64] = {0};
  
  memset(cmd, 0, 64);
  sprintf(cmd, "AT+CGACT=0,6");
            
  err = at_send_command(cmd, &p_response);
  if (err < 0 || p_response->success == 0){
    goto error;
  }

  at_response_free(p_response);
  return TRUE;

error:
  at_response_free(p_response);
  return FALSE;
}


/**�Ͽ��������ӣ�ʵ��Ϊpdpȥ����
*@param     flymode:   ��ʱ��֧�֣�����ΪFLASE
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������������ͶϿ��ˣ�indCb��֪ͨ�ϲ�Ӧ��
           ���ӶϿ�������״̬��ص�OPENAT_NETWORK_READY״̬
           ��ǰ����socket����Ҳ��ʧЧ����Ҫclose��
**/                                        
BOOL iot_network_disconnect(BOOL flymode)
{
    if(g_s_nwMode == 4) //4G
    {
        return TRUE;
    }
    else if(g_s_nwMode == 2)
    {
        if(gsmGprsPDPDeactive())
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    
}                          

