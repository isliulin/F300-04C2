#ifndef _BUZZER_H
#define _BUZZER_H


inline void BuzzerOn(void)
{
    GPIO_BitWrite(GPIO1, 0, 1);
}

inline void BuzzerOff(void)
{
    GPIO_BitWrite(GPIO1, 0, 0);
}


extern void Buzzer_Init(void);
extern void Buzzer_PowerOn(void);
extern void BuzzerStart(u32 fu32_HoldTime);
extern void OPBuzzer(void);
extern void BuzzerStart(u32 fu32_HoldTime);
extern void BuzzerS(void);

#endif
