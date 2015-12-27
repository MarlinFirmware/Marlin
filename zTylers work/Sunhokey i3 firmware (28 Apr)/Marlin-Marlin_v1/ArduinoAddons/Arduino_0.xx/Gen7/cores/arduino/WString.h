/*
  WString.h - String library for Wiring & Arduino
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef String_h
#define String_h

//#include "WProgram.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class String
{
  public:
    // constructors
    String( const char *value = "" );
    String( const String &value );
    String( const char );
    String( const unsigned char );
    String( const int, const int base=10);
    String( const unsigned int, const int base=10 );
    String( const long, const int base=10 );
    String( const unsigned long, const int base=10 );
    ~String() { free(_buffer); _length = _capacity = 0;}     //added _length = _capacity = 0;

    // operators
    const String & operator = ( const String &rhs );
    const String & operator +=( const String &rhs );
    //const String & operator +=( const char );
    int operator ==( const String &rhs ) const;
    int	operator !=( const String &rhs ) const;
    int	operator < ( const String &rhs ) const;
    int	operator > ( const String &rhs ) const;
    int	operator <=( const String &rhs ) const;
    int	operator >=( const String &rhs ) const;
    char operator []( unsigned int index ) const;
    char& operator []( unsigned int index );
    //operator const char *() const { return _buffer; }
    
    // general methods
    char charAt( unsigned int index ) const;
    int	compareTo( const String &anotherString ) const;
    unsigned char endsWith( const String &suffix ) const;
    unsigned char equals( const String &anObject ) const;
    unsigned char equalsIgnoreCase( const String &anotherString ) const;
    int	indexOf( char ch ) const;
    int	indexOf( char ch, unsigned int fromIndex ) const;
    int	indexOf( const String &str ) const;
    int	indexOf( const String &str, unsigned int fromIndex ) const;
    int	lastIndexOf( char ch ) const;
    int	lastIndexOf( char ch, unsigned int fromIndex ) const;
    int	lastIndexOf( const String &str ) const;
    int	lastIndexOf( const String &str, unsigned int fromIndex ) const;
    const unsigned int length( ) const { return _length; }
    void setCharAt(unsigned int index, const char ch);
    unsigned char startsWith( const String &prefix ) const;
    unsigned char startsWith( const String &prefix, unsigned int toffset ) const;
    String substring( unsigned int beginIndex ) const;
    String substring( unsigned int beginIndex, unsigned int endIndex ) const;
    String toLowerCase( ) const;
    String toUpperCase( ) const;
    String trim( ) const;
    void getBytes(unsigned char *buf, unsigned int bufsize);
    void toCharArray(char *buf, unsigned int bufsize);
    long toInt( );
    const String& concat( const String &str );
    String replace( char oldChar, char newChar );
    String replace( const String& match, const String& replace );
    friend String operator + ( String lhs, const String &rhs );

  protected:
    char *_buffer;	     // the actual char array
    unsigned int _capacity;  // the array length minus one (for the '\0')
    unsigned int _length;    // the String length (not counting the '\0')

    void getBuffer(unsigned int maxStrLen);

  private:

};

// allocate buffer space
inline void String::getBuffer(unsigned int maxStrLen)
{
  _capacity = maxStrLen;
  _buffer = (char *) malloc(_capacity + 1);
  if (_buffer == NULL) _length = _capacity = 0;
}

inline String operator+( String lhs, const String &rhs )
{
  return lhs += rhs;
}


#endif
