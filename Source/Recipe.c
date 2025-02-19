/*
Description:            配方参数设置相关函数
Author:                 丛海旭
Date:                   2013-12-9
*/

#include "include.h"



struct RECIPE gs_Rec;
u32 gu32_RecCodeSwitch;
u32 gu32_RecCode;
u32 gu32_RecNum;

u8 gu8_RecInTime;



const u16 gu16_RecRealItem[]=
{0, 1,
//F1  F1.1 F1.2 F1.3 F1.4 F1.5 F1.6
0x10, 0x11,0x12,0x13,0x14,0x15,0x16,
//F2 F2.1 F2.2 F2.3
0x20,0x21,0x22,0x23,
//F3 F3.1 F3.2 F3.3 F3.4 F3.5 F3.6 F3.7 F3.8 F3.9 F3.10 F3.11 F3.12
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x310,0x311,0x312,
//F4 F4.1 F4.2 F4.3
0x40,0x41,0x42,0x43,
//F5 F5.1 F5.2 F5.3 F5.4 F5.5 F5.6
0x50,0x51,0x52,0x53,0x54,0x55,0x56,
//F6 F6.1 F6.2
0x60,0x61,0x62,
//F7
0x70,
//F8 F8.1 F8.2 F8.3 F8.4
0x80,0x81,0x82,0x83,0x84,
//F9 F9.1 F9.2 F9.3
0x90,0x91,0x92,0x93,
//FA FA.1
0xA0,0xA1};
const u8 gu8_RecRightItem[] =        //按→键获得下一参数
{0, 1,
//F1    F1.1      F1.2      F1.3      F1.4      F1.5      F1.6
REC_F2, REC_F1_2, REC_F1_3, REC_F1_4, REC_F1_5, REC_F1_6, REC_F1_1,
//F2    F2.1      F2.2      F2.3
REC_F3, REC_F2_2, REC_F2_3, REC_F2_1,
//F3    F3.1      F3.2      F3.3      F3.4      F3.5      F3.6      F3.7      F3.8      F3.9       F3.10      F3.11      F3.12    
REC_F4, REC_F3_2, REC_F3_3, REC_F3_4, REC_F3_5, REC_F3_6, REC_F3_7, REC_F3_8, REC_F3_9, REC_F3_10, REC_F3_11, REC_F3_12, REC_F3_1,
//F4    F4.1      F4.2      F4.3
REC_F5, REC_F4_2, REC_F4_3, REC_F4_1,
//F5    F5.1      F5.2      F5.3      F5.4      F5.5      F5.6
REC_F6, REC_F5_2, REC_F5_3, REC_F5_4, REC_F5_5, REC_F5_6, REC_F5_1,
//F6    F6.1      F6.2
REC_F7, REC_F6_2, REC_F6_1,
//F7
REC_F8,
//F8    F8.1      F8.2      F8.3      F8.4
REC_F9, REC_F8_2, REC_F8_3, REC_F8_4, REC_F8_1,
//F9    F9.1      F9.2      F9.3
REC_FA, REC_F9_2, REC_F9_3, REC_F9_1,
//FA    FA.1
REC_F1, REC_FA_1};

const u8 gu8_RecEnterItem[] =        //按ENTER键获得下一参数
{0,       REC_F1,
//F1      F1.1      F1.2      F1.3      F1.4      F1.5      F1.6
REC_F1_1, REC_F1_2, REC_F1_3, REC_F1_4, REC_F1_5, REC_F1_6, REC_F1_1,
//F2      F2.1      F2.2      F2.3
REC_F2_1, REC_F2_2, REC_F2_3, REC_F2_1,
//F3      F3.1      F3.2      F3.3      F3.4      F3.5      F3.6      F3.7      F3.8      F3.9       F3.10      F3.11      F3.12     
REC_F3_1, REC_F3_2, REC_F3_3, REC_F3_4, REC_F3_5, REC_F3_6, REC_F3_7, REC_F3_8, REC_F3_9, REC_F3_10, REC_F3_11, REC_F3_12, REC_F3_1,
//F4      F4.1      F4.2      F4.3
REC_F4_1, REC_F4_2, REC_F4_3, REC_F4_1,
//F5      F5.1      F5.2      F5.3      F5.4      F5.5      F5.6
REC_F5_1, REC_F5_2, REC_F5_3, REC_F5_4, REC_F5_5, REC_F5_6, REC_F5_1,
//F6      F6.1      F6.2
REC_F6_1, REC_F6_2, REC_F6_1,
//F7
REC_F8,
//F8      F8.1      F8.2      F8.3      F8.4
REC_F8_1, REC_F8_2, REC_F8_3, REC_F8_4, REC_F8_1,
//F9      F9.1      F9.2      F9.3
REC_F9_1, REC_F9_2, REC_F9_3, REC_F9_1,
//FA      FA.1
REC_FA_1, REC_FA};



