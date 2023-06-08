#ifndef PREWIEW_H
#define PREWIEW_H

/*****************  gcode内嵌预览图读取相关定义(开始)  ******************/
// 图片文件标识位(长度4字节)
#define PIC_HEADER      "begin"
// #define PIC_PRESENT_LEN	4
// 图片长度位(长度4字节)
// #define PIC_DATA_LEN	4
// 图片类型(长度1字节)
// #define PIC_FORMAT_LEN	1		// 图片格式长度（字节）
enum{
	PIC_FORMAT_JPG    = 0x00, // jpg格式图片
	PIC_FORMAT_PNG    = 0x01, // png格式图片	
	PIC_FORMAT_MAX    = 0x02, // 
};
#define FORMAT_JPG "jpg"
#define FORMAT_PNG "png"
// 分辨率(长度1字节)
// #define PIC_RESOLITION_LEN		1		// 图片分辨率长度（字节）
enum{
	PIC_RESOLITION_36_36   = 0x00, // 分辨率 = 36*36	
	PIC_RESOLITION_48_48   = 0x01,	// 分辨率 = 48*48	
    PIC_RESOLITION_64_64   = 0x02,	// 分辨率 = 64*64
    PIC_RESOLITION_96_96   = 0x03,	// 分辨率 = 96*96
    PIC_RESOLITION_144_144 = 0x04,	// 分辨率 = 144*144
    PIC_RESOLITION_200_200 = 0x05,	// 分辨率 = 200*200
	PIC_RESOLITION_300_300 = 0x06,	// 分辨率 = 300*300
    PIC_RESOLITION_600_600,
	PIC_RESOLITION_MAX,		// gcode无图片
};
typedef struct
{
    unsigned long addr;         // 变量地址
    unsigned long spAddr;       // 描述指针
    unsigned int  brightness;   // 亮度（0x0000 - 0x0100, 单位为 1/256）
    unsigned int  LeftUp_X;     // 显示区域的左上角X
    unsigned int  LeftUp_Y;     // 显示区域的左上角Y
    unsigned int  RightDown_X;  // 显示区域的右下角X
    unsigned int  RightDown_Y;  // 显示区域的右下角Y
}DwinBrightness_t;
#define RESOLITION_36_36    "36*36"
#define RESOLITION_48_48    "48*48"
#define RESOLITION_64_64    "64*64"
#define RESOLITION_96_96    "96*96"
#define RESOLITION_144_144  "144*144"
#define RESOLITION_200_200  "200*200"
#define RESOLITION_300_300  "300*300"
#define RESOLITION_600_600  "600*600"
#define VP_BRIGHTNESS_PRINT               0x8800

#define VP_OVERLAY_PIC_PTINT              0xB000              /* 打印界面的预览图 */

/* 打印界面的模型亮度，用于表示倒计时的一种方式(占40字节，0xA000 ~ 0xA01F) */
#define SP_ADDR_BRIGHTNESS_PRINT            0x9000
// // 模型行起始位(2字节)
// #define PIC_START_LINE_LEN		2		// 图片起始行长度（字节）
// // 模型行结束位(2字节)
// #define PIC_END_LINE_LEN		2		// 图片结束行长度（字节）
// // 模型高度(2字节)
// #define PIC_HIGH_LINE_LEN		2		// 图片高度长度（字节）
// // 图片数据前引导位
// #define DATA_BEFOR_PIC_LENTH	(PIC_FORMAT_LEN + PIC_RESOLITION_LEN + PIC_START_LINE_LEN + PIC_END_LINE_LEN + PIC_HIGH_LINE_LEN)
// 函数返回信息
enum{
	PIC_OK,				// 图片显示ok
	PIC_FORMAT_ERR,		// 图片格式错误
	PIC_RESOLITION_ERR,	// 图片分辨率错误
	PIC_MISS_ERR,		// gcode无图片
};
      
#define PRIWIEW_PIC_FORMAT_NEED         PIC_FORMAT_JPG
#define PRIWIEW_PIC_RESOLITION_NEED     PIC_RESOLITION_96_96
#define PRINT_PIC_FORMAT_NEED           PIC_FORMAT_JPG
#define PRINT_PIC_RESOLITION_NEED       PIC_RESOLITION_300_300

#define DACAI_PRIWIEW_PIC_ADDR1  "3:/20.jpg"        // 大彩文件选择预览图1
#define DACAI_PRIWIEW_PIC_ADDR2  "3:/22.jpg"        // 大彩文件选择预览图2
#define DACAI_PRIWIEW_PIC_ADDR3  "3:/23.jpg"        // 大彩文件选择预览图3
#define DACAI_PRIWIEW_PIC_ADDR4  "3:/24.jpg"        // 大彩文件选择预览图4
#define DACAI_PRINT_PIC_ADDR     "3:/31.jpg"        // 大彩打印预览图1

/*****************  gcode内嵌预览图读取相关定义(结束)  ******************/
void RefreshBrightnessAtPrint(uint16_t persent);
bool gcodePicGetDataFormBase64(char * buf, unsigned long picLen, bool resetFlag);
bool gcodePicDataRead(unsigned long picLenth, char isDisplay, unsigned long jpgAddr);
char gcodePicExistjudge(char *fileName, unsigned int targitPicAddr, const char targitPicFormat, const char targitPicResolution);
char gcodePicDataSendToDwin(char *fileName, unsigned int jpgAddr, unsigned char jpgFormat, unsigned char jpgResolution);
void gcodePicDispalyOnOff(unsigned int jpgAddr, bool onoff);
#endif