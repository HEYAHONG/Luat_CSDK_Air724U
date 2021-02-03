/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_uart.c
 * Author:  liweiqiang
 * Version: V0.1
 * Date:    2012/10/15
 *
 * Description:
 *  2013.08.07 liweiqiang       ����uart1֧��
 **************************************************************************/
#if 1
#include "string.h"

#include "am_openat.h"
#include "cycle_queue.h"
#include "assert.h"

#include "lplatform.h"
#include "platform_malloc.h"
#include "platform_conf.h"
#include "platform_rtos.h"
//#include "dcl.h"
#include "malloc.h"

//#include "teldef.h"

#define COS_WAIT_FOREVER            OPENAT_OS_SUSPENDED

//vat recv queue
/*+\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */
#define VATC_RX_BUF_SIZE            8192
#define VATC_READ_BUF_SIZE          512
/*-\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */

#define UART_RX_BUF_SIZE            2048

// ��ʱ������
#define RX_BUFFER_SIZE          256
/*+\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
#define READ_BUFFER_SIZE        126
/*-\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */

#define PHY_PORT(ID)            (uartmap[ID].port)

typedef struct UartPhyContextTag
{
    CycleQueue  rxqueue;
    uint8       temprxbuff[RX_BUFFER_SIZE];
/*+\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
    uint8       readbuf[READ_BUFFER_SIZE];
    uint8       readindex;
    uint8       readsize;
/*-\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
}UartPhyContext;

typedef struct UartMapTag
{
    const E_AMOPENAT_UART_PORT port;
    const PUART_MESSAGE        msg;
}UartMap;
/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
typedef enum{
	UART_NO_EVENT_CONFIG = -1,
	UART_RX_EVENT_CONFIG,
	UART_TX_DONE_EVENT_CONFIG,
	UART_MAX_EVENT_CONFIG
}PLATFORM_UART_EVENT_CONFIG;

typedef struct{
	PLATFORM_UART_EVENT_CONFIG UartConfigEvent;
	int UartConfigId;
}PLATFORM_UART_CONFIG;
/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
typedef struct UartContextTag
{
    uint8 opened;
    uint8 workmode; //uart������ʾ��ʽ:1:�û���ѯ ����:��Ϣ��ʾ 2: host uart ID A2����͸��
    /*+\NEW\zhutianhua\2018.12.27 14:20\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
    uint8 rs485Io;
    uint8 rs485ValidLevel;
	/*+\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/
	uint32 rs485DelayTime;
	/*-\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/
    /*-\NEW\zhutianhua\2018.12.27 14:20\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
    /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
    uint32 uartBand;//�洢���ڵĲ����ʡ�485ͨѶ��Ҫ����oe��ֹͣʱ��
    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
}UartContext;

/*+\NEW\zhutianhua\2018.12.27 15:8\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
#define RS485_INVALID_LEVEL 0xFF
/*-\NEW\zhutianhua\2018.12.27 15:8\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/

/*+\NEW\liweiqiang\2014.7.21\����AM002_LUA��ĿRAM�������벻��������*/
#if defined(LOW_MEMORY_SUPPORT)
static UartPhyContext uartPhyContext[3]/*openat uart 1 & 2 & host uart*/;
#else
static uint8 uart1RxBuff[UART_RX_BUF_SIZE];
static uint8 uart2RxBuff[UART_RX_BUF_SIZE];
/*+\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
static uint8 uart3RxBuff[UART_RX_BUF_SIZE];
/*-\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/

static UartPhyContext uartPhyContext[3]/*openat uart 1 & 2 & host uart*/ =
{
    // OPENAT_UART_1,
    {
        .rxqueue = {
            uart1RxBuff,
            UART_RX_BUF_SIZE,
            0,
            0,
            1,
            0,
            0,
        },
    },
    // OPENAT_UART_2,
    {
        .rxqueue = {
            uart2RxBuff,
            UART_RX_BUF_SIZE,
            0,
            0,
            1,
            0,
            0,
        },
    },
/*+\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
    // OPENAT_UART_3,
    {
        .rxqueue = {
            uart3RxBuff,
            UART_RX_BUF_SIZE,
            0,
            0,
            1,
            0,
            0,
        },
    },
/*-\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
};
#endif
/*-\NEW\liweiqiang\2014.7.21\����AM002_LUA��ĿRAM�������벻��������*/

/*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
HANDLE rs485_oe_task_hand = NULL;

typedef enum
{
  enable_rs485_oe = 0x10,
  disable_rs485_oe,
  close_rs485_oe
}rs485_oe_event;

/*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/

static void uart0_message_handle(T_AMOPENAT_UART_MESSAGE* evt);
static void uart1_message_handle(T_AMOPENAT_UART_MESSAGE* evt);
static void uart2_message_handle(T_AMOPENAT_UART_MESSAGE* evt);
/*+\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
static void uart3_message_handle(T_AMOPENAT_UART_MESSAGE* evt);
/*-\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/

static const UartMap uartmap[NUM_UART] = 
{
    {OPENAT_UART_1,uart1_message_handle},
    {OPENAT_UART_2,uart2_message_handle},
/*+\NEW\zhuwangbin\2019.12.31\���uart3����*/
    {OPENAT_UART_3,uart3_message_handle},
/*-\NEW\zhuwangbin\2019.12.31\���uart3����*/
/*-\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
};
/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
static PLATFORM_UART_CONFIG gUartConfig = {
	.UartConfigEvent = UART_NO_EVENT_CONFIG,
	.UartConfigId = -1,
};
/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
static UartContext uartContext[NUM_UART];

static HANDLE hAtcReadSem = 0;
static uint8 vatcRxBuff[VATC_RX_BUF_SIZE];
CycleQueue vatcRx_Q = {
    vatcRxBuff,
    VATC_RX_BUF_SIZE,
    0,
    0,
    1,
    0,
    0,
};
/*+\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */
static uint8 vatc_read_buffer[VATC_READ_BUF_SIZE];
static uint16 vatc_read_buf_size;
static uint16 vatc_read_buf_index;
/*-\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */

/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
static HANDLE hUsbReadSem = 0;
static uint8 usbRxBuff[VATC_RX_BUF_SIZE];
CycleQueue usbRx_Q = {
    usbRxBuff,
    VATC_RX_BUF_SIZE,
    0,
    0,
    1,
    0,
    0,
};

static uint8 usb_read_buffer[VATC_READ_BUF_SIZE];
static uint16 usb_read_buf_size;
static uint16 usb_read_buf_index;
u32 usbdata_mode = 0;
/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */

/*+\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/
static char debugStdoutBuffer[128];//openat�ӿڵ�print�ӿ�buff���Ϊ127�ֽ�����ͬ��
/*-\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/
static UINT16 debugStdoutCachedCount = 0;
/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
static void sendUartMessage(int uart_id,platform_msg_type event_id)
{
    PlatformMsgData msgData;
   
    msgData.uart_id = uart_id;

    OPENAT_print("uart sendUartMessage uart %d tick %d", uart_id, OPENAT_get_system_tick());

	if(gUartConfig.UartConfigId == uart_id && 
			((UART_TX_DONE_EVENT_CONFIG == gUartConfig.UartConfigEvent && MSG_ID_RTOS_UART_TX_DONE == event_id) 
			|| (UART_RX_EVENT_CONFIG == gUartConfig.UartConfigEvent && MSG_ID_RTOS_UART_RX_DATA == event_id)))
	{
		platform_rtos_send_high_priority(event_id,&msgData);
	}
	else

	{
		platform_rtos_send(event_id, &msgData);
	}
}
/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
/*+\NEW\zhuwangbin\2018.8.10\���OPENAT_DRV_EVT_UART_TX_DONE_IND�ϱ�*/
static void sendUartTxDoneMessage(int uart_id)
{
    PlatformMsgData msgData;
    
    msgData.uart_id = uart_id;

    platform_rtos_send(MSG_ID_RTOS_UART_TX_DONE, &msgData);
}
/*-\NEW\zhuwangbin\2018.8.10\���OPENAT_DRV_EVT_UART_TX_DONE_IND�ϱ�*/

static void uart_message_handle(uint8 id, T_AMOPENAT_UART_MESSAGE* evt)
{
    uint32 length;
    uint8 phyid = uartmap[id].port;
/*+\NEW\liweiqiang\2013.4.7\�Ż�uart/atc���ݽ�����Ϣ��ʾ,���ⷢ��Ϣ����Ƶ������ϵͳ�޷���Ӧ */
    BOOL needMsg = FALSE; // buffer�ǿյ�ʱ��,�������ݲ���Ҫ����ʾ
    int count = (evt->param.dataLen / RX_BUFFER_SIZE);
    int i = 0;

    if(evt->evtId == OPENAT_DRV_EVT_UART_RX_DATA_IND)
    {
        for(i = 0; i < count; i++)
        {
            length = IVTBL(read_uart)(phyid, uartPhyContext[phyid].temprxbuff, RX_BUFFER_SIZE, 0);
            
            if(length != 0)
            {
                if(!needMsg)
                {
                    needMsg = uartPhyContext[phyid].rxqueue.empty ? TRUE : FALSE;
                }
                // �˴�������ü��ϱ���,д�뻺����uart�ж�,��ȡ������lua shell�߳�
                QueueInsert(&uartPhyContext[phyid].rxqueue, uartPhyContext[phyid].temprxbuff, length);
            }
        }

        if(evt->param.dataLen % RX_BUFFER_SIZE)
        {
            length = IVTBL(read_uart)(phyid, 
                uartPhyContext[phyid].temprxbuff,
                evt->param.dataLen % RX_BUFFER_SIZE, 
                0);
            
            if(length != 0)
            {
                if(!needMsg)
                {
                    needMsg = uartPhyContext[phyid].rxqueue.empty ? TRUE : FALSE;
                }
                // �˴�������ü��ϱ���,д�뻺����uart�ж�,��ȡ������lua shell�߳�
                QueueInsert(&uartPhyContext[phyid].rxqueue, uartPhyContext[phyid].temprxbuff, length);
            }
        }

        if(needMsg)
        {
			/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
            sendUartMessage(id+1,MSG_ID_RTOS_UART_RX_DATA);
			/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
        }   
    }
	/*+\NEW\zhuwangbin\2018.8.10\���OPENAT_DRV_EVT_UART_TX_DONE_IND�ϱ�*/
    else if(evt->evtId == OPENAT_DRV_EVT_UART_TX_DONE_IND)
    {
        /*+\NEW\zhutianhua\2018.12.27 15:33\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
        platform_uart_disable_rs485_oe(id);
        /*-\NEW\zhutianhua\2018.12.27 15:33\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
		/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
        sendUartMessage(id+1,MSG_ID_RTOS_UART_TX_DONE);
		/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
    }
	/*-\NEW\zhuwangbin\2018.8.10\���OPENAT_DRV_EVT_UART_TX_DONE_IND�ϱ�*/
/*-\NEW\liweiqiang\2013.4.7\�Ż�uart/atc���ݽ�����Ϣ��ʾ,���ⷢ��Ϣ����Ƶ������ϵͳ�޷���Ӧ */
}


static void uart1_message_handle(T_AMOPENAT_UART_MESSAGE* evt)
{
    uart_message_handle(0, evt);
}

static void uart2_message_handle(T_AMOPENAT_UART_MESSAGE* evt)
{
    uart_message_handle(1, evt);
}
/*+\NEW\zhuwangbin\2019.12.31\���uart3����*/
static void uart3_message_handle(T_AMOPENAT_UART_MESSAGE* evt)
{
    uart_message_handle(2, evt);
}
/*-\NEW\zhuwangbin\2019.12.31\���uart3����*/

/*+\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
static void host_uart_recv(UINT8 *data, UINT32 length)
{
    uint8 phyid = OPENAT_UART_3;
    BOOL needMsg = FALSE; // buffer�ǿյ�ʱ��,�������ݲ���Ҫ����ʾ

    if(length != 0)
    {
        needMsg = uartPhyContext[phyid].rxqueue.empty ? TRUE : FALSE;
        
        QueueInsert(&uartPhyContext[phyid].rxqueue, data, length);
    }

    if(needMsg)
    {
        sendUartMessage(3,MSG_ID_RTOS_UART_RX_DATA);
    }
}
/*-\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */

/****************************************************************************
 *
 * Function: PlatformUartOpen
 *
 * Parameters: 
 *         void
 *
 * Returns: void 
 *
 * Description: �򿪴���
 *
 ****************************************************************************/
 /*+\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
static u32 uart_phy_open( unsigned id, u32 baud, int databits, int parity, int stopbits, u32 mode, u32 txDoneReport)
/*-\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
{
    T_AMOPENAT_UART_PARAM uartParam;
    
    if(uartContext[id].opened)
        return baud;

    /*+\NEW\zhutianhua\2018.12.27 15:19\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
    uartContext[id].rs485ValidLevel = RS485_INVALID_LEVEL;
    /*-\NEW\zhutianhua\2018.12.27 15:19\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/

/*+\NEW\liweiqiang\2014.7.21\����AM002_LUA��ĿRAM�������벻��������*/
    if(!uartPhyContext[PHY_PORT(id)].rxqueue.buf)
    {
        uartPhyContext[PHY_PORT(id)].rxqueue.buf = lualibc_calloc(1, UART_RX_BUF_SIZE);
        uartPhyContext[PHY_PORT(id)].rxqueue.size = UART_RX_BUF_SIZE;
        QueueClean(&uartPhyContext[PHY_PORT(id)].rxqueue);
    }
/*-\NEW\liweiqiang\2014.7.21\����AM002_LUA��ĿRAM�������벻��������*/
#if 0
/*+\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
    if(PHY_PORT(id) == uart_port3 && mode == 2)
    {
        if(1)
        {
            uartContext[id].opened = 1;
            uartContext[id].workmode = mode;
            return baud;
        }
        else
        {
            return 0;
        }
    }
/*-\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
#endif
    /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
    uartContext[id].uartBand = baud;
    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
    uartParam.baud  = baud;
    uartParam.dataBits = databits;

    switch(stopbits)
    {
        case PLATFORM_UART_STOPBITS_1:
            uartParam.stopBits = 1;
            break;
            
        case PLATFORM_UART_STOPBITS_2:
            uartParam.stopBits = 2;
            break;

        case PLATFORM_UART_STOPBITS_1_5:
        default:
            goto uart_open_error;
            break;
    }

    switch(parity)
    {
        case PLATFORM_UART_PARITY_EVEN:
            uartParam.parity = OPENAT_UART_EVEN_PARITY;
            break;

        case PLATFORM_UART_PARITY_ODD:
            uartParam.parity = OPENAT_UART_ODD_PARITY;
            break;

        case PLATFORM_UART_PARITY_NONE:
            uartParam.parity = OPENAT_UART_NO_PARITY;
            break;

        default:
            goto uart_open_error;
            break;
    }

/*+\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/
    if(PHY_PORT(id) == OPENAT_UART_3)
    {
        uartParam.flowControl = OPENAT_UART_FLOWCONTROL_NONE;
    }
    else
    {
        uartParam.flowControl = OPENAT_UART_FLOWCONTROL_NONE;
    }
/*-\NEW\liweiqiang\2013.8.31\����host uartͨѶ֧��*/

    if(platform_get_console_port() == id)
    {
        uartParam.uartMsgHande = NULL;
    }
    else
    {
        if(mode == 1)
        {
            uartParam.uartMsgHande = NULL;
        }
        else
        {
            uartParam.uartMsgHande = uartmap[id].msg;
        }
    }
	
	/*+\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
    uartParam.txDoneReport = txDoneReport;
	/*-\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/

    if(TRUE == IVTBL(config_uart)(PHY_PORT(id), &uartParam))
    {
        uartContext[id].opened = 1;
        uartContext[id].workmode = mode;
        return baud;
    }
    else
    {
        PUB_TRACE("config_uart : error ");
    }

uart_open_error:
    return 0;
}

/*+\NEW\liweiqiang\2013.4.20\����uart.close�ӿ� */
static u32 uart_phy_close(unsigned id)
{
    u32 ret;

    if(!uartContext[id].opened)
        return PLATFORM_OK;

#if 0
/*+\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
    if(PHY_PORT(id) == uart_port3 && uartContext[id].workmode == 2)
    {
        // host uart����ģʽ����Ҫ�ر�
        return PLATFORM_OK;
    }
/*-\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
#endif    
    ret = IVTBL(close_uart)(PHY_PORT(id)) ? PLATFORM_OK : PLATFORM_ERR;

    uartContext[id].opened = FALSE;

    QueueClean(&uartPhyContext[PHY_PORT(id)].rxqueue);
/*+\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
    uartPhyContext[PHY_PORT(id)].readindex = uartPhyContext[PHY_PORT(id)].readsize = 0;
/*-\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */

    return ret;
}
/*-\NEW\liweiqiang\2013.4.20\����uart.close�ӿ� */
#if 0

static HANDLE g_uartLoop = NULL;
extern UINT8 uartIrqStatus[];
extern UINT32 uartIrqTick[];
extern UINT32 g_uartTxCount;
extern UINT32 g_uartTxIntCount;
extern UINT32 g_uartRxIntCount;

static void uartTimeout( void* p )
{
  UINT8 i;
  for(i = 0; i < 5; i++)
  {
    PUB_TRACE("uartIrq on %d status[%d] = 0x%x, txCount=%d, txIrqCount=%d rxIrqCount = %d", uartIrqTick[i], i, uartIrqStatus[i], g_uartTxCount, 
        g_uartTxIntCount, g_uartRxIntCount);
  }
  OPENAT_start_timer(g_uartLoop, 1000);
}
#endif
static u32 uart_phy_write(u8 id, uint8 *data_p, uint16 length)
{
    if(!uartContext[id].opened)
        return 0;

    /*+\NEW\zhutianhua\2018.12.27 15:28\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
    platform_uart_enable_rs485_oe(id);
    /*-\NEW\zhutianhua\2018.12.27 15:28\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/

#if 0
/*+\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
    if(PHY_PORT(id) == uart_port3 && uartContext[id].workmode == 2)
    {
        //IVTBL(host_send_data)(data_p, length);
        return length;
    }
/*-\NEW\liweiqiang\2014.1.2\host uart ID 0xA2����͸��֧�� */
    

    if(!g_uartLoop)
        {
          g_uartLoop = OPENAT_create_timer(uartTimeout, NULL);
        }
        OPENAT_start_timer(g_uartLoop, 1000);
#endif

    return IVTBL(write_uart)(PHY_PORT(id), data_p, length);
}

static u32 uart_phy_read(u8 id, uint8 *data_p, uint16 length, u32 timeout)
{    
    if(!uartContext[id].opened)
        return 0;

    if(uartContext[id].workmode == 1)
    {
        // �û���ѯ��ʽֱ�Ӵ�uart�ӿڶ�ȡ
        return IVTBL(read_uart)(PHY_PORT(id), data_p, length, timeout);
    }
    else
    {
        // ��Ϣ��ʾ��ʽ:�ӻ��λ�������ȡ
/*+\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
        u32 rcvdlen = 0;
        UartPhyContext *context = &uartPhyContext[PHY_PORT(id)];

        while(rcvdlen < length)
        {
            if(context->readindex >= context->readsize)
            {
            /*+\NEW\liweiqiang\2014.4.12\���Ӵ��ڻ��������ݱ��� */
                context->readsize = QueueDelete(&context->rxqueue, context->readbuf, READ_BUFFER_SIZE);
            /*-\NEW\liweiqiang\2014.4.12\���Ӵ��ڻ��������ݱ��� */

                context->readindex = 0;

                if(context->readsize == 0) break;
            }

            data_p[rcvdlen++] = context->readbuf[context->readindex++];
        }
        
        return rcvdlen;
/*-\NEW\liweiqiang\2014.4.12\�Ż��Դ���uart��ÿ��ֻ��ȡһ���ֽڵĴ��� */
    }
}

/*+\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */
static u32 vatc_read(uint8 *data_p, uint16 length, u32 timeout)
{
    u32 rcvdlen = 0;

    while(rcvdlen < length)
    {
        if(vatc_read_buf_index >= vatc_read_buf_size)
        {
            OPENAT_wait_semaphore(hAtcReadSem, 0);
            vatc_read_buf_size = QueueDelete(&vatcRx_Q, vatc_read_buffer, VATC_READ_BUF_SIZE);
            OPENAT_release_semaphore(hAtcReadSem);
            vatc_read_buf_index = 0;

            if(vatc_read_buf_size == 0) break;
        }

        data_p[rcvdlen++] = vatc_read_buffer[vatc_read_buf_index++];
    }
    
    return rcvdlen;
}
/*-\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */

/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
static u32 usb_read(uint8 *data_p, uint16 length, u32 timeout)
{
    u32 rcvdlen = 0;

    while(rcvdlen < length)
    {
        if(usb_read_buf_index >= usb_read_buf_size)
        {
            OPENAT_wait_semaphore(hUsbReadSem, 0);
            usb_read_buf_size = QueueDelete(&usbRx_Q, usb_read_buffer, VATC_READ_BUF_SIZE);
            OPENAT_release_semaphore(hUsbReadSem);
            usb_read_buf_index = 0;

            if(usb_read_buf_size == 0) break;
        }

        data_p[rcvdlen++] = usb_read_buffer[usb_read_buf_index++];
    }
    
    return rcvdlen;
}
/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */

/*+\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/
static void debugPortFlush(void)
{
    if(debugStdoutCachedCount != 0)
    {
        debugStdoutBuffer[debugStdoutCachedCount] = '\0';
        OPENAT_lua_print(debugStdoutBuffer);
        
        memset(debugStdoutBuffer, 0, sizeof(debugStdoutBuffer));
        debugStdoutCachedCount = 0;
    }
}

static void debugPortWrite(const u8 *buff, u16 len)
{
    u16 i;
    u8 data;

    for(i = 0; i < len; i++)
    {
        data = buff[i];
        
        if(data == '\r' || data == '\n')
        {
            debugPortFlush();
        }
        else
        {
            if(debugStdoutCachedCount < sizeof(debugStdoutBuffer) - 1)
                debugStdoutBuffer[debugStdoutCachedCount++] = data;
            else
            {
                debugPortFlush();
                debugStdoutBuffer[debugStdoutCachedCount++] = data;
            }
        }
    }
}
/*-\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/

u32 vatc_mode = 0;




void RILAPI_ReceiveData(void *data, int len)
{
/*+\NEW\liweiqiang\2013.4.7\�Ż�uart/atc���ݽ�����Ϣ��ʾ,���ⷢ��Ϣ����Ƶ������ϵͳ�޷���Ӧ */
    BOOL needMsg = FALSE; // buffer�ǿյ�ʱ��,�������ݲ���Ҫ����ʾ
    
    OPENAT_wait_semaphore(hAtcReadSem, COS_WAIT_FOREVER);
    needMsg = vatcRx_Q.empty ? TRUE : FALSE;
    QueueInsert(&vatcRx_Q, data, len);
    OPENAT_release_semaphore(hAtcReadSem);

    if(needMsg && (vatc_mode != 1))
    {	
		/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
        sendUartMessage(PLATFORM_UART_ID_ATC,MSG_ID_RTOS_UART_RX_DATA);
		/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
    }
/*-\NEW\liweiqiang\2013.4.7\�Ż�uart/atc���ݽ�����Ϣ��ʾ,���ⷢ��Ϣ����Ƶ������ϵͳ�޷���Ӧ */
}

void platform_setup_vat_queue(void)
{
    QueueClean(&vatcRx_Q);
    hAtcReadSem = OPENAT_create_semaphore(1);
}

/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
u32 Get_usbdata_mode(void)
{
	return usbdata_mode;
}

void USBAPI_ReceiveData(void *data, int len)
{
    BOOL needMsg = FALSE; // buffer�ǿյ�ʱ��,�������ݲ���Ҫ����ʾ
    
    OPENAT_wait_semaphore(hUsbReadSem, COS_WAIT_FOREVER);
    needMsg = usbRx_Q.empty ? TRUE : FALSE;
    QueueInsert(&usbRx_Q, data, len);
    OPENAT_release_semaphore(hUsbReadSem);

    if(needMsg)
    {
		/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
        sendUartMessage(PLATFORM_PORT_ID_USB,MSG_ID_RTOS_UART_RX_DATA);
		/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
    }
}

/**+\BUG3623\zhuwangbin\2020.11.18\uart.on(uart.USB, "receive" ������**/
BOOL USBAPI_IsEmpty(void)
{
	BOOL isEmpty;

	OPENAT_wait_semaphore(hUsbReadSem, COS_WAIT_FOREVER);
	isEmpty = usbRx_Q.empty ? TRUE : FALSE;
	OPENAT_release_semaphore(hUsbReadSem);

	return isEmpty;
}


