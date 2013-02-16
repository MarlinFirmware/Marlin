
#ifndef Dogm_h
#define Dogm_h

#include <stdint.h>
#include "utility/dogm128.h"
#include "Print.h"

class Dogm : public Print {
    uint8_t a0Pin;
    uint8_t csPin;
    uint8_t is_req_init;	// if call to Init() is required
    uint8_t size;
    uint8_t rot;
    uint8_t libinfo_done;
    DOG_PGM_P fptr;
    void Init(void);
    void xy_char_correction(uint8_t);
  public:
    // text cursor position
    uint8_t tx, ty;
  
    Dogm(uint8_t pin_a0); 
    Dogm(uint8_t pin_a0, uint8_t pin_cs); 
      
    void start(void);
    uint8_t next(void) { return dog_NextPage(); }
    
    void setContrast(uint8_t val);
    void setInvertPixelMode(uint8_t val);
    
    void setPixelValue(uint8_t val) { dog_SetPixelValue(val); }


    
    void setPixel(uint8_t x, uint8_t y) { dog_SetPixel(x, y); }
    void clrPixel(uint8_t x, uint8_t y) { dog_ClrPixel(x, y); }
    void xorPixel(uint8_t x, uint8_t y) { dog_XorPixel(x,y); }	/* v1.01 */

    /* x1 must be lower or equal to x2 */
    void setHLine(uint8_t x1, uint8_t x2, uint8_t y) { dog_SetHLine(x1, x2, y); }/* v1.01 */
    void clrHLine(uint8_t x1, uint8_t x2, uint8_t y) { dog_ClrHLine(x1, x2, y); }/* v1.03 */
    void xorHLine(uint8_t x1, uint8_t x2, uint8_t y) { dog_XorHLine(x1, x2, y); }/* v1.03 */

    /* y1 must be lower or equal to y2 */
    void setVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_SetVLine(x, y1, y2); }
    void clrVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_ClrVLine(x, y1, y2); }
    void xorVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_XorVLine(x, y1, y2); }

    /* x1 must be lower or equal to x2 */
    /* y1 must be lower or equal to y2 */
    void setBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_SetBox(x1, y1, x2, y2); }
    void clrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_ClrBox(x1, y1, x2, y2); }
    void xorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_XorBox(x1, y1, x2, y2); }

    /* Bitmap */
    void setHBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w) { dog_SetHBitmap(x,y,bitmap,w); }/* v1.01 */
    void setHBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w) { dog_SetHBitmapP(x,y,bitmap,w); }/* v1.01 */
    void setBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h) { dog_SetBitmap(x,y,bitmap,w,h); } /* v1.01 */
    void setBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w, uint8_t h) { dog_SetBitmapP(x,y,bitmap,w,h); } /* v1.01 */
    
    /* Font */
    void setFont(DOG_PGM_P font) { fptr = font; }
    void setXY(uint8_t x, uint8_t y) { tx = x; ty = y; }
    void setRot(uint8_t rotation) { rot = rotation; }
    
    void drawChar(uint8_t c);
    void drawStr(const char *s);
    uint8_t getStrWidth(const char *s) { return dog_GetStrWidth(fptr, s); }
#if defined(ARDUINO) && ARDUINO >= 100
    size_t write(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); return 1;}
#else
    void write(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); }
#endif

    /* Draw functions */
    void setSize(uint8_t s) { size = s; }
    void drawPoint(uint8_t x, uint8_t y) {  dog_DrawPoint( x, y, size); }
    void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {dog_DrawLine(x1, y1, x2, y2, size);}
    void drawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1) {dog_DrawArc(mx, my, r, w0, w1, size);}
    
    /* splash screen */
    void showLibInfo(void);

};

#endif 
