/*
Description:            工作参数设置相关函数
Author:                 丛海旭
Date:                   2013-12-9
*/
#include "include.h"


#define SETUP_CHECKCODE     0           //基本工作参数密码校验
#define DISP_SETUP          1           //显示"SEtUP"
#define SETUP_B1            2           //Mode 秤体工作模式相关参数
#define SETUP_B1_1          3           //秤体模式
#define SETUP_B1_2          4           //无斗毛净重
#define SETUP_B1_3          5           //给料方式
#define SETUP_B1_4          6           //自动清零间隔  
#define SETUP_B1_5          7           //判稳超时时间
#define SETUP_B1_6          8           //副显示内容
#define SETUP_B2            9           //Base 秤体基本参数设置
#define SETUP_B2_1          10          //清零范围      
#define SETUP_B2_2          11          //判稳范围      
#define SETUP_B2_3          12          //零点跟踪范围  
#define SETUP_B2_4          13          //AD数字滤波参数 
#define SETUP_B2_5          14          //卸料滤波参数   
#define SETUP_B2_6          15          //定值滤波参数   
#define SETUP_B2_7          16          //AD采样次数
#define SETUP_B2_8          17          //扩展接口类型 
#define SETUP_B3            18          //Switch 功能开关参数        
#define SETUP_B3_1          19          //上电自动清零开关      
#define SETUP_B3_2          20          //手动卸料累计开关      
#define SETUP_B3_3          21          //定值重量保持开关      
#define SETUP_B3_4          22          //累计查询密码开关      
#define SETUP_B3_5          23          //累计清除密码开关      
#define SETUP_B3_6          24          //累计范围扩展开关
#define SETUP_B4            25          //Et1 扩展口1参数       
#define SETUP_B4_1          26          //通讯方式              
#define SETUP_B4_2          27          //设备号                
#define SETUP_B4_3          28          //波特率                
#define SETUP_B4_4          29          //数据格式              
#define SETUP_B4_5          30          //modbus高低字顺序      
#define SETUP_B4_6          31          //自动打印开关          
#define SETUP_B4_7          32          //打印格式
#define SETUP_B4_8          33          //打印语言  
#define SETUP_B4_9          34          //打印走纸行数
#define SETUP_B5            35          //Et2 扩展口2参数       
#define SETUP_B5_1          36          //通讯方式              
#define SETUP_B5_2          37          //设备号                
#define SETUP_B5_3          38          //波特率                
#define SETUP_B5_4          39          //数据格式              
#define SETUP_B5_5          40          //modbus高低字顺序      
#define SETUP_B5_6          41          //自动打印开关          
#define SETUP_B5_7          42          //打印格式
#define SETUP_B5_8          43          //打印语言  
#define SETUP_B5_9          44          //打印语言  
#define SETUP_B6            45          //no2 双无斗互锁模式相关参数
#define SETUP_B6_1          46          //双无斗秤互锁模式同时松袋开关
#define SETUP_B6_2          47          //输送机启动延时时间
#define SETUP_B6_3          48          //输送机运行时间
#define SETUP_B6_4          49          //再次夹袋延时启动加料时间
#define SETUP_B7            50          //自适应开关
#define SETUP_B7_1          51          //中投自适应开关
#define SETUP_B7_2          52          //中投修正最大门限 %
#define SETUP_B7_3          53          //中投修正等级 1~8
#define SETUP_B7_4          54          //中投修正时间分布范围
#define SETUP_B7_5          55          //小投自适应开关
#define SETUP_B7_6          56          //小投修正最大门限 %
#define SETUP_B7_7          57          //小投修正幅度 1~3
#define SETUP_B7_8          58          //
#define SETUP_B7_9          59          //
#define SETUP_B8            60          //工作参数密码开关      
#define SETUP_B8_1          61          //工作参数密码          
#define SETUP_END           62




struct SETUP gs_Setup;

u32 gu32_SetupCodeSwitch;              //工作参数密码开关
u32 gu32_SetupCode;                    //工作参数密码

u8 gu8_SetupInTime = 0;                //工作参数密码输入错误次数


const u16 gu16_SetupRealItem[]=       //用来显示参数下角标
//密码输入 setup     
  {0,      1,        
// B1   B1.1 B1.2 B1.3 B1.4 B1.5 B1.6
   10,  11,  12,  13,  14,  15,  16, 
// B2   B2.1  B2.2  B2.3  B2.4  B2.5  B2.6  B2.7  B2.8
   20,  21,   22 ,  23,   24,   25,   26,   27,   28,
// B3   B3.1  B3.2  B3.3  B3.4  B3.5  B3.6
   30,  31,   32,   33,   34,   35,   36, 
// B4   B4.1  B4.2  B4.3  B4.4  B4.5  B4.6  B4.7  B4.8  B4.9
   40,  41,   42,   43,   44,   45,   46,   47,   48,   49,
// B5   B5.1  B5.2  B5.3  B5.4  B5.5  B5.6  B5.7  B5.8  B5.9
   50,  51,   52,   53,   54,   55,   56,   57,   58,   59,
// B6   B6.1  B6.2  B6.3  B6.4
   60,  61,   62,   63,   64,
// B7   B7.1  B7.2  B7.3  B7.4  B7.5  B7.6  B7.7  B7.8  B7.9
   70,  71,   72,   73,   74,   75,   76,   77,   78,   79,
// B8   B8.1
   80,  81};


