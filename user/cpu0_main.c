/*********************************************************************************************************************
*
********************************************************************************************************************/
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
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


uint32 time_use = 0;
int params[3][8]={0};
uint16 fps,fps_i;

uint8 slave_flag=0;
uint8 show_flag=1;
void init()
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口

    tft180_init();
    key_init(10);
    pit_ms_init(CCU60_CH0, 10);
    pit_ms_init(CCU60_CH1, 1000);
//    pit_ms_init(CCU61_CH0, 30);

    mt9v03x_init();
    read_params();

    reinit();
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
            tft180_show_string(0, 16, "press 2=slave");
	    }

	    else if(key_get_state(1)==KEY_SHORT_PRESS)
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
            slave_flag=!slave_flag;
        }

	    process_one_frame();
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
