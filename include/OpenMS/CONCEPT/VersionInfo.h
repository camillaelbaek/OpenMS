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

#ifndef OPENMS_CONCEPT_VERSIONINFO_H
#define OPENMS_CONCEPT_VERSIONINFO_H

#include <OpenMS/CONCEPT/Exception.h>

//
//  master data for all revisions is stored in this define!
//
#define OPENMS_RELEASE_STRING             "0.9.0"

namespace OpenMS
{

	/**	
		@brief Version information class.
		
		The OpenMS release and version data can be retrieved as a string
		or as integers.
			
		The VersionInfo class contains only static methods.

		@ingroup Concept
	*/
	class VersionInfo
	{
		public:

		/** Return the version number, which is defined in OPENMS_RELEASE_STRING
		*/
		static const char* getVersion() throw();

		/** Return the major revision number.
				The part of the release number before the first dot.
		*/
		static int getMajorRevision();

		/** Return the minor revision number.
				The part of the release number after the first and before the second dot.
		*/
		static int getMinorRevision();

		protected:

		static const char* RELEASE_DATE_AND_TIME;
	  static const char* RELEASE_WHAT_STRING;
	};
	
}

#endif // OPENMS_CONCEPT_VERSIONINFO_H
