///////////////////////////////////////////////////////////////////////////////
/// \file LightManager.h
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza
///         Joaquin Herrero
///
/// \brief Management class for the state of the printer's light.
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

#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "Singleton.h"
#include "Subject.h"

#include <stdint.h>

class LightManager : public Subject<uint8_t>
{
	public:
		typedef Singleton<LightManager> single;

	public:
		LightManager();

		void state(bool state);
		bool state();

		static void setMode();
		uint8_t getMode();

		void notify();

	private:
		void mode(uint8_t mode);
		uint8_t mode();

	private:
		bool m_state;
		uint8_t m_mode;
};

#endif //LIGHT_MANAGER_H
