#ifndef _LV_DRAW_PRINT_FILE_
#define _LV_DRAW_PRINT_FILE_

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

typedef struct 
{
	int cur_page_first_offset;
	int cur_page_last_offset;
	int curPage;
} DIR_OFFSET;
extern DIR_OFFSET dir_offset[10];

#define FILE_NUM 6
#define SHORT_NEME_LEN 13
#define NAME_CUT_LEN 23

#define MAX_DIR_LEVEL  10

typedef struct
{
  //char longName[FILE_NUM][LONG_FILENAME_LENGTH];
  char file_name[FILE_NUM][SHORT_NEME_LEN*MAX_DIR_LEVEL+1];
  char curDirPath[SHORT_NEME_LEN*MAX_DIR_LEVEL+1];
  char long_name[FILE_NUM][SHORT_NEME_LEN*2+1];
  char IsFolder[FILE_NUM];
  char Sd_file_cnt;
  char sd_file_index;
  char Sd_file_offset;
}LIST_FILE;
extern LIST_FILE list_file;

extern void disp_gcode_icon(uint8_t file_num);
extern void lv_draw_print_file(void);
extern void lv_open_gcode_file(char *path);
extern void lv_gcode_file_read(uint8_t *data_buf);
extern void lv_close_gcode_file();
extern void cutFileName(char *path, int len, int bytePerLine,  char *outStr);
extern int ascii2dec_test(char *ascii);
extern void lv_clear_print_file();

//extern void disp_temp_ready_print();
#if defined(__cplusplus)
}    /* Make sure we have C-declarations in C++ programs */
#endif

#endif

