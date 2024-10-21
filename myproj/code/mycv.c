/*
 * mycv.c
 *
 *  Created on: 2024年10月6日
 *      Author: zzw
 */
#include "zf_common_headfile.h"

uint8 temp_image_1[HEIGHT][WIDTH];
uint8 temp_image_2[HEIGHT][WIDTH];

uint8 (*process_input_image)[WIDTH];
uint8 (*process_output_image)[WIDTH];

uint8 g_thres_value=0;
int8 g_thres_bias=0;

uint8 kernel_3x3[3][3]={{1,1,1},{1,1,1},{1,1,1}};
uint8 kernel_5x5[5][5]={{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1},{1,1,1,1,1}};

uint8 longest_white_num=0;
uint8 longest_white_col_avg=0;

uint8 left_sideline[HEIGHT];
uint8 right_sideline[HEIGHT];
uint8 track_line[HEIGHT];

uint8 foresight_up=0;
uint8 foresight_down=0;
uint8 foresight_up_dy;
uint8 foresight_down_dy;
uint8 line_width[HEIGHT]={
        100,100,100,98,96, 94,92,89,87,85,
        83,82,81,78,76, 75,74,72,71,68,
        66,64,62,60,59, 57,55,53,51,49,
        48,46,44,42,41, 38,37,35,33,31,
        30,28,25,23,21, 20,19,17,15,14
};//50个
float g_error;

void under_sample_25x(const unsigned char image[HEIGHT][WIDTH], unsigned char output[SAMPLED_HEIGHT][SAMPLED_WIDTH])
{
    uint8 nx,ny,x,y,delta_x,delta_y;
    for(y=1;y<188;y+=5)
        for(x=0;x<120;x+=5)
        {
        }
}
void get_binary(uint8 *input_image,uint8 *output_image)
{
    int i;
    for(i=0;i!=HEIGHT*WIDTH;++i)
        output_image[i]=255*g_thres_table[input_image[i]];
}
/*dialate:膨胀白色*/
void dilate(const uint8 image[HEIGHT][WIDTH], uint8 output[HEIGHT][WIDTH], const uint8 *kernel, uint8 kernel_size)
{
    uint8 kernel_r = kernel_size / 2; // 假设核是正方形，k是核的一半大小
    uint8 nx,ny,x,y;
    int8 delta_x,delta_y;
    memset(output, 0, HEIGHT * WIDTH); // 清空输出图像

    for (y = kernel_r; y < HEIGHT-kernel_r; y++)
    {
        for (x = kernel_r; x < WIDTH-kernel_r; x++)
        {
            // 仅处理白色像素
            if (image[y][x] == 255)
            { // 白色像素
//                output[y][x]=255;
                for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
                {
                    for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
                    {
                        ny = y + delta_y;
                        nx = x + delta_x;
                        // 检查索引是否在图像范围内
//                        if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
//                        {
                            // 使用结构元素的值来确定膨胀
                            output[ny][nx] = 255*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]; // 将对应位置标记为白色
//                            output[ny][nx] = 255; // 将对应位置标记为白色
//                        }
                    }
                }
            }
        }
    }
}
/*convolution:卷积*/
uint8 contable[10]={0,0,0,0,0,255,255,255,255,255};
void convolution(const uint8 image[HEIGHT][WIDTH], uint8 output[HEIGHT][WIDTH], const uint8 *kernel, uint8 kernel_size)
{
    uint8 kernel_r = kernel_size / 2; // 假设核是正方形，k是核的一半大小
    int16 nx,ny,x,y;
    int8 delta_x,delta_y;
    int16 sum;
    memset(output, 0, HEIGHT * WIDTH); // 清空输出图像

    for (y = kernel_r; y < HEIGHT-kernel_r; y++)
    {
        for (x = kernel_r; x < WIDTH-kernel_r; x++)
        {

            for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
            {
                for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
                {
                    ny = y + delta_y;
                    nx = x + delta_x;
                    // 使用结构元素的值来确定卷积
                    sum+= 2*image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]-255; // 将对应位置标记为白色
//                    sum+= image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]/255; // 将对应位置标记为白色
                }
            }
            output[y][x] = sum<0?0:255; // 将对应位置标记为白色
//            output[y][x] =contable[sum];
            sum=0;
        }
    }
    for (y = 0; y < HEIGHT; y++)
    {
        x=0;
        for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
        {
            for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
            {
                ny = y + delta_y;
                nx = x + delta_x;
                // 使用结构元素的值来确定卷积
                if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                {
                    sum+= 2*image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]-255; // 将对应位置标记为白色
                }
            }
        }
        output[y][x] = sum<0?0:255; // 将对应位置标记为白色
        sum=0;
        x=WIDTH-1;
        for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
        {
            for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
            {
                ny = y + delta_y;
                nx = x + delta_x;
                // 使用结构元素的值来确定卷积
                if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                {
                    sum+= 2*image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]-255; // 将对应位置标记为白色
                }
            }
        }
        output[y][x] = sum<0?0:255; // 将对应位置标记为白色
        sum=0;
    }
    for (x = kernel_r; x < WIDTH; x++)
    {
        y=0;
        for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
        {
            for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
            {
                ny = y + delta_y;
                nx = x + delta_x;
                // 使用结构元素的值来确定卷积
                if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                {
                    sum+= 2*image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]-255; // 将对应位置标记为白色
                }
            }
        }
        output[y][x] = sum<0?0:255; // 将对应位置标记为白色
        sum=0;
        y=HEIGHT-1;
        for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
        {
            for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
            {
                ny = y + delta_y;
                nx = x + delta_x;
                // 使用结构元素的值来确定卷积
                if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                {
                    sum+= 2*image[ny][nx]*kernel[(delta_y + kernel_r)*kernel_size+delta_x + kernel_r]-255; // 将对应位置标记为白色
                }
            }
        }
        output[y][x] = sum<0?0:255; // 将对应位置标记为白色
        sum=0;
    }
