#ifndef _STOPANDRUN_H
#define _STOPANDRUN_H

#include "71x_type.h"


extern bool gb_FastSetFlg;         //快速设置标志位
extern u32 gu32_FastSetTemp[6];    //快速设置配方参数的暂存
extern bool gb_UpdateRecFlg;       //需要更新配方标志
extern u8 gu8_FastSetSigTemp;      //快速设置修改参数的标志
extern bool gb_FastCalFlg;         //快速标定标志位

extern void Key_StopState(void);
extern void Disp_StopRunState(void);
extern void Key_FastSetState(void);


#endif

