/*
  Stream.h - base class for character-based streams.
  Copyright (c) 2010 David A. Mellis.  All right reserved.

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

#ifndef Stream_h
#define Stream_h

#include <inttypes.h>
#include "Print.h"

class Stream : public Print
{
  public:
	Stream() : _timeout(1000), read_error(0) {}
	virtual int available() = 0;
	virtual int read() = 0;
	virtual int peek() = 0;
	virtual void flush() = 0;

	void setTimeout(unsigned long timeout);
	bool find(char *target);
	bool find(char *target, size_t length);
	bool findUntil(char *target, char *terminator);
	bool findUntil(char *target, size_t targetLen, char *terminate, size_t termLen);
	long parseInt();
	long parseInt(char skipChar);
	float parseFloat();
	float parseFloat(char skipChar);
	size_t readBytes(char *buffer, size_t length);
	size_t readBytesUntil(char terminator, char *buffer, size_t length);
	String readString(size_t max = 120);
	String readStringUntil(char terminator, size_t max = 120);
	int getReadError() { return read_error; }
	void clearReadError() { setReadError(0); }
  protected:
	void setReadError(int err = 1) { read_error = err; }
	unsigned long _timeout;
  private:
	char read_error;
	int timedRead();
	int timedPeek();
	int peekNextDigit();
};

#endif	
