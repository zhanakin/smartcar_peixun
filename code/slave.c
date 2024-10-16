#include "zf_common_headfile.h"

#define PWM_CH1                 (ATOM1_CH5_P20_9)
#define PWM_CH2                 (ATOM0_CH7_P20_8)
#define PWM_CH3                 (ATOM0_CH3_P21_5)
#define PWM_CH4                 (ATOM0_CH2_P21_4)
#define num_in_onepage 6

extern int params[3][8];

static int8_t key_i,page=0,param_i=0,param_page=0,pos=4;

char param_name[3][8][9]={
        {"pwm_freq",
        "pwm_duty",
        "exp_time",
        "th_bias",
        "sight_up",
        "sight_dn",
        "",
        ""},
        {"duty21.2",
        "duty21.3",
        "duty21.4",
        "duty21.5",
        "param1",
        "param1",
        "",
        ""},
        {"param2",
        "param2",
        "param2",
        "param2",
        "param2",
        "param2",
        "",
        ""}
};
//extern key_state_enum key_state[KEY_NUMBER];
void reinit()
{
    pwm_init(MOTOR1_PWM_CH1,params[0][0],params[1][0]);
    pwm_init(MOTOR1_PWM_CH2,params[0][0],params[1][1]);
    pwm_init(MOTOR2_PWM_CH1,params[0][0],params[1][2]);
    pwm_init(MOTOR2_PWM_CH2,params[0][0],params[1][3]);

    mt9v03x_set_exposure_time(params[0][2]);
    g_thres_bias=params[0][3];
    foresight_up=params[0][4];
    foresight_down=params[0][5];
}
int numlen(int n)
{
    int ret=1;
    n/=10;
    while(n!=0)
    {
        n/=10;
        ++ret;
    }
    return ret;
}
int my_pow(int i)
{
    int ret=1;
    while(i!=0)
    {
        ret*=10;
        --i;
    }
    return ret;
}
/*page1=选参数；page2=修改
* page1:长按3=进入page2，34=换页，12=选参数，长按4=退出
* page2:34=选位数，12=改，长按4=回page1*/
/*有五向按键之后：page1=选参数；page2=修改
 *               page1:中=进入page2，左右=换页，上下=选参数，长按中=退出
 *               page2:左右=选位数，上下=改，中=回page1*/