/*********************************************************************
函数名称: u32 GetRecipeItemValue(u8 fuc_Item)
功    能: 得到对应的配方值
说    明: 
入口参数: fuc_Item = 参数项
返 回 值: 配方参数值
设    计: 丛海旭               时    间: 2011-3-23
修    改:                      时    间: 
*********************************************************************/
u32 GetRecipeItemValue(u8 fuc_Item)
{
    u32 ful_Value;
    
    switch(fuc_Item)
    {
        case DISP_RECNUM:
            ful_Value = gu32_RecNum; break;
        case REC_F1_1:
            ful_Value = gs_Rec.Target; break;
        case REC_F1_2:
            ful_Value = gs_Rec.Large; break;
        case REC_F1_3:
            ful_Value = gs_Rec.Middle; break;
        case REC_F1_4:
            ful_Value = gs_Rec.Small; break;
        case REC_F1_5:
            ful_Value = gs_Rec.NearZero; break;
        case REC_F1_6:
            ful_Value = gs_Rec.FlapBagWt; break;
        case REC_F2:
            ful_Value = gs_Rec.OUSwitch; break;
        case REC_F2_1:
            ful_Value = gs_Rec.OverValue; break;
        case REC_F2_2:
            ful_Value = gs_Rec.UnderValue; break;
        case REC_F2_3:
            ful_Value = gs_Rec.OUStopSwitch; break;
        case REC_F3_1:
            ful_Value = gs_Rec.TimeT1; break;
        case REC_F3_2:
            ful_Value = gs_Rec.TimeT2b; break;
        case REC_F3_3:
            ful_Value = gs_Rec.TimeT3; break;
        case REC_F3_4:
            ful_Value = gs_Rec.TimeT4; break;
        case REC_F3_5:
            ful_Value = gs_Rec.TimeT5; break;
        case REC_F3_6:
            ful_Value = gs_Rec.TimeT6; break;
        case REC_F3_7:
            ful_Value = gs_Rec.TimeT7; break;
        case REC_F3_8:
            ful_Value = gs_Rec.TimeT2a; break;
        case REC_F3_9:
            ful_Value = gs_Rec.TimeT9; break;
        case REC_F3_10:
            ful_Value = gs_Rec.TimeT10; break;
        case REC_F3_11:
            ful_Value = gs_Rec.TimeT11; break;
        case REC_F3_12:
            ful_Value = gs_Rec.TimeT12; break;
        case REC_F4:
            ful_Value = gs_Rec.CorrectFallSwitch; break;
        case REC_F4_1:
            ful_Value = gs_Rec.CorrectFallTime; break;
        case REC_F4_2:
            ful_Value = gs_Rec.CorrectFallRng1; break;
        case REC_F4_3:
            ful_Value = gs_Rec.CorrectFallRng2; break;
        case REC_F5:
            ful_Value = gs_Rec.FlapBagSwitch; break;
        case REC_F5_1:
            ful_Value = gs_Rec.FlapBagAddNum; break;
        case REC_F5_2:
            ful_Value = gs_Rec.FlapBagNum; break;
        case REC_F5_3:
            ful_Value = gs_Rec.FlapBagDelay; break;
        case REC_F5_4:
            ful_Value = gs_Rec.FlapBagTimeHi; break;
        case REC_F5_5:
            ful_Value = gs_Rec.FlapBagTimeLow; break;
        case REC_F5_6:
            ful_Value = gs_Rec.FlapBagExtTime; break;
        case REC_F6:
            ful_Value = gs_Rec.SmallPluseSwitch; break;
        case REC_F6_1:
            ful_Value = gs_Rec.SmallPluseHi; break;
        case REC_F6_2:
            ful_Value = gs_Rec.SmallPluseLow; break;
        case REC_F7:
            ful_Value = gs_Rec.CombinedNum; break;
        case REC_F8:
            ful_Value = gs_Rec.DiscShakeSwitch; break;
        case REC_F8_1:
            ful_Value = gs_Rec.DiscEffTime; break;
        case REC_F8_2:
            ful_Value = gs_Rec.DiscShakeTimeHi; break;
        case REC_F8_3:
            ful_Value = gs_Rec.DiscShakeTimeLow; break;
        case REC_F8_4:
            ful_Value = gs_Rec.DiscShakeNum; break;
        case REC_F9:
            ful_Value = gs_Rec.PrintCodeSwitch; break;
        case REC_F9_1:
            ful_Value = gs_Rec.PrintCodeRunDelay; break;
        case REC_F9_2:
            ful_Value = gs_Rec.PrintCodeTime; break;
        case REC_F9_3:
            ful_Value = gs_Rec.SheildDiscSwitch; break;
        case REC_FA:
            ful_Value = gu32_RecCodeSwitch; break;
        case REC_FA_1:
            ful_Value = gu32_RecCode; break;
        default:
            ful_Value = 0;
            break;
    }
    return ful_Value;
}

