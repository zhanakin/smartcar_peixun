/*
 * mycv.c
 *
 *  Created on: 2024��10��6��
 *      Author: zzw
 */


#define WIDTH 188
#define HEIGHT 120

void dilate(const unsigned char image[HEIGHT][WIDTH], const uint8 **kernel, unsigned char output[HEIGHT][WIDTH], uint8 kernel_size)
{
    uint8 kernel_r = kernel_size / 2; // ������������Σ�k�Ǻ˵�һ���С
    uint8 nx,ny,x,y,delta_x,delta_y;
    memset(output, 0, HEIGHT * WIDTH); // ������ͼ��

    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            // �������ɫ����
            if (image[y][x] == 1)
            { // ��ɫ����
                for (delta_y = -kernel_r; delta_y <= kernel_r; delta_y++)
                {
                    for (delta_x = -kernel_r; delta_x <= kernel_r; delta_x++)
                    {
                        ny = y + delta_y;
                        nx = x + delta_x;
                        // ��������Ƿ���ͼ��Χ��
                        if (ny >= 0 && ny < HEIGHT && nx >= 0 && nx < WIDTH)
                        {
                            // ʹ�ýṹԪ�ص�ֵ��ȷ������
                            if (kernel[delta_y + kernel_r][delta_x + kernel_r] == 1)
                            {
                                output[ny][nx] = 1; // ����Ӧλ�ñ��Ϊ��ɫ
                            }
                        }
                    }
                }
            }
        }
    }
}
