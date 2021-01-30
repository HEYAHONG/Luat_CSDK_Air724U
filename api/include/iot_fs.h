#ifndef __IOT_FS_H__
#define __IOT_FS_H__

#include "iot_os.h"


/**
 * @defgroup iot_sdk_fs �ļ�ϵͳ�ӿ�
 * @{
 */
	/**@example fs/demo_fs.c
	* fs�ӿ�ʾ��
	*/ 

/**���ļ�
*@param		pszFileNameUniLe:		�ļ�ȫ·������
*@param		iFlag:		�򿪱�־��ϸ��μ�E_AMOPENAT_FILE_OPEN_FLAG
*@return	INT32: 	    �����ļ����
**/
INT32 iot_fs_open_file(                           
                        char* pszFileNameUniLe,
                        UINT32 iFlag         
                  );

/**�ر��ļ�
*@param		iFd:		�ļ������open_file �� create_file ���ص�
*@return	INT32: 	    ����ֵС��0ʧ��, ����ɹ�
**/
INT32 iot_fs_close_file(                     
                        INT32 iFd           
                   );

/**��ȡ�ļ�
*@param		iFd:		�ļ������open_file �� create_file ���ص�
*@param		pBuf:		���ݱ���ָ��
*@param		iLen:		buf����
*@return	INT32: 	    ���ض�ȡ����, С��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_read_file(                            
                        INT32 iFd,            
                        UINT8 *pBuf,            
                        UINT32 iLen            
                  );

/**д���ļ�
*@param		iFd:		�ļ������open_file �� create_file ���ص�
*@param		pBuf:		��Ҫд�������ָ��
*@param		iLen:		���ݳ���
*@return	INT32: 	    ����д�볤��, С��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_write_file(                           
                        INT32 iFd,            
                        UINT8 *pBuf,          
                        UINT32 iLen             
                   );

/**����д��flash
*@param		iFd:		�ļ������open_file �� create_file ���ص�
*@return	INT32: 	    ��������д�볤��, С��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_flush_file(                           
                        INT32 iFd              
                   );    

/**�ļ���λ
*@note  ����iOffset�ĺ���ȡ����iOrigin��ֵ.
*@param		iFd:		�ļ������open_file �� create_file ���ص�
*@param		iOffset:	ƫ����
*@param		iOrigin:	������ϸ��μ�E_AMOPENAT_FILE_SEEK_FLAG
*@return	INT32: 	    �����ļ���ƫ����
**/
INT32 iot_fs_seek_file(                           
                        INT32 iFd,            
                        INT32 iOffset,         
                        UINT8 iOrigin          
                  );

/**�����ļ�
*@param		pszFileNameUniLe:	�ļ�ȫ·������
*@return	INT32: 	            �����ļ����, С��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_create_file(                          
                        char* pszFileNameUniLe   
                    );

/**ɾ���ļ�
*@param		pszFileNameUniLe:	�ļ�ȫ·������
*@return	INT32: 	            ����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_delete_file(                          
                        char* pszFileNameUniLe
                    );

/**�л���ǰ����Ŀ¼
*@param		pszDirNameUniLe:	Ŀ¼·��
*@return	INT32: 	����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_change_dir(                            
                        char* pszDirNameUniLe  
                   );

/**����Ŀ¼
*@param		pszDirNameUniLe:	Ŀ¼·��
*@param		iMode:	            Ŀ¼���ԣ���ϸ��μ�E_AMOPENAT_FILE_ATTR_TAG
*@return	INT32: 	����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_make_dir(                              
                        char* pszDirNameUniLe, 
                        UINT32 iMode          
                 );

/**ɾ��Ŀ¼
*@param		pszDirNameUniLe:	Ŀ¼·��,��Ŀ¼����Ϊ�գ��ӿڲ��ܷ��سɹ�
*@return	INT32: 	����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_remove_dir(                          
                        char* pszDirNameUniLe  
                   );

/**��ȡ��ǰ·��
*@param		pCurDirUniLe:	Ŀ¼·��
*@param		uUnicodeSize:	    �洢Ŀ¼��Ϣ�ռ��С
*@return	INT32: 	����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_get_current_dir(                    
                        char* pCurDirUniLe,   
                        UINT32 uUnicodeSize   
                        );

/**��ȡ�ļ�ϵͳ��Ϣ
*@param		path:	�ļ�ϵͳ�ĸ�Ŀ¼
*@param		fileInfo:	�ļ�ϵͳ��Ϣ
*@return	INT32 ����ֵС��0��ʾʧ��,����ɹ�
**/
INT32 iot_fs_get_fs_info(                         
                        char       *path,         
                        T_AMOPENAT_FILE_INFO               *fileInfo                 
                   );


/**��ȡ�ļ���С�ӿ�
*@param	   pszFileNameUniLe:   �ļ�ȫ·������
*@return   UINT32:			   �����ļ��Ĵ�С
**/
UINT32 iot_fs_file_size(
                    char* pszFileNameUniLe
                );

/**�򿪲��ң����һ�ȡ�ļ������ļ���
*@param		dirName:	·��
*@param     findResult: ��Ӧ·���µĵ�һ���ļ�
*@return	fileInfo: 	���ҷ��صľ��������ֵС��0��ʾʧ��,����ɹ�
**/                   
INT32 iot_fs_find_first(
     char* dirName,
     PAMOPENAT_FS_FIND_DATA findResult
);

/**��ȡ�ļ������ļ���
*@param		iFd:  iot_fs_find_first�ӿڷ���ֵ
*@param     findResult: ��Ӧ·���µ��ļ�
*@return	fileInfo: 	����С��0��ʾû��ʣ���ļ�������ֵ��ʾ�����ļ�
**/                   
INT32 iot_fs_find_next(
     INT32 iFd, 
     PAMOPENAT_FS_FIND_DATA findResult
);

/**�رղ���
*@param		iFd:  iot_fs_find_first�ӿڷ���ֵ
*@return	fileInfo: 	����ֵС��0��ʾʧ��,����ɹ�
**/           
INT32 iot_fs_find_close(
     INT32 iFd
);

/**�ļ���λ�ӿ�
*@param 	iFd:		�ļ������open_file �� create_file ���ص�
*@return	INT32:		�����ļ���ƫ����
**/
INT32 iot_fs_ftell(    
							INT32 iFd			
					  );


/**mount�û��Լ����ļ�ϵͳ
*@param	param  �ļ�ϵͳ���ýṹ��
*@note		path: �ļ�ϵͳ��Ŀ¼,
*@note      offset; flash ��ַƫ����
*@note		size: �ļ�ϵͳ��С
*@note		exFlash: �Ƿ����ⲿflash
*@note		ע:path��/��ͷ,������Ҫ���ڵ���5 ����"/APP1"
*@note      ֧�ֵ�flash�ͺ�
			XT25W32B	0x16600b
			XT25W64B	0x17600b
			XM25QU64A	0x173820
			XM25QU64B	0x175020
			XM25QU32C	0x165020
			XM25QU16C	0x155020
			P25Q64H		0x176085
			GD25LE64E	0x1760c8
			GD25LQ128C	0x1860c8
			W25Q64JV	0x1740ef
			GD25Q127C	0x1840c8
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/ 
BOOL iot_fs_mount(T_AMOPENAT_USER_FSMOUNT * param);

/**unmount�û��Լ����ļ�ϵͳ
*@param	param  �ļ�ϵͳ���ýṹ��
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/ 
BOOL iot_fs_unmount(T_AMOPENAT_USER_FSMOUNT * param);

/**unmount�û��Լ����ļ�ϵͳ
*@param	param  �ļ�ϵͳ���ýṹ��
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/
BOOL iot_fs_format(T_AMOPENAT_USER_FSMOUNT * param);

/**SD����mount fat�ļ�ϵͳ
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/
BOOL iot_fs_mount_sdcard(void);

/**umount SD�����fat�ļ�ϵͳ
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/
BOOL iot_fs_umount_sdcard(void);

/**format SD������ļ�ϵͳ
*@return	BOOL: TRUE-�ɹ� FALSE-ʧ��
**/
BOOL iot_fs_format_sdcard(void);


/** @}*/

#endif
