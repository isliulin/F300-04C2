/*
Description:            标定操作相关函数
Author:                 丛海旭
Date:                   2013-12-9
*/
#include "include.h"


#define CAL_CHECKCODE   0
#define DISP_CALON      1
#define CAL_UNIT        2
#define CAL_POINT       3
#define CAL_DIV         4
#define CAL_CAP         5
#define CAL_SENSOR      6
#define CAL_MVOUT       7
#define CAL_ZERO_1      8
#define CAL_ZERO_2      9
#define CAL_LOAD_1      10
#define CAL_LOAD_2      11
#define CAL_LOAD_3      12
#define CAL_INPUTCODE   13
#define DISP_CALEND     14
#define CAL_END         15
//增益校准
#define CAL_CORRECT_1   16
#define CAL_CORRECT_2   17


const u32 cu32_MvMax[4] = {0, 17000, 27000, 37000};

//标定相关参数
struct CALIB gs_Cal;
u32 gu32_CalCode;
u32 gu32_CalLoadAD;
u32 gu32_CalLoadWt;
u32 gu32_DataTemp;

u8 gu8_CalibInTime;
u32 gu32_TimeDelay;             //一般的延时计数器
bool  gb_GainSig;               //标定时的负号标志
u32 gu32_CalDispWt;     //增益校准输入显示重量缓存



/*********************************************************************
函数名称: u32 AdToMv(u32 ful_AdCode)
功    能: ad码转换成毫伏数
说    明: 
入口参数: ful_AdCode = ad码
返 回 值: ad码对应的毫伏数
设    计: 丛海旭               时    间: 2013-10-12
修    改:                      时    间: 
*********************************************************************/
u32 AdToMv(u32 ful_AdCode)
{
    u64 lu64_MvData;
    
    if (gs_Cal.Sensor == 1)
    {
        lu64_MvData = (u64)ful_AdCode * SEN1mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    else if (gs_Cal.Sensor == 3)
    {
        lu64_MvData = (u64)ful_AdCode * SEN3mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    else
    {
        lu64_MvData = (u64)ful_AdCode * SEN2mVMAX + (ADMAX/2);
        lu64_MvData /= ADMAX;
    }
    
    return (u32)lu64_MvData;
}

/*********************************************************************
函数名称: u32 MvToAd(u32 ful_MvData)
功    能: 毫伏数转换成ad码
说    明: 
入口参数: ful_MvData = 毫伏数
返 回 值: 毫伏数对应的ad码
设    计: 丛海旭               时    间: 2011-3-17
修    改:                      时    间: 
*********************************************************************/
u32 MvToAd(u32 ful_MvData)
{
    u64 lu64_AdCode;
    
    if (gs_Cal.Sensor == 1)
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN1mVMAX/2);
        lu64_AdCode /= SEN1mVMAX;
    }
    else if (gs_Cal.Sensor == 3)
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN3mVMAX/2);
        lu64_AdCode /= SEN3mVMAX;
    }
    else
    {
        lu64_AdCode = (u64)ful_MvData * ADMAX + (SEN2mVMAX/2);
        lu64_AdCode /= SEN2mVMAX;
    }
    
    return (u32)lu64_AdCode;
}

/*********************************************************************
函数名称: u8 CalDataInputCheck(void)
功    能: 数据输入合理性检查
说    明: 
入口参数: 
返 回 值: 数据错误类型
设    计: 丛海旭               时    间: 2011-4-18
修    改:                      时    间: 
*********************************************************************/
bool CalDataInputCheck(void)
{
    bool lb_err = false;
    
    switch(gu8_Item)
    {
        case CAL_CAP:               //最大量程不能大于最小分度的100000倍
            if ((ulParameter.ul > gs_Cal.Div * 100000) || (ulParameter.ul < gs_Cal.Div))
                lb_err = true;        //error
            break;
        case CAL_ZERO_1:
//            if (gb_OflFlg == true)    //原判断条件错误 
            if (gb_AdOflFlg == true)    //修复显示OFL不能标定零点的问题 v1.01
                lb_err = true;
            break;
        case CAL_ZERO_2:
            if (ulParameter.ul > 12000 * gs_Cal.Sensor || ulParameter.ul == 0)
                lb_err = true;        //Over
            break;
        case CAL_LOAD_1:
            if (ulParameter.ul == 0 || 
                AdToMv(gu32_AdFedValue) > (gs_Cal.Sensor * 12000 + gs_Cal.Sensor * 600))       //增益毫伏数不能为0，允许传感器5%的误差，（gu32_CalSensor * 12000）*105%
                lb_err = true;
            break;
        case CAL_LOAD_2:
            if (ulParameter.ul == 0 || 
                ulParameter.ul > (gs_Cal.Sensor * 12000 + gs_Cal.Sensor * 600))
                lb_err = true;
            break;
        case CAL_LOAD_3:           //增益标定重量
        case CAL_CORRECT_1:
        case CAL_CORRECT_2:
            if (ulParameter.ul == 0)
                lb_err = true;
            break;
        default:
            break;
    }
    
    return lb_err;
}

/*********************************************************************
函数名称: u32 GetCalibItemValue(u8 fuc_Item)
功    能: 取标定参数值
说    明: 
入口参数: fuc_Item = 标定参数项目
返 回 值: 对应该项目的数值
设    计: 丛海旭               时    间: 2011-1-4
修    改:                      时    间: 
*********************************************************************/
u32 GetCalibItemValue(u8 fuc_Item)
{
    u32 ful_Value = 0;
    
    switch(fuc_Item)
    {
        case CAL_UNIT:
            ful_Value = gs_Cal.Unit;
            break;
        case CAL_POINT:
            ful_Value = gs_Cal.Point;
            break;
        case CAL_DIV:
            ful_Value = gs_Cal.Div;
            break;
        case CAL_CAP:
            ful_Value = gs_Cal.Cap;
            break;
        case CAL_SENSOR:
            ful_Value = gs_Cal.Sensor;
            break;
        case CAL_LOAD_1:
            if(gu32_AdFedValue >= gs_Cal.Zero)
                ful_Value = gu32_AdFedValue - gs_Cal.Zero;
            break;
        case CAL_INPUTCODE:
            ful_Value = 0;
            break;
        case CAL_CORRECT_1:
            gu32_CalDispWt = 0; //清除显示重量输入缓存
            ful_Value = 0;
            break;
        default:
            break;
    }
    
    return ful_Value;
}

