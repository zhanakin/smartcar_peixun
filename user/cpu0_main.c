/*********************************************************************************************************************
*
********************************************************************************************************************/
#include "includes.h"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// *************************** 例程硬件连接说明 ***************************
// 使用逐飞科技 英飞凌TriCore 调试下载器连接
//      直接将下载器正确连接在核心板的调试下载接口即可
// 使用 USB-TTL 模块连接
//      模块管脚            单片机管脚
//      USB-TTL-RX          查看 zf_common_debug.h 文件中 DEBUG_UART_TX_PIN 宏定义的引脚 默认 P14_0
//      USB-TTL-TX          查看 zf_common_debug.h 文件中 DEBUG_UART_RX_PIN 宏定义的引脚 默认 P14_1
//      USB-TTL-GND         核心板电源地 GND
//      USB-TTL-3V3         核心板 3V3 电源


// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程，单独使用核心板与调试下载器或者 USB-TTL 模块，在断电情况下完成连接
// 2.将调试下载器或者 USB-TTL 模块连接电脑，完成上电
// 3.电脑上使用串口助手打开对应的串口，串口波特率为 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义 默认 115200，核心板按下复位按键
// 4.可以在串口助手上看到如下串口信息：
//      time start.
//      Timer counted 540 us.
//      time start.
//      Timer counted 65 ms.
//      time start.
//      Timer counted 32700 ms.
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查


// **************************** 代码区域 ****************************
#define PWM_CH1                 (ATOM1_CH5_P20_9)
#define PWM_CH2                 (ATOM0_CH7_P20_8)
#define PWM_CH3                 (ATOM0_CH3_P21_5)
#define PWM_CH4                 (ATOM0_CH2_P21_4)

uint32 time_use = 0;
int params[3][8]={0};
void init()
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口

    tft180_init();
    key_init(10);
    pit_ms_init(CCU60_CH0, 10);

    read_params();

    pwm_init(PWM_CH1,params[0][0],params[0][1]);
    pwm_init(ATOM0_CH6_P02_6,params[0][0],params[0][1]);
}


int core0_main(void)
{
    // 此处编写用户代码 例如外设初始化代码等
    init();
    cpu_wait_event_ready();         // 等待所有核心初始化完毕

//    show_stars();
//    slave();
    tft180_clear();
    tft180_show_string(0, 0, "press 1=showstar");
    tft180_show_string(0, 16, "press 2=slave");
	while (TRUE)
	{
        // 此处编写需要循环执行的代码
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



        // 此处编写需要循环执行的代码
	}
}

#pragma section all restore
// **************************** 代码区域 ****************************

// *************************** 例程常见问题说明 ***************************
// 遇到问题时请按照以下问题检查列表检查
// 问题1：串口没有数据
//      查看串口助手打开的是否是正确的串口，检查打开的 COM 口是否对应的是调试下载器或者 USB-TTL 模块的 COM 口
//      如果是使用逐飞科技 英飞凌TriCore 调试下载器连接，那么检查下载器线是否松动，检查核心板串口跳线是否已经焊接，串口跳线查看核心板原理图即可找到
//      如果是使用 USB-TTL 模块连接，那么检查连线是否正常是否松动，模块 TX 是否连接的核心板的 RX，模块 RX 是否连接的核心板的 TX
// 问题2：串口数据乱码
//      查看串口助手设置的波特率是否与程序设置一致，程序中 zf_common_debug.h 文件中 DEBUG_UART_BAUDRATE 宏定义为 debug uart 使用的串口波特率
