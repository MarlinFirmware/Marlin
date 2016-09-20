///////////////////////////////////////////////////////////////////////////////
/// \file HeatedbedManager.h
///
/// \author Koldo Imanol de Miguel
///
/// \brief Management class for the state of the heated bed.
///
/// Copyright (c) 2016 BQ - Mundo Reader S.L.
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

#ifndef HEATEDBED_MANAGER
#define HEATEDBED_MANAGER

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class HeatedbedManager : public Subject<uint8_t>
{
	public:
		typedef Singleton<HeatedbedManager> single;
		
	public:
		HeatedbedManager();
		
		static void toggleMode();
		uint8_t getMode();
		
		bool detected();
		void disableHeatedbed();
		
		void notify();
	
	private:
		void mode(uint8_t mode);
		uint8_t mode();
	
	private:
		bool m_bed_detected;
		uint8_t m_mode;
};

#endif // HEATEDBED_MANAGER
