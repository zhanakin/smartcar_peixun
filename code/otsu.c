/*
 * otsu.c
 *
 *  Created on: 2024��10��7��
 *      Author: zzw
 */
#include "zf_common_headfile.h"

#define DOWNSAMPLE_S 255
#define DOWNSAMPLE_X 1
#define DOWNSAMPLE_Y 1
#define DOWNSAMPLE_C 2
#define IMAGE_HEIGHT 120
#define IMAGE_WIDTH 188
#define SEARCH_LEFT 0
#define SEARCH_RIGHT 188
#define g_image mt9v03x_image

//uint8 g_image[IMAGE_HEIGHT][IMAGE_WIDTH];

uint8 g_otsu_enable=1;
uint16 g_thres_min=80;
uint16 g_thres_max=200;
uint8 g_thres_table[256] = {0};

// ���¶�ֵ�����ұ�
void image_update_thresvalue()
{
    static uint8 init_flag = 0;
    static uint8 last_threshold_value = 0;

    if (!init_flag)
    {
        for (uint16 i = 0; i < g_thres_value; i++)
        {
            g_thres_table[i] = 0;
        }
        for (uint16 i = g_thres_value; i < 256; i++)
        {
            g_thres_table[i] = 1;
        }
        init_flag = 1;
    }
    else
    {
        if (last_threshold_value < g_thres_value)
        {
            for (uint16 i = last_threshold_value; i < g_thres_value; i++)
            {
                g_thres_table[i] = 0;
            }
        }
        else
        {
            for (uint16 i = g_thres_value; i < last_threshold_value; i++)
            {
                g_thres_table[i] = 1;
            }
        }
    }

    last_threshold_value = g_thres_value;
}

// ���ڷ���OTSU�����ֵ
uint8 image_fast_otsu()
{
    if (!g_otsu_enable)
    {
        return 0;
    }

    uint16 histogram[DOWNSAMPLE_S] = { 0 }; // ֱ��ͼ
    uint32 histstd[DOWNSAMPLE_S] = { 0 };   // ����

    // ֱ��ͼͳ��
    for (uint16 y = 0; y < IMAGE_HEIGHT; y += DOWNSAMPLE_Y)
    {
        for (uint16 x = SEARCH_LEFT; x <= SEARCH_RIGHT; x += DOWNSAMPLE_X)
        {
            histogram[g_image[y][x] / DOWNSAMPLE_C]++;
        }
    }

    uint16 background_mean = 0;
    uint16 foreground_mean = 0;
    uint32 background_sum = 0;
    uint32 foreground_sum = 0;
    uint16 background_n = 0;
    uint16 foreground_n = 0;
    uint16 i = g_thres_min / DOWNSAMPLE_C;

    // ��ʼ�����ֱ�
    for (uint16 k = 0; k < i; k++)
    {
        background_n += histogram[k];
        background_sum += histogram[k] * k;
    }

    for (uint16 k = i; k < DOWNSAMPLE_S; k++)
    {
        foreground_n += histogram[k];
        foreground_sum += histogram[k] * k;
    }

    // ����������䷽��
    for (; i <= g_thres_max / DOWNSAMPLE_C; i++)
    {
        background_n += histogram[i];
        foreground_n -= histogram[i];

        if (!background_n)
        {
            continue;
        }
        if (!foreground_n)
        {
            break;
        }
        if (!histogram[i])
        {
            histstd[i] = histstd[i - 1];
        }

        background_sum += histogram[i] * i;
        foreground_sum -= histogram[i] * i;

        background_mean = background_sum / background_n;
        foreground_mean = foreground_sum / foreground_n;

        histstd[i] = background_n * foreground_n * (int32)(background_mean - foreground_mean) * (int32)(background_mean - foreground_mean);
    }

    uint32 temp = 0x00;
    uint16 thres = 0;
    uint16 thres_n = 0;

    // Ѱ�ҷ�����С�ĻҶȼ�������ж����ȡƽ��
    for (uint8 i = g_thres_min / DOWNSAMPLE_C; i <= g_thres_max / DOWNSAMPLE_C; i++)
    {
        if (histstd[i])
        {
            if (histstd[i] > temp)
            {
                temp = histstd[i];
                thres = i;
                thres_n = 1;
            }
            else if (histstd[i] == temp)
            {
                thres += i;
                thres_n++;
            }
        }
    }

    static uint16 flag = 0;
    static float last_thres = 0.0;

    // ��ֹ��Ч���
    if (thres)
    {
        // �״ν�����¼�¼��ֵ
        if (!flag)
        {
            last_thres = thres / (float)thres_n;
            flag = 1;
        }

        last_thres = 0.95 * last_thres + 0.05 * thres / (float)thres_n;

        g_thres_value = (uint8)(last_thres * DOWNSAMPLE_C);
    }
    image_update_thresvalue();
    return g_thres_value;
}




uint8 otsuThreshold()
{
    #define GrayScale 256
    int pixelCount[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ���ظ���
    float pixelPro[GrayScale] = {0};//ÿ���Ҷ�ֵ��ռ�����ر���
    int i,j;
    int Sumpix = WIDTH * HEIGHT;   //�����ص�
    uint8 threshold = 0;

    //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
             pixelCount[g_image[i][j]]++;
        }
    }
    float u = 0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / Sumpix;   //����ÿ������������ͼ���еı���
        u += i * pixelPro[i];  //��ƽ���Ҷ�
    }

    float maxVariance=0.0;  //�����䷽��
    float w0 = 0, avgValue  = 0;  //w0 ǰ������ ��avgValue ǰ��ƽ���Ҷ�
    for(i = 0; i < g_thres_max; i++)     //ÿһ��ѭ������һ��������䷽����� (����for����Ϊ1��)
    {
        w0 += pixelPro[i];  //���赱ǰ�Ҷ�iΪ��ֵ, 0~i �Ҷ�������ռ����ͼ��ı�����ǰ������
        avgValue  += i * pixelPro[i];

        float variance = pow((avgValue/w0 - u), 2) * w0 /(1 - w0);    //��䷽��
        if(variance > maxVariance)
        {
            maxVariance = variance;
            threshold = i;
        }
    }


    return threshold;

}


