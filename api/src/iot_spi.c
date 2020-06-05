#include "iot_spi.h"


/**����spi
*@param		port:		SPI ���
*@param		cfg:		��ʼ������
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_spi_open(
                    E_AMOPENAT_SPI_PORT  port,         
                    T_AMOPENAT_SPI_PARAM *cfg          
              )
{
    return OPENAT_OpenSPI(port, cfg);
}

/**��ȡspi����
*@param		port:		SPI ���
*@param		buf:		�洢���ݵ�ַ
*@param		bufLen:		�洢�ռ䳤��
*@return	UINT32: 	ʵ�ʶ�ȡ����
**/
UINT32 iot_spi_read(                                   
                    E_AMOPENAT_SPI_PORT port,         
                    UINT8* buf,                        
                    UINT32 bufLen                      
              )
{
    return OPENAT_ReadSPI(port, buf, bufLen);
}

/**д��spi����
*@param		port:		SPI ���
*@param		buf:		д�����ݵ�ַ
*@param		bufLen:		д�����ݳ���
*@return	UINT32: 	ʵ��д�볤��
**/
UINT32 iot_spi_write(                                   
                    E_AMOPENAT_SPI_PORT port,         
                    CONST UINT8* buf,                   
                    UINT32 bufLen                       
               )
{
    return OPENAT_WriteSPI(port, buf, bufLen, 0);
}

/**spiȫ˫����д
*@note      ȫ˫����ʽ��д����д������ͬ
*@param		port:		SPI ���
*@param		txBuf:		д����
*@param		rxBuf:		������
*@param		len:		��д����
*@return	UINT32: 	ʵ��д�볤��
**/
UINT32 iot_spi_rw(                                        
                    E_AMOPENAT_SPI_PORT port,         
                    CONST UINT8* txBuf,                
                    UINT8* rxBuf,                      
                    UINT32 len                         
            )
{
    return OPENAT_RwSPI(port, txBuf, rxBuf, len);
}


/**�ر�spi
*@param		port:		SPI ���
*@return	TRUE: 	    �ɹ�
*           FALSE:      ʧ��
**/
BOOL iot_spi_close(
                    E_AMOPENAT_SPI_PORT  port
            )
{
    return OPENAT_CloseSPI(port);
}