void slave_show()
{
    int i;
    tft180_clear();
    if(page==0)
    {
        tft180_show_string(0, 0, "params:");
        for(i=0;i!=num_in_onepage;++i)
        {
            tft180_show_string(10, 20*i+16, param_name[param_page][i]);
            if(params[param_page][i]<0)
            {
                tft180_show_string(80, 20*i+16, "-");
                tft180_show_uint(88, 20*i+16, -params[param_page][i], 5);
            }
            else
                tft180_show_uint(88, 20*i+16, params[param_page][i], 5);
        }
        tft180_show_string(0, 20*param_i+16, ">");
        tft180_show_string(40, 144, "page");
        tft180_show_int(80, 144, param_page+1, 1);
    }
    else if(page==1)
    {
        int len=numlen(params[param_page][param_i]);
        tft180_show_string(0, 0, "params:");
        tft180_show_string(8*(pos+1)+80, 20*param_i+32, "^");
        for(i=0;i!=num_in_onepage;++i)
        {
            if(i!=param_i)
            {
                tft180_show_string(10, 20*i+16, param_name[param_page][i]);
                if(params[param_page][i]<0)
                {
                    tft180_show_string(80, 20*i+16, "-");
                    tft180_show_uint(88, 20*i+16, -params[param_page][i], 5);
                }
                else
                    tft180_show_uint(88, 20*i+16, params[param_page][i], 5);
            }
        }
        tft180_show_string(0, 20*param_i+16, ">");
        tft180_show_string(40, 144, "page");
        tft180_show_int(80, 144, param_page+1, 1);

        tft180_show_string(10, 20*param_i+16, param_name[param_page][param_i]);
        for(i=0;i!=5-len;++i)
        {
            tft180_show_int(8*(i+1)+80, 20*param_i+16, 0,1);
        }
        if(params[param_page][param_i]<0)
        {
            tft180_show_string(80, 20*param_i+16, "-");
            tft180_show_uint(8*(6-len)+80, 20*param_i+16, -params[param_page][param_i], len);
        }
        else
            tft180_show_uint(8*(6-len)+80, 20*param_i+16, params[param_page][param_i], len);
//        tft180_show_string(0, 0, param_name[param_page][param_i]);
//        for(i=0;i!=5-len;++i)
//        {
//            tft180_show_int(8*(i+1), 40, 0,1);
//        }
//        if(params[param_page][param_i]<0)
//        {
//            tft180_show_string(0, 40, "-");
//            tft180_show_int(8*(6-len), 40, -params[param_page][param_i], 5);
//        }
//        else
//            tft180_show_int(8*(6-len), 40, params[param_page][param_i], 5);
//        tft180_show_string(8*(pos+1), 56, "^");
//        tft180_show_int(0, 80, pos, 5);
    }

}
void read_params()
{
    flash_buffer_clear();
    flash_read_page_to_buffer(0, 11);
    for(int j=0;j!=3;++j)
    {
        for(int i=0;i!=8;++i)
        {
            params[j][i]=flash_union_buffer[j*8+i].int32_type;
        }
    }
}
void write_params()
{
    flash_erase_page(0,11);
    flash_buffer_clear();
    for(int j=0;j!=3;++j)
    {
        for(int i=0;i!=8;++i)
        {
            flash_union_buffer[j*8+i].int32_type=params[j][i];
        }
    }
    flash_write_page_from_buffer(0, 11);
    flash_buffer_clear();
}
void slave()
{
    //page0:所有参数 page1:修改参数
    page=0;
    param_i=0;
    param_page=0;
    pos=0;
    read_params();
    slave_show();

    while(1)
    {
        for(key_i=0;key_i!=KEY_NUMBER;++key_i)
            if(key_get_state(key_i)!=KEY_RELEASE)
            {
                //key0,1:上下 key2,3:左右 长按 key2:下一个页面 key3:上一个页面
                switch(key_i)
                {
                case 0:
                    if(key_get_state(key_i)==KEY_SHORT_PRESS)
                    {
                        if(page==0)
                        {
                            if(param_i==0)
                                param_i=num_in_onepage-1;
                            else
                                --param_i;
                        }
                        else if(page==1)
                        {
//                            if((params[param_page][param_i]/my_pow(4-pos))%10==9)
//                                params[param_page][param_i]-=9*my_pow(4-pos);
//                            else
                            params[param_page][param_i]+=my_pow(4-pos);
                        }
                    }
                    else
                    {
                        if(param_page==0)
                            param_page=2;
                        else
                            --param_page;
                    }
                    break;
                case 1:
                    if(key_get_state(key_i)==KEY_SHORT_PRESS)
                    {
                        if(page==0)
                        {
                            if(param_i==num_in_onepage-1)
                                param_i=0;
                            else
                                ++param_i;
                        }
                        else if(page==1)
                        {
//                            if((params[param_page][param_i]/my_pow(4-pos))%10==0)
//                                params[param_page][param_i]+=9*my_pow(4-pos);
//                            else
                                params[param_page][param_i]-=my_pow(4-pos);
                        }
                    }
                    else
                    {
                        if(param_page==2)
                            param_page=0;
                        else
                            ++param_page;
                    }
                    break;
                case 2:
//                    if(key_get_state(key_i)==KEY_LONG_PRESS)
//                    {
//                        if(page==0)
//                        {
//                            page=1;
//                            pos=4;
//                        }
//
//                    }
//                    else
//                    {
//                        if(pos==4)
//                            pos=0;
//                        else
//                            ++pos;
//                    }
                    if(key_get_state(key_i)==KEY_SHORT_PRESS)
                    {
                        if(page==0)
                        {
                            page=1;
                            pos=4;
                        }
                        else if(page==1)
                        {
                            if(pos==4)
                                pos=0;
                            else
                                ++pos;
                        }
                    }
                    break;
                case 3:
                    if(key_get_state(key_i)==KEY_LONG_PRESS)
                    {

                        if(page==0)
                        {
                            write_params();
                            tft180_clear();
                            tft180_show_string(0, 0, "save and exit!");
                            reinit();
                            system_delay_ms(500);
                            return;
                        }
//                        while(key_get_state(key_i)==KEY_LONG_PRESS);
                    }
                    else
                    {
//                        if(pos==0)
//                            pos=4;
//                        else
//                            --pos;
                        if(page==1)
                        {
                            write_params();
                            reinit();
                            page=0;
                        }
                    }

                    break;
                }
                key_clear_all_state();
                slave_show();
                while(key_get_state(key_i)!=KEY_RELEASE);
            }
    }
}
void show_stars()
{
    int i,j,n;
    tft180_clear();
//    n=0;

    flash_buffer_clear();
    flash_read_page_to_buffer(0, 10);
    n=flash_union_buffer[0].int32_type;
    if(n>159||n<0)
        n=0;
//    n=0;
    for(i=0;i!=n+1;++i)
    {
        tft180_show_string(i%16*8,i/16*16, "*");
    }
    while(1)
    {
        if(key_get_state(3)==KEY_SHORT_PRESS)
        {
            key_clear_all_state();
            while(1)
            {
                if(key_get_state(3)==KEY_SHORT_PRESS)
                {
                    key_clear_all_state();
                    break;
                }
                if(key_get_state(0)==KEY_SHORT_PRESS)
                {
                    --n;
                    key_clear_all_state();
                    tft180_clear();
                    for(i=0;i!=n;++i)
                    {
                        tft180_show_string(i%16*8,i/16*16, "*");
                    }
                    tft180_show_string(50,144, "  ");
                    tft180_show_int(50,144, n,3);
                }
                if(key_get_state(1)==KEY_SHORT_PRESS)
                {
                    ++n;
                    key_clear_all_state();
                    tft180_clear();
                    for(i=0;i!=n;++i)
                    {
                        tft180_show_string(i%16*8,i/16*16, "*");
                    }
                    tft180_show_string(50,144, "  ");
                    tft180_show_int(50,144, n,3);
                }
                if(key_get_state(2)==KEY_SHORT_PRESS)
                {
                    key_clear_all_state();

                    flash_erase_page(0,10);
                    flash_buffer_clear();
                    flash_union_buffer[0].int32_type=n;

                    flash_write_page_from_buffer(0, 10);
                    flash_buffer_clear();
                    return;
                }
            }
        }
//        for(i=0;i!=n+1;++i)
//        {
//            tft180_show_string(i%16*8,i/16*16, "*");
//        }
        tft180_show_string(n%16*8,n/16*16, "*");
        ++n;
        if(n==160)
        {
            tft180_clear();
            n=0;
        }
        tft180_show_string(50,144, "  ");
        tft180_show_int(50,144, n,3);
        system_delay_ms(100);
    }
    flash_erase_page(0,10);
    flash_buffer_clear();
    flash_union_buffer[0].int32_type=n;

    flash_write_page_from_buffer(0, 10);
    flash_buffer_clear();
}
