

#include "../inc/MarlinConfig.h"
#include "tft/tft.h"

#if ENABLED(THUMBNAILS_PREVIEW)

#include "thumbnails.h"
#include "tft/images/empty_tumbnail_img.h"

Thumbnails      thumbnails;



Thumbnails::Thumbnails()
{
	memset(&decode_info, 0, sizeof(decode_info));
	decode_info.old_draw_y = -1;
}



bool    Thumbnails::Open(char *fname)
{
 	const char	TSTRING[] = "thumbnail begin ";
	const uint32_t	tstrlen = sizeof(TSTRING)-1;

    int32_t		linereaded = 0;
    char		*tpos;

    if (!card.isFileOpen())
    {
        card.closefile();
    }
	
    memset(&decode_info, 0, sizeof(decode_info));
	decode_info.old_draw_y = -1;

    card.openFileRead(fname, 9);
    if (!card.isFileOpen())
    {
        return FALSE;
    }

	// ищем признаки встроенного предпросмотра
	for (uint32_t i = 0; i < 256; i++)
	{
		linereaded = ReadLine(strbuff, 255);
		if (linereaded == -1)
			return FALSE;
		strbuff[linereaded] = 0;
		
        tpos = strstr((char*)strbuff, TSTRING);
		if (tpos != NULL)
		{
			tpos += tstrlen;
			sscanf(tpos, "%lux%lu %lu", &decode_info.img_width, &decode_info.img_height, &decode_info.img_base64_size);
			if (decode_info.img_width >= THUMB_MIN_WIDTH && decode_info.img_width <= THUMB_MAX_WIDTH)
				break;
			else
				decode_info.img_base64_size = 0;
		}
	}

	// не нашли
	if (decode_info.img_base64_size == 0)
    {
        card.closefile();
		return FALSE;
    }

	// запоминаем позицию начала кодированного PNG в файле
	decode_info.srcfile_begin_pos = card.getIndex();

    return TRUE;
}




void    Thumbnails::Close()
{
    if (decode_info.img_base64_size == 0)
        return;

    DEBUG("Thumbnails: closing");

    png.close();
    card.closefile();
}




void    Thumbnails::DrawThumbnail(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    if (decode_info.img_base64_size == 0)
        return;
    
	// вычисляем масштабирование
	float s1 = (float)decode_info.img_width / (float)w;
	float s2 = (float)decode_info.img_height / (float)h;
	if (s1 > s2)
		decode_info.scale = s1;
	else
		decode_info.scale = s2;

    // пересчитываем координаты верхнего левого угла с учетов масштабирования
    decode_info.draw_width = decode_info.img_width / decode_info.scale;
    decode_info.draw_height = decode_info.img_height / decode_info.scale;
    if (decode_info.draw_width < w)
        x += (w - decode_info.draw_width) / 2;
    if (decode_info.draw_height < h)
        y += (h - decode_info.draw_height) / 2;

    int rc = png.open("", PNGOpen, PNGClose, PNGRead, PNGSeek, PNGDraw);
    if (rc != PNG_SUCCESS)
    {
        return;
    }

    tft.set_window(x, y, x+decode_info.draw_width-1, y+decode_info.draw_height-1);
//    tft.set_window(x, y, x+w, y+h);
    rc = png.decode(NULL, 0);
}




void    Thumbnails::DrawDefaultThumbnail(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    int rc = png.openFLASH((uint8_t*)empty_tumbnail_img, sizeof(empty_tumbnail_img), PNGDraw);
    if (rc != PNG_SUCCESS)
    {
        return;
    }

    decode_info.img_width = png.getWidth();
    decode_info.img_height = png.getHeight();

	// вычисляем масштабирование
	float s1 = (float)decode_info.img_width / (float)w;
	float s2 = (float)decode_info.img_height / (float)h;
	if (s1 > s2)
		decode_info.scale = s1;
	else
		decode_info.scale = s2;

    // пересчитываем координаты верхнего левого угла с учетов масштабирования
    decode_info.draw_width = decode_info.img_width / decode_info.scale;
    decode_info.draw_height = decode_info.img_height / decode_info.scale;
    if (decode_info.draw_width < w)
        x += (w - decode_info.draw_width) / 2;
    if (decode_info.draw_height < h)
        y += (h - decode_info.draw_height) / 2;


    tft.set_window(x, y, x+decode_info.draw_width-1, y+decode_info.draw_height-1);
//    tft.set_window(x, y, x+w, y+h);
    rc = png.decode(NULL, 0);
}




int32_t    Thumbnails::ReadLine(uint8_t* buff, uint32_t buffsize)
{
	int16_t	    rd = 0;
	int32_t		pos = 0;
	uint32_t	file_index = card.getIndex();

    rd = card.read(buff, buffsize);
	if (rd > 0)
	{
		// ищем конец строки в считанных данных
		while (pos < rd && buff[pos] != '\n')
		{
			pos++;
		}
		card.setIndex(file_index + pos + 1);
	}
    else
    {
        return -1;
    }

	return pos;
}




void    Thumbnails::base64_decode_block(uint8_t *in, uint8_t *out )
{   
    out[0] = (uint8_t) (in[0] << 2 | in[1] >> 4);
    out[1] = (uint8_t) (in[1] << 4 | in[2] >> 2);
    out[2] = (uint8_t) (((in[2] << 6) & 0xc0) | in[3]);
}




uint32_t    Thumbnails::base64_read(uint8_t* buff,	uint32_t size)
{
	static const char cd64[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
	uint32_t	count = 0;
	int16_t		rd;

	while (count < size)
	{
		// переносим декодированные данные в буфер JPEG
		while (decode_info.buff_current_decpos < decode_info.buff_ready_decbytes)
		{
			if (buff)
				buff[count] = binbuff[decode_info.buff_current_decpos];
			count++;
			decode_info.buff_current_decpos++;
			decode_info.imgfile_pos++;

			if (count >= size)
				break;
		}
		if (count >= size)
			break;

		// читаем и декодируем из base64 очередную порцию
		decode_info.buff_current_decpos = 0;
		decode_info.buff_ready_decbytes = 0;
		// считываем сырые данные
        rd = card.read(strbuff + decode_info.remain_rawbytes, 240);
		if (rd <= 0)
			return 0;
        
		decode_info.buff_readed_rawbytes = decode_info.remain_rawbytes + rd;

		// чистим считанное от лишних символов (переводы строки, пробелы и т.п.)
		uint32_t src_pos = 0, dst_pos = 0;
		while (src_pos < decode_info.buff_readed_rawbytes)
		{
			if ((strbuff[src_pos] > 47 && strbuff[src_pos] < 58)			// 0-9
				|| (strbuff[src_pos] > 64 && strbuff[src_pos] < 91)			// A-Z
				|| (strbuff[src_pos] > 96 && strbuff[src_pos] < 123)		// a-z
				|| strbuff[src_pos] == 43									// +
				|| strbuff[src_pos] == 47									// /
				)
			{
				strbuff[dst_pos++] = strbuff[src_pos++];
			}
			else
			{
				src_pos++;
			}
				
		}
		decode_info.buff_readed_rawbytes = dst_pos;
		dst_pos = 0;
		src_pos = 0;
		if (decode_info.buff_readed_rawbytes > 3)
		{
			rd = decode_info.buff_readed_rawbytes >> 2;
			rd <<= 2;
			decode_info.remain_rawbytes = decode_info.buff_readed_rawbytes % 4;
		}
		else
		{
			rd = decode_info.buff_readed_rawbytes;
			decode_info.remain_rawbytes = 0;
		}

		// декодируем из base64
		uint8_t		len = 0, i = 0, v = 0;
		uint8_t		in[4];
		uint8_t		out[3];
		while(src_pos < (uint32_t)rd)
		{
			for(len = 0, i = 0; i < 4 && src_pos < (uint32_t)rd; i++)
			{
				v = 0;
				v = strbuff[src_pos++];
				decode_info.buff_readed_rawbytes--;
				v = cd64[ v - 43 ];
				if( v != 0 )
				{
					v = ((v == '$') ? 0 : v - 61);
				}
				len++;
				if( v != 0 )
				{
					in[i] = v - 1;
				}
			}
			if(len > 0)
			{
				base64_decode_block( in, out );
				for( i = 0; i < len - 1; i++ )
				{
					binbuff[dst_pos] = out[i];
					dst_pos++;
				}
				decode_info.buff_ready_decbytes += len - 1;
			}
		}
		if (decode_info.remain_rawbytes > 0)
		{
			memcpy(strbuff, strbuff+src_pos, decode_info.remain_rawbytes);
		}
	}

	return count;
}




void*    Thumbnails::PNGOpen(const char *filename, int32_t *size)
{
	*size = thumbnails.decode_info.img_base64_size / 4 * 3;
	return &thumbnails.decode_info;
}




void    Thumbnails::PNGClose(void *handle)
{
}




int32_t    Thumbnails::PNGRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
//    DEBUG("Thumbnails: data read: length=%lu, fpos=%lu", length, thumbnails.decode_info.imgfile_pos);

	uint32_t res = thumbnails.base64_read(buffer, length);
	return res;
}