/*********************************************************************
函数名称: void Key_CalibState(void)
功    能: 标定处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-12
修    改:                      时    间: 
*********************************************************************/
void Key_CalibState(void)
{
    u8 luc_DataLen = 0;      //输入数据位数
    static u32 su32_MvZeroTemp;     //毫伏数显示时按清零键保存的零点值
    
    gb_GainSig = false;
    
    if (gu32_KeyCode == K_RIGHT)     //→键显示切换到下一项
    {
        if (gu8_Item < DISP_CALEND)
        {
            if (gu8_Item != CAL_CHECKCODE)
                gu8_Item++;
            if (gu8_Item == CAL_ZERO_2)     //跳过手动标零点
                gu8_Item++;
            if (gu8_Item == CAL_LOAD_2)     //跳过手动标增益
                gu8_Item += 2;
            if (gu8_Item == CAL_LOAD_3)     //手动输入增益毫伏数时按右键不能跳到下一项
                gu8_Item--;
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = 0;
            if (gb_FastCalFlg && gu8_Item == CAL_INPUTCODE)
            {   //快速标定跳过密码修改界面 v1.01
                gu8_Item = DISP_CALEND;
            }
        }
        ulParameter.ul = GetCalibItemValue(gu8_Item);
        gb_FirstNumKeyFlg = true;
        gu8_DataInLength = 0;
        gu32_TimeDelay = gu32_Sys10ms;
        if (gu8_Item == DISP_CALEND)
            gu32_TimeDelay = gu32_Sys10ms;
    }
    
    //构造每一项的显示数值
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:             //密码输入
        case CAL_CAP:                   //最大量程
        case CAL_ZERO_2:               //标定零点2，手动输入零点毫伏值
        case CAL_LOAD_2:               //标定增益2，手动输入增益毫伏值
        case CAL_LOAD_3:               //标定增益3，输入砝码重量
        case CAL_CORRECT_1:     //增益校准 输入显示重量
        case CAL_CORRECT_2:     //增益校准 输入实际重量
            luc_DataLen = 6;            //6位数据输入
            break;
        case DISP_CALON:            //显示CAL on一秒后进入标定
            gb_FirstNumKeyFlg = false;
            if (gu32_Sys10ms - gu32_TimeDelay >= 100)
            {
                if (gb_FastCalFlg)
                {   //快速标定直接进入零点标定
                    gu8_Item = CAL_ZERO_1;
                }
                else
                {   //标准标定从单位开始
                    gu8_Item = CAL_UNIT;
                    ulParameter.ul = gs_Cal.Unit;
                }
                gb_FirstNumKeyFlg = true;
            }
            break;
        case CAL_UNIT:                  //标定单位
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //单位选择用↑键修改
            break;
        case CAL_POINT:                 //小数点
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 4);      //小数点用↑键修改
            break;
        case CAL_DIV:                   //最小分度
            if (gu32_KeyCode == K_INC)        //分度值标定用↑键修改
            {
                switch(ulParameter.ul)
                {
                    case 1:
                        ulParameter.ul = 2;
                        break;
                    case 2:
                        ulParameter.ul = 5;
                        break;
                    case 5:
                        ulParameter.ul = 10;
                        break;
                    case 10:
                        ulParameter.ul = 20;
                        break;
                    case 20:
                        ulParameter.ul = 50;
                        break;
                    default:
                        ulParameter.ul = 1;
                        break;
                }
            }
            break;
        case CAL_SENSOR:                //传感器灵敏度
            ulParameter.ul = Key_IncInput(ulParameter.ul, 1, 3);      //传感器灵敏度用↑键修改
            break;
        case CAL_MVOUT:               //毫伏数显示
            if(gu32_AdFedValue >= su32_MvZeroTemp)
            {
                ulParameter.ul = gu32_AdFedValue - su32_MvZeroTemp;
                gb_GainSig = false;
            }
            else
            {
                ulParameter.ul = su32_MvZeroTemp - gu32_AdFedValue;
                gb_GainSig = true;
            }
            break;
        case CAL_LOAD_1:               //增益标定
            if(gu32_AdFedValue >= gs_Cal.Zero)
            {
                ulParameter.ul = gu32_AdFedValue - gs_Cal.Zero;
                gb_GainSig = false;
            }
            else
            {
                ulParameter.ul = gs_Cal.Zero - gu32_AdFedValue;
                gb_GainSig = true;
            }
            break;
        case CAL_INPUTCODE:             //密码设置
//            gb_FirstNumKeyFlg = false;
            if (gu32_KeyCode == K_SET)           //进入密码设置状态
            {
                gb_SetFlg = true;
                gb_FirstNumKeyFlg = true;
            }
            if (gb_SetFlg)
            {
                if (gu32_KeyCode == K_ESC)       //退出密码设置状态
                {
                    gb_SetFlg = false;
                    gu32_KeyCode = 0;
                    gu8_CalibInTime = 0;
                    gu8_DataInLength = 0;
                }
                else
                    luc_DataLen = 6;
            }
            break;
        case DISP_CALEND:               //标定结束
            if (gu32_Sys10ms - gu32_TimeDelay >= 100)
            {
                gu32_SystemState = STOPSTATE;
//                Read_CalibData();
//                FluxInit();     //更新流量单位和小数点
            }
            break;
        default:
            break;
    }
    
    //数据输入
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    
    //每一项下的按键处理程序
    switch(gu32_KeyCode)
    {
        case K_SET:     //进入增益校准
            if (gu8_Item >= CAL_UNIT && gu8_Item <= CAL_LOAD_3)
            {
                gu8_Item = CAL_CORRECT_1;
                ulParameter.ul = GetCalibItemValue(gu8_Item);
            }
            break;
        case K_ZERO:
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = gu32_AdFedValue;
            break;
        case K_INC:             //set↑键处理
            //零点标定1或增益标定1时按set键进入零点标定2或增益标定2，手动输入毫伏数
            if (gu8_Item == CAL_ZERO_1 || gu8_Item == CAL_LOAD_1)
            {
                gu8_Item++;
                ulParameter.ul = 0;
            }
            break;
        case K_ESC:             //退出键处理
            if ((gu8_Item == CAL_MVOUT) && su32_MvZeroTemp != 0)
            {
                su32_MvZeroTemp = 0;
                break;
            }
            if (gu8_Item == CAL_LOAD_2 || gu8_Item == CAL_LOAD_3)        //手动标增益界面按ESC退出到增益毫伏显示状态
                gu8_Item = CAL_LOAD_1;
            else if (gu8_Item == CAL_ZERO_2)
                gu8_Item = CAL_ZERO_1;
            else
            {
                gu8_Item = DISP_CALEND;
                gu32_TimeDelay = gu32_Sys10ms;
            }
            break;
        case K_ENTER:           //确认键处理
            if (CalDataInputCheck())      //输入数据的合理性检查
            {
                ErrorDeal(0, 100);
                ulParameter.ul = GetCalibItemValue(gu8_Item);   //数据错误后返回原值
                return;
            }
            switch(gu8_Item)
            {
                case CAL_CHECKCODE:     //密码输入
                    gu8_CalibInTime++;
                    if ((ulParameter.ul == gu32_CalCode && gu8_DataInLength == 6) || 
                        (ulParameter.ul == SUPERCODE  && gu8_DataInLength == 6))
                    {
                        gu8_CalibInTime = 0;
                        ulParameter.ul = 0;
                    }
                    else if (gu8_CalibInTime >= 3)      //密码输入错误超过3次
                    {
                        ErrorDeal(4, 0);        //显示error4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();
                        VFD_WriteDispRamArry();
                        while(1);
                    }
                    else            //密码输入错误，提示error
                    {
                        gb_FirstNumKeyFlg = true;
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0;
                        gu8_DataInLength = 0;
                        return;
                    }
                    break;
                case CAL_UNIT:
                    if (gs_Cal.Unit != ulParameter.ul)
                    {   //修改单位后清除累计
                        gs_Cal.Unit = ulParameter.ul;
                        FRAM_WriteDW(FMAddr_CalUnit, gs_Cal.Unit);  //保存单位
                        ClrSum();
                    }
                    break;
                case CAL_POINT:
                    if (gs_Cal.Point != ulParameter.ul)
                    {   //修改小数点后清除累计
                        gs_Cal.Point = ulParameter.ul;
                        FRAM_WriteDW(FMAddr_CalPoint, gs_Cal.Point);   //保存小数点
                        ClrSum();
                    }
                    break;
                case CAL_DIV:       //修改分度值后，修改adk 浮动零点
//                    if (gs_Cal.Div != ulParameter.ul)
//                    {
//                        UpdateRecF1(ulParameter.ul);  //分度修改更新配方F1
//                    }
                    gs_Cal.ADK = FRAM_ReadFloat(FMAddr_CalADK);    //读出旧adk
                    gs_Cal.ADK /= (float)gs_Cal.Div;                     //除以分度值
                    gu32_FloatZero *= gs_Cal.Div;
                    gs_Cal.Div = ulParameter.ul;                    //得到新分度值
                    gs_Cal.ADK *= (float)gs_Cal.Div;                     //乘以新adk
                    gu32_FloatZero /= gs_Cal.Div;                  //浮动零点处理
                    FRAM_WriteDW(FMAddr_CalDiv, gs_Cal.Div);        //保存新分度值
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);;    //保存新adk
                    
                    if (gs_Cal.Cap > gs_Cal.Div * 100000)            //最大量程大于分度值的30000倍则修改最大量程
                    {
                        gs_Cal.Cap = gs_Cal.Div * 100000;
                        FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);
                    }
                    break;
                case CAL_CAP:
                    gs_Cal.Cap = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_CalCap, gs_Cal.Cap);    //保存最大量程
                    break;
                case CAL_SENSOR:
                    gs_Cal.Sensor = ulParameter.ul;
                    FRAM_WriteDW(FMAddr_CalSensor, gs_Cal.Sensor);      //保存传感器灵敏度
                    break;
                case CAL_ZERO_1:
                    if (gb_MvStabFlg == false)      //不稳定不能标零点
                        return;
                    gs_Cal.Zero = gu32_AdFedValue;
                    FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);  //保存零点
                    gu32_FloatZero = 0;
                    break;
                case CAL_ZERO_2:
                    gs_Cal.Zero = MvToAd(ulParameter.ul);       //输入毫伏值对应的ad码
                    FRAM_WriteDW(FMAddr_CalZero, gs_Cal.Zero);  //保存零点
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_1:
                    if (gb_GainSig == true || gb_MvStabFlg == false)
                        return;
                    gu32_CalLoadAD = ulParameter.ul;
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_2:
                    gu32_CalLoadAD = MvToAd(ulParameter.ul);       //输入毫伏值对应的ad码
                    gu32_FloatZero = 0;
                    break;
                case CAL_LOAD_3:
                    gu32_CalLoadWt = ulParameter.ul;
                    //         ADc * (2^13)
                    //  ADK = ------------------
                    //         (Wc / dd) * 4
