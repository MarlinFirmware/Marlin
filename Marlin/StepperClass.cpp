///////////////////////////////////////////////////////////////////////////////
/// \file StepperClass.cpp
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

#include "StepperClass.h"

#include <Arduino.h>

Stepper::Stepper(uint8_t pin, bool polarity)
	: m_enabled(false)
	, m_enable_pin(pin)
	, m_enable_polarity(polarity)
{ 
	disable();
}

Stepper::~Stepper()
{ }

bool Stepper::isEnabled()
{
	return m_enabled;
}

void Stepper::enable()
{
	digitalWrite(m_enable_pin, m_enable_polarity);
	m_enabled = true;
}

void Stepper::disable()
{
	digitalWrite(m_enable_pin, !m_enable_polarity);
	m_enabled = false;
}