const u8 gu8_SetupRightItem[] =        //按→键获得下一参数
//密码输入 setup     
  {0,      SETUP_B1, 
// B1        B1.1        B1.2        B1.3        B1.4        B1.5        B1.6       
   SETUP_B2, SETUP_B1_2, SETUP_B1_3, SETUP_B1_4, SETUP_B1_5, SETUP_B1_6, SETUP_B1_1,
// B2        B2.1        B2.2        B2.3        B2.4        B2.5        B2.6        B2.7        B2.8      
   SETUP_B3, SETUP_B2_2, SETUP_B2_3, SETUP_B2_4, SETUP_B2_5, SETUP_B2_6, SETUP_B2_7, SETUP_B2_1, SETUP_B2_1,
// B3        B3.1        B3.2        B3.3        B3.4        B3.5        B3.6       
   SETUP_B4, SETUP_B3_2, SETUP_B3_3, SETUP_B3_4, SETUP_B3_5, SETUP_B3_6, SETUP_B3_1,
// B4        B4.1        B4.2        B4.3        B4.4        B4.5        B4.6        B4.7        B4.8        B4.9
   SETUP_B5, SETUP_B4_2, SETUP_B4_3, SETUP_B4_4, SETUP_B4_5, SETUP_B4_6, SETUP_B4_7, SETUP_B4_8, SETUP_B4_9, SETUP_B4_1,
// B5        B5.1        B5.2        B5.3        B5.4        B5.5        B5.6        B5.7        B5.8        B5.9
   SETUP_B6, SETUP_B5_2, SETUP_B5_3, SETUP_B5_4, SETUP_B5_5, SETUP_B5_6, SETUP_B5_7, SETUP_B5_8, SETUP_B5_9, SETUP_B5_1,
// B6        B6.1        B6.2        B6.3        B6.4
   SETUP_B8, SETUP_B6_2, SETUP_B6_3, SETUP_B6_4, SETUP_B6_1,
// B7        B7.1        B7.2        B7.3        B7.4        B7.5        B7.6        B7.7        B7.8        B7.9
   SETUP_B8, SETUP_B7_2, SETUP_B7_3, SETUP_B7_4, SETUP_B7_5, SETUP_B7_6, SETUP_B7_7, SETUP_B7_8, SETUP_B7_9, SETUP_B7_1,
// B8        B8.1
   SETUP_B1, SETUP_B8_1};
   
const u8 gu8_SetupEnterItem[] =        //按ENTER键获得下一参数
//密码输入     setup     
  {DISP_SETUP, SETUP_B1, 
// B1          B1.1        B1.2        B1.3        B1.4        B1.5        B1.6       
   SETUP_B1_1, SETUP_B1_2, SETUP_B1_3, SETUP_B1_4, SETUP_B1_5, SETUP_B1_6, SETUP_B1_1,
// B2          B2.1        B2.2        B2.3        B2.4        B2.5        B2.6        B2.7        B2.8      
   SETUP_B2_1, SETUP_B2_2, SETUP_B2_3, SETUP_B2_4, SETUP_B2_5, SETUP_B2_6, SETUP_B2_7, SETUP_B2_1, SETUP_B2_1,
// B3          B3.1        B3.2        B3.3        B3.4        B3.5        B3.6       
   SETUP_B3_1, SETUP_B3_2, SETUP_B3_3, SETUP_B3_4, SETUP_B3_5, SETUP_B3_6, SETUP_B3_1,
// B4          B4.1        B4.2        B4.3        B4.4        B4.5        B4.6        B4.7        B4.8        B4.9       
   SETUP_B4_1, SETUP_B4_2, SETUP_B4_3, SETUP_B4_4, SETUP_B4_5, SETUP_B4_6, SETUP_B4_7, SETUP_B4_8, SETUP_B4_9, SETUP_B4_1,
// B5          B5.1        B5.2        B5.3        B5.4        B5.5        B5.6        B5.7        B5.8        B5.9       
   SETUP_B5_1, SETUP_B5_2, SETUP_B5_3, SETUP_B5_4, SETUP_B5_5, SETUP_B5_6, SETUP_B5_7, SETUP_B5_8, SETUP_B5_9, SETUP_B5_1,
// B6          B6.1        B6.2        B6.3        B6.4
   SETUP_B6_1, SETUP_B6_2, SETUP_B6_3, SETUP_B6_4, SETUP_B6_1,
// B7          B7.1        B7.2        B7.3        B7.4        B7.5        B7.6        B7.7        B7.8        B7.9
   SETUP_B7_1, SETUP_B7_2, SETUP_B7_3, SETUP_B7_4, SETUP_B7_5, SETUP_B7_6, SETUP_B7_7, SETUP_B7_8, SETUP_B7_9, SETUP_B7_1,
// B8          B8.1
   SETUP_B8_1, SETUP_B8};



