/*
 * mycv.c
 *
 *  Created on: 2024年10月6日
 *      Author: zzw
 */


#define WIDTH 188
#define HEIGHT 120

void dilate(const unsigned char image[HEIGHT][WIDTH], const uint8 **kernel, unsigned char output[HEIGHT][WIDTH], uint8 kernel_size)
{
    uint8 kernel_r = kernel_size / 2; // 假设核是正方形，k是核的一半大小
    uint8 nx,ny,x,y,delta_x,delta_y;
    memset(output, 0, HEIGHT * WIDTH); // 清空输出图像

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            // 仅处理白色像素
            if (image[y][x] == 1)
            { // 白色像素
                for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
                {
                    for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
                    {
                        ny = y + delta_y;
                        nx = x + delta_x;
                        // 检查索引是否在图像范围内
                        if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                        {
                            // 使用结构元素的值来确定膨胀
                            if (kernel[delta_y + kernel_r][delta_x + kernel_r] == 1)
                            {
                                output[ny][nx] = 1; // 将对应位置标记为白色
                            }
                        }
                    }
                }
            }
        }
    }
}
