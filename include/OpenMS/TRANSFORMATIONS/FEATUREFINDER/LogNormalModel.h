// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2007 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Clemens Groepl, Marcel Grunert $
// --------------------------------------------------------------------------

#ifndef OPENMS_TRANSFORMATIONS_FEATUREFINDER_LOGNORMALMODEL_H
#define OPENMS_TRANSFORMATIONS_FEATUREFINDER_LOGNORMALMODEL_H

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/InterpolationModel.h>
#include <OpenMS/MATH/STATISTICS/BasicStatistics.h>


namespace OpenMS
{
	
	/** 
		@brief Log-normal distribution model for elution profiles.
		 
		@ref LogNormalModel_Parameters are explained on a separate page.

		@ingroup FeatureFinder
	*/
	class LogNormalModel
		: public InterpolationModel
	{

	 public:
		typedef InterpolationModel::CoordinateType CoordinateType;
		typedef Math::BasicStatistics<CoordinateType > BasicStatistics;

		/// Default constructor
		LogNormalModel();

		/// copy constructor
		LogNormalModel(const LogNormalModel& source);

		/// destructor
		virtual ~LogNormalModel();

		/// assignment operator
		virtual LogNormalModel& operator = (const LogNormalModel& source);

		/// create new LogNormalModel object (needed by Factory)
		static BaseModel<1>* create()
		{
			return new LogNormalModel();
  	}

		/// name of the model (needed by Factory)
		static const String getProductName()
		{
			return "LogNormalModel";
		}

		/// set offset without being computing all over and without any discrepancy
		void setOffset(double offset);

		/// set sample/supporting points of interpolation
		void setSamples();

		/// get the center of the Gaussian model i.e. the position of the maximum
		CoordinateType getCenter() const;

	 protected:
		CoordinateType  min_;
		CoordinateType  max_;
		BasicStatistics statistics_;
		CoordinateType height_;
		CoordinateType width_;
		CoordinateType symmetry_;
		CoordinateType retention_;
		CoordinateType r_;

		void updateMembers_();
	};
}

#endif // OPENMS_TRANSFORMATIONS_FEATUREFINDER_LOGNORMALMODEL_H
