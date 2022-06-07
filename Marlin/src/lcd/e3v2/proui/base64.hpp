/**
 * Base64 encoder/decoder for arduino repo
 * Uses common web conventions - '+' for 62, '/' for 63, '=' for padding.
 * Note that invalid base64 characters are interpreted as padding.
 * Author: Densaugeo
 * Maintainer: Densaugeo
 * Version: 1.2.1.1
 * Changed unsigned int to uint16_t for use in the professional Ender 3V2/S1 firmware
 * Url: https://www.arduino.cc/reference/en/libraries/base64/
 */

#ifndef BASE64_H_INCLUDED
#define BASE64_H_INCLUDED

/* binary_to_base64:
 *   Description:
 *     Converts a single byte from a binary value to the corresponding base64 character
 *   Parameters:
 *     v - Byte to convert
 *   Returns:
 *     ascii code of base64 character. If byte is >= 64, then there is not corresponding base64 character
 *     and 255 is returned
 */
unsigned char binary_to_base64(unsigned char v);

/* base64_to_binary:
 *   Description:
 *     Converts a single byte from a base64 character to the corresponding binary value
 *   Parameters:
 *     c - Base64 character (as ascii code)
 *   Returns:
 *     6-bit binary value
 */
unsigned char base64_to_binary(unsigned char c);

/* encode_base64_length:
 *   Description:
 *     Calculates length of base64 string needed for a given number of binary bytes
 *   Parameters:
 *     input_length - Amount of binary data in bytes
 *   Returns:
 *     Number of base64 characters needed to encode input_length bytes of binary data
 */
uint16_t encode_base64_length(uint16_t input_length);

/* decode_base64_length:
 *   Description:
 *     Calculates number of bytes of binary data in a base64 string
 *     Variant that does not use input_length no longer used within library, retained for API compatibility
 *   Parameters:
 *     input - Base64-encoded null-terminated string
 *     input_length (optional) - Number of bytes to read from input pointer
 *   Returns:
 *     Number of bytes of binary data in input
 */
uint16_t decode_base64_length(unsigned char input[]);
uint16_t decode_base64_length(unsigned char input[], uint16_t input_length);

/* encode_base64:
 *   Description:
 *     Converts an array of bytes to a base64 null-terminated string
 *   Parameters:
 *     input - Pointer to input data
 *     input_length - Number of bytes to read from input pointer
 *     output - Pointer to output string. Null terminator will be added automatically
 *   Returns:
 *     Length of encoded string in bytes (not including null terminator)
 */
uint16_t encode_base64(unsigned char input[], uint16_t input_length, unsigned char output[]);

/* decode_base64:
 *   Description:
 *     Converts a base64 null-terminated string to an array of bytes
 *   Parameters:
 *     input - Pointer to input string
 *     input_length (optional) - Number of bytes to read from input pointer
 *     output - Pointer to output array
 *   Returns:
 *     Number of bytes in the decoded binary
 */
uint16_t decode_base64(unsigned char input[], unsigned char output[]);
uint16_t decode_base64(unsigned char input[], uint16_t input_length, unsigned char output[]);

unsigned char binary_to_base64(unsigned char v) {
  // Capital letters - 'A' is ascii 65 and base64 0
  if (v < 26) return v + 'A';

  // Lowercase letters - 'a' is ascii 97 and base64 26
  if (v < 52) return v + 71;

  // Digits - '0' is ascii 48 and base64 52
  if (v < 62) return v - 4;

  // '+' is ascii 43 and base64 62
  if (v == 62) return '+';

  // '/' is ascii 47 and base64 63
  if (v == 63) return '/';

  return 64;
}

unsigned char base64_to_binary(unsigned char c) {
  // Capital letters - 'A' is ascii 65 and base64 0
  if ('A' <= c && c <= 'Z') return c - 'A';

  // Lowercase letters - 'a' is ascii 97 and base64 26
  if ('a' <= c && c <= 'z') return c - 71;

  // Digits - '0' is ascii 48 and base64 52
  if ('0' <= c && c <= '9') return c + 4;

  // '+' is ascii 43 and base64 62
  if (c == '+') return 62;

  // '/' is ascii 47 and base64 63
  if (c == '/') return 63;

  return 255;
}

uint16_t encode_base64_length(uint16_t input_length) {
  return (input_length + 2)/3*4;
}

uint16_t decode_base64_length(unsigned char input[]) {
  return decode_base64_length(input, -1);
}

uint16_t decode_base64_length(unsigned char input[], uint16_t input_length) {
  unsigned char *start = input;

  while (base64_to_binary(input[0]) < 64 && (unsigned char)(input - start) < input_length) {
    ++input;
  }

  input_length = input - start;
  return input_length/4*3 + (input_length % 4 ? input_length % 4 - 1 : 0);
}

uint16_t encode_base64(unsigned char input[], uint16_t input_length, unsigned char output[]) {
  uint16_t full_sets = input_length/3;

  // While there are still full sets of 24 bits...
  for (uint16_t i = 0; i < full_sets; ++i) {
    output[0] = binary_to_base64(                         input[0] >> 2);
    output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
    output[2] = binary_to_base64((input[1] & 0x0F) << 2 | input[2] >> 6);
    output[3] = binary_to_base64( input[2] & 0x3F);

    input += 3;
    output += 4;
  }

  switch(input_length % 3) {
    case 0:
      output[0] = '\0';
      break;
    case 1:
      output[0] = binary_to_base64(                         input[0] >> 2);
      output[1] = binary_to_base64((input[0] & 0x03) << 4);
      output[2] = '=';
      output[3] = '=';
      output[4] = '\0';
      break;
    case 2:
      output[0] = binary_to_base64(                         input[0] >> 2);
      output[1] = binary_to_base64((input[0] & 0x03) << 4 | input[1] >> 4);
      output[2] = binary_to_base64((input[1] & 0x0F) << 2);
      output[3] = '=';
      output[4] = '\0';
      break;
  }

  return encode_base64_length(input_length);
}

uint16_t decode_base64(unsigned char input[], unsigned char output[]) {
  return decode_base64(input, -1, output);
}

uint16_t decode_base64(unsigned char input[], uint16_t input_length, unsigned char output[]) {
  uint16_t output_length = decode_base64_length(input, input_length);

  // While there are still full sets of 24 bits...
  for (uint16_t i = 2; i < output_length; i += 3) {
    output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
    output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
    output[2] = base64_to_binary(input[2]) << 6 | base64_to_binary(input[3]);

    input += 4;
    output += 3;
  }

  switch(output_length % 3) {
    case 1:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      break;
    case 2:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
      break;
  }

  return output_length;
}

#endif // ifndef
