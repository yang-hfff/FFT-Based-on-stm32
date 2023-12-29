# STM32F103快速傅里叶变换（基于官方库）

作者：阳海峰

## 文件

- stm32_dsp.h（必选）
- table_fft.h（必选）
- cr4_fft_64_stm32.s（可选）
- cr4_fft_256_stm256.s（可选）
- cr4_fft_256_stm1024.s（可选）

## 函数

- *cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT)*
- *cr4_fft_64_stm32(lBufOutArray, lBufInArray, NPT)*
- *cr4_fft_32_stm32(lBufOutArray, lBufInArray, NPT)*
>参数解释：

**lBufOutArray**：输出数组，32位。==对于小端，低16位存实部，高16位存虚部；对于大端，低16位存虚部，高16位存实部。==

**lBufInArray**：输入数组，32位。==对于小端，低16位存实部，高16位存虚部；对于大端，低16位存虚部，高16位存实部。==

**NPT**：采样点数，==必须是4的整数次幂==，最好是64、256、1024。

## 频谱

> **获取幅度谱**

```C
/*获取幅度谱（双边）*/
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
```

> **获取角度谱**

```C
/*获取角度度谱（双边）*/
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
```

> **注意**：双边与要转化成单边要经过如下处理：

$$
F_{单边}(n) = \begin{cases}
2F_{双边}(n) & n \neq 0 \\
F_{双边}(n) & n = 0 
\end{cases}
$$


## 其他

### 量纲
<<<<<<< HEAD
幅度谱量纲与输入一样，角度谱单位为弧度。
=======
幅度谱量纲与FFT输入一样，角度谱单位为弧度。
>>>>>>> 5069ff2d817eb2907e193ef0c6df96f8935deb85

### 输入范围
输入在16位有符号数范围内。

### 性能

> Complex radix 4,16-bit FFT,coefficients in Flash memory

| 点数 | 24MHZ耗时 |   48MHZ耗时   |   72MHZ耗时   |
| :--: | :--: | :--: | :--: |
| 64 | 0.16ms | 0.093ms | 0.078ms |
| 256 | 0.876ms | 0.52ms | 0.437ms |
| 1024 | 4.174ms | 2.382ms | 2.138ms |

> Complex radix 4,16-bit FFT,coefficients in RAM

| 点数 | 24MHZ耗时 |   48MHZ耗时   |   72MHZ耗时   |
| :--: | :--: | :--: | :--: |
| 64 | 0.16ms | 0.084ms | 0.066ms |
| 256 | 0.876ms | 0.462ms | 0.362ms |
| 1024 | 4.174ms | 2.126ms | 1.768ms |

### 点数

选用*cr4_fft_64_stm32*函数时，NPT低于64性能并不会提升，此库还是按64去算。
选用*cr4_fft_256_stm32*函数时，NPT低于256性能并不会提升，此库还是按256去算。
选用*cr4_fft_1024_stm32*函数时，NPT低于1024性能并不会提升，此库还是按1024去算。

### 频率与索引
先梳理几个概念

|        变量        |                             解释                             |
| :----------------: | :----------------------------------------------------------: |
|    模拟频率$f$     |            每秒经历多少个周期，单位$Hz$，即$1/s$             |
| 模拟角频率$\Omega$ | 每秒经历多少弧度，每个周期弧度为$2\pi$，即$2\pi f$，单位$rad/s$ |
|  数字频率$\omega$  |             每个采样点间隔之间的弧度，单位$rad$              |
|   采样频率$f_s$    |                  每秒对连续信号采样多少个点                  |



>  数字频率与索引

$$
w = \frac {2\pi}{N}n
$$

> 模拟频率与数字频率

$$
\Omega = \omega f_s
$$