/*********************************************************************
函数名称: u32 GetSetupItemValue(u8 fuc_Item)
功    能: 得到对应参数的参数值
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2011-3-11
修    改:                      时    间: 
*********************************************************************/
u32 GetSetupItemValue(u8 fuc_Item)
{
    u32 ful_Value;
    
    switch(fuc_Item)
    {
        case SETUP_B1_1:
            ful_Value = gs_Setup.TareMode; break;
        case SETUP_B1_2:
            ful_Value = gs_Setup.GorNMode; break;
        case SETUP_B1_3:
            ful_Value = gs_Setup.FeedMode; break;
        case SETUP_B1_4:
            ful_Value = gs_Setup.AutoClrZeroInterval; break;
        case SETUP_B1_5:
            ful_Value = gs_Setup.JudgeStabOverTime; break;
        case SETUP_B1_6:
            ful_Value = gs_Setup.AuxDispType; break;
        case SETUP_B2_1:
            ful_Value = gs_Setup.ClrZeroRng; break;
        case SETUP_B2_2:
            ful_Value = gs_Setup.JudgeStabRng; break;
        case SETUP_B2_3:
            ful_Value = gs_Setup.TraceZeroRng; break;
        case SETUP_B2_4:
            ful_Value = gs_Setup.AdFilterLevel; break;
        case SETUP_B2_5:
            ful_Value = gs_Setup.AdFilterLevelDisc; break;
        case SETUP_B2_6:
            ful_Value = gs_Setup.AdFilterLevelComp; break;
        case SETUP_B2_7:
            ful_Value = gs_Setup.AdSamplingRate; break;
        case SETUP_B2_8:
            ful_Value = gs_Setup.ExtType; break;
        case SETUP_B3_1:
            ful_Value = gs_Setup.PowerOnZeroSwitch; break;
        case SETUP_B3_2:
            ful_Value = gs_Setup.DiscSumSwitch; break;
        case SETUP_B3_3:
            ful_Value = gs_Setup.ResultHoldSwitch; break;
        case SETUP_B3_4:
            ful_Value = gs_Setup.SumCheckCodeSwitch; break;
        case SETUP_B3_5:
            ful_Value = gs_Setup.SumClrCodeSwitch; break;
        case SETUP_B3_6:
            ful_Value = gs_Setup.SumExtendSwitch; break;
        case SETUP_B4_1:
            ful_Value = gs_Setup.ComMode_1; break;
        case SETUP_B4_2:
            ful_Value = gs_Setup.TareNumber_1; break;
        case SETUP_B4_3: 
            if (gu8_ExtType == PBUS_UART)
                ful_Value = gu32_GSDType;
            else
                ful_Value = gs_Setup.BaudRate_1; break;
        case SETUP_B4_4:
            if (gu8_ExtType == PBUS_UART)
                ful_Value = gu32_FollowMasterSwitch; 
            else
                ful_Value = gs_Setup.ParityMode_1; break;
        case SETUP_B4_5:
            ful_Value = gs_Setup.HiLo_1; break;
        case SETUP_B4_6:
            ful_Value = gs_Setup.PrintSwitch_1; break;
        case SETUP_B4_7:
            ful_Value = gs_Setup.PrintType_1; break;
        case SETUP_B4_8:
            ful_Value = gs_Setup.PrintLanguage_1; break;
        case SETUP_B4_9:
            ful_Value = gs_Setup.PrintBlankRows_1; break;
        case SETUP_B5_1:
            ful_Value = gs_Setup.ComMode_2; break;
        case SETUP_B5_2:
            ful_Value = gs_Setup.TareNumber_2; break;
        case SETUP_B5_3:
            ful_Value = gs_Setup.BaudRate_2; break;
        case SETUP_B5_4:
            ful_Value = gs_Setup.ParityMode_2; break;
        case SETUP_B5_5:
            ful_Value = gs_Setup.HiLo_2; break;
        case SETUP_B5_6:
            ful_Value = gs_Setup.PrintSwitch_2; break;
        case SETUP_B5_7:
            ful_Value = gs_Setup.PrintType_2; break;
        case SETUP_B5_8:
            ful_Value = gs_Setup.PrintLanguage_2; break;
        case SETUP_B5_9:
            ful_Value = gs_Setup.PrintBlankRows_2; break;
        case SETUP_B6_1:
            ful_Value = gs_Setup.No2PackOffTwoSwitch; break;
        case SETUP_B6_2:
            ful_Value = gs_Setup.No2ConveyorDelayTime; break;
        case SETUP_B6_3:
            ful_Value = gs_Setup.No2ConveyorRunTime; break;
        case SETUP_B6_4:
            ful_Value = gs_Setup.No2AddAgainDelayTime; break;
        case SETUP_B7:
            ful_Value = gs_Setup.AdaptSW; break;
        case SETUP_B7_1:
            ful_Value = gs_Setup.AdaptSP2SW; break;
        case SETUP_B7_2:
            ful_Value = gs_Setup.AdaptSP2LimitRng; break;
        case SETUP_B7_3:
            ful_Value = gs_Setup.AdaptSP2Level; break;
        case SETUP_B7_4:
            ful_Value = gs_Setup.AdaptSP2TimeThreshold; break;
        case SETUP_B7_5:
            ful_Value = gs_Setup.AdaptSP3SW; break;
        case SETUP_B7_6:
            ful_Value = gs_Setup.AdaptSP3LimitRng; break;
        case SETUP_B7_7:
            ful_Value = gs_Setup.AdaptSP3Scope; break;
        case SETUP_B7_8:
            ful_Value = gs_Setup.AdaptSP2Scope; break;
        case SETUP_B7_9:
            ful_Value = gs_Setup.Adapt7_9; break;
        case SETUP_B8:      //工作参数密码开关             
            ful_Value = gu32_SetupCodeSwitch; break;
        case SETUP_B8_1:    //工作参数密码                 
            ful_Value = gu32_SetupCode; break;
        default:
            ful_Value = 0; break;
    }
    
    return ful_Value;
}

/*********************************************************************
函数名称: u16 GetSetupPort(fuc_Item)
功    能: 得到工作参数对应的存储地址
说    明: 
入口参数: fuc_Item = 参数项
返 回 值: 
设    计: 丛海旭               时    间: 2011-3-11
修    改:                      时    间: 
*********************************************************************/
u16 GetSetupPort(u8 fuc_Item)
{
    u16 lui_FMAddress = 0;
    
    switch (fuc_Item)
    {
        case SETUP_B1_1:
            gs_Setup.TareMode = ulParameter.ul;
            lui_FMAddress = FMAddr_TareMode;
            break;
        case SETUP_B1_2:
            gs_Setup.GorNMode = ulParameter.ul;
            lui_FMAddress = FMAddr_GorNMode;
            break;
        case SETUP_B1_3:
            gs_Setup.FeedMode = ulParameter.ul;
            lui_FMAddress = FMAddr_FeedMode;
            break;
        case SETUP_B1_4:
            gs_Setup.AutoClrZeroInterval = ulParameter.ul;
            lui_FMAddress = FMAddr_AutoClrZeroInterval;
            break;
        case SETUP_B1_5:
            gs_Setup.JudgeStabOverTime = ulParameter.ul;
            lui_FMAddress = FMAddr_JudgeStabOverTime;
            break;
        case SETUP_B1_6:
            gs_Setup.AuxDispType = ulParameter.ul;
            lui_FMAddress = FMAddr_AuxDispType;
            break;
        case SETUP_B2_1:
            gs_Setup.ClrZeroRng = ulParameter.ul;
            lui_FMAddress = FMAddr_ClrZeroRng;
            break;
        case SETUP_B2_2:
            gs_Setup.JudgeStabRng = ulParameter.ul;
            lui_FMAddress = FMAddr_JudgeStabRng;
            break;
        case SETUP_B2_3:
            gs_Setup.TraceZeroRng = ulParameter.ul;
            lui_FMAddress = FMAddr_TraceZeroRng;
            break;
        case SETUP_B2_4:
            gs_Setup.AdFilterLevel = ulParameter.ul;
            lui_FMAddress = FMAddr_AdFilterLevel;
            break;
        case SETUP_B2_5:
            gs_Setup.AdFilterLevelDisc = ulParameter.ul;
            lui_FMAddress = FMAddr_AdFilterLevelDisc;
            break;
        case SETUP_B2_6:
            gs_Setup.AdFilterLevelComp = ulParameter.ul;
            lui_FMAddress = FMAddr_AdFilterLevelComp;
            break;
        case SETUP_B2_7:
            gs_Setup.AdSamplingRate = ulParameter.ul;
            lui_FMAddress = FMAddr_AdSamplingRate;
            break;
        case SETUP_B2_8:
            gs_Setup.ExtType = ulParameter.ul;
            lui_FMAddress = FMAddr_ExtType;
            break;
        case SETUP_B3_1:
            gs_Setup.PowerOnZeroSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_PowerOnZeroSwitch;
            break;
        case SETUP_B3_2:
            gs_Setup.DiscSumSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscSumSwitch;
            break;
        case SETUP_B3_3:
            gs_Setup.ResultHoldSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_ResultHoldSwitch;
            break;
        case SETUP_B3_4:
            gs_Setup.SumCheckCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SumCheckCodeSwitch;
            break;
        case SETUP_B3_5:
            gs_Setup.SumClrCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SumClrCodeSwitch;
            break;
        case SETUP_B3_6:
            gs_Setup.SumExtendSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SumExtendSwitch;
            break;
        case SETUP_B4_1:
            gs_Setup.ComMode_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_ComMode_1;
            break;
        case SETUP_B4_2:
            gs_Setup.TareNumber_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_TareNumber_1;
            break;
        case SETUP_B4_3:
            if (gu8_ExtType == PBUS_UART)
            {
                gu32_GSDType = ulParameter.ul;
                lui_FMAddress = FMAddr_GSDType;
            }
            else
            {
                gs_Setup.BaudRate_1 = ulParameter.ul;
                lui_FMAddress = FMAddr_BaudRate_1;
            }
            break;
        case SETUP_B4_4:
            if (gu8_ExtType == PBUS_UART)
            {
                gu32_FollowMasterSwitch = ulParameter.ul;
                lui_FMAddress = FMAddr_FollowMasterSwitch;
            }
            else
            {
                gs_Setup.ParityMode_1 = ulParameter.ul;
                lui_FMAddress = FMAddr_ParityMode_1;
            }
            break;
        case SETUP_B4_5:
            gs_Setup.HiLo_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_HiLo_1;
            break;
        case SETUP_B4_6:
            gs_Setup.PrintSwitch_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintSwitch_1;
            break;
        case SETUP_B4_7:
            gs_Setup.PrintType_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintType_1;
            break;
        case SETUP_B4_8:
            gs_Setup.PrintLanguage_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintLanguage_1;
            break;
        case SETUP_B4_9:
            gs_Setup.PrintBlankRows_1 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintBlankRows_1;
            break;
        case SETUP_B5_1:
            gs_Setup.ComMode_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_ComMode_2;
            break;
        case SETUP_B5_2:
            gs_Setup.TareNumber_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_TareNumber_2;
            break;
        case SETUP_B5_3:
            gs_Setup.BaudRate_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_BaudRate_2;
            break;
        case SETUP_B5_4:
            gs_Setup.ParityMode_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_ParityMode_2;
            break;
        case SETUP_B5_5:
            gs_Setup.HiLo_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_HiLo_2;
            break;
        case SETUP_B5_6:
            gs_Setup.PrintSwitch_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintSwitch_2;
            break;
        case SETUP_B5_7:
            gs_Setup.PrintType_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintType_2;
            break;
        case SETUP_B5_8:
            gs_Setup.PrintLanguage_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintLanguage_2;
            break;
        case SETUP_B5_9:
            gs_Setup.PrintBlankRows_2 = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintBlankRows_2;
            break;
        case SETUP_B6_1:
            gs_Setup.No2PackOffTwoSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_No2PackOffTwoSwitch;
            break;
        case SETUP_B6_2:
            gs_Setup.No2ConveyorDelayTime = ulParameter.ul;
            lui_FMAddress = FMAddr_No2ConveyorDelayTime;
            break;
        case SETUP_B6_3:
            gs_Setup.No2ConveyorRunTime = ulParameter.ul;
            lui_FMAddress = FMAddr_No2ConveyorRunTime;
            break;
        case SETUP_B6_4:
            gs_Setup.No2AddAgainDelayTime = ulParameter.ul;
            lui_FMAddress = FMAddr_No2AddAgainDelayTime;
            break;
        case SETUP_B7:
            gs_Setup.AdaptSW = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSW;
            break;
        case SETUP_B7_1:
            gs_Setup.AdaptSP2SW = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP2SW;
            break;
        case SETUP_B7_2:
            gs_Setup.AdaptSP2LimitRng = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP2LimitRng;
            break;
        case SETUP_B7_3:
            gs_Setup.AdaptSP2Level = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP2Level;
            break;
        case SETUP_B7_4:
            gs_Setup.AdaptSP2TimeThreshold = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP2TimeThreshold;
            break;
        case SETUP_B7_5:
            gs_Setup.AdaptSP3SW = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP3SW;
            break;
        case SETUP_B7_6:
            gs_Setup.AdaptSP3LimitRng = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP3LimitRng;
            break;
        case SETUP_B7_7:
            gs_Setup.AdaptSP3Scope = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP3Scope;
            break;
        case SETUP_B7_8:
            gs_Setup.AdaptSP2Scope = ulParameter.ul;
            lui_FMAddress = FMAddr_AdaptSP2Scope;
            break;
        case SETUP_B7_9:
            gs_Setup.Adapt7_9 = ulParameter.ul;
            lui_FMAddress = FMAddr_Adapt7_9;
            break;
        case SETUP_B8:      //工作参数密码开关             
            gu32_SetupCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SetupCodeSwitch;
            break;
        case SETUP_B8_1:    //工作参数密码                 
            gu32_SetupCode = ulParameter.ul;
            lui_FMAddress = FMAddr_SetupCode;
            break;
        default:
            break;
    }
    return lui_FMAddress;
}

