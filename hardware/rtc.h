#ifndef RTC_H
#define RTC_H

#include <time.h>

void RTC_Init(void);

uint8_t RTC_ReadRegister(uint8_t addr);

void RTC_WriteRegister(uint8_t addr, uint8_t value);

void RTC_EnableOscillator(bool enabled);

void RTC_GetTimeStruct(struct tm *time);
void RTC_SetTimeStruct(struct tm *time);

#endif /* RTC_H */
