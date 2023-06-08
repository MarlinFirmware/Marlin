#ifndef UTF8_UNICODE_H
#define UTF8_UNICODE_H

int enc_utf8_to_unicode(const char* pInput, unsigned short int *Unic);

int enc_utf8_to_unicode_one(const unsigned char* pInput, unsigned short int *Unic);

int enc_get_utf8_size(const unsigned char pInput);

#endif /* BASE64_H */