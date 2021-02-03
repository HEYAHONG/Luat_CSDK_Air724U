/*+\NEW\zhuwangbin\2020.05.01\���disp camera����*/
#ifndef __PLATFORM_CAMERA_H__
#define __PLATFORM_CAMERA_H__

/*-\NEW\zhuwangbin\2020.8.22\ lua�汾��camera�Ĵ����ɽű�����*/
typedef struct T_PLATFORM_CAMERA_REG_TAG
{
    UINT16      addr;
    UINT16      value;
}PLATFORM_CAMERA_REG, *PPLATFORM_CAMERA_REG;

typedef enum
{
    PLATFORM_SPI_MODE_NO = 0,         // parallel sensor in use
    PLATFORM_SPI_MODE_SLAVE ,        // SPI sensor as SPI slave
    PLATFORM_SPI_MODE_MASTER1,     // SPI sensor as SPI master, 1 sdo output with SSN 
    PLATFORM_SPI_MODE_MASTER2_1, // SPI sensor as SPI master, 1 sdo output without SSN
    PLATFORM_SPI_MODE_MASTER2_2, // SPI sensor as SPI master, 2 sdos output 
    PLATFORM_SPI_MODE_MASTER2_4, // SPI sensor as SPI master, 4 sdos output
    PLATFORM_SPI_MODE_UNDEF,
} PLATFORM_CAMERA_SPI_MODE_E;

typedef enum
{
    PLATFORM_SPI_OUT_Y0_U0_Y1_V0 = 0,
    PLATFORM_SPI_OUT_Y0_V0_Y1_U0,
    PLATFORM_SPI_OUT_U0_Y0_V0_Y1,
    PLATFORM_SPI_OUT_U0_Y1_V0_Y0,
    PLATFORM_SPI_OUT_V0_Y1_U0_Y0,
    PLATFORM_SPI_OUT_V0_Y0_U0_Y1,
    PLATFORM_SPI_OUT_Y1_V0_Y0_U0,
    PLATFORM_SPI_OUT_Y1_U0_Y0_V0,
} PLATFORM_CAMERA_SPI_YUV_OUT_E;

typedef enum
{
	PLATFORM_SPI_SPEED_DEFAULT,
	PLATFORM_SPI_SPEED_SDR, /*��������*/
	PLATFORM_SPI_SPEED_DDR, /*˫������*/
	PLATFORM_SPI_SPEED_QDR, /*�ı����� �ݲ�֧��*/
}PLATFORM_SPI_SPEED_MODE_E;

/*camera sensor��������*/
typedef struct T_PLATFORM_CAMERA_PARAM_TAG
{
    UINT8       i2cSlaveAddr;               /* ����ͷi2c���ʵ�ַ */
    UINT16      sensorWidth;                /* ����ͷ�Ŀ� */
    UINT16      sensorHeight;				/* ����ͷ�ĸ� */    
    PPLATFORM_CAMERA_REG initRegTable_p;  /* ����ͷ��ʼ��ָ��� */
    UINT16 initRegTableCount;          /* ����ͷ��ʼ��ָ���� */
    PLATFORM_CAMERA_REG idReg;          /* ����ͷID�Ĵ�����ֵ */
    PLATFORM_CAMERA_SPI_MODE_E       spi_mode; /*����ͷSPIģʽ*/
    PLATFORM_CAMERA_SPI_YUV_OUT_E  spi_yuv_out; /*����ͷYUV��ʽ*/
	PLATFORM_SPI_SPEED_MODE_E spi_speed; /*����ͷ�ɼ�����*/
}T_PLATFORM_CAMERA_PARAM;
/*-\NEW\zhuwangbin\2020.8.22\ lua�汾��camera�Ĵ����ɽű�����*/

BOOL platform_camera_poweron(BOOL video_mode, int nCamType, BOOL bZbarScan, BOOL bMirror, BOOL bJump);


BOOL platform_camera_poweroff(void);


BOOL platform_camera_preview_open(u16 offsetx, u16 offsety,u16 startx, u16 starty, u16 endx, u16 endy);



BOOL platform_camera_preview_close(void);



BOOL platform_camera_capture(u16 width, u16 height, u16 quality);


BOOL platform_camera_save_photo(const char* filename);


/*+\NEW\zhuwangbin\2020.7.14\���camera sensorд�Ĵ����ӿ�*/
BOOL platform_CameraWriteReg(int *pInitCmd, int nInitCmdSize);
/*+\NEW\zhuwangbin\2020.7.14\���camera sensorд�Ĵ����ӿ�*/

/*+\NEW\zhuwangbin\2020.7.20\���camera ��ת��������*/
BOOL platform_camera_preview_zoom(int zoom);
BOOL platform_camera_preview_rotation(int rotation);
/*-\NEW\zhuwangbin\2020.7.20\���camera ��ת��������*/


#endif
/*-\NEW\zhuwangbin\2020.05.01\���disp camera����*/