/*********************************************************************
函数名称: u16 GetRecipePort(u8 fuc_Item)
功    能: 返回配方参数对应的储存地址
说    明: 
入口参数: fuc_Item = 参数项
返 回 值: 配方参数存储地址
设    计: 丛海旭               时    间: 2013-12-24
修    改:                      时    间: 
*********************************************************************/
u16 GetRecipePort(u8 fuc_Item)
{
    u16 lui_FMAddress;
    
    switch (fuc_Item)
    {
        case DISP_RECNUM:
            gu32_RecNum = ulParameter.ul;
            lui_FMAddress = FMAddr_RecNum;
            break;
        case REC_F1_1:
            gs_Rec.Target = ulParameter.ul;
            lui_FMAddress = FMAddr_Target(gu32_RecNum);
            break;
        case REC_F1_2:
            gs_Rec.Large = ulParameter.ul;
            lui_FMAddress = FMAddr_Large(gu32_RecNum);
            break;
        case REC_F1_3:
            gs_Rec.Middle = ulParameter.ul;
            lui_FMAddress = FMAddr_Middle(gu32_RecNum);
            break;
        case REC_F1_4:
            gs_Rec.Small = ulParameter.ul;
            lui_FMAddress = FMAddr_Small(gu32_RecNum);
            break;
        case REC_F1_5:
            gs_Rec.NearZero = ulParameter.ul;
            lui_FMAddress = FMAddr_NearZero(gu32_RecNum);
            break;
        case REC_F1_6:
            gs_Rec.FlapBagWt = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagWt(gu32_RecNum);
            break;
        case REC_F2:
            gs_Rec.OUSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_OUSwitch(gu32_RecNum);
            break;
        case REC_F2_1:
            gs_Rec.OverValue = ulParameter.ul;
            lui_FMAddress = FMAddr_OverValue(gu32_RecNum);
            break;
        case REC_F2_2:
            gs_Rec.UnderValue = ulParameter.ul;
            lui_FMAddress = FMAddr_UnderValue(gu32_RecNum);
            break;
        case REC_F2_3:
            gs_Rec.OUStopSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_OUStopSwitch(gu32_RecNum);
            break;
        case REC_F3_1:
            gs_Rec.TimeT1 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT1(gu32_RecNum);
            break;
        case REC_F3_2:
            gs_Rec.TimeT2b = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT2b(gu32_RecNum);
            break;
        case REC_F3_3:
            gs_Rec.TimeT3 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT3(gu32_RecNum);
            break;
        case REC_F3_4:
            gs_Rec.TimeT4 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT4(gu32_RecNum);
            break;
        case REC_F3_5:
            gs_Rec.TimeT5 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT5(gu32_RecNum);
            break;
        case REC_F3_6:
            gs_Rec.TimeT6 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT6(gu32_RecNum);
            break;
        case REC_F3_7:
            gs_Rec.TimeT7 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT7(gu32_RecNum);
            break;
        case REC_F3_8:
            gs_Rec.TimeT2a = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT2a(gu32_RecNum);
            break;
        case REC_F3_9:
            gs_Rec.TimeT9 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT9(gu32_RecNum);
            break;
        case REC_F3_10:
            gs_Rec.TimeT10 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT10(gu32_RecNum);
            break;
        case REC_F3_11:
            gs_Rec.TimeT11 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT11(gu32_RecNum);
            break;
        case REC_F3_12:
            gs_Rec.TimeT12 = ulParameter.ul;
            lui_FMAddress = FMAddr_TimeT12(gu32_RecNum);
            break;
        case REC_F4:
            gs_Rec.CorrectFallSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallSwitch(gu32_RecNum);
            break;
        case REC_F4_1:
            gs_Rec.CorrectFallTime = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallTime(gu32_RecNum);
            break;
        case REC_F4_2:
            gs_Rec.CorrectFallRng1 = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallRng1(gu32_RecNum);
            break;
        case REC_F4_3:
            gs_Rec.CorrectFallRng2 = ulParameter.ul;
            lui_FMAddress = FMAddr_CorrectFallRng2(gu32_RecNum);
            break;
        case REC_F5:
            gs_Rec.FlapBagSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagSwitch(gu32_RecNum);
            break;
        case REC_F5_1:
            gs_Rec.FlapBagAddNum = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagAddNum(gu32_RecNum);
            break;
        case REC_F5_2:
            gs_Rec.FlapBagNum = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagNum(gu32_RecNum);
            break;
        case REC_F5_3:
            gs_Rec.FlapBagDelay = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagDelay(gu32_RecNum);
            break;
        case REC_F5_4:
            gs_Rec.FlapBagTimeHi = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagTimeHi(gu32_RecNum);
            break;
        case REC_F5_5:
            gs_Rec.FlapBagTimeLow = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagTimeLow(gu32_RecNum);
            break;
        case REC_F5_6:
            gs_Rec.FlapBagExtTime = ulParameter.ul;
            lui_FMAddress = FMAddr_FlapBagExtTime(gu32_RecNum);
            break;
        case REC_F6:
            gs_Rec.SmallPluseSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseSwitch(gu32_RecNum);
            break;
        case REC_F6_1:
            gs_Rec.SmallPluseHi = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseHi(gu32_RecNum);
            break;
        case REC_F6_2:
            gs_Rec.SmallPluseLow = ulParameter.ul;
            lui_FMAddress = FMAddr_SmallPluseLow(gu32_RecNum);
            break;
        case REC_F7:
            gs_Rec.CombinedNum = ulParameter.ul;
            lui_FMAddress = FMAddr_CombinedNum(gu32_RecNum);
            break;
        case REC_F8:
            gs_Rec.DiscShakeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeSwitch(gu32_RecNum);
            break;
        case REC_F8_1:
            gs_Rec.DiscEffTime = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscEffTime(gu32_RecNum);
            break;
        case REC_F8_2:
            gs_Rec.DiscShakeTimeHi = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeTimeHi(gu32_RecNum);
            break;
        case REC_F8_3:
            gs_Rec.DiscShakeTimeLow = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeTimeLow(gu32_RecNum);
            break;
        case REC_F8_4:
            gs_Rec.DiscShakeNum = ulParameter.ul;
            lui_FMAddress = FMAddr_DiscShakeNum(gu32_RecNum);
            break;
        case REC_F9:
            gs_Rec.PrintCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeSwitch(gu32_RecNum);
            break;
        case REC_F9_1:
            gs_Rec.PrintCodeRunDelay = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeRunDelay(gu32_RecNum);
            break;
        case REC_F9_2:
            gs_Rec.PrintCodeTime = ulParameter.ul;
            lui_FMAddress = FMAddr_PrintCodeTime(gu32_RecNum);
            break;
        case REC_F9_3:
            gs_Rec.SheildDiscSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_SheildDiscSwitch(gu32_RecNum);
            break;
        case REC_FA:
            gu32_RecCodeSwitch = ulParameter.ul;
            lui_FMAddress = FMAddr_RecCodeSwitch;
            break;
        case REC_FA_1:
            gu32_RecCode = ulParameter.ul;
            lui_FMAddress = FMAddr_RecCode;
            break;
        default:
            lui_FMAddress = 0;
            break;
    }
    return lui_FMAddress;
}

