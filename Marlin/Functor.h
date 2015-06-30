///////////////////////////////////////////////////////////////////////////////
/// \file Functor.h
///
/// \author Ivan Galvez Junquera
/// \date 23/12/2013
/// \ingroup common
///
/// \brief Template for Functor idiom.
///
/// Copyright 2013-2015 Ivan Galvez Junquera (ivgalvez@gmail.com).
/// http://gitlab.com/ivgalvez-sandbox
///
/// Distributed under the OSI-approved BSD "July 22 1999" style license;
/// Complete text of the original BSD "July 22 1999" license can be found in
/// /usr/share/common-licenses/BSD on Debian systems.
///
/// This software is distributed WITHOUT ANY WARRANTY; without even the
/// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
///
///////////////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_H
#define FUNCTOR_H

static void do_nothing() {};

template <typename R, typename... Args>
	class Functor
{
	public:
		typedef R (*FuncPtr)(Args... args);

	public:
		Functor (FuncPtr fptr)
			: m_fptr(fptr)
		{ };

		R action(Args... args)
		{
			return m_fptr(args...);
		};

	private:
		FuncPtr m_fptr;
};
#endif //FUNCTOR_H