/*********************************************************************
函数名称: u8 GetSetupNextItem(u8 fu8_Item, bool fb_RightEnterFlg)
功    能: 工作参数获得下一项
说    明: 
入口参数: fu8_Item:当前Item   fb_RightEnterFlg:0-Right键 1-Enter键
返 回 值: 
设    计: 丛海旭               时    间: 2014-4-8
修    改:                      时    间: 
*********************************************************************/
u8 GetSetupNextItem(u8 fu8_Item, bool fb_RightEnterFlg)
{
    if (fb_RightEnterFlg == false)
    {   //右键返回项目处理
        fu8_Item = gu8_SetupRightItem[fu8_Item];    //获取按→键的项目
    }
    else
    {
        //Enter键返回项目处理
        fu8_Item = gu8_SetupEnterItem[fu8_Item];         //进入下一项
    }
    if (gs_Setup.TareMode != NO2_A && gs_Setup.TareMode != NO2_B)
    {   //非无斗模式，没有B6选项
        if (fu8_Item >= SETUP_B6 && fu8_Item < SETUP_B7)
        {
            fu8_Item = SETUP_B7;
        }
    }
    
//COM_PRINT       0   //打印
//COM_CONT        1   //cont连续方式
//COM_READ        2   //read命令方式
//COM_READ_O      3   //read命令方式
//COM_BUS_R       4   //Modbus RTU方式
//COM_BUS_A       5   //Modbus ASCII方式
//COM_PBUS        6   //PROFIBUS通讯
    if (fu8_Item == SETUP_B4_5)
    {   //PROFIBUS通讯只有前4项
        if (gs_Setup.ComMode_1 == COM_PBUS)
            fu8_Item = SETUP_B4_1;
    }
    if (fu8_Item == SETUP_B4_5)
    {   //Modbus方式才能选高低字存储顺序 Print才能选打印相关参数
        if (gs_Setup.ComMode_1 == COM_CONT || gs_Setup.ComMode_1 == COM_READ || gs_Setup.ComMode_1 == COM_READ_O)
            fu8_Item = SETUP_B4_1;
        if (gs_Setup.ComMode_1 == COM_PRINT)
            fu8_Item = SETUP_B4_6;
    }
    if (fu8_Item == SETUP_B4_6)
    {
        if (gs_Setup.ComMode_1 != COM_PRINT)
            fu8_Item = SETUP_B4_1;
    }
    if (fu8_Item == SETUP_B5_5)
    {   //Modbus方式才能选高低字存储顺序 Print才能选打印相关参数
        if (gs_Setup.ComMode_2 == COM_CONT || gs_Setup.ComMode_2 == COM_READ || gs_Setup.ComMode_2 == COM_READ_O)
            fu8_Item = SETUP_B5_1;
        if (gs_Setup.ComMode_2 == COM_PRINT)
            fu8_Item = SETUP_B5_6;
    }
    if (fu8_Item == SETUP_B5_6)
    {
        if (gs_Setup.ComMode_2 != COM_PRINT)
            fu8_Item = SETUP_B5_1;
    }
    
    return fu8_Item;
}