//                    gs_Cal.ADK = RoundOff(gu32_CalLoadAD << 13, gu32_CalLoadWt * 4) * gs_Cal.Div;
                    gs_Cal.ADK = ((float)gu32_CalLoadAD)/((float)gu32_CalLoadWt * 4.0f) * (float)gs_Cal.Div;
                    FRAM_WriteFloat(FMAddr_CalADK, gs_Cal.ADK);    //保存ADK
                    gu32_FloatZero = 0;
                    break;
                case CAL_INPUTCODE:
                    if (gb_SetFlg == true)
                    {
                        gu8_CalibInTime++;
                        if (gu8_CalibInTime >= 2)
                        {
                            if (ulParameter.ul == gu32_DataTemp && gu8_DataInLength == 6)         //两次输入的密码一致
                            {
                                gu8_CalibInTime = 0;
                                gb_SetFlg = false;
                                gu32_CalCode = ulParameter.ul;
                                FRAM_WriteDW(FMAddr_CalCode, gu32_CalCode);          //保存密码
                            }
                            else
                            {
                                gu8_CalibInTime = 0;
                                ErrorDeal(0, 100);
                                gb_FirstNumKeyFlg = true;
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                        else
                        {
                            if (gu8_DataInLength == 6)      //密码输入必须6位
                            {
                                gu32_DataTemp = ulParameter.ul;      //暂存第一次密码设置值
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                            else
                            {
                                gu8_CalibInTime = 0;
                                ErrorDeal(0, 100);
                                gb_FirstNumKeyFlg = true;
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                    }
                    break;
                case CAL_CORRECT_1:     //增益校准 保存输入的显示重量
                    gu32_CalDispWt = ulParameter.ul;
                    gu8_Item++;
                    break;
                case CAL_CORRECT_2:     //根据输入的实际重量和显示重量重新计算ADK
                    gs_Cal.ADK = ((float)gu32_CalDispWt)*gs_Cal.ADK/((float)ulParameter.ul);
                    FRAM_WriteFloat( FMAddr_CalADK, gs_Cal.ADK );    //保存ADK
                    gu32_FloatZero = RoundOff(gu32_FloatZero * ulParameter.ul, gu32_CalDispWt);
                    gu32_CalDispWt = 0;
                    gu8_Item = CAL_INPUTCODE;
                    break;
                default:
                    break;
            }
            gu8_Item++;
            if (gu8_Item == CAL_ZERO_2 || gu8_Item == CAL_LOAD_2)
                gu8_Item++;
            if (gu8_Item >= CAL_END)
                gu8_Item--;
            if (gu8_Item == CAL_MVOUT)
                su32_MvZeroTemp = 0;
            if (gb_FastCalFlg && gu8_Item == CAL_INPUTCODE)
            {   //快速标定跳过密码修改界面 v1.01
                gu8_Item = DISP_CALEND;
            }
            ulParameter.ul = GetCalibItemValue(gu8_Item);
            gu8_DataInLength = 0;
            gb_FirstNumKeyFlg = true;
            if (gu8_Item == DISP_CALEND || gu8_Item == DISP_CALON)  //显示CALON和CALEND时的计时
                gu32_TimeDelay = gu32_Sys10ms;
            break;
        default:
            break;
    }
}

/*********************************************************************
函数名称: void Disp_CalibState(void)
功    能: 标定的显示处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-12
修    改:                      时    间: 
*********************************************************************/
void Disp_CalibState(void)
{
    u32 lu32_Point = 0;
    u32 lu32_Unint = 0;
    bool  lb_Ofl = false;
    
    
    gb_FlashFlg = false;
    
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:         //密码校验
            Disp_InputCode();
            gb_FlashFlg = true;
            break;
        case DISP_CALON:        //进入标定显示CAL on
            VFD_MainDisp_Str("CAL ON");
            break;
        case CAL_UNIT:
            VFD_AuxDisp_Str("-1-");
            break;
        case CAL_POINT:
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-2-");
            break;
        case CAL_DIV:           //标定分度值
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-3-");
            break;
        case CAL_CAP:           //最大量程
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-4-");
            break;
        case CAL_SENSOR:
            lu32_Unint = UNIT_none;
            VFD_AuxDisp_Str("-5-");
            break;
        case CAL_MVOUT:       //毫伏数显示
            lu32_Point = 3;
            ulParameter.ul = AdToMv(ulParameter.ul);
            if (AdToMv(gu32_AdFedValue) >= cu32_MvMax[gs_Cal.Sensor])
                lb_Ofl = true;
            lu32_Unint = UNIT_none;
            VFD_AuxDisp_Str("-6-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            break;
        case CAL_ZERO_1:       //标定零点显示ZEro
            lu32_Point = 3;
            lu32_Unint = UNIT_none;
            lb_Ofl = gb_AdOflFlg;
            ulParameter.ul = AdToMv(gu32_AdFedValue);
            VFD_AuxDisp_Str("-7-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            VFD_FlgDisp_ZERO(true);
            break;
        case CAL_ZERO_2:       //手动标定零点 输入毫伏数
            gb_FlashFlg = true;
            lu32_Point = 3;
            VFD_AuxDisp_Str("-7-");
            break;
        case CAL_LOAD_1:       //标定增益显示LoAd
            lb_Ofl = gb_AdOflFlg;
            lu32_Point = 3;
            lu32_Unint = UNIT_none;
            ulParameter.ul = AdToMv(ulParameter.ul);
            VFD_AuxDisp_Str("-8-");
            VFD_FlgDisp_STAB(gb_MvStabFlg);
            break;
        case CAL_LOAD_2:
            gb_FlashFlg = true;
            lu32_Point = 3;
            VFD_AuxDisp_Str("-8-");
            break;
        case CAL_LOAD_3:
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("-9-");
            break;
        case CAL_INPUTCODE:
            Disp_InputCode();       //密码输入显示处理
            VFD_AuxDisp_Str("PAS");
            if (gb_SetFlg == true)
                gb_FlashFlg = true;
            break;
        case DISP_CALEND:           //标定完成显示CALEnd
            VFD_MainDisp_Str("CALEND");
            break;
        case CAL_CORRECT_1:         //增益校准1输入显示重量
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("C-1");
            break;
        case CAL_CORRECT_2:         //增益校准2输入实际重量
            gb_FlashFlg = true;
            lu32_Point = gs_Cal.Point;
            lu32_Unint = gs_Cal.Unit;
            VFD_AuxDisp_Str("C-2");
            break;
        default:
            break;
    }
    
    switch (gu8_Item)
    {
        case CAL_CHECKCODE:
        case CAL_INPUTCODE:
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        case DISP_CALON:
        case DISP_CALEND:
            break;
        case CAL_UNIT:
            VFD_UnitDisp(ulParameter.ul);            //显示单位
            break;
        case CAL_POINT:
            Disp_Main_Data(0, ulParameter.ul, 6, true, false);               //显示小数点位数
            VFD_UnitDisp(lu32_Unint);                  //单位显示
            break;
        case CAL_ZERO_2:
        case CAL_LOAD_2:
            Disp_Main_Data(ulParameter.ul, lu32_Point, 6, false, false);       //A显示值
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        default:
            Disp_Main_Data(ulParameter.ul, lu32_Point, 6, true, lb_Ofl);       //显示值
            VFD_MainSigDisp(gb_GainSig);          //显示负号
            Flash_MainDisp(0x01);                //闪烁处理
            VFD_UnitDisp(lu32_Unint);                  //单位显示
            break;
    }
}
