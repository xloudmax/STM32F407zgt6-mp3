#ifndef __ENCODER_H
#define __ENCODER_H
#define INCREMENT_PER_ROTATION  1 // Adjust as needed (e.g., 4 for 4 increments per rotation)

#include "stm32f4xx.h"

// ��ʼ��������
void Encoder_Init(void);

// ��ȡ��������ǰֵ
int32_t Encoder_Get(void);

#endif /* __ENCODER_H */