/*********************************************************************
函数名称: void Key_SetupState(void)
功    能: 工作参数设置处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-19
修    改:                      时    间: 
*********************************************************************/
void Key_SetupState(void)
{
    u8 luc_DataLen = 0;      //输入数据位数
    
    gb_FlashFlg = false;
    
    if (gu32_KeyCode == K_RIGHT)
    {
        gu8_Item = GetSetupNextItem(gu8_Item, false);  //获取按→键的项目
        ulParameter.ul = GetSetupItemValue(gu8_Item);
        gu8_DataInLength = 0;
        gb_FirstNumKeyFlg = true;
    }
    switch (gu8_Item)
    {
        case SETUP_CHECKCODE:       //密码输入
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            break;
        case DISP_SETUP:
            if (gu32_Sys10ms - gu32_TimeDelay >= 100)
            {
                gb_FirstNumKeyFlg = true;
                gu8_Item = SETUP_B1;
            }
            break;
        case SETUP_B2_2:              //判稳范围
        case SETUP_B4_2:           //串行口1设备号
        case SETUP_B5_2:           //串行口2设备号
        case SETUP_B2_1:           //清零范围
        case SETUP_B1_4:           //自动清零间隔
            luc_DataLen = 2;        //输入
            break;
        case SETUP_B1_5:           //判稳超时时间
        case SETUP_B6_2:            //输送机启动延时时间
        case SETUP_B6_3:            //输送机运行时间
        case SETUP_B6_4:            //再次夹袋延时启动加料时间
        case SETUP_B7_2:
        case SETUP_B7_6:
        case SETUP_B7_3:
        case SETUP_B7_7:
        case SETUP_B7_8:
        case SETUP_B7_9:
            luc_DataLen = 3;        //输入
            break;
        case SETUP_B7_4:
            luc_DataLen = 4;
            break;
        case SETUP_B4_8:            //打印语言
        case SETUP_B5_8:            //打印语言
        case SETUP_B4_5:           //串行口1 HiLo
        case SETUP_B5_5:           //串行口2 HiLo
        case SETUP_B4_6:           //串行口1自动打印开关
        case SETUP_B5_6:           //串行口2自动打印开关
        case SETUP_B3_1:              //上电自动清零开关
        case SETUP_B3_2:             //手动卸料累计开关
        case SETUP_B3_3:        //定值重量保持开关
        case SETUP_B3_4:        //累计查询密码开关
        case SETUP_B3_5:        //累计清除密码开关
        case SETUP_B3_6:        //累计范围扩展开关
        case SETUP_B1_3:              //投料方式
        case SETUP_B1_2:             //毛净重模式
        case SETUP_B8:             //密码开关
        case SETUP_B7:
        case SETUP_B7_1:
        case SETUP_B7_5:
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //两项切换，用↑键修改
            break;
        case SETUP_B1_6:            //副显示内容 0~2该参数即可以用数字键输入也可以用↑输入
            luc_DataLen = 1;        //输入
        case SETUP_B4_7:            //打印格式
        case SETUP_B5_7:            //打印格式
        case SETUP_B6_1:            //双无斗秤互锁模式同时松袋开关
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 2);      //3项切换，用↑键修改
            break;
        case SETUP_B2_7:             //AD采样次数
        case SETUP_B2_8:             //扩展接口类型
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //4项切换，用↑键修改
            break;
        case SETUP_B4_1:           //串行口1通讯方式
            if (gs_Setup.ComMode_1 == COM_PBUS)
            {   //PROFIBUS通讯时固定为PBUS，不能调
                break;
            }
        case SETUP_B5_1:           //串行口2通讯方式
        case SETUP_B1_1:             //秤体模式
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 5);      //5项切换，用↑键修改
            break;
        case SETUP_B4_3:           //串行口1波特率
            if (gu8_ExtType == PBUS_UART)
            {   //PROFIBUS只有GSD-1和GSD-2两个选项
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //2项切换，用↑键修改
                break;
            }
        case SETUP_B5_3:           //串行口2波特率
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 10);      //8项切换，用↑键修改
            break;
        case SETUP_B4_4:           //串行口1校验方式
            if (gu8_ExtType == PBUS_UART)
            {   
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //
                break;
            }
            if (gs_Setup.ComMode_1 >= COM_BUS_R)
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //modbus通讯只能选择数据位是8位的检验方式
            else
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 6);
            break;  
        case SETUP_B5_4:           //串行口2校验方式
            if (gs_Setup.ComMode_2 >= COM_BUS_R)
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //modbus通讯只能选择数据位是8位的检验方式
            else
                ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 6);
            break;  
        case SETUP_B2_3:              //零点跟踪范围
        case SETUP_B2_4:              //AD数字滤波级数
        case SETUP_B2_5:              //卸料滤波参数
        case SETUP_B2_6:              //定值滤波参数
        case SETUP_B4_9:              //串行口1打印走纸行数
        case SETUP_B5_9:              //串行口2打印走纸行数
            luc_DataLen = 1;
            break;
        case SETUP_B8_1:
            gb_FirstNumKeyFlg = false;
            if (gu32_KeyCode == K_SET)           //进入密码设置状态
                gb_SetFlg = true;
            if (gb_SetFlg)
            {
                if (gu32_KeyCode == K_ESC)       //退出密码设置状态
                {
                    gb_SetFlg = false;
                    gu32_KeyCode = 0;
                    gu8_SetupInTime = 0;
                    gu8_DataInLength = 0;
                }
                else
                    luc_DataLen = 6;
            }
            break;
        default:
            break;
    }
    
    //数据输入
    ulParameter.ul = Key_NumInput(ulParameter.ul, luc_DataLen);
    if (gu8_Item == SETUP_B1_6 && ulParameter.ul > 2)
    {   //B1.6副显示类型只有0~2 超过范围自动变为原值
        ulParameter.ul = gs_Setup.AuxDispType;
    }
    
    //每一项下的按键处理程序
    switch(gu32_KeyCode)
    {
        case K_ESC:             //退出键处理
            gb_SetFlg = false;
            gb_FirstNumKeyFlg = true;
            if (gu8_Item == SETUP_B8_1)        //密
            {
                gu8_SetupInTime = 0;                //清除密码输入错误次数计数器
                gu8_DataInLength = 0;               //清除数据输入位数
                gu8_Item = SETUP_B8;
            }
            else if (gu8_Item > SETUP_B7 && gu8_Item < SETUP_B8)
                gu8_Item = SETUP_B7;
            else if (gu8_Item > SETUP_B6 && gu8_Item < SETUP_B7)
                gu8_Item = SETUP_B6;
            else if (gu8_Item > SETUP_B5 && gu8_Item < SETUP_B6)
                gu8_Item = SETUP_B5;
            else if (gu8_Item > SETUP_B4 && gu8_Item < SETUP_B5)
                gu8_Item = SETUP_B4;
            else if (gu8_Item > SETUP_B3 && gu8_Item < SETUP_B4)
                gu8_Item = SETUP_B3;
            else if (gu8_Item > SETUP_B2 && gu8_Item < SETUP_B3)
                gu8_Item = SETUP_B2;
            else if (gu8_Item > SETUP_B1 && gu8_Item < SETUP_B2)
                gu8_Item = SETUP_B1;
            else
            {
                gu32_SystemState = STOPSTATE;
            }
            ulParameter.ul = GetSetupItemValue(gu8_Item);
            break;
        case K_ENTER:           //确认键处理
            gb_FirstNumKeyFlg = true;
            switch(gu8_Item)
            {
                case SETUP_CHECKCODE:       //密码输入
                    gu8_SetupInTime++;
                    if ((ulParameter.ul == gu32_SetupCode && gu8_DataInLength ==6) || 
                        (ulParameter.ul == SUPERCODE && gu8_DataInLength ==6))        //输入密码等于设定密码或超级密码
                    {
                        gu8_SetupInTime = 0;            //清除密码输入错误次数计数器
                        ulParameter.ul = 0;
                        gu32_TimeDelay = gu32_Sys10ms;
                    }
                    else if (gu8_SetupInTime >= 3)      //密码输入错误超过3次
                    {
                        ErrorDeal(4, 0);        //显示error4
                        Disp_Error();
                        VFD_CopyDispArryToDispRam();
                        VFD_WriteDispRamArry();
                        while(1);
                    }
                    else            //密码输入错误，提示error
                    {
                        ErrorDeal(0, 100);
                        ulParameter.ul = 0;
                        gu8_DataInLength = 0;
                        return;
                    }
                    break;
                case SETUP_B8_1:           //密码设置状态
                    if (gb_SetFlg == true)
                    {
                        gu8_SetupInTime++;
                        if (gu8_SetupInTime >= 2)
                        {
                            if (ulParameter.ul == gu32_DataTemp && gu8_DataInLength == 6)         //两次输入的密码一致
                            {
                                gu8_SetupInTime = 0;
                                gb_SetFlg = false;
                                gu32_SetupCode = ulParameter.ul;
                                FRAM_WriteDW(FMAddr_SetupCode, gu32_SetupCode);          //保存密码
                            }
                            else
                            {
                                gu8_SetupInTime = 0;
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
                                gu8_SetupInTime = 0;
                                ErrorDeal(0, 100);
                                gb_FirstNumKeyFlg = true;
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                    }
                    break;
                case SETUP_B4_2:           //串行口1秤号
                case SETUP_B5_2:           //串行口1秤号
                case SETUP_B2_1:              //清零范围
                    if (ulParameter.ul == 0)            //这些参数值不能为0
                    {
                        ErrorDeal(0, 100);    //显示error
                        return;
                    }
                    break;
                case SETUP_B4_1:      //通讯方式由Print,read或cont改为bus后，校验数据7位的话默认改为18E1
                    if (ulParameter.ul == COM_BUS_R && gs_Setup.ComMode_1 != COM_BUS_R)
                    {
                        if (gs_Setup.ParityMode_1 > 3)
                        {
                            gs_Setup.ParityMode_1 = 1;
                            FRAM_WriteDW(FMAddr_ParityMode_1, gs_Setup.ParityMode_1);
                        }
                    }
                    break;
                case SETUP_B5_1:      //通讯方式由Print,read或cont改为bus后，校验数据7位的话默认改为18E1
                    if (ulParameter.ul == COM_BUS_R && gs_Setup.ComMode_2 != COM_BUS_R)
                    {
                        if (gs_Setup.ParityMode_2 > 3)
                        {
                            gs_Setup.ParityMode_2 = 1;
                            FRAM_WriteDW(FMAddr_ParityMode_2, gs_Setup.ParityMode_2);
                        }
                    }
                    break;
                case SETUP_B1_1:
                    if (ulParameter.ul != gs_Setup.TareMode)
                    {   
                        TareModeChange(ulParameter.ul);
                    }
                    break;
                default:
                    break;
            }
            if ((gu8_Item != SETUP_CHECKCODE) && (gu8_Item != SETUP_B1) &&
                (gu8_Item != SETUP_B2) && (gu8_Item != SETUP_B3) && 
                (gu8_Item != SETUP_B4) && (gu8_Item != SETUP_B5) && (gu8_Item != SETUP_B6))
                FRAM_WriteDW(GetSetupPort(gu8_Item), ulParameter.ul);         //保存设置的工作参数值
            //修改串口相关参数要重新初始化串口
            if (gu8_Item == SETUP_B4_1 || gu8_Item == SETUP_B4_2 || gu8_Item == SETUP_B4_3 || gu8_Item == SETUP_B4_4)
                Uart1_Init();
            if (gu8_Item == SETUP_B5_1 || gu8_Item == SETUP_B5_2 || gu8_Item == SETUP_B5_3 || gu8_Item == SETUP_B5_4)
                Uart0_Init();
            //修改AD转换速度要重新初始化AD
            if (gu8_Item == SETUP_B2_7)
                gb_AdOKFlg = AD_Init(gs_Setup.AdSamplingRate);
            //工作参数密码保护开关关闭时跳到B1项
            if (gu8_Item == SETUP_B8 && ulParameter.ul == 0)
                gu8_Item = SETUP_B1;
            else
                gu8_Item = GetSetupNextItem(gu8_Item, true);         //进入下一项
            ulParameter.ul = GetSetupItemValue(gu8_Item);
            gu8_DataInLength = 0;
            gb_FirstNumKeyFlg = true;
            break;
        default:
            break;
    }
}

/*********************************************************************
函数名称: void Disp_SetupState(void)
功    能: 工作参数显示相关
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-19
修    改:                      时    间: 
*********************************************************************/
void Disp_SetupState(void)
{
    u8 luc_DispNumber = 0;       //显示位数
    u8 luc_Point = 0;
    u16 lu16_RealItem = gu16_SetupRealItem[gu8_Item];
    
    gb_FlashFlg = false;
    //Bxx的显示
    if (gu8_Item != DISP_SETUP)
    {
        if (lu16_RealItem % 10 == 0)  //大项，如B1/B2/B3...
        {
            gu8_AuxDispArray[1] = SEG_B;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealItem / 10];
        }
        else    //小项 如B1.1/B1.2...
        {
            gu8_AuxDispArray[0] = SEG_B;
            gu8_AuxDispArray[1] = Value_To_SEG[lu16_RealItem % 100 / 10] | 0x80;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealItem % 10];
        }
    }
    
    switch (gu8_Item)
    {
        case SETUP_CHECKCODE:       //密码输入
            Disp_InputCode();       //密码输入显示处理
            gb_FlashFlg = true;
            break;
        case DISP_SETUP:
            VFD_MainDisp_Str(" SETUP");
            break;
        case SETUP_B1:
            VFD_MainDisp_Str("  MODE");
            break;
        case SETUP_B2:
            VFD_MainDisp_Str("  BASE");
            break;
        case SETUP_B3:
            VFD_MainDisp_Str("SWITCH");
            break;
        case SETUP_B4:
            VFD_MainDisp_Str(" EXT-1");
            break;
        case SETUP_B5:
            VFD_MainDisp_Str(" EXT-2");
            break;
        case SETUP_B6:
            VFD_MainDisp_Str("   NO2");
            break;
        case SETUP_B2_2:              //判稳范围
        case SETUP_B4_2:           //串行口1秤号
        case SETUP_B5_2:           //串行口2秤号
        case SETUP_B2_1:              //清零范围
        case SETUP_B1_4:              //自动清零间隔
            luc_DispNumber = 2;
            gb_FlashFlg = true;
            break;
        case SETUP_B1_5:              //判稳超时时间
        case SETUP_B6_2:            //输送机启动延时时间
        case SETUP_B6_3:            //输送机运行时间
        case SETUP_B6_4:            //再次夹袋延时启动加料时间
        case SETUP_B7_2:
        case SETUP_B7_6:
            luc_Point = 1;
        case SETUP_B7_3:
        case SETUP_B7_7:
        case SETUP_B7_8:
        case SETUP_B7_9:
            luc_DispNumber = 3;
            gb_FlashFlg = true;
            break;
        case SETUP_B7_4:
            luc_DispNumber = 4;
            gb_FlashFlg = true;
            break;
        case SETUP_B2_3:              //零点跟踪范围
        case SETUP_B2_4:              //AD数字滤波级数
        case SETUP_B2_5:              //卸料滤波参数
        case SETUP_B2_6:              //定值滤波参数
        case SETUP_B4_9:              //串行口1打印走纸行数
        case SETUP_B5_9:              //串行口2打印走纸行数
            luc_DispNumber = 1;
            gb_FlashFlg = true;
            break;
        case SETUP_B1_6:            //副显示内容
            luc_DispNumber = 1;
            break;
        case SETUP_B4_6:           //串行口1打印开关
        case SETUP_B5_6:           //串行口2打印开关
        case SETUP_B3_1:              //上电自动清零开关
        case SETUP_B3_2:             //手动卸料累计开关
        case SETUP_B3_3:        //定值重量保持开关
        case SETUP_B3_4:        //累计查询密码开关
        case SETUP_B3_5:        //累计清除密码开关
        case SETUP_B3_6:        //累计范围扩展开关
        case SETUP_B8:          //密码开关
        case SETUP_B7:
        case SETUP_B7_1:
        case SETUP_B7_5:
            Disp_OnOff(ulParameter.ul);           //显示开关
            break;
        case SETUP_B1_1:
            Disp_TareMode(ulParameter.ul);        //显示binyes binno
            break;
        case SETUP_B1_2:
            Disp_GorNMode(ulParameter.ul);        //显示gross net
            break;
        case SETUP_B2_7:             //AD采样次数
            Disp_AdSamplingRate(ulParameter.ul);        //120 240 480 960
            break;
        case SETUP_B2_8:            //扩展接口类型
            Disp_ExtType(ulParameter.ul);
            break;
        case SETUP_B4_3:
            if (gu8_ExtType == PBUS_UART)
            {   //PROFIBUS通讯此项目为GSD文件选择
                Disp_GSDType(ulParameter.ul);
                break;
            }
        case SETUP_B5_3:
            Disp_BaudRate(ulParameter.ul);        //波特率显示
            break;
        case SETUP_B4_1:
        case SETUP_B5_1:
            Disp_ComMode(ulParameter.ul);         //显示通讯方式
            break;
        case SETUP_B1_3:
            Disp_FeedMode(ulParameter.ul);        //投料方式
            break;
        case SETUP_B4_4:
            if (gu8_ExtType == PBUS_UART)
            {
                Disp_OnOff(ulParameter.ul);           //显示开关
                break;
            }
        case SETUP_B5_4:
            Disp_ParityMode(ulParameter.ul);      //校验方式
            break;
        case SETUP_B4_5:
        case SETUP_B5_5:
            Disp_HiLo(ulParameter.ul);            //HiLo
            break;
        case SETUP_B4_7:            //打印格式
        case SETUP_B5_7:            //打印格式
            Disp_PrintType(ulParameter.ul);
            break;
        case SETUP_B4_8:            //打印语言
        case SETUP_B5_8:            //打印语言
            Disp_PrintLanguage(ulParameter.ul);
            break;
        case SETUP_B6_1:            //双无斗秤互锁模式同时松袋开关
            Disp_No2PackOffTwoSwitch(ulParameter.ul);
            break;
        case SETUP_B8_1:
            Disp_InputCode();       //密码输入显示处理
            VFD_AuxDisp_Str("B71");
            gu8_AuxDispArray[1] |= 0x80;    //显示小数点
            if (gb_SetFlg == true)
                gb_FlashFlg = true;
            break;
    }
    
    switch (gu8_Item)
    {
        case SETUP_CHECKCODE:       //密码输入
        case SETUP_B8_1:           //密码设置
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        case SETUP_B4_2:           //串行口1秤号
        case SETUP_B5_2:           //串行口2秤号
        case SETUP_B2_1:              //清零范围
        case SETUP_B1_4:              //自动清零间隔
        case SETUP_B1_5:           //判稳超时时间
        case SETUP_B1_6:            //副显示内容
        case SETUP_B2_3:              //零点跟踪范围
        case SETUP_B2_2:              //判稳范围
        case SETUP_B2_4:              //AD数字滤波级数
        case SETUP_B2_5:              //卸料滤波参数
        case SETUP_B2_6:              //定值滤波参数
        case SETUP_B4_9:              //串行口1打印走纸行数
        case SETUP_B5_9:              //串行口2打印走纸行数
        case SETUP_B6_2:            //输送机启动延时时间
        case SETUP_B6_3:            //输送机运行时间
        case SETUP_B6_4:            //再次夹袋延时启动加料时间
        case SETUP_B7_2:
        case SETUP_B7_6:
        case SETUP_B7_3:
        case SETUP_B7_7:
        case SETUP_B7_8:
        case SETUP_B7_9:
        case SETUP_B7_4:
            Disp_Main_Data(ulParameter.ul, luc_Point, luc_DispNumber, false, false);       //显示
            Flash_MainDisp(0x01);                //闪烁处理
            break;
        default:
            break;
    }
}

/*********************************************************************
函数名称: void TareModeChange(u32 fu32_NewTareMode)
功    能: 秤体模式改变后要处理的东西
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2014-04-28
修    改:                      时    间: 
*********************************************************************/
void TareModeChange(u32 fu32_NewTareMode)
{
    //秤体模式改变后清除累计、批次
    //累计
    ClrSum();
    //批次
    gu32_BatchTime = 0;
    gu32_BatchTimeCnt = 0;
    FRAM_WriteDW(FMAddr_BatchTime, gu32_BatchTime);
    FRAM_WriteDW(FMAddr_BatchTimeCnt, gu32_BatchTimeCnt);
    //更新开关量自定义数组指针
    IoDefPointerRefresh(fu32_NewTareMode);
}
