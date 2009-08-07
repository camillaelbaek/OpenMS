// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Andreas Bertsch $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------


#ifndef OPENMS_ANALYSIS_DENOVO_COMPNOVOIONSCORINGCID_H
#define OPENMS_ANALYSIS_DENOVO_COMPNOVOIONSCORINGCID_H

// OpenMS includes
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>
#include <OpenMS/DATASTRUCTURES/Map.h>
#include <OpenMS/COMPARISON/SPECTRA/ZhangSimilarityScore.h>
#include <OpenMS/ANALYSIS/DENOVO/MassDecomposition.h>
#include <OpenMS/ANALYSIS/DENOVO/MassDecompositionAlgorithm.h>
#include <OpenMS/ANALYSIS/DENOVO/CompNovoIonScoringBase.h>

// stl includes
#include <vector>

namespace OpenMS
{
	/**
	  @brief  run with CompNovoIonScoringCID

		@ref CompNovoIonScoringCID_Parameters are explained on a separate page.
		
		@ingroup Analysis_ID
	*/
	class OPENMS_DLLAPI CompNovoIonScoringCID : public CompNovoIonScoringBase
	{

		public:

			typedef CompNovoIonScoringBase::IsotopeType IsotopeType;
			typedef CompNovoIonScoringBase::IonScore IonScore;

						
			/** @name constructors and destructors
			 */
			//@{
			/// default constructor
			CompNovoIonScoringCID();
			
			/// copy constructor
			CompNovoIonScoringCID(const CompNovoIonScoringCID& source);
			
			/// destructor
			virtual ~CompNovoIonScoringCID();
			//@}
		
			///
			CompNovoIonScoringCID& operator = (const CompNovoIonScoringCID& source);

			/** @name Accessors
			 */
			//@{
			void scoreSpectrum(Map<DoubleReal, IonScore>& CID_ion_scores, PeakSpectrum& CID_spec, DoubleReal precursor_weight, UInt charge);
			//@}

		protected:

			void scoreWitnessSet_(UInt charge, DoubleReal precursor_weight, Map<DoubleReal, IonScore>& CID_nodes, const PeakSpectrum& CID_orig_spec);
	};

}

#endif