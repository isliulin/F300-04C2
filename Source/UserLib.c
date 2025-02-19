/*************************************************
Project:                GM8804C-2_ARM
File name:              UserLib.c
Description:            常用函数库 数据类型转换等
Author:                 丛海旭
Version:                1.0
Date:                   2013-12-5
Others:                 
History:
*************************************************/
#include "UserLib.h"



/*********************************************************************
函数名称: u32 RoundOff(u32 ful_DataA, u32 ful_DataB)
功    能: 除法后四舍五入
说    明: 
入口参数: ful_DataA = 被除数，ful_DataB = 除数
返 回 值: 除法后的四舍五入结果
设    计: 丛海旭               时    间: 2011-1-10
修    改:                      时    间: 
*********************************************************************/
u32 RoundOff(u32 ful_DataA, u32 ful_DataB)
{
    return (ful_DataA % ful_DataB * 2) >= ful_DataB ? (ful_DataA / ful_DataB + 1) : (ful_DataA / ful_DataB);
}

void ShortDelay(void)
{
    u8 i;
    for(i=0;i<2;i++);
}

void MidDelay(void)
{
    u8 i;
    for(i=0;i<4;i++);
}

void LongDelay(void)
{
    u8 i;
    for(i=0;i<8;i++);
}

void delay(u32 u32DelayTime)
{
    u32 i;

    while(u32DelayTime>0)
    {
        u32DelayTime--;

        for(i=0;i<=10000;i++)
        {
            ;
        }
    }
}

//数据处理
void BIN4toNBCD(u32 ulBin,u8* BcdArry,u8 Length)
{
    u8  i;

    for(i=0;i<Length;i++)
        *(BcdArry++)=0;

    for(i=0;i<Length;i++){

        *(--BcdArry) = ulBin % 10;
        ulBin/=10;

        if(ulBin==0){
            break;
        }
    }
}

bool HextoChar(u8* HexArry,u8 Length)
{
    u8 i;
    for(i=0;i<Length;i++){
        if(HexArry[i]>0x0f){
            return false;
        }
    }

    for(i=0;i<Length;i++){
        if(HexArry[i]<10){
            HexArry[i]+='0';
        }else{
            HexArry[i]+=('A'-10);
        }
    }

    return true;
}


void BIN4toNHex(u32 ulBin,u8* HexArry,u8 Length)
{
    u8  i;
    //u8*  HexArryBak=HexArry;
    for(i=0;i<Length;i++){
        *(HexArry++)=0;
    }

    for(i=0;(i<Length)&&(i<8);i++){
        *(--HexArry)=ulBin&0x0f;
        ulBin>>=4;
    }
    //return(HextoChar(HexArryBak,Length));
}

void BIN4toNHexASC(u32 ulBin,u8* HexArry,u8 Length)
{
    BIN4toNHex(ulBin,HexArry,Length);
    HextoChar(HexArry,Length);
}

void BIN4toNASC(u32 ulBin,u8* ASCArry,u8 Length)
{
    u8 i;
    BIN4toNBCD(ulBin,ASCArry,Length);
    for(i=0;i<Length;i++){
        (*ASCArry++) +='0';
    }
}

void BIN4toNASC_Echo0(u32 ulBin,u8* ASCArry,u8 Length)
{
    u8 i;
    bool lb_Flg = false;
    BIN4toNBCD(ulBin,ASCArry,Length);
    for(i=0;i<Length;i++)
    {
        if (*ASCArry != 0 || lb_Flg == true)
        {
            (*ASCArry) +='0';
            lb_Flg = true;
        }   //最后一位0不能消隐 并且一旦有不为0的数之后，后面的0都不消隐
        else if (lb_Flg == false)
        {
            if (i == Length - 1)
                (*ASCArry) = '0';
            else
                (*ASCArry) = ' ';
        }
        ASCArry++;
    }
}

//u32转为ASCII码带小数点，消隐，存入数组中占7个字节 一般是重量显示转换
void BIN4toNASC_Point(u32 ulBin,u8* ASCArry,u8 Length, u32 Point)
{
    u8 i;
    
    *ASCArry = '0';
    BIN4toNASC(ulBin, ASCArry+1, Length);
    if (Point != 0)
    {//小数点不为0
        for (i = 0; i < Length - Point; i++)
            *(ASCArry+i) = *(ASCArry+i+1);
        *(ASCArry+i) = '.';
    }
    for (i = 0; i <= Length - 1; i++)
    {//消隐
        if (*ASCArry == '0' && *(ASCArry+1) != '.')
        {
            *ASCArry = ' ';
            ASCArry++;
        }
        else
            break;
    }
}

bool ASCto4Bin(u32* pu32Bin,u8* ASCArry,u8 Length)
{
    u8 i;
    bool SpaceEndFlg=false;
    bool PointGetedFlg=false;       //已经获得过一次小数点
    u32 u32Bin=0;

    if(Length>10){
        return false;
    }

    for(i=0;i<Length;i++){
        if(ASCArry[i]==' '){//高位空格作为0处理
            if(SpaceEndFlg){
                return false;
            }
            u32Bin*=10;
        }else if((!PointGetedFlg)&&(ASCArry[i]=='.')){//小数点
            if(PointGetedFlg){
                return false;
                }
            PointGetedFlg=true;
            //小数点位不需要处理
        }else if(ASCArry[i]>'9'||ASCArry[i]<'0'){
            return false;
        }else{
            SpaceEndFlg=true;
            u32Bin*=10;
            u32Bin+=(ASCArry[i]-'0');
        }
    }

    *pu32Bin=u32Bin;

    return true;
}

//ASCII码转成十六进制 chx 2014-3-31
void ASCtoHex(u8* ASCArry, u8* HexArry, u8 AscLen)
{
    u8 i;
    
    for (i = 0; i < AscLen; i++)
    {
        if (i % 2 == 0)
        {   //Hex高位
            if (*(ASCArry+i) <= '9')    //0~9的ASCII码
                *(HexArry+i/2) = (*(ASCArry+i) - '0');
            else    //A~F的ASCII码
                *(HexArry+i/2) = (*(ASCArry+i) - '7');
        }
        else
        {   //Hex低位
            if (*(ASCArry+i) <= '9')    //0~9的ASCII码
                *(HexArry+i/2) = (*(HexArry+i/2)<<4 | (*(ASCArry+i) - '0'));
            else    //A~F的ASCII码
                *(HexArry+i/2) = (*(HexArry+i/2)<<4 | (*(ASCArry+i) - '7'));
        }
        
    }
}

//十六进制码转成ASCII chx 2014-3-31
void HextoASC(u8* HexArry, u8* ASCArry, u8 HexLen)
{
    u8 i;
    if (HexLen == 0)
        return;
    //由大到小转换，这样HexArry和ASCArry就可以是同一个地址的指针了
    for (i = HexLen - 1; ; i--)
    {
        *(ASCArry+i*2+1) = (*(HexArry+i) & 0x0F) + (((*(HexArry+i) & 0x0F) > 9) ? '7' : '0');
        *(ASCArry+i*2) = (*(HexArry+i) >> 4) + (((*(HexArry+i) >> 4) > 9) ? '7' : '0');
        if (i == 0)
            break;
    }
}