int32_t    Thumbnails::PNGSeek(PNGFILE *handle, int32_t position)
{
//    DEBUG("Thumbnails: file seek: pos=%ld", position);

    uint32_t length = 0;

	if ((uint32_t)position >= thumbnails.decode_info.imgfile_pos)
	{
		length = position - thumbnails.decode_info.imgfile_pos;
	}
	else
	{
		card.setIndex(thumbnails.decode_info.srcfile_begin_pos);
		length = position;
		thumbnails.decode_info.buff_current_decpos = 0;
		thumbnails.decode_info.buff_ready_decbytes = 0;
		thumbnails.decode_info.buff_readed_rawbytes = 0;
		thumbnails.decode_info.imgfile_pos = 0;
		thumbnails.decode_info.remain_rawbytes = 0;
	}

	

	thumbnails.base64_read(NULL, length);
	return thumbnails.decode_info.imgfile_pos;
}




// Function to draw pixels to the display
void    Thumbnails::PNGDraw(PNGDRAW *pDraw)
{
    // DEBUG("Thumbnails: draw: y=%ld, w=%ld", pDraw->y, pDraw->iWidth);

    uint16_t    *spixline = thumbnails.linebuff;
    uint16_t    *dpixline = thumbnails.linebuff + THUMB_MAX_WIDTH;
	int32_t	    curr_x = -1, new_x = 0, curr_y = 0, src_x = 0;

	curr_y = (uint32_t)((float)pDraw->y / thumbnails.decode_info.scale);
    if (curr_y == thumbnails.decode_info.old_draw_y)
        return;
    
    // конвертируем пиксели в 16 бит
    thumbnails.png.getLineAsRGB565(pDraw, spixline, PNG_RGB565_LITTLE_ENDIAN, 0xFFFFFFFF);

	while (thumbnails.decode_info.old_draw_y < curr_y)
	{
		while (src_x < pDraw->iWidth)
		{
			new_x = (uint32_t)((float)src_x / thumbnails.decode_info.scale);
			while (new_x > curr_x)
			{
				curr_x++;
				dpixline[curr_x] = spixline[src_x];
			}
			src_x++;
		}
		thumbnails.decode_info.old_draw_y++;
        tft.write_sequence(dpixline, thumbnails.decode_info.draw_width);
	}

}




#endif  // ENABLED(THUMBNAILS_PREVIEW)