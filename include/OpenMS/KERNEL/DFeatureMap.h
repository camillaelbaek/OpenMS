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
// $Id: DFeatureMap.h,v 1.20 2006/06/09 14:46:54 marc_sturm Exp $
// $Author: marc_sturm $
// $Maintainer: Ole Schulz-Trieglaff $
// --------------------------------------------------------------------------

#ifndef OPENMS_KERNEL_DFEATUREMAP_H
#define OPENMS_KERNEL_DFEATUREMAP_H

#include <OpenMS/config.h>
#include <OpenMS/KERNEL/DFeature.h>
#include <OpenMS/KERNEL/KernelTraits.h>
#include <OpenMS/METADATA/ExperimentalSettings.h>

#include <algorithm>
#include <vector>

namespace OpenMS
{

	/**	
		@brief A container for (composite) features.
		
		A map is a container holding D-dimensional features,
		which in turn represent chemical entities (peptides, proteins, etc.) found
		in a D-dimensional experiment.
		Maps are implemented as vectors of features and have basically the same interface
		as an STL vector has (model of Random Access Container and Back Insertion Sequence).
		Maps are typically created from peak data of 2D runs through the FeatureFinder.
		
		@todo write tests for sortBy... methods (Ole)
		
		@ingroup Kernel, Serialization
	*/
	template <Size D, typename TraitsT = KernelTraits, typename FeatureT = DFeature<D,TraitsT> >
	class DFeatureMap
		: public std::vector<FeatureT>, public ExperimentalSettings
	{
	 public:
		/**	
			 @name Type definitions
		*/
		//@{
		typedef TraitsT TraitsType;
		typedef FeatureT FeatureType;
		typedef std::vector<FeatureType> Base;
		typedef std::vector<FeatureType> ContainerType;
		typedef typename ContainerType::iterator Iterator;
		typedef typename ContainerType::const_iterator ConstIterator;
		typedef typename ContainerType::reverse_iterator ReverseIterator;
		typedef typename ContainerType::const_reverse_iterator ConstReverseIterator;
		typedef FeatureType& Reference;typedef const FeatureType& ConstReference;

		// STL compatibility
		typedef FeatureType value_type;
		typedef FeatureType* pointer;
		typedef const FeatureType* const_pointer;
		typedef Reference reference;
		typedef ConstReference const_reference;
		typedef typename ContainerType::size_type size_type;
		typedef typename ContainerType::difference_type difference_type;
		typedef Iterator iterator;
		typedef ConstIterator const_iterator;
		typedef ReverseIterator reverse_iterator;
		typedef ConstReverseIterator const_reverse_iterator;
		//@}
	
		/**	
			 @name Constructors and Destructor
		*/
		//@{
		/// Default constructor
		DFeatureMap() {}
		/// Copy constructor
		DFeatureMap(const DFeatureMap& map) :
			Base(map),
			ExperimentalSettings(map)
		{}
		/// Destructor
		virtual ~DFeatureMap() {}
		//@}
			
		/// Assignment operator
		DFeatureMap& operator = (const DFeatureMap& rhs)
		{
			if (&rhs==this) return *this;
				
			Base::operator=(rhs);
			ExperimentalSettings::operator=(rhs);
				
			return *this;
		}

		/// Equality operator
		bool operator == (const DFeatureMap& rhs) const
		{
			return
				std::operator==(*this, rhs) && 			
				ExperimentalSettings::operator==(rhs)
				;				
		}
			
		/// Equality operator
		bool operator != (const DFeatureMap& rhs) const
		{
			return !(operator==(rhs));
		}
			
		/** @brief Sorting features by intensity. */
		void sortByIntensity() 
		{ 
			typename DFeatureMap<D>::iterator beg = this->begin();
			typename DFeatureMap<D>::iterator ed  = this->end();
			std::sort(beg, ed, typename FeatureType::IntensityLess() ); 
		}
			
		/** @brief Sorting features by position.
			
			Lexicographical sorting from dimention 0 to dimension D is performed.
		
		*/
		void sortByPosition(Index i = -1) 
		{ 
			OPENMS_PRECONDITION(i < Index(D), "illegal dimension");
			OPENMS_PRECONDITION(-2 < i, "illegal dimension");
			
			std::sort(this->begin(), this->end(), typename FeatureType::PositionLess() );
		}
			
		void sortByOverallQuality()
		{
			typename DFeatureMap<D>::iterator beg = this->begin();
			typename DFeatureMap<D>::iterator ed  = this->end();
			std::sort(beg, ed, typename FeatureType::OverallQualityLess() ); 
		}

		/**@name Serialization
		 */	
		//@{
	 public:
		/// Serialization interface
		template<class Archive>
		void serialize(Archive & ar, const unsigned int /* version */ )
		{
			ar & boost::serialization::make_nvp("vector",boost::serialization::base_object<std::vector<FeatureT> >(*this));
			// TODO: serialization of base object ExperimentalSettings
		}
		//@}

		/// Serialization
		friend class boost::serialization::access;
		
	};
	
	/// Print the contents to a stream.
	template <Size D>
	std::ostream& operator << (std::ostream& os, const DFeatureMap<D>& map)
	{
		os << "# -- DFEATUREMAP BEGIN --"<< std::endl;
		os << "# POSITION \tINTENSITY\tOVERALLQUALITY\tCHARGE" << std::endl; 
		for (typename DFeatureMap<D>::const_iterator iter = map.begin(); iter!=map.end(); iter++)
		{
			os << iter->getPosition() << '\t'
				 << iter->getIntensity() << '\t'
				 << iter->getOverallQuality() << '\t'
				 << iter->getCharge()
				 << std::endl;
		}
		os << "# -- DFEATUREMAP END --"<< std::endl;
		return os;
	}
	
} // namespace OpenMS

#endif // OPENMS_KERNEL_DFEATUREMAP_H
