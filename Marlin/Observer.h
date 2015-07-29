///////////////////////////////////////////////////////////////////////////////
/// \file Observer.h
///
/// \author Ivan Galvez Junquera
///
/// \brief Template Observer pattern implementation.
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

#ifndef OBSERVER_H
#define OBSERVER_H

template <typename T> class Subject;

template <typename T>
	class Observer
{
	public:
		Observer(Subject<T> * model);
		virtual ~Observer();
		virtual void update(T value) = 0;
		void connect();

	public:
		Subject<T> * m_model;
};

#include "Subject.h"

template <typename T>
	Observer<T>::Observer(Subject<T> * model)
	: m_model(model)
{
	if (m_model != 0)
	{
		m_model->attach(this);
	}
}

template <typename T>
	Observer<T>::~Observer()
{
	if (m_model != 0)
	{
		m_model->dettach();
	}
}

template <typename T>
	void Observer<T>::connect()
{
	if (m_model != 0)
	{
		m_model->notify();
	}
}
#endif //OBSERVER_H
