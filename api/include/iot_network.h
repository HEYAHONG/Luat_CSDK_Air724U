#ifndef __IOT_NETWORK_H__
#define __IOT_NETWORK_H__

#include "am_openat.h"

// #define OPENAT_NETWORK_ISP_LENGTH (64)
// #define OPENAT_NETWORK_IMSI_LENGHT (64)
#define OPENAT_NETWORK_APN_LENGTH (64)
#define OPENAT_NETWORK_PASSWORD_LENGTH	(64)
#define OPENAT_NETWORK_USER_NAME_LENGTH (64)


typedef enum
{
    /*!< ����Ͽ� ��ʾGPRS���粻���ã��޷������������ӣ��п��ܿ��Դ�绰*/
	OPENAT_NETWORK_DISCONNECT            		= 0x00,
    /*!< ���������� ��ʾGPRS������ã����Խ�����·����*/
	OPENAT_NETWORK_READY,
	/*!< ��·���ڼ��� */
	OPENAT_NETWORK_LINKING,
    /*!< ��·�Ѿ����� PDP�Ѿ��������ͨ��socket�ӿڽ�����������*/
	OPENAT_NETWORK_LINKED,
	/*!< ��·����ȥ���� */
	OPENAT_NETWORK_GOING_DOWN,
}E_OPENAT_NETWORK_STATE;

typedef VOID(*F_OPENAT_NETWORK_IND_CB)(E_OPENAT_NETWORK_STATE state);



typedef enum
{
    /*!< sim��״̬δ֪*/
	OPENAT_NETWORK_UNKNOWN=0,
    /*!< sim��״̬��Ч*/
	OPENAT_NETWORK_TRUE,
    /*!< SIMδ�����PIN��δ����*/
	OPENAT_NETWORK_FALSE=255,
}E_OPENAT_SIM_STATE;

typedef struct
{
	/*!< ����״̬ */
	E_OPENAT_NETWORK_STATE state;
	/*!< �����źţ�0-31 (ֵԽ���ź�Խ��) */
	UINT8 csq;
	/*!< SIM��״̬ */
	E_OPENAT_SIM_STATE  simpresent;
}T_OPENAT_NETWORK_STATUS;

typedef struct
{
	char apn[OPENAT_NETWORK_APN_LENGTH];
	char username[OPENAT_NETWORK_USER_NAME_LENGTH];
	char password[OPENAT_NETWORK_PASSWORD_LENGTH];
}T_OPENAT_NETWORK_CONNECT;




/**
 * @defgroup iot_sdk_network ����ӿ�
 * @{
 */
/**��ȡ����״̬
*@param     status:   ��������״̬
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��            
**/                                
BOOL iot_network_get_status (
                            T_OPENAT_NETWORK_STATUS* status
                            );
/**��������״̬�ص�����
*@param     indCb:   �ص�����
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
**/                            
BOOL iot_network_set_cb(F_OPENAT_NETWORK_IND_CB indCb);
/**�����������ӣ�ʵ��Ϊpdp��������
*@param     connectParam:  �������Ӳ�������Ҫ����APN��username��passwrd��Ϣ
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������Ӿͳɹ��ˣ�indCb��֪ͨ�ϲ�Ӧ�����������Ƿ�ɹ������ӳɹ�������OPENAT_NETWORK_LINKED״̬
           ����socket����֮ǰ����Ҫ������������
           ��������֮ǰ��״̬��ҪΪOPENAT_NETWORK_READY״̬�����������ʧ��
**/                          
BOOL iot_network_connect(T_OPENAT_NETWORK_CONNECT* connectParam);
/**�Ͽ��������ӣ�ʵ��Ϊpdpȥ����
*@param     flymode:   ��ʱ��֧�֣�����ΪFLASE
*@return    TRUE:    �ɹ�
            FLASE:   ʧ��
@note      �ú���Ϊ�첽���������غ󲻴����������������ͶϿ��ˣ�indCb��֪ͨ�ϲ�Ӧ��
           ���ӶϿ�������״̬��ص�OPENAT_NETWORK_READY״̬
           ��ǰ����socket����Ҳ��ʧЧ����Ҫclose��
**/                                        
BOOL iot_network_disconnect(BOOL flymode);

/** @}*/

#endif

