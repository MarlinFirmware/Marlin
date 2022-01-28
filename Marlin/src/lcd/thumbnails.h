
#pragma once

#include "../inc/MarlinConfig.h"

#if ENABLED(THUMBNAILS_PREVIEW)

#include "../sd/cardreader.h"

#include "../module/PNGdec/PNGdec.h"

#define		THUMB_MAX_WIDTH		640
#define		THUMB_MIN_WIDTH		100

typedef struct
{
	uint32_t	img_width;
	uint32_t	img_height;
	uint32_t	img_base64_size;
	uint32_t	srcfile_begin_pos;
	uint32_t	buff_ready_decbytes;
	uint32_t	buff_current_decpos;
	uint32_t	buff_readed_rawbytes;
	uint32_t	imgfile_pos;
	uint32_t	remain_rawbytes;
	uint32_t	draw_width;
	uint32_t	draw_height;
	float		scale;
	int32_t		old_draw_y;
} DECODE_INFO;



class Thumbnails
{
    private:
 
        DECODE_INFO         decode_info;
        PNG			        png;
        uint16_t     		linebuff[THUMB_MAX_WIDTH*2];
        uint8_t		        strbuff[256];
        uint8_t		        binbuff[256];
        bool                is_thumb;

        int32_t		        ReadLine(uint8_t* buff, uint32_t buffsize);
        uint32_t            base64_read(uint8_t* buff,	uint32_t size);
        void                base64_decode_block(uint8_t *in, uint8_t *out);

        static void*		PNGOpen(const char *filename, int32_t *size);
        static void		    PNGClose(void *handle);
        static int32_t		PNGRead(PNGFILE *handle, uint8_t *buffer, int32_t length);
        static int32_t		PNGSeek(PNGFILE *handle, int32_t position);
        static void		    PNGDraw(PNGDRAW *pDraw);

    public:
        Thumbnails();
        
        bool		        Open(char *fname);
        void		        Close();
        void                DrawThumbnail(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
        void                DrawDefaultThumbnail(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

};



extern Thumbnails      thumbnails;


#endif  // ENABLED(THUMBNAILS_PREVIEW)