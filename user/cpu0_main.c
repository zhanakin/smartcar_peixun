/*********************************************************************************************************************
*
********************************************************************************************************************/
#include "includes.h"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// *************************** ����Ӳ������˵�� ***************************
// ʹ����ɿƼ� Ӣ����TriCore ��������������
//      ֱ�ӽ���������ȷ�����ں��İ�ĵ������ؽӿڼ���
// ʹ�� USB-TTL ģ������
//      ģ��ܽ�            ��Ƭ���ܽ�
//      USB-TTL-RX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_TX_PIN �궨������� Ĭ�� P14_0
//      USB-TTL-TX          �鿴 zf_common_debug.h �ļ��� DEBUG_UART_RX_PIN �궨������� Ĭ�� P14_1
//      USB-TTL-GND         ���İ��Դ�� GND
//      USB-TTL-3V3         ���İ� 3V3 ��Դ


// *************************** ���̲���˵�� ***************************
// 1.���İ���¼��ɱ����̣�����ʹ�ú��İ���������������� USB-TTL ģ�飬�ڶϵ�������������
// 2.���������������� USB-TTL ģ�����ӵ��ԣ�����ϵ�
// 3.������ʹ�ô������ִ򿪶�Ӧ�Ĵ��ڣ����ڲ�����Ϊ zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨�� Ĭ�� 115200�����İ尴�¸�λ����
// 4.�����ڴ��������Ͽ������´�����Ϣ��
//      time start.
//      Timer counted 540 us.
//      time start.
//      Timer counted 65 ms.
//      time start.
//      Timer counted 32700 ms.
// �������������˵�����ز��� ����ձ��ļ����·� ���̳�������˵�� �����Ų�


// **************************** �������� ****************************
#define PWM_CH1                 (ATOM1_CH5_P20_9)
#define PWM_CH2                 (ATOM0_CH7_P20_8)
#define PWM_CH3                 (ATOM0_CH3_P21_5)
#define PWM_CH4                 (ATOM0_CH2_P21_4)

uint32 time_use = 0;
int params[3][8]={0};
void init()
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���

    tft180_init();
    key_init(10);
    pit_ms_init(CCU60_CH0, 10);

    read_params();

    pwm_init(PWM_CH1,params[0][0],params[0][1]);
    pwm_init(ATOM0_CH6_P02_6,params[0][0],params[0][1]);
}


int core0_main(void)
{
    // �˴���д�û����� ���������ʼ�������
    init();
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

//    show_stars();
//    slave();
    tft180_clear();
    tft180_show_string(0, 0, "press 1=showstar");
    tft180_show_string(0, 16, "press 2=slave");
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���
	    if(key_get_state(0)==KEY_SHORT_PRESS)
	    {
	        key_clear_all_state();
	        show_stars();
	        tft180_clear();
            tft180_show_string(0, 0, "press 1=showstar");
            tft180_show_string(0, 16, "press 2=slave");
	    }

	    else if(key_get_state(1)==KEY_SHORT_PRESS)
	    {
	        key_clear_all_state();
	        slave();
            tft180_clear();
            tft180_show_string(0, 0, "press 1=showstar");
            tft180_show_string(0, 16, "press 2=slave");
//	        pwm_init(PWM_CH1,17000,50);
	    }



        // �˴���д��Ҫѭ��ִ�еĴ���
	}
}

#pragma section all restore
// **************************** �������� ****************************

// *************************** ���̳�������˵�� ***************************
// ��������ʱ�밴�������������б���
// ����1������û������
//      �鿴�������ִ򿪵��Ƿ�����ȷ�Ĵ��ڣ����򿪵� COM ���Ƿ��Ӧ���ǵ������������� USB-TTL ģ��� COM ��
//      �����ʹ����ɿƼ� Ӣ����TriCore �������������ӣ���ô������������Ƿ��ɶ��������İ崮�������Ƿ��Ѿ����ӣ��������߲鿴���İ�ԭ��ͼ�����ҵ�
//      �����ʹ�� USB-TTL ģ�����ӣ���ô��������Ƿ������Ƿ��ɶ���ģ�� TX �Ƿ����ӵĺ��İ�� RX��ģ�� RX �Ƿ����ӵĺ��İ�� TX
// ����2��������������
//      �鿴�����������õĲ������Ƿ����������һ�£������� zf_common_debug.h �ļ��� DEBUG_UART_BAUDRATE �궨��Ϊ debug uart ʹ�õĴ��ڲ�����
