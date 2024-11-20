#ifndef __ENCODER_H
#define __ENCODER_H
#define INCREMENT_PER_ROTATION  1 // Adjust as needed (e.g., 4 for 4 increments per rotation)

#include "stm32f4xx.h"

// 初始化编码器
void Encoder_Init(void);

// 获取编码器当前值
int32_t Encoder_Get(void);

#endif /* __ENCODER_H */
