#ifndef TOTP_H
#define TOTP_H

void TOTP_Init(void);

bool TOTP_ReadKeyFromFile(char* filename);

uint32_t TOTP_GetToken(uint timestamp);
uint32_t TOTP_GetCurrentToken(void);

#endif /* TOTP_H */
