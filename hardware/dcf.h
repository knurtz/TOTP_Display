#ifndef DCF_H
#define DCF_H

void DCF_Init(void);
void DCF_Interrupt(uint32_t events);
bool DCF_DecodeTelegram(void);

#endif /* DCF_H */