/*
Description:            时间和日期的相关功能
Author:                 丛海旭
Date:                   2013-12-9
*/
#include "include.h"

sTime  SysDataTime;             //系统日期和时间

/*********************************************************************
函数名称: void Input_DateTime(u8 fuc_Data, u8 fuc_Place, sTime fs_DT)
功    能: 输入时间日期存入结构体
说    明: 
入口参数: fuc_Data = 输入值，fuc_Place = 输入位置，fs_DT = 存入的时间结构体
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-23
修    改:                      时    间: 
*********************************************************************/
void Input_DateTime(u8 fuc_Data, u8 fuc_Place, u8 *p)
{
    if (fuc_Place <= 5)                 //注意sTime结构体中有一个 .day 没有使用，所以大于5以后指针要+1跳过 .day
        p = (p + fuc_Place / 2);
        
    if (fuc_Place % 2 == 0)
    {
        *p &= 0x0F;
        *p |= (fuc_Data << 4);
    }
    else
    {
        *p &= 0xF0;
        *p |= fuc_Data;
    }
}

/*********************************************************************
函数名称: void Key_DateTimeState(void)
功    能: 时间显示按键处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-23
修    改:                      时    间: 
*********************************************************************/
void Key_DateTimeState(void)
{
    sTime ls_Time;
    
    if (gb_SetFlg == true)         //日期时间设置状态
    {
        switch (gu32_KeyCode)       //按键处理
        {
            case K_1:
            case K_2:
            case K_3:
            case K_4:
            case K_5:
            case K_6:
            case K_7:
            case K_8:
            case K_9:
            case K_0: 
                if (gu32_SystemState == TIMESTATE)          //输入新的时间
                    Input_DateTime(VFD_KeyCode_To_Value(gu32_KeyCode), gu8_Item, &SysDataTime.hour);
                else          //输入新的日期
                    Input_DateTime(VFD_KeyCode_To_Value(gu32_KeyCode), gu8_Item, &SysDataTime.year);
                gu8_Item++;
                break;
            case K_RIGHT:
                gu8_Item++;
                break;
            case K_ESC:
                gb_FlashFlg = false;
                gb_SetFlg = false;
                break;
            case K_ENTER:           //判断输入的时间和日期是否符合格式 不符合提示error
                if ((gu32_SystemState == TIMESTATE && RTC_FrameTime(SysDataTime) == true)
                  ||(gu32_SystemState == DATESTATE && RTC_FrameDate(SysDataTime) == true))
                {
                    RTC_WriteDateTime(&SysDataTime);         //写入时间和日期数据
                    gb_FlashFlg = false;
                    gb_SetFlg = false;
                    
                    RTC_ReadDateTime(&ls_Time);      //重新读时间,因为实时时钟芯片可能自调整日期.
                    ls_Time.year = (ls_Time.year >> 4) * 10 + (ls_Time.year & 0x0F);
                    ls_Time.month = (ls_Time.month >> 4) * 10 + (ls_Time.month & 0x0F);
                    ls_Time.date = (ls_Time.date >> 4) * 10 + (ls_Time.date & 0x0F);
                    
                    //更新最后比较日期,防止通过修改系统日期来躲避自动关机.
                    //测试时,可以通过屏蔽下列语句,人为修改系统日期来模拟关机时间到
                    FRAM_WriteDW(FMAddr_LeaveDays, gu32_LeaveDays);
                    FRAM_WriteDW(FMAddr_CmpYear, ls_Time.year);
                    FRAM_WriteDW(FMAddr_CmpMonth, ls_Time.month);
                    FRAM_WriteDW(FMAddr_CmpDate, ls_Time.date); 
                    gb_JudgeShutDownFlg = true;
                }
                else
                {
                    ErrorDeal(0, 100);             //提示错误
                    RTC_ReadDateTime(&SysDataTime);      //读系统日期和时间
                    gu8_Item = 0;
                }
                break;
            default:
                break;
        }
        if (gu8_Item > 5)
            gu8_Item = 0;
    }
    else                        //日期时间显示状态
    {
        RTC_ReadDateTime(&SysDataTime);        //读系统日期和时间
        
        switch (gu32_KeyCode)       //按键处理
        {
            case K_5_5S:
                gu32_KeyCode = 0;
                ulParameter.ul = 0xFFFFFFFA;//为兼容6位和7位密码输入十进制的0xFFFFFFFA最后一位是0，输入6位密码时最高位是0不影响6位密码的数值
                gu8_Item = 0;
                gu8_DataInLength = 0;
                gb_SetFlg = false;
                gb_FirstNumKeyFlg = true;
                gb_FlashFlg = false;
                gu32_SystemState = SHUTDOWNSTATE;
                Key_ShutdownState();
                break;
            case K_ESC:
                gu32_SystemState = STOPSTATE;
                break;
            case K_SET:
                gb_FlashFlg = true;
                gb_SetFlg = true;
                gu8_Item = 0;
                break;
            default:
                break;
        }
    }
}

/*********************************************************************
函数名称: void Disp_DateTimeState(void)
功    能: 时间显示和设置
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-23
修    改:                      时    间: 
*********************************************************************/
void Disp_DateTimeState(void)
{
    if (gu32_SystemState == TIMESTATE)
    {   //系统时间转为ASCII码存入主显示数组
        RTC_sTimetoASC(&SysDataTime, gu8_MainDispArray);
        VFD_AuxDisp_Str("CL ");
    }
    else
    {  //系统日期转为ASCII码存入主显示数组
        RTC_sDatetoASC(&SysDataTime, gu8_MainDispArray);
        VFD_AuxDisp_Str("DA ");
    }
    //ASCII码转为显示段码
    VFD_MainDisp_ASCII(gu8_MainDispArray);
    gu8_MainDispArray[1] |= 0x80;
    gu8_MainDispArray[3] |= 0x80;
    
    Flash_MainDisp(0x20 >> gu8_Item);              //日期时间设置时的位闪烁处理
}




