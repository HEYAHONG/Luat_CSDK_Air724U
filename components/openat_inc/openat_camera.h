/*********************************************************
  Copyright (C), AirM2M Tech. Co., Ltd.
  Author: Jack.li
  Description: AMOPENAT ����ƽ̨
  Others:
  History: 
    Version�� Date:       Author:   Modification:
    V0.1      2013.1.28   Jack.li     �����ļ�
*********************************************************/
#ifndef OPENAT_CAMERA_H
#define OPENAT_CAMERA_H

BOOL OPENAT_InitCamera(T_AMOPENAT_CAMERA_PARAM *cameraParam);

BOOL OPENAT_CameraPoweron(BOOL videoMode);
BOOL OPENAT_CameraPowerOff(void);
BOOL OPENAT_CameraPreviewOpen(T_AMOPENAT_CAM_PREVIEW_PARAM *previewParam);
BOOL OPENAT_CameraPreviewClose(void);
BOOL OPENAT_CameraCapture(T_AMOPENAT_CAM_CAPTURE_PARAM *captureParam);
BOOL OPENAT_CameraSavePhoto(INT32 iFd);

/*+\NEW\Jack.li\2013.2.9\��������ͷ��Ƶ¼�ƽӿ� */
BOOL OPENAT_CameraVideoRecordStart(INT32 iFd);
BOOL OPENAT_CameraVideoRecordPause(void);
BOOL OPENAT_CameraVideoRecordResume(void);
BOOL OPENAT_CameraVideoRecordStop(void);
/*-\NEW\Jack.li\2013.2.9\��������ͷ��Ƶ¼�ƽӿ� */

#endif /* OPENAT_CAMERA_H */

