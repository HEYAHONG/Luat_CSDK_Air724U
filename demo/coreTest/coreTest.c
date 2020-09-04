/***************
	demo_hello
****************/

#include "iot_debug.h"
#include "iot_os.h"

#define fortest(fun, num, ms, ...) \
	for (char i = 0; i < num; i++) \
	{                              \
		fun(__VA_ARGS__);          \
		iot_os_sleep(ms);          \
	}

extern bool adcTest(E_AMOPENAT_ADC_CHANNEL channel);
extern void flashTest(UINT32 begain_addr, UINT32 end_addr);
extern void datetimeTest(void);
extern bool fsTest(char *file);
extern void networkTest(void);
extern bool ftpTest(void);
extern bool gsmlocTest(void);
extern void httpTest(void);
extern void pwdTest(void);
extern void RilTest(void);
extern void socketTest(char *mode);
extern void vatTest(void);
extern void zbarTest(void);
extern bool ttsTest(char *text, u32 len);

int appimg_enter(void *param)
{
	networkTest();
	extern bool networkstatus;
	while (networkstatus == FALSE)
		iot_os_sleep(500);
	//�رտ��Ź�����������������Ĭ�ϴ�
	iot_debug_set_fault_mode(OPENAT_FAULT_HANG);
	//�򿪵�����Ϣ��Ĭ�Ϲر�
	iot_vat_send_cmd("AT^TRACECTRL=0,1,3\r\n", sizeof("AT^TRACECTRL=0,1,3\r\n"));
	iot_debug_print("[hello]appimg_enter");
	while (1)
	{
		fortest(zbarTest, 1, 200);
		fortest(adcTest, 1, 200, OPENAT_ADC_2);
		fortest(adcTest, 1, 200, OPENAT_ADC_3);
		fortest(flashTest, 1, 200, 0x320000, 0x330000);
		fortest(datetimeTest, 1, 200);
		fortest(fsTest, 1, 200, "/fs.test");

		//fortest(ftpTest, 0, 1000); //�ظ����Ա�����������Ҳ���ļ�û��ɾ����

		//fortest(gsmlocTest, 1, 200);//��ʱ��ѭ�����Իᵼ������
		fortest(httpTest, 1, 200); //�д�����Ϣ���ҿ�����
		fortest(pwdTest, 1, 200);
		fortest(RilTest, 1, 200);

		fortest(socketTest, 1, 200, "DNS");
		fortest(socketTest, 1, 200, "UDP");
		fortest(socketTest, 1, 200, "TCP");
		//fortest(vatTest, 1, 200);//ѭ�����Լ��ξͻ�����
		//fortest(ttsTest, 1, 200, "1", sizeof("1"));//�����Ӿͻ�����
	}
	return 0;
}

void appimg_exit(void)
{
	iot_debug_print("[hello]appimg_exit");
}
