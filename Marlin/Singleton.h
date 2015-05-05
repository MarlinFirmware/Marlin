///////////////////////////////////////////////////////////////////////////////
/// \file Singleton.h
///
/// \author Ivan Galvez Junquera
/// \date 23/12/2008
/// \ingroup common
///
/// \brief Template for Singleton instantiation.
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

//! \addtogroup common Common: Classes and functions of common use.
//! @{

#ifndef SINGLETON_H
#define SINGLETON_H

namespace common
{
	//! \brief Template class for Singleton instantiation.
	//! The template follows the Meyers Singleton Pattern.
	//! <BR>
	//! A typedef for your implemented class is recommended to make use of the template.
	//!
	//! \warning Singletons have serious drawbacks, specially when dealing with multithreaded
	//! environments. Check Alexandrescu's 'Modern C++'.
	//! \ingroup common
	//! \author Ivan Galvez Junquera
	template <typename T>
		class Singleton
	{
		public:
			//! \brief This method returns the existing instante.
			//! The construction relies on the fact that function-static objects are only
			//! initialized when the function is first being called upon.
			//!
			//! \warning C++03 does not officially guarantee that the construction of static function
			//! objects is thread safe. So this method must be guarded with a critical section. 
			//! <BR>
			//! However, gcc has an explicit patch as part of the compiler that guarantees that each 
			//! static function object will only be initialized once even in the presence of threads.
			//!
			//! \returns An instance reference to the class, so the caller cannot be
			//! tempted to delete the singleton
			static T & getInstance()
			{
				static T m_instance;
				return m_instance;
			}

			//! \brief This method returns the existing instance.
			//! The construction relies on the fact that function-static objects are only
			//! initialized when the function is first being called upon.
			//! <BR>
			//! This version of the method allows the use of a constructor parameter for
			//! the singleton class.
			//!
			//! \warning C++03 does not officially guarantee that the construction of static function
			//! objects is thread safe. So this method must be guarded with a critical section. 
			//! <BR>
			//! However, gcc has an explicit patch as part of the compiler that guarantees that each 
			//! static function object will only be initialized once even in the presence of threads.
			//!
			//! \param arg1 Parameter for the constructor of the class.
			//!
			//! \returns An instance reference to the class, so the caller cannot be
			//! tempted to delete the singleton
			template <class Arg1> 
				static T & getInstance(Arg1 arg1)
			{
				static T m_instance(arg1);
				return m_instance;
			}

			//! \brief This method returns the existing instance.
			//! The construction relies on the fact that function-static objects are only
			//! initialized when the function is first being called upon.
			//! <BR>
			//! This version of the method allows the use of 2 constructor parameter for
			//! the singleton class. Obviously, without variadic templates this solution 
			//! doesn't scale up.
			//!
			//! \warning C++03 does not officially guarantee that the construction of static function
			//! objects is thread safe. So this method must be guarded with a critical section. 
			//! <BR>
			//! However, gcc has an explicit patch as part of the compiler that guarantees that each 
			//! static function object will only be initialized once even in the presence of threads.
			//!
			//! \param arg1 First parameter for the constructor of the class.
			//! \param arg2 Second parameter for the constructor of the class.
			//!
			//! \returns An instance reference to the class, so the caller cannot be
			//! tempted to delete the singleton
			template <class Arg1, class Arg2> 
				static T & getInstance(Arg1 arg1, Arg2 arg2)
			{
				static T m_instance(arg1, arg2);
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
			Singleton(Singleton const & orig);

			//! \brief Assign operator.
			//! The assign operator is hidden to avoid copies of the class.
			Singleton & operator=(Singleton const &);
	};
}
#endif // SINGLETON_H
//! @}
