/*********************************************************************************************************************
*
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
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


uint32 time_use = 0;
uint16 fps,fps_i;

uint8 slave_flag=0;
uint8 show_flag=1;
void init()
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���

    tft180_init();
    key_init(10);
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 1000);
//    pit_ms_init(CCU61_CH0, 30);

    mt9v03x_init();
    read_params();

//    encoder_quad_init(TIM5_ENCODER, TIM5_ENCODER_CH1_P10_3, TIM5_ENCODER_CH2_P10_1);
    encoder_dir_init(TIM4_ENCODER, TIM4_ENCODER_CH1_P02_8, TIM4_ENCODER_CH2_P00_9);
    encoder_dir_init(TIM6_ENCODER, TIM6_ENCODER_CH1_P20_3, TIM6_ENCODER_CH2_P20_0);
    pwm_init(ATOM1_CH1_P33_9,50,750);
    reinit();
}


int core0_main(void)
{
    // �˴���д�û����� ���������ʼ�������
    init();
    cpu_wait_event_ready();         // �ȴ����к��ĳ�ʼ�����

    tft180_clear();
    tft180_show_string(0, 0, "press 2=slave");
    int enc_temp=encoder_get_count(TIM4_ENCODER);
    int height_temp=0;
	while (TRUE)
	{
        // �˴���д��Ҫѭ��ִ�еĴ���
	    if(key_get_state(1)==KEY_SHORT_PRESS)
        {
            key_clear_all_state();
            slave_flag=1;
            slave();
            slave_flag=0;
            tft180_clear();
//            tft180_show_int(0,96,params[1][0]-params[1][1],5);
//            tft180_show_int(0,126,params[1][2]-params[1][3],5);
            tft180_show_string(0, 16, "press 2=slave");
        }
        else if(key_get_state(2)==KEY_SHORT_PRESS)
        {
            key_clear_all_state();
            width_show();
            show_flag=!show_flag;
        }

//	    if(enc_temp!=encoder_get_count(TIM4_ENCODER))
//        {
//	        enc_temp=encoder_get_count(TIM4_ENCODER);
//	        pwm_set_duty(ATOM1_CH1_P33_9,enc_temp/4);
//	        tft180_show_int(80,116,enc_temp/4,5);
//        }
//        tft180_show_int(0,122,encoder_get_count(TIM4_ENCODER),5);
//        tft180_show_int(0,138,encoder_get_count(TIM6_ENCODER),5);

	    process_one_frame();
	    int8 angle_out=(int8)angle_pid_calc(g_error, 0);
	    servo_output(angle_out);
	    tft180_show_int(100,96,angle_out,2);

        ++fps_i;
        tft180_show_uint(100,80,fps,3);
        tft180_show_string(100,64,"fps");

	    if(slave_flag==1||show_flag==0)
	            continue;
	    tft_show_camera();

//        tft180_show_string(0, 80, "speed1:");
//        tft180_show_string(56, 96, "/10000");
//        tft180_show_string(0, 112, "speed2:");
//        tft180_show_string(56, 128, "/10000");

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
