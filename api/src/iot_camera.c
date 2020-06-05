#include "iot_camera.h"

/**����ͷ��ʼ��
*@param		cameraParam:		��ʼ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_init(T_AMOPENAT_CAMERA_PARAM *cameraParam)
{
  return IVTBL(InitCamera)(cameraParam);
}

/**������ͷ
*@param		videoMode:		�Ƿ���Ƶģʽ
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_poweron(BOOL videoMode)
{
  return IVTBL(CameraPoweron)(videoMode);
}
/**�ر�����ͷ
*@return  TRUE:       �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_poweroff(void)
{
  IVTBL(CameraPowerOff)();
  return TRUE;
}
/**��ʼԤ��
*@param  previewParam:       Ԥ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��

**/
BOOL iot_camera_preview_open(T_AMOPENAT_CAM_PREVIEW_PARAM *previewParam)
{
  return IVTBL(CameraPreviewOpen)(previewParam);
}
/**�˳�Ԥ��
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��

**/
BOOL iot_camera_preview_close(void)
{
  return IVTBL(CameraPreviewClose)();
}
/**����
*@param  fileName:      ����ͼƬ���ļ���
*@param  captureParam:       Ԥ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_camera_capture(char *fileName, T_AMOPENAT_CAM_CAPTURE_PARAM *captureParam)
{ 
	INT32 fd;
		
	if (!fileName)
	{
		return FALSE;
	}
	
	iot_fs_delete_file(fileName);
	fd = iot_fs_create_file(fileName);

	if (fd < 0)
	{
		return FALSE; 
	}
	
	if(!IVTBL(CameraCapture)(captureParam))
	{
		iot_fs_close_file(fd);
		return FALSE;
	}

	if (!IVTBL(CameraSavePhoto)(fd))
	{
		iot_fs_close_file(fd);
		return FALSE; 
	}

	iot_fs_close_file(fd);

	return TRUE;
}