u32 USBAPI_FreeSpace(void)
{
	u32 space;
	
	OPENAT_wait_semaphore(hUsbReadSem, COS_WAIT_FOREVER);
	space = QueueGetFreeSpace(&usbRx_Q);
	OPENAT_release_semaphore(hUsbReadSem);

	return space;
}
/**-\BUG3623\zhuwangbin\2020.11.18\uart.on(uart.USB, "receive" ������**/

void platform_setup_usb_queue(void)
{
    QueueClean(&usbRx_Q);
    hUsbReadSem = OPENAT_create_semaphore(1);
}
/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */

/*+\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
u32 platform_uart_setup( unsigned id, u32 baud, int databits, int parity, int stopbits, u32 mode, u32 txDoneReport)
/*-\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
{      
    u32 ret = baud;

    if(PLATFORM_UART_ID_ATC == id)
    {
        vatc_mode = mode;
        /* �ڳ�ʼ��vat֮ǰ�ͳ�ʼ��������,����lua���Ƴ�ʼ��,����©��vat������ */
    }
    else if(PLATFORM_PORT_ID_DEBUG == id)
    {   
        memset(debugStdoutBuffer, 0, sizeof(debugStdoutBuffer));
        debugStdoutCachedCount = 0;
    }
	/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
	else if(PLATFORM_PORT_ID_USB == id)
    {   
		usbdata_mode = 1;
    }
	/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
    else
    {
/*+\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
        ret = uart_phy_open(id, baud, databits, parity, stopbits, mode, txDoneReport);
/*-\NEW\zhuwangbin\2018.8.31\��Ӳ����ж��Ƿ��ϱ�UART TXDONE*/
	}

    return ret;
}

/*+\NEW\zhutianhua\2018.12.27 14:54\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
void platform_uart_enable_rs485_oe( unsigned char id)
{
    if(uartContext[id].opened && uartContext[id].rs485ValidLevel != RS485_INVALID_LEVEL)
    {
    /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
       //IVTBL(set_gpio)(platform_pio_get_gpio_port(uartContext[id].rs485Io), uartContext[id].rs485ValidLevel);
       //OPENAT_print("platform_uart_enable_rs485_oe id=%d, realIO=%d, level=%d\n", id, platform_pio_get_gpio_port(uartContext[id].rs485Io), uartContext[id].rs485ValidLevel);
        OPENAT_send_message(rs485_oe_task_hand, enable_rs485_oe, (void *)&id, sizeof(id));
    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
	}
}

void platform_uart_disable_rs485_oe( unsigned char id)
{
    if(uartContext[id].opened && uartContext[id].rs485ValidLevel != RS485_INVALID_LEVEL)
    {
    /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
       //IVTBL(set_gpio)(platform_pio_get_gpio_port(uartContext[id].rs485Io), (uartContext[id].rs485ValidLevel==0) ? 1 : 0);
       //OPENAT_print("platform_uart_disable_rs485_oe id=%d, realIO=%d, level=%d\n", id, platform_pio_get_gpio_port(uartContext[id].rs485Io), (uartContext[id].rs485ValidLevel==0) ? 1 : 0);
        OPENAT_send_message(rs485_oe_task_hand, disable_rs485_oe, (void *)&id, sizeof(id));
    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
    }
}

void platform_uart_close_rs485_oe( unsigned char id)
{
    if(uartContext[id].opened && uartContext[id].rs485ValidLevel != RS485_INVALID_LEVEL)
    {
    /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
        //platform_uart_disable_rs485_oe(id);
        //OPENAT_print("platform_uart_close_rs485_oe id=%d, realIO=%d\n", id, platform_pio_get_gpio_port(uartContext[id].rs485Io));
        //IVTBL(close_gpio)(platform_pio_get_gpio_port(uartContext[id].rs485Io));        
        //uartContext[id].rs485ValidLevel = RS485_INVALID_LEVEL;
        OPENAT_send_message(rs485_oe_task_hand, close_rs485_oe, (void *)&id, sizeof(id));
    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
    }
}

/*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
void rs485_oe_task(void* p)
{
    char *oe_pin = NULL;
	int msgId;
    while(1)
    {
        OPENAT_wait_message(rs485_oe_task_hand, &msgId, &oe_pin, OPENAT_OS_SUSPENDED);
        switch (msgId)
        {
        case enable_rs485_oe:
            IVTBL(set_gpio)(platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io), uartContext[*oe_pin].rs485ValidLevel);
            OPENAT_print("platform_uart_enable_rs485_oe id=%d, realIO=%d, level=%d\n", *oe_pin, platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io), uartContext[*oe_pin].rs485ValidLevel);
            break;
        case disable_rs485_oe:
            // Modbus ͨѶʱ�涨����������һ�����������3.5���ַ��ٷ�����һ����������3.5�ַ���Ҫ�������������豸���������ݣ��ѽ���
            //1*1000*1000��һ����ʱ�䡣
            //(1 * 1000 * 1000 / oe_band)������һ��λ��Ҫ����ms
            //����9600bps����˼����˵ÿ1�루Ҳ����1000���룩����9600��λ��
            //osiDelayUS(4 * 10 * (1 * 1000 * 1000 / uartContext[*oe_pin].uartBand));
            //����ط�����˵Ӧ��д4�����ǵ�������ȹ�����ʵ���Ѿ���ȥһ��ʱ���ˣ�Ϊ����ߴ���Ч�ʡ�����Ӧ�ļ���
            /*+\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/
            if (uartContext[*oe_pin].rs485DelayTime)
            {
				osiDelayUS(uartContext[*oe_pin].rs485DelayTime);
			}
			else
			{
				osiDelayUS(5 * (1 * 1000 * 1000 / uartContext[*oe_pin].uartBand));
            }
			/*-\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/
			IVTBL(set_gpio)(platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io), (uartContext[*oe_pin].rs485ValidLevel==0) ? 1 : 0);
            OPENAT_print("platform_uart_disable_rs485_oe id=%d, realIO=%d, level=%d\n", *oe_pin, platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io), (uartContext[*oe_pin].rs485ValidLevel==0) ? 1 : 0);
            break;
        case close_rs485_oe:
            IVTBL(set_gpio)(platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io), (uartContext[*oe_pin].rs485ValidLevel==0) ? 1 : 0);
            OPENAT_print("platform_uart_close_rs485_oe id=%d, realIO=%d\n", *oe_pin, platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io));
            IVTBL(close_gpio)(platform_pio_get_gpio_port(uartContext[*oe_pin].rs485Io));        
            uartContext[*oe_pin].rs485ValidLevel = RS485_INVALID_LEVEL;
            break;
        default:
            break;
        }
		/*+\bug_3639\rww\2020.11.18\�ڴ�й©����*/
		if (oe_pin != NULL)
		{
			OPENAT_free(oe_pin);
		}
		/*-\bug_3639\rww\2020.11.18\�ڴ�й©����*/
    }
}

