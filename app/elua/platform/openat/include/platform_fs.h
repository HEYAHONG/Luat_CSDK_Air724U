/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_fs.h
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2012/11/27
 *
 * Description:
 * 
 **************************************************************************/

#ifndef __PLATFORM_FS_H__
#define __PLATFORM_FS_H__
#include <devman.h>
#include "am_openat_fs.h"

/*+\new\wj\2020.9.1\����mount��unmount��format�ӿ�*/
typedef enum
{
	E_PLATFROM_FLASH_INTERNAL, // mount �ڲ���flash����
    E_PLATFROM_FLASH_EXTERN_PINLCD, // mount �ⲿ��flash����ʹ��LCD pin�Ÿ���  V_LCD����
    E_PLATFROM_FLASH_EXTERN_PINGPIO,// mount �ⲿ��flash����ʹ��GPIO pin�Ÿ��� V_PAD_1V8����
}PLATFORM_FLASH_TYPE;


typedef struct
{
	char *path;    //mount���ļ�ϵͳ��Ŀ¼ ����>=5
	UINT32 offset; //flash ��ַƫ����
	UINT32 size;  //�ļ�ϵͳ�Ĵ�С
	PLATFORM_FLASH_TYPE exFlash;
	UINT8 clkDiv; //�ⲿflash��Ƶ��Χ2-255. clk=166M/clkDiv	
}PLATFORM_FS_MOUNT_PARAM;

/*-\new\wj\2020.9.1\����mount��unmount��format�ӿ�*/
const DM_DEVICE* platform_fs_init(void);

int platformfs_removedir_r( const char *path );

int platformfs_changedir_r( const char *path );

int platformfs_removedir_rec_r( const char *path );


int platformfs_findfirst_r( const char *path, PAMOPENAT_FS_FIND_DATA find_data);


int platformfs_findnext_r(int find_id, PAMOPENAT_FS_FIND_DATA find_data);


int platformfs_findclose_r(int find_id);

int platformfs_makedir_r( const char *path, int mode );

_ssize_t platformfs_get_size_r(const char *path);
/*+\BUG\wangyuan\2020.06.11\��sdcard���ء�ж�ء���ʽ�������ŵ�io����*/
/*+\new\wj\2020.9.1\����mount��unmount��format�ӿ�*/
BOOL platform_sdcard_fsMount(void);

BOOL platform_sdcard_fsUMount(void);

BOOL platformfs_Format_sdcard(void);
/*-\new\wj\2020.9.1\����mount��unmount��format�ӿ�*/
/*-\BUG\wangyuan\2020.06.11\��sdcard���ء�ж�ء���ʽ�������ŵ�io����*/
#endif //__PLATFORM_FS_H__

/*+\bug2991\zhuwangbin\2020.06.11\����lua otp�ӿ�*/
BOOL platformfs_otp_erase(UINT16 address, UINT16 size);

BOOL platformfs_otp_write(UINT16 address, char * data, UINT32 size);

BOOL platformfs_otp_read(UINT16 address, char * data, UINT32 size);

BOOL platformfs_otp_lock(UINT16 address, UINT16 size);
/*-\bug2991\zhuwangbin\2020.06.11\����lua otp�ӿ�*/
