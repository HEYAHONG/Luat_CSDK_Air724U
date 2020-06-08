#ifndef __IOT_CAMERA_H__
#define __IOT_CAMERA_H__

#include "iot_os.h"

/**
 * @defgroup iot_sdk_device ����ӿ�
 * @{
 */
	/**@example zbar/demo_zbar_camera.c
	* camera�ӿ�ʾ��
	*/ 

/**
 * @defgroup iot_sdk_camera ����ͷ�ӿ�
 * @{
 */

/**����ͷ��ʼ��
*@param		cameraParam:		��ʼ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_init(T_AMOPENAT_CAMERA_PARAM *cameraParam);

/**������ͷ
*@param		videoMode:		�Ƿ���Ƶģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_poweron(BOOL videoMode);  
/**�ر�����ͷ
*@return  TRUE:       �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_poweroff(void); 
/**��ʼԤ��
*@param  previewParam:       Ԥ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_preview_open(T_AMOPENAT_CAM_PREVIEW_PARAM *previewParam);
/**�˳�Ԥ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_preview_close(void);
/**����
*@param  fileName:      ����ͼƬ���ļ���
*@param  captureParam:       Ԥ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_capture(char *fileName, T_AMOPENAT_CAM_CAPTURE_PARAM *captureParam);
   
/** @}*/
/** @}*/


#endif


