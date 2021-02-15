// Read a two dimensional array from a CSV file.
//
#include <SPI.h>
#include <SdFat.h>
#define CS_PIN SS

// 5 X 4 array
#define ROW_DIM 5
#define COL_DIM 4

SdFat SD;
File file;

/*
 * Read a file one field at a time.
 *
 * file - File to read.
 *
 * str - Character array for the field.
 *
 * size - Size of str array.
 *
 * delim - String containing field delimiters.
 *
 * return - length of field including terminating delimiter.
 *
 * Note, the last character of str will not be a delimiter if
 * a read error occurs, the field is too long, or the file
 * does not end with a delimiter.  Consider this an error
 * if not at end-of-file.
 *
 */
size_t readField(File* file, char* str, size_t size, const char* delim) {
  char ch;
  size_t n = 0;
  while ((n + 1) < size && file->read(&ch, 1) == 1) {
    // Delete CR.
    if (ch == '\r') {
      continue;
    }
    str[n++] = ch;
    if (strchr(delim, ch)) {
        break;
    }
  }
  str[n] = '\0';
  return n;
}
//------------------------------------------------------------------------------
#define errorHalt(msg) {Serial.println(F(msg)); SysCall::halt();}
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  Serial.println("Type any character to start");
  while (!Serial.available()) {
    SysCall::yield();
  }
  // Initialize the SD.
  if (!SD.begin(CS_PIN)) {
    errorHalt("begin failed");
  } 
  // Create or open the file.
  file = SD.open("READNUM.TXT", FILE_WRITE);
  if (!file) {
    errorHalt("open failed");
  }
  // Rewind file so test data is not appended.
  file.rewind();

  // Write test data.
  file.print(F(
    "11,12,13,14\r\n"
    "21,22,23,24\r\n"
    "31,32,33,34\r\n"
    "41,42,43,44\r\n"
    "51,52,53,54"     // Allow missing endl at eof.
    ));

  // Rewind the file for read.
  file.rewind();

  // Array for data.
  int array[ROW_DIM][COL_DIM];
  int i = 0;     // First array index.
  int j = 0;     // Second array index
  size_t n;      // Length of returned field with delimiter.
  char str[20];  // Must hold longest field with delimiter and zero byte.
  char *ptr;     // Test for valid field.

  // Read the file and store the data.
  
  for (i = 0; i < ROW_DIM; i++) {
    for (j = 0; j < COL_DIM; j++) {
      n = readField(&file, str, sizeof(str), ",\n");
      if (n == 0) {
        errorHalt("Too few lines");
      }
      array[i][j] = strtol(str, &ptr, 10);
      if (ptr == str) {
        errorHalt("bad number");
      }
      while (*ptr == ' ') {
        ptr++;
      }
      if (*ptr != ',' && *ptr != '\n' && *ptr != '\0') {
        errorHalt("extra characters in field");
      }
      if (j < (COL_DIM-1) && str[n-1] != ',') {
        errorHalt("line with too few fields");
      }
    }
    // Allow missing endl at eof.
    if (str[n-1] != '\n' && file.available()) {
      errorHalt("missing endl");
    }    
  }

  // Print the array.
  for (i = 0; i < ROW_DIM; i++) {
    for (j = 0; j < COL_DIM; j++) {
      if (j) {
        Serial.print(' ');
      }
      Serial.print(array[i][j]);
    }
    Serial.println();
  }
  Serial.println("Done");
  file.close();
}
//------------------------------------------------------------------------------
void loop() {
}