/*+\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/
/*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
u32 platform_uart_setup_rs485_oe(unsigned id, u32 rs485IO, u32 rs485ValidLevel, u32 rs485DelayTime)
{      
    u32 ret = PLATFORM_ERR;

    if(id < NUM_UART) //������˿ڲſ�������
    {
        if(uartContext[id].opened)
        {
            T_AMOPENAT_GPIO_CFG cfg;
            cfg.mode = OPENAT_GPIO_OUTPUT;
            cfg.param.defaultState = (rs485ValidLevel==0) ? 1 : 0;
            IVTBL(config_gpio)(platform_pio_get_gpio_port(rs485IO), &cfg);
            
            uartContext[id].rs485Io = rs485IO;
            uartContext[id].rs485ValidLevel = rs485ValidLevel;
			uartContext[id].rs485DelayTime = rs485DelayTime;
            OPENAT_print("platform_uart_setup_rs485_oe id=%d, io=%d, level=%d\n", id, rs485IO, cfg.param.defaultState);
            /*+\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
			if(rs485_oe_task_hand == NULL)
            {
                OPENAT_create_task(&rs485_oe_task_hand, rs485_oe_task, NULL, NULL,
                                            2 * 1024, 30,
                                            0,
                                            0,
                                            "rs485_oe_task");
            }
		    /*-\NEW\czm\2020.9.11\bug:2929 485����1200�����������һ�ֽڴ���*/
            ret = PLATFORM_OK;
        }
    }

    return ret;
}
/*-\NEW\zhutianhua\2018.12.27 14:54\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
/*-\bug4024\zhuwangbin\2020.12.25\uart.set_rs485_oe��ӿ�ѡ����,��������485�ӳ�ʱ��*/

