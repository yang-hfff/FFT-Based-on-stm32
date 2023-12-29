#include "sys.h"
#include "delay.h"
#include "usart.h"	
#include "stm32_dsp.h"
#include "table_fft.h"
#include <math.h>

#define NPT     1024
#define PI2     6.28318530717959
#define Fs      44800

uint32_t lBufInArray[NPT];
uint32_t lBufOutArray[NPT];
uint32_t lBufMagArray[NPT];
float lBufAngArray[NPT];

void InitBufInArray()
{
    unsigned short i;
    float fx;
    
    for(i=0; i<NPT; i++)
    {
        fx = i < 10? 32760:0;
//        fx = 1500 * sin(PI2 * i * 350.0 / Fs) +
//        2700 * sin(PI2 * i * 8400.0 / Fs) +
//        4000 * sin(PI2 * i * 18725.0 / Fs);
        lBufInArray[i] = ((signed short)fx);
    }
}


/*
获取幅度谱（双边）
*/
void GetPowerMag(uint32_t *arr_in,uint32_t *arr_out,uint16_t n)
{
    signed short L_16,H_16;
    unsigned short i;
    
    for(i = 0;i < n; i++)
    {
        L_16  = *((signed short *)(arr_in + i));
        H_16  = *((signed short *)(arr_in + i) + 1);
        arr_out[i] = sqrt((float)H_16 * (float)H_16 + (float)L_16 * (float)L_16);
    }
}

/*
获取角度谱（双边）
*/
void GetPowerAng(uint32_t *arr_in,float *arr_out,uint16_t n)
{
    signed short L_16,H_16;
    unsigned short i;
    
    for(i = 0;i < n; i++)
    {
        L_16  = *((signed short *)(arr_in + i));
        H_16  = *((signed short *)(arr_in + i) + 1);
        arr_out[i] = atan2((double)H_16,(double)L_16);
    }
}


int main(void)
{	
    uart_init(115200);
    
    InitBufInArray();
    
    cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT);
    
    GetPowerMag(lBufOutArray,lBufMagArray,NPT);
    GetPowerAng(lBufOutArray,lBufAngArray,NPT);
    
    puts("\r\n幅度谱：\r\n");
    for(int i = 0;i < NPT;i++)
    {
        printf("%d,",lBufMagArray[i]);
    }
    
    puts("\r\n角度谱：\r\n");
    for(int i = 0;i < NPT;i++)
    {
        printf("%f,",lBufAngArray[i]);
    }
    
    
    for(;;);
}