/*********************************************************************
函数名称: bool RecDataInputCheck(void)
功    能: 数据输入合理性检查
说    明: 
入口参数: 
返 回 值: 数据错误
设    计: 丛海旭               时    间: 2013-12-24
修    改:                      时    间: 
*********************************************************************/
bool RecDataInputCheck(void)
{
    bool lb_err = false;
    
    switch(gu8_Item)
    {
        case DISP_RECNUM:
            if (ulParameter.ul > 20 || ulParameter.ul == 0)
                lb_err = true;
            break;
        case REC_F1_1:
        case REC_F1_2:
        case REC_F1_3:
        case REC_F1_4:
        case REC_F1_5:
        case REC_F1_6:
        case REC_F2_1:              //超量值
        case REC_F2_2:              //欠量值
            if (ulParameter.ul > gs_Cal.Cap)            //以上值设置不能超过最大量程
                lb_err = true;
            break;
        default:
            break;
    }
    
    return lb_err;
}


/*********************************************************************
函数名称: void Key_RecipeState(void)
功    能: 配方设置相关按键和数据处理
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-24
修    改:                      时    间: 
*********************************************************************/
void Key_RecipeState(void)
{
    u8 i;
    u8 luc_DataLen = 0;      //输入数据位数
        
    if (gu32_KeyCode == K_RIGHT)
    {
        gb_FirstNumKeyFlg = true;
        gu8_Item  = gu8_RecRightItem[gu8_Item ];                  //得到按→键的下一项参数
        ulParameter.ul = GetRecipeItemValue(gu8_Item);        //获取对应项目的参数值
    }
    switch (gu8_Item)
    {
        case DISP_RECNUM:           //配方号设置
            luc_DataLen = 2;
            if (gu32_KeyCode == K_SET)               //按set键进入配方设置
            {
                if (gu32_RecCodeSwitch == 0)         //工作参数密码开关关闭则直接进入F1
                {
                    gu8_Item  = REC_F1;
                    luc_DataLen = 0;
                    gb_FirstNumKeyFlg = true;
                }
                else
                {
                    gu8_Item = REC_CHECKCODE;       //进入密码输入状态
                    luc_DataLen = 6;
                    gb_FirstNumKeyFlg = false;
                    ulParameter.ul = 0;
                }
                gu8_DataInLength = 0;
                gb_FlashFlg = false;
            }
            else if (gu32_KeyCode != K_NONE && gu32_KeyCode != K_RIGHT)        //输入其他按键时配方号闪烁
            {
                gb_FlashFlg = true;
            }
            break;
        case REC_CHECKCODE:         //密码输入
            luc_DataLen = 6;
            gb_FirstNumKeyFlg = false;
            break;
        case REC_F1_1:
        case REC_F1_2:
        case REC_F1_3:
        case REC_F1_4:
        case REC_F1_5:
        case REC_F1_6:
        case REC_F2_1:              //超量值
        case REC_F2_2:              //欠量值
             luc_DataLen = 6;
             break;
        case REC_F2:                //超欠差报警开关
        case REC_F2_3:              //超欠差暂停开关
        case REC_F4:                //落差修正开关
        case REC_F6:                //小投点动输出开关
        case REC_F8:                //卸料震打开关
        case REC_F9:                //打码开关
        case REC_F9_3:              //打码屏蔽卸料开关
        case REC_FA:                //配方参数设置密码开关
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 1);      //两项切换，用↑键修改
            break;
        case REC_F5:                //拍袋开关
            ulParameter.ul = Key_IncInput(ulParameter.ul, 0, 3);      //4项切换，用↑键修改
            break;
        case REC_F4_1:              //落差修正次数
        case REC_F4_2:              //落差修正范围
        case REC_F5_1:              //加料中拍袋次数
        case REC_F5_2:              //定值后拍袋次数
        case REC_F7:                //单秤组合次数
        case REC_F8_4:              //卸料震打次数
            luc_DataLen = 2;
            break;
        case REC_F3_1:              //加料延时T1
        case REC_F3_2:              //禁止比较时间T2A
        case REC_F3_3:              //禁止比较时间T2B
        case REC_F3_4:              //超差检测时间T3
        case REC_F3_5:              //定值保持时间T4
        case REC_F3_6:              //卸料延时T5
        case REC_F3_7:              //夹袋延时T6
        case REC_F3_8:              //松袋延时T7
        case REC_F3_9:              //提袋延时T9         
        case REC_F3_10:             //提袋输出有效时间T10
        case REC_F3_11:             //推袋延时T11        
        case REC_F3_12:             //推袋输出有效时间T12
        case REC_F5_3:              //拍袋延时
        case REC_F5_4:              //拍袋有效电平时间
        case REC_F5_5:              //拍袋无效电平时间
        case REC_F5_6:              //额外拍袋输出有效时间
        case REC_F6_1:              //点动有效电平时间
        case REC_F6_2:              //点动无效电平时间
        case REC_F8_1:              //卸料有效时间
        case REC_F8_2:              //卸料震打高电平
        case REC_F8_3:              //卸料震打低电平
        case REC_F9_1:              //打码启动延时
        case REC_F9_2:              //打码时间
            luc_DataLen = 3;
            break;
        case REC_F4_3:              //落差修正幅度
            ulParameter.ul = Key_IncInput(ulParameter.ul, 1, 3);      //三项切换，用↑键修改
            break;
        case REC_FA_1:
            gb_FirstNumKeyFlg = false;
            if (gu32_KeyCode == K_SET)           //进入密码设置状态
                gb_SetFlg = true;
            if (gb_SetFlg)
            {
                if (gu32_KeyCode == K_ESC)       //退出密码设置状态
                {
                    gb_SetFlg = false;
                    gu32_KeyCode = 0;
                    gu8_RecInTime = 0;
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
    
    //每一项下的按键处理程序
    switch(gu32_KeyCode)
    {
        case K_ESC:             //退出键处理
            gb_FlashFlg = false;
            gb_SetFlg = false;
            gb_FirstNumKeyFlg = true;
            gu8_DataInLength = 0;
            if ((gu8_Item > REC_F1) && 
               (gu16_RecRealItem[gu8_Item] % 0x10 != 0))    //小项退出到对应的大项
            {
                for (i = REC_F1; i < REC_END; i++)
                {
                    if (gu16_RecRealItem[i] == (gu16_RecRealItem[gu8_Item]/0x10*0x10))  //得到该项对应的大项值
                    {
                        gu8_Item = i;               //退出到对应大项
                        ulParameter.ul = GetRecipeItemValue(gu8_Item);
                    }
                }
            }
            else            //大项时按退出则退出到待机状态
                gu32_SystemState = STOPSTATE;
            break;
        case K_ENTER:           //确认键处理
            gb_FirstNumKeyFlg = true;
            if (RecDataInputCheck())            //输入数据范围检查
            {
                ErrorDeal(0, 100);
                ulParameter.ul = GetRecipeItemValue(gu8_Item);
                return;
            }
            switch (gu8_Item)
            {
                case DISP_RECNUM:
                    if (gu32_RecNum != ulParameter.ul)       //修改配方后重新读取配方参数值
                        Read_RecipeData(ulParameter.ul);
                    gb_FlashFlg = false;
                    break;
                case REC_CHECKCODE:       //密码输入
                    gu8_RecInTime++;
                    if ((ulParameter.ul == gu32_RecCode && gu8_DataInLength == 6) || 
                        (ulParameter.ul == SUPERCODE && gu8_DataInLength == 6))        //输入密码等于设定密码或超级密码
                    {
                        gu8_RecInTime = 0;            //清除密码输入错误次数计数器
                        ulParameter.ul = 0;
                    }
                    else if (gu8_RecInTime >= 3)      //密码输入错误超过3次
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
                case REC_FA_1:           //密码设置状态
                    if (gb_SetFlg == true)
                    {
                        gu8_RecInTime++;
                        if (gu8_RecInTime >= 2)
                        {
                            if (ulParameter.ul == gu32_DataTemp && gu8_DataInLength == 6)         //两次输入的密码一致
                            {
                                gu8_RecInTime = 0;
                                gb_SetFlg = false;
                                gu32_RecCode = ulParameter.ul;
                                FRAM_WriteDW(FMAddr_RecCode, gu32_RecCode);          //保存密码
                            }
                            else
                            {
                                gu8_RecInTime = 0;
                                ErrorDeal(0, 100);
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
                                gu8_RecInTime = 0;
                                ErrorDeal(0, 100);
                                ulParameter.ul = 0;
                                gu8_DataInLength = 0;
                                return;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }
            if (gu8_Item != REC_CHECKCODE)              //密码校验时不保存
                FRAM_WriteDW(GetRecipePort(gu8_Item), ulParameter.ul);         //保存设置的工作参数值
            if (gu8_Item == REC_F2 || gu8_Item == REC_F4 || gu8_Item == REC_F5 
             || gu8_Item == REC_F6 || gu8_Item == REC_F7 || gu8_Item == REC_F8 
             || gu8_Item == REC_F9 || gu8_Item == REC_FA)
            {
                if (ulParameter.ul != 0)                //开关打开时进入小项
                    gu8_Item = gu8_RecEnterItem[gu8_Item];
                else                                    //开关关闭时进入下一个大项
                    gu8_Item = gu8_RecRightItem[gu8_Item];
            }
            else
            {
                gu8_Item  = gu8_RecEnterItem[gu8_Item ];
            }
            ulParameter.ul = GetRecipeItemValue(gu8_Item);           //得到对应参数的值
            gu8_DataInLength = 0;
            break;
    }
}


/*********************************************************************
函数名称: void Disp_RecipeState(void)
功    能: 配方设置显示相关
说    明: 
入口参数: 
返 回 值: 
设    计: 丛海旭               时    间: 2013-12-24
修    改:                      时    间: 
*********************************************************************/
void Disp_RecipeState(void)
{
    u8 luc_DispNumber = 0;       //数据显示位数
    u8 luc_Point = 0;            //小数点
    u32 luc_Unit = UNIT_none;             //单位
    u16 lu16_RealRecItem = gu16_RecRealItem[gu8_Item];
    
    if (gu8_Item != 0)
        gb_FlashFlg = false;
        
    //Fxx的显示
    if (gu8_Item != REC_CHECKCODE && gu8_Item != DISP_RECNUM)
    {
        if (lu16_RealRecItem / 0x100 != 0)  //3.10 3.11等...
        {
            gu8_AuxDispArray[0] = Value_To_SEG[lu16_RealRecItem/0x100] | 0x80;
            gu8_AuxDispArray[1] = Value_To_SEG[lu16_RealRecItem%0x100/0x10];
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem%0x10];
        }
        else if (lu16_RealRecItem % 0x10 == 0)  //大项，如F1/F2/F3...
        {
            gu8_AuxDispArray[1] = SEG_F;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem/0x10];
        }
        else    //小项 如F1.1/F1.2...
        {
            gu8_AuxDispArray[0] = SEG_F;
            gu8_AuxDispArray[1] = Value_To_SEG[lu16_RealRecItem/0x10] | 0x80;
            gu8_AuxDispArray[2] = Value_To_SEG[lu16_RealRecItem%0x10];
        }
    }
    
    switch (gu8_Item)
    {
        case DISP_RECNUM:            //配方号
            VFD_AuxDisp_Str("REC");
            luc_Point = 0;
            luc_DispNumber = 2;
            break;
        case REC_CHECKCODE:         //密码输入
            Disp_InputCode();       //密码输入显示处理
            gb_FlashFlg = true;
            break;
        case REC_F1:
            VFD_MainDisp_Str("U_LSET");
            break;
        case REC_F3:
            VFD_MainDisp_Str("TI SET");
            break;
        case REC_F1_1:              //
        case REC_F1_2:              //
        case REC_F1_3:              //
        case REC_F1_4:              //
        case REC_F1_5:              //
        case REC_F1_6:              //
        case REC_F2_1:
        case REC_F2_2:
            luc_Point = gs_Cal.Point;
            gb_FlashFlg = true;
            luc_DispNumber = 6;
            luc_Unit = gs_Cal.Unit;
            break;
        case REC_F2:
        case REC_F2_3:
        case REC_F4:
        case REC_F6:
        case REC_F8:
        case REC_F9:                //打码开关
        case REC_F9_3:              //打码屏蔽卸料开关
        case REC_FA:
            Disp_OnOff(ulParameter.ul);           //显示开关
            break;
        case REC_F5:
            Disp_FlapOnOff(ulParameter.ul);           //显示开关
            break;
        case REC_F4_2:              //落差修正范围
            luc_DispNumber = 2;     // x.x 两位数，小数点为1
            luc_Point = 1;
            gb_FlashFlg = true;
            break;
        case REC_F3_1:              //加料延时T1
        case REC_F3_2:              //中投禁止比较时间T2A
        case REC_F3_3:              //小投禁止比较时间T2B
        case REC_F3_4:              //超差检测时间T3
        case REC_F3_5:              //定值保持时间T4
        case REC_F3_6:              //卸料延时T5
        case REC_F3_7:              //夹袋延时T6
        case REC_F3_8:              //松袋延时T7
        case REC_F3_9:              //提袋延时T9         
        case REC_F3_10:             //提袋输出有效时间T10
        case REC_F3_11:             //推袋延时T11        
        case REC_F3_12:             //推袋输出有效时间T12
        case REC_F5_3:              //拍袋延时
        case REC_F5_4:              //拍袋有效电平时间
        case REC_F5_5:              //拍袋无效电平时间
        case REC_F5_6:              //额外拍袋输出有效时间
        case REC_F6_1:              //点动有效电平时间
        case REC_F6_2:              //点动无效电平时间
        case REC_F8_1:              //卸料有效时间
        case REC_F8_2:              //卸料震打高电平
        case REC_F8_3:              //卸料震打低电平
        case REC_F9_1:              //打码启动延时
        case REC_F9_2:              //打码时间
            luc_DispNumber = 3;     //时间设置 xx.x 三位数，小数点为1
            luc_Point = 1;
            gb_FlashFlg = true;
            break;
        case REC_F5_1:              //加料中拍袋次数
        case REC_F5_2:              //定值后拍袋次数
        case REC_F4_1:              //落差修正次数
        case REC_F7:                //单秤组合次数
        case REC_F8_4:              //卸料震打次数
            luc_DispNumber = 2;     //两位数,小数点位数为0
            luc_Point = 0;
            gb_FlashFlg = true;
            break;
        case REC_F4_3:              //落差修正幅度
            luc_DispNumber = 1;     //显示1位，该项用↑键修改，
            break;
        case REC_FA_1:
            Disp_InputCode();       //密码输入显示处理
            gu8_AuxDispArray[0] = SEG_F;
            gu8_AuxDispArray[1] = SEG_A | 0x80;
            gu8_AuxDispArray[2] = SEG_1;
            if (gb_SetFlg == true)
                gb_FlashFlg = true;
            break;
        default:
            break;
    }
    
    switch (gu8_Item)
    {
        case REC_CHECKCODE:
        case REC_FA_1:
            Flash_MainDisp(0x01);                //闪烁处理
        case REC_F1:
        case REC_F2:
        case REC_F2_3:
        case REC_F3:
        case REC_F4:
        case REC_F5:
        case REC_F6:
        case REC_F8:
        case REC_F9:
        case REC_F9_3:
        case REC_FA: 
            break;
        default:
            Disp_Main_Data(ulParameter.ul, luc_Point, luc_DispNumber, false, false);       //显示值
            Flash_MainDisp(0x01);            //闪烁处理
            VFD_UnitDisp(luc_Unit);              //单位显示
            break;
    }
}
