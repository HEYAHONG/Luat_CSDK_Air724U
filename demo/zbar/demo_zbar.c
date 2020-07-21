/* Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */
#include "iot_os.h"
#include "iot_debug.h"

#include "demo_zbar.h"

static HANDLE g_s_zbar_task;
unsigned char *gScannerBuff = NULL;

/*��ȡyuv�����е�y*/
static unsigned char * zbar_scannerY(unsigned char *data)
{
	unsigned char *src = data,*end = data + (CAM_SENSOR_WIDTH * CAM_SENSOR_HEIGHT *2);
	unsigned char *dst = gScannerBuff;
	while (src < end)
	{
		src ++;
		*dst = *src;
		src ++;
		dst++;
	}//End of while;

    return gScannerBuff;
}

/*������ά���е�����*/
static void zbar_scannerRun(int width, int height, int size, unsigned char *dataInput)
{
	int len;
	char *data;
	
	//��������� handle != 0 ��ʾ����ɹ�
	int handle = iot_zbar_scannerOpen(width, height, size, dataInput);
    
	if (handle)
	{
		do
		{        
			// ����ɹ���ȡ��ά����Ϣ
			data = iot_zbar_getData(handle, &len);
			data[len] = 0;

			iot_debug_print("[zbar] zbar_scanner_run come in handle_data %s", data);

		 // �ж��Ƿ�����һ������
		}while(iot_zbar_findNextData(handle) > 0);

		// �ͷž��
		iot_zbar_scannerClose(handle);
	}
}

/*Ԥ���ص�����, ��ȡԤ������*/
void camera_evevt_callback(T_AMOPENAT_CAMERA_MESSAGE *pMsg)
{

	iot_debug_print("[zbar] camera_evevt_callback");
    switch(pMsg->evtId)
    {
        case OPENAT_DRV_EVT_CAMERA_DATA_IND:
        {
            // ��ȡcamera�õ������ݣ� ���͵�zbartask ȥ����
            zbar_scannerRun(CAM_SENSOR_WIDTH, CAM_SENSOR_HEIGHT,CAM_SENSOR_HEIGHT*CAM_SENSOR_WIDTH, zbar_scannerY((unsigned char *)pMsg->dataParam.data));
            break;
        }
        default:
            break;
    }
}

static void demo_ota_task(PVOID pParameter)
{
	gScannerBuff = iot_os_malloc(CAM_SENSOR_HEIGHT*CAM_SENSOR_WIDTH);

	lcdInit();

	cameraInit(camera_evevt_callback);
}


int appimg_enter(void *param)
{    
	iot_debug_print("[zbar] appimg_enter");


	g_s_zbar_task = iot_os_create_task(demo_ota_task,
                        NULL,
                        10*1024,
                        5,
                        OPENAT_OS_CREATE_DEFAULT,
                        "DEMO_ZBAR");
	
    return 0;
}

void appimg_exit(void)
{
    iot_debug_print("[zbar] appimg_exit");
}