/*+\NEW\liweiqiang\2013.4.20\����uart.close�ӿ� */
u32 platform_uart_close( unsigned id )
{
    u32 ret = PLATFORM_ERR;
    
    if(id < NUM_UART) //������˿ڲſ��Թر�
    {
        /*+\NEW\zhutianhua\2018.12.27 15:8\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
        platform_uart_close_rs485_oe(id);
        /*-\NEW\zhutianhua\2018.12.27 15:8\����uart.set_rs485_oe�ӿڣ�������rs485 ioʹ��*/
        return uart_phy_close(id);
    }
	/*+\BUG\shenyuanyuan\2020.4.10\usb at��Ӧ�ÿ�����at��data���ܹ���֮������л�*/
	else if( id == PLATFORM_PORT_ID_USB)
	{
		usbdata_mode = 0;
		return PLATFORM_OK;
	}
	/*-\BUG\shenyuanyuan\2020.4.10\usb at��Ӧ�ÿ�����at��data���ܹ���֮������л�*/

    return ret;
}
/*-\NEW\liweiqiang\2013.4.20\����uart.close�ӿ� */

u32 platform_s_uart_send( unsigned id, u8 data )
{
    u32 ret = 1;

    
    if(PLATFORM_UART_ID_ATC == id)
    {
        IVTBL(send_at_command)(&data, 1);
    }
    else if(PLATFORM_PORT_ID_DEBUG == id)
    {
/*+\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/
        debugPortWrite(&data, 1);
/*-\NEW\liweiqiang\2013.4.7\�Ż�debug�����*/
    }
    else
    {
        return uart_phy_write(id, &data, 1);
    }

    return ret;
}

