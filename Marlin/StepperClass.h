///////////////////////////////////////////////////////////////////////////////
/// \file StepperClass.h
///
/// \author Ruy Garcia
///
/// \brief Abstraction class for steppers.
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

#ifndef STEPPER_CLASS_H
#define STEPPER_CLASS_H

#include <stdint.h>

class Stepper
{
	public:
		Stepper() {};
		Stepper(uint8_t pin, bool polarity);
		~Stepper();

		bool isEnabled();

		void enable();
		void disable();

	private:
		bool m_enabled;

		uint8_t m_enable_pin;
		bool m_enable_polarity;
};

#endif //STEPPER_CLASS_H