//    for (y = 0; y < HEIGHT; y++)
//    {
//        output[y][0]=image[y][0];
//        output[y][WIDTH-1]=image[y][WIDTH-1];
//    }
//    for (x = kernel_r; x < WIDTH; x++)
//    {
//        output[0][x]=image[0][x];
//        output[HEIGHT-1][x]=image[HEIGHT-1][x];
//    }
}
void find_longest_white(const uint8 image[HEIGHT][WIDTH])
{
    longest_white_num=0;
    uint8 longest_white_count=0;
    uint8 longest_white_cols[WIDTH];

    uint8 white_num;
    uint16 col_sum=0;

    int i,j;
    for(i=0;i!=WIDTH;++i)
    {
        white_num=0;
        for(j=HEIGHT-1;j!=-1;--j)
        {
            if(image[j][i]==BLACK||j==0)
            {
                white_num=HEIGHT-1-j;
                break;
            }
        }
        if(white_num>longest_white_num)
        {
            longest_white_num=white_num;
            longest_white_count=0;
//            memset(longest_white_cols,0,WIDTH);
            longest_white_cols[longest_white_count]=i;
            ++longest_white_count;
        }
        else if(white_num==longest_white_num)
        {
            longest_white_cols[longest_white_count]=i;
            ++longest_white_count;
        }
    }
    for(i=0;i!=longest_white_count;++i)
    {
        col_sum+=longest_white_cols[i];
    }
    longest_white_col_avg=(uint8)(col_sum/longest_white_count);

    foresight_up_dy=min(longest_white_num,foresight_up);
    foresight_down_dy=max(0,foresight_up_dy-10);
    //need:验证avg列的白列长度=最长，否则可能是Y形路
}
void find_sideline(const uint8 image[HEIGHT][WIDTH])
{
    uint8 left_found_num=0;
    uint8 right_found_num=0;
    uint8 l_start_x;
    int i,j;

    l_start_x=longest_white_col_avg;
    for(i=0;i!=longest_white_num;++i)
    {
        //搜左线
        for(j=longest_white_col_avg;j!=-1;--j)
        {
            if(image[HEIGHT-1-i][j]==BLACK||j==0)
            {
                left_sideline[i]=j;
                break;
            }
        }
        //搜右线
        for(j=longest_white_col_avg;j!=WIDTH;++j)
        {
            if(image[HEIGHT-1-i][j]==BLACK||j==WIDTH-1)
            {
                right_sideline[i]=j;
                break;
            }
        }
//        //搜左线
//        for(j=longest_white_col_avg;j!=-1;--j)
//        {
//            if(image[HEIGHT-1-i][j]==BLACK||j==0)
//            {
//                left_sideline[i]=j;
//                break;
//            }
//        }
//        //搜右线
//        for(j=longest_white_col_avg;j!=WIDTH;++j)
//        {
//            if(image[HEIGHT-1-i][j]==BLACK||j==WIDTH-1)
//            {
//                right_sideline[i]=j;
//                break;
//            }
//        }
        //need:1.丢线（左右边界==摄像头边界时的处理）
        //      （若单侧丢线按另一侧+半赛道；都丢线就舍弃行）
        //     2.上一行搜到时，从上一行结果偏右7个单位处开始向左搜（学长算法）
        //
        //
        track_line[i]=(left_sideline[i]+right_sideline[i])/2;
    }
}
float error_calc()
{
    uint8 i;
    float sum=0;
    for(i=foresight_down_dy;i!=foresight_up_dy;++i)
    {
        sum+=WIDTH/2-track_line[i]-1;
    }
    return sum/(foresight_up_dy-foresight_down_dy);
}
void process_one_frame()
{
    //大津法计算阈值
//    g_thres_value=image_fast_otsu()+g_thres_bias;
    g_thres_value=otsuThreshold()+g_thres_bias;
    image_update_thresvalue();
    //计算二值化图像
    process_output_image=temp_image_1;
    get_binary(mt9v03x_image[0],process_output_image);
    //卷积 消噪
//    process_input_image=temp_image_1;
//    process_output_image=temp_image_2;
//    convolution(process_input_image,process_output_image,kernel_3x3[0],3);
    //卷积会把上下边界卷成全黑，原因未知
    find_longest_white(process_output_image);
    find_sideline(process_output_image);

    g_error=error_calc();
//    process_input_image=temp_image_2;
//    process_output_image=temp_image_1;
//    dilate(process_input_image,process_output_image,kernel_5x5[0],5);
//    convolution(dil_output_image,dil_input_image,kernel_3x3[0],3);
//        convolution(dil_input_image,dil_output_image,kernel_5x5[0],5);

}
void tft_show_camera()
{
    tft180_show_gray_image(0,0,mt9v03x_image[0],MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);
//    tft180_show_gray_image(0,80,mt9v03x_image[0],MT9V03X_W,MT9V03X_H,128,80,g_thres_value);
    tft180_show_gray_image(0,HEIGHT,process_output_image[0],MT9V03X_W,MT9V03X_H,MT9V03X_W,MT9V03X_H,0);

    //中心线
    tft180_draw_line(WIDTH/2, HEIGHT, WIDTH/2, 2*HEIGHT-1, RGB565_RED);
    //最长白列
    tft180_draw_line(longest_white_col_avg, 2*HEIGHT-1-longest_white_num, longest_white_col_avg,2*HEIGHT-1, RGB565_BLUE);
    //前瞻行
    tft180_draw_line(0, 2*HEIGHT-1-foresight_up_dy, WIDTH, 2*HEIGHT-1-foresight_up_dy, RGB565_BLUE);
    tft180_draw_line(0, 2*HEIGHT-1-foresight_down_dy, WIDTH, 2*HEIGHT-1-foresight_down_dy, RGB565_BLUE);
    //左右边线
    for(int i=0;i<longest_white_num;++i)
    {
        tft180_draw_point(left_sideline[i], 2*HEIGHT-1-i, RGB565_YELLOW);
        tft180_draw_point(right_sideline[i], 2*HEIGHT-1-i, RGB565_YELLOW);
        tft180_draw_point(track_line[i], 2*HEIGHT-1-i, RGB565_RED);
    }

//    tft180_show_uint(100,112,longest_white_col_avg,3);
//    tft180_show_uint(100,128,longest_white_num,3);
    tft180_show_float(84, 112, g_error, 2, 1);
    tft180_show_uint(100,144,g_thres_value,3);

}
void width_show()
{
    int width1=0,width2=0;
    int height=0;
    int j;
    tft180_clear();
    process_one_frame();
    tft_show_camera();
    uint8 (*image)[WIDTH]=process_output_image;
    while(1)
    {
        if(key_get_state(1)==KEY_SHORT_PRESS)
        {
            key_clear_all_state();
            height++;
            if(height==60)
                height=0;
            for(j=longest_white_col_avg;j!=-1;--j)
            {
                if(image[HEIGHT-1-height][j]==BLACK||j==0)
                {
                    width1=j;
                    break;
                }
            }
            //搜右线
            for(j=longest_white_col_avg;j!=WIDTH;++j)
            {
                if(image[HEIGHT-1-height][j]==BLACK||j==WIDTH-1)
                {
                    width2=j;
                    break;
                }
            }
        tft180_show_int(0, 120, height, 2);
        tft180_show_int(0, 136, width2-width1, 3);
        }
    }
}
