///////////////////////////////////////////////////////////////////////////////
/// \file Singleton.h
///
/// \author Ivan Galvez Junquera
/// \date 23/12/2008
/// \ingroup common
///
/// \brief Template for Singleton instantiation.
/// C++11 version using veriadic templates.
///
/// Copyright 2008-2015 Ivan Galvez Junquera (ivgalvez@gmail.com).
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

#ifndef SINGLETON_H
#define SINGLETON_H

//! \brief Template class for Singleton instantiation.
//! The template follows the Meyers Singleton Pattern.
//! <BR>
//! A typedef for your implemented class is recommended to make use of the template.
//!
//! \warning Singletons have serious drawbacks, although C++11 does guarantee that
//! this construction is thread safe.
//! \ingroup common
//! \author Ivan Galvez Junquera
template <typename T>
	class Singleton
{
	public:
		//! \brief This method returns the existing instance.
		//! The construction relies on the fact that function-static objects are only
		//! initialized when the function is first being called upon.
		//!
		//! \param args Variable list of generic arguments to be passed to the class'
		//! constructor.
		//!
		//! \returns An instance reference to the class, so the caller cannot be
		//! tempted to delete the singleton
		template <typename... Args>
			static T & instance(Args... args)
		{
			static T m_instance(args...);
			return m_instance;
		}

	protected:
		//! \brief Constructor of regulated scope.
		//! To access the instance use the getInstance method instead.
		Singleton()
		{ };

		//!\brief Hidden destructor
		//!
		//! \warning There is a potential problem of accessing the object after destruction.
		//! This happens if you access the object from the destructor of another global
		//! (non local static) variable. You must make sure to force the order of destruction
		//! to be the exact inverse of the order of construction.
		virtual ~Singleton()
		{ };

	private:
		//! \brief Copy constructor.
		//! The copy constructor is hidden.
		//!
		//! \param orig Original instance to be copied.
		Singleton(Singleton const & orig) = delete;

		//! \brief Assign operator.
		//! The assign operator is hidden to avoid copies of the class.
		Singleton & operator=(Singleton const &) = delete;
};
#endif // SINGLETON_H
