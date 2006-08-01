// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework 
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2006 -- Oliver Kohlbacher, Knut Reinert
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// --------------------------------------------------------------------------
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_DATASTRUCTURES_STRING_H
#define OPENMS_DATASTRUCTURES_STRING_H

#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/CONCEPT/Exception.h>
#include <OpenMS/DATASTRUCTURES/DataValue.h>

#include <string>
#include <vector>

namespace OpenMS
{
	/**	
		@defgroup Datastructures Datastructures
		
		@brief Auxilary datastructures.
	*/

	/**	
		@brief A more convenient string class.
		
		It based on std::string but adds a lot of methods for convenience.
		
		@ingroup Datastructures
	*/
	class String:
		public std::string
	{
		public:

		/// Empty string for comparisons
		static const String EMPTY;
		
		/** @name Type definitions
		*/
		//@{	
		/// Iterator
		typedef iterator	Iterator;
		/// Const Iterator
		typedef const_iterator	ConstIterator;
		/// Reverse Iterator
		typedef reverse_iterator	ReverseIterator;
		/// Const reverse Iterator
		typedef const_reverse_iterator	ConstReverseIterator;
		/// Size type
		typedef size_type	SizeType;

		//@}		

		/** @name Constants
		*/
		//@{	
		/// Handle that is used for an undefined position
		static const SignedInt NPOS;
		//@}	
		
		/**	@name Constructors
		*/
		//@{
		/// Default constructor
		String();
		/// Copy constructor
		String(const std::string& s);
		/// Constructor from char*
		String(const char* s);
		/// Constructor from a char
		String(const char c);
		/// Constructor from char* (only @p length chracters)
		String(const char* s, SizeType length);
		/// Constructor from char (repeats the char @p len times)
		String(size_t len, char c);
		/// Constructor from a char range
		template <class InputIterator>
		String(InputIterator first, InputIterator last)
		: std::string(first,last)
		{
			
		}
		/// Constructor from an integer
		String(int i);
		/// Constructor from an unsigned integer
		String(unsigned int i);
		/// Constructor from an integer
		String(short int i);
		/// Constructor from an unsigned integer
		String(short unsigned int i);
		/// Constructor from an integer
		String(long int i);
		/// Constructor from an unsigned integer
		String(long unsigned int i);
		/// Constructor from float (precision is 7)
		String(float f);
		/// Constructor from double (precision is 10)
		String(double d);
		/// Constructor from long double (precision is 16)
		String(long double d);
		/**
			@brief  Constructor from double, that is converted to string of maximum length @p size. 
		
		 If @p d is larger, scientific notation is used.
		*/
		String(double d, UnsignedInt size);
		/// Constructor from DataValue (casted to String)
		String(const DataValue& d);

		//@}

		/** @name Predicates
		*/
		//@{
		/// true if String begins with @p string, false otherwise
		bool hasPrefix(const String& string) const;

		/// true if String ends with @p string, false otherwise
		bool hasSuffix(const String& string) const;

		/// true if String contains the @p string, false otherwise
		bool hasSubstring(const String& string) const;

		/// true if String contains the @p byte, false otherwise
		bool has(Byte byte) const;
		//@}


		/** @name Accessors
		*/
		//@{
		/// returns the prefix of length @p length
		String prefix(SizeType length) const 
			throw(Exception::IndexOverflow);
		
		/// returns the suffix of length @p length
		String suffix(SizeType length) const 
			throw(Exception::IndexOverflow);

		/// returns the prefix of length @p length
		String prefix(SignedInt length) const 
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);
		
		/// returns the suffix of length @p length
		String suffix(SignedInt length) const 
			throw(Exception::IndexUnderflow, Exception::IndexOverflow);
		
		/// returns the prefix up to char @p delim
		String prefix(char delim) const 
			throw(Exception::ElementNotFound<char>);
		
		/// returns the suffix up to char @p delim
		String suffix(char delim) const 
			throw(Exception::ElementNotFound<char>);
		/**
			@brief Returns a substring
			
			If @p start denotes a position beyond this truncation, an empty string will be returned.	
			@param start  start position of the substring.<br> 
			              If start is negative, the returned string will start at the start'th character from the end of string.
			@param n length of the substring.<br> 
			         If a negative length is given, then that many characters will be omitted from the end of string.<br>
			         If NPOS is given, the end is the last character.<br>
		*/
		String substr(SignedInt start = 0, SignedInt n = NPOS) const;
		
		//@}
		
		
		/** @name Mutators
		*/
		//@{
		/// inverts the direction of the string
		String& reverse();
		
		/// removes whitespaces (space, tab, line feed, carriage return) at the beginning and the end of the string
		String& trim();
		
		///Adds @p c on the left side until the size of the string is @p size
		String& fillLeft(char c, UnsignedInt size);
		
		///Adds @p c on the right side until the size of the string is @p size
		String& fillRight(char c, UnsignedInt size);

		///Converts the string to uppercase
		void toUpper();
		
		///Converts the string to lowercase
		void toLower();
		//@}

		/** @name Converters
		*/
		//@{

		/**
			@brief Conversion to int
		
			This method extracts only the integral part of the string.
			If you want the result rounded, use toFloat() and round the result.
		*/
		int toInt() const
      throw(Exception::ConversionError);

		/// Conversion to float
		float toFloat() const
      throw(Exception::ConversionError);    

		/// Conversion to double
		double toDouble() const
      throw(Exception::ConversionError);    

		//@}


		///returns a random string of the given length. It consists of [0-9a-zA-Z]
		static String random(Size length);
		
		/**
			@brief splits a string into @p substrings using @p splitter as delimiter
			
			If the @p splitter is not found, @p substrings is empty.
			@return if the splitter was found (once or multiple times) in the string
		*/
		bool split(char splitter, std::vector<String>& substrings) const;
		
		
		///convatenates all elements from @p first to @p last-1 and inserts @p glue between the elements
		void implode(std::vector<String>::iterator first, std::vector<String>::iterator last, const std::string& glue = "");
		
	};
} // namespace OPENMS

#endif // OPENMS_DATASTRUCTURES_HASHMAP_H
