#include "../inc/MarlinConfig.h"
#ifdef LONGER3D
#ifdef Chinese
#include "gbk_font16.h"
#include "LGT_MACRO.h"
#include "W25Qxx.h"
extern  W25Qxx W25QxxFlash;
#include "display_image.h"
extern display_image Display_Screen;
#include "../../lcd/menu/touch/lcd.h"
extern uint16_t color,bgColor;

void lcd_print_gbk(uint16_t x, uint16_t y,char *str)
{
    uint16_t x0=x,y0=y;
    uint8_t bHz=0; 
    uint16_t width=200;
    uint16_t height=16;
    while (*str!=0)
    {
        if(!bHz)
        {
            uint16_t str1=*str;
            if(str1>0x80) {bHz=1;}//中文
            else              //字符
			{      
				if(x>(x0+width-8))//换行
			    {				   
					y+=16;
					x=x0;	   
				}							    
				if(y>(y0+height-8)) break; //越界返回
				if(str1==13) //换行符号
	            {         
					y+=16;
					x=x0;
					str++; 
				}  
				else lcdPrint(x,y,str) ;
				str++; 
				x+=8; //字符，为全字的一半
			}
        }
        else   //中文
        {
             bHz=0;
            if(x>(x0+width-16))  //换行
            {	    
				y+=16;
				x=x0;		  
			}
            if(y>(y0+height-16)) break;//越界返回
            SHOW_FONT16(x,y,str);  //显示汉字
            str+=2;
            x+=16;       //下一个汉字偏移
        }
    }
    
}
//（x,y）:显示起始坐标
// font  :要显示的汉字
void SHOW_FONT16(uint16_t x, uint16_t y, char *font)
{
    uint8_t dzk[72],temp;
    //uint8_t csize=(16/8+((16%8)?1:0))*(16);  //得到字体一个字符对应点阵集所占的字节数
    uint8_t csize=32;
    uint16_t y0=y;
    Get_HzMat(font,dzk,(uint8_t)16);   //得到相应大小的点阵数据
    for(uint8_t i=0;i<csize;i++)
    {
        temp=dzk[i];
        for(uint8_t j=0;j<8;j++)
        {
            if(temp&0x80)
            {
              //  Display_Screen.LCD_DrawPoint(x,y);
            LCD_SetCursor(x,y);	
	        LCD_WriteRAM_Prepare();	
	        LCD_WriteValue_LCD_RAM(color);   
            }
            else
             {
                // Display_Screen.LCD_DrawPoint(x,y);
                LCD_SetCursor(x,y);	
                LCD_WriteRAM_Prepare();	
                LCD_WriteValue_LCD_RAM(bgColor);   
             }   
            temp<<=1;
             y++;
            if((y-y0)==16)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
}
//从字库中查找出字模
//code：字符串的开始地址
//mat:数据存放地址   (size/8+((size%8)?1:0))*(size) bytes
void Get_HzMat(char *code,uint8_t* mat,uint8_t size)
{
    unsigned char qh,ql;
    uint8_t csize=32,i;
    unsigned long foffset; 
    qh=*code;
	ql=*(++code);
    if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//非 常用汉字
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	    return; //结束访问
	}   
    if(ql<0x7f) ql-=0x40;
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	//字库中的字节偏移量
    W25QxxFlash.W25QXX_Read(mat,foffset+FONT16_ADDR_GBK,csize);
}
#endif
#endif