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
// $Id: Tagging.h,v 1.2 2006/06/02 14:19:11 marc_sturm Exp $
// $Author: marc_sturm $
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_METADATA_TAGGING_H
#define OPENMS_METADATA_TAGGING_H

#include <OpenMS/METADATA/Modification.h>

namespace OpenMS 
{
	/**
		@brief Meta information about tagging of a sample e.g. ICAT labeling.
		
		Holds information about the mass difference between light and heavy tag.
		All other relevant information is provided by Modification.
		
		@ingroup Metadata
	*/
  class Tagging: public Modification
  {
    public:
    	/// Isotope variants (light and heavy)
    	enum IsotopeVariant{LIGHT,HEAVY,SIZE_OF_ISOTOPEVARIANT};
			/// Names of isotope variants
			static const std::string NamesOfIsotopeVariant[SIZE_OF_ISOTOPEVARIANT];
    	
    	/// default constructor
    	Tagging();
    	/// copy constructor
    	Tagging(const Tagging&);
    	/// destructor
    	virtual ~Tagging();
			
			/// assignment operator
    	Tagging& operator=(const Tagging&);

    	/// Equality operator
      virtual bool operator== (const SampleTreatment& rhs) const;
      			
			/// clone method. See SampleTreatment
			virtual SampleTreatment* clone() const;
			
			/// returns the mass difference between light and heavy variant
		  float getMassShift() const;
		  /// sets the mass difference between light and heavy variant
		  void setMassShift(float mass_shift);
		
			/// returns the isotope variant of the tag (light or heavy)
		  const IsotopeVariant& getVariant() const;
		  /// sets the isotope variant of the tag (light or heavy)
		  void setVariant(const IsotopeVariant& variant);
			
    protected:
			float mass_shift_;
			IsotopeVariant variant_;
  };
} // namespace OpenMS

#endif // OPENMS_METADATA_TAGGING_H



