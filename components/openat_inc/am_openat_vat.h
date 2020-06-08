/*********************************************************
  Copyright (C), AirM2M Tech. Co., Ltd.
  Author: lifei
  Description: AMOPENAT ����ƽ̨
  Others:
  History: 
    Version�� Date:       Author:   Modification:
    V0.1      2012.12.14  lifei     �����ļ�
*********************************************************/
#ifndef AM_OPENAT_VAT_H
#define AM_OPENAT_VAT_H

#include "am_openat_common.h"


typedef VOID (*PAT_MESSAGE)(UINT8 *pData, UINT16 length);

/*+\NEW\zhuwangbin\2020.4.22\���LUA_POC��Ŀ�� ͨ������ͨ������POC�Ŀ���*/
typedef BOOL (*PAT_POC_MESSAGE)(char *pData, int length);
/*-\NEW\zhuwangbin\2020.4.22\���LUA_POC��Ŀ�� ͨ������ͨ������POC�Ŀ���*/


BOOL OPENAT_vat_init(PAT_MESSAGE resp_cb);
BOOL OPENAT_vat_send_at( const char* pAtCommand, unsigned nLength );

int vat_test_enter(void *param);



#endif /* AM_OPENAT_VAT_H */

