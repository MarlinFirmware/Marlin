///////////////////////////////////////////////////////////////////////////////
/// \file Option.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Interface definition for Option objects.
///   
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com 
/// 
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///      
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef OPTION
#define OPTION

#include <stdint.h>

namespace ui
{
	namespace option
	{
		struct Size
		{
			Size(uint8_t width, uint8_t height)
				: width(width)
				, height(height)
			{ };

			uint8_t width;
			uint8_t height;
		};
	}

	class Option
	{
		public:
			Option(option::Size const & size, const char * text = 0);
			virtual ~Option();

			uint8_t const & width() const;
			uint8_t const & height() const;
			virtual const char * text() const;
			virtual void draw() = 0;

			virtual void press();
		
		protected:
			option::Size m_size;
			const char * m_text;
	};
}

#endif //OPTION