/*+\NEW\liweiqiang\2013.4.7\�޸�uart���ݷ���Ϊbuffer��ʽ */
/*+\BUG\wangyuan\2020.04.03\usb���⴮�ڹ��ܲ���ʹ��*/
//#include "at_engine.h"
//extern atDevice_t *gAtDevice;
/*-\BUG\wangyuan\2020.04.03\usb���⴮�ڹ��ܲ���ʹ��*/
u32 platform_s_uart_send_buff( unsigned id, const u8 *buff, u16 len )
{
    u32 ret = len;

    
    if(PLATFORM_UART_ID_ATC == id)
    {
        IVTBL(send_at_command)((UINT8*)buff, len);
    }
    else if(PLATFORM_PORT_ID_DEBUG == id)
    {
        debugPortWrite(buff, len);
    }
	/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
	// else if(PLATFORM_PORT_ID_USB == id)
    // {  
    // 	int    plen = 0;
	// 	char   *pbuf = NULL;
	//     if(buff != NULL && gAtDevice != NULL)
	//     {
    //         /*+\BUG\hedonghao\2019.9.2\ʹ��usbdata��demo��pc�˴��ڹ���ͨ��usbö�ٳ���usb at�ں�ģ��ͨ�ţ����ڹ��߷���һЩ�����ַ���β������(����ټ��ϻس�����)��ģ������������� */
	//         plen = len+3;
	//         pbuf = (char *)malloc(plen * sizeof(char));
	//         ASSERT(pbuf != NULL);
    //         sprintf(pbuf, "%s\r\n",buff);
    //         pbuf[plen - 1] = '\0';
	// 		/*+\BUG\wangyuan\2020.04.03\usb���⴮�ڹ��ܲ���ʹ��*/
	// 		atDeviceWrite(gAtDevice, buff, len);
	// 		/*-\BUG\wangyuan\2020.04.03\usb���⴮�ڹ��ܲ���ʹ��*/
    //         /*-\BUG\hedonghao\2019.9.2\ʹ��usbdata��demo��pc�˴��ڹ���ͨ��usbö�ٳ���usb at�ں�ģ��ͨ�ţ����ڹ��߷���һЩ�����ַ���β������(����ټ��ϻس�����)��ģ������������� */
	// 		free(pbuf);
	// 	}
    // }
	/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
    else
    {
        OPENAT_print("uart phy write:%d %d", id,len);
        return uart_phy_write(id, (uint8 *)buff, len);
    }

    return ret;
}
/*-\NEW\liweiqiang\2013.4.7\�޸�uart���ݷ���Ϊbuffer��ʽ */

/* ���ݾɰ汾��sleep�ӿ� */
void platform_os_sleep(u32 ms)
{
    IVTBL(sleep)(ms);
}

int platform_s_uart_recv( unsigned id, s32 timeout )
{
    uint8 rcvdBuf[1];
    int rcvdLength = 0;
    
    if(PLATFORM_UART_ID_ATC == id)
    {
/*+\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */
        rcvdLength = vatc_read(rcvdBuf, 1, timeout);
/*-\NEW\liweiqiang\2014.4.12\�Ż�������ATÿ��ֻ��ȡһ���ֽڵĴ��� */
    }
	/*+\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
	else if(PLATFORM_PORT_ID_USB == id)
    {
        rcvdLength = usb_read(rcvdBuf, 1, timeout);
    }
	/*-\NEW\shenyuanyuan\2019.5.8\��lua�汾��usb AT�ڸ�Ϊlua�ű��ɿ��Ƶ���ͨ���ݴ���� */
    else if(PLATFORM_PORT_ID_DEBUG == id)
    {
        // debug�ڶ�ȡֱ�Ӷ�������
        HANDLE hDebugPortReadSem = IVTBL(create_semaphore)(0);
        PUB_TRACE("[platform_s_uart_recv]: read from debug port, stop!");
        IVTBL(wait_semaphore)(hDebugPortReadSem, COS_WAIT_FOREVER);
        return -1;
    }
    else
    {
        rcvdLength = uart_phy_read(id, rcvdBuf, 1, timeout);
    }
    
    return rcvdLength ? rcvdBuf[0] : -1;
}

int platform_s_uart_set_flow_control( unsigned id, int type )
{
  return PLATFORM_ERR;
}
/*+\new\wj\2020.11.13\����2G�汾 uart.config����*/
int platform_uart_config_event(int uartId, PLATFORM_UART_EVENT_CONFIG event)
{
	gUartConfig.UartConfigEvent = event;
	gUartConfig.UartConfigId = uartId;
	return 1;
}
/*-\new\wj\2020.11.13\����2G�汾 uart.config����*/
#endif
