// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//									 OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//	Copyright (C) 2003-2009 -- Oliver Kohlbacher, Knut Reinert
//
//	This library is free software; you can redistribute it and/or
//	modify it under the terms of the GNU Lesser General Public
//	License as published by the Free Software Foundation; either
//	version 2.1 of the License, or (at your option) any later version.
//
//	This library is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the GNU
//	Lesser General Public License for more details.
//
//	You should have received a copy of the GNU Lesser General Public
//	License along with this library; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
//
// --------------------------------------------------------------------------
// $Maintainer: Andreas Bertsch $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#ifndef OPENMS_KERNEL_CHROMATOGRAMTOOLS_H
#define OPENMS_KERNEL_CHROMATOGRAMTOOLS_H

#include <OpenMS/DATASTRUCTURES/Map.h>
#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/KERNEL/MSChromatogram.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/KERNEL/RangeUtils.h>

namespace OpenMS
{
	/**
		@brief Conversion class to interconvert chromatograms

		There are basically two methods implemented, conversion of
		chromatograms into spectra representation and vice versa.
		
		@ingroup Kernel
	*/
	//template <typename ChromatogramT = MSChromatogram<>, typename ExperimentT = MSExperiment<> >
	class ChromatogramTools
	{

		public:

			/// @name Constructors and destructors
			//@{
			/// default constructor
			ChromatogramTools()
			{
			}

			/// copy constructor
			ChromatogramTools(const ChromatogramTools& /*rhs*/)
			{
			}

			/// destructor
			virtual ~ChromatogramTools()
			{
			}
			//@}

			/// @name Accessors
			//@{
			 /** @brief converts the chromatogram to a list of spectra with instrument settings SRM

          This conversion may be necessary as most of the spectra formats do not support
          chromatograms, except of mzML. However, most formats support e.g. SRM chromatogram
          as a list of spectra with instrument settings SRM and a separate spectrum for 
          each data point. The disadvantage of storing chromatograms in spectra is its
          exhaustive memory consumption.
      */
			template<typename ChromatogramType, typename ExperimentType>
      void convertChromatogramsToSpectra(ExperimentType& exp, const std::vector<ChromatogramType>& chromatograms)
      {

      }

      /** @brief converts e.g. SRM spectra to chromatograms
          
          This conversion is necessary to convert chromatograms, e.g. from SRM or MRM
          experiments to real chromatograms. mzML 1.1.0 has support for chromatograms
          which can be stored much more efficiently than spectra based chromatograms.
          However, most other file formats do not support chromatograms.

					If @param remove_spectra is set to true, the chromatogram spectra are removed from the experiment
      */
			template<typename ChromatogramType, typename ExperimentType>
      void convertSpectraToChromatograms(std::vector<ChromatogramType>& chromatograms, ExperimentType& exp, bool remove_spectra = false)
      {
				typedef typename ExperimentType::SpectrumType SpectrumType;
				Map<DoubleReal, Map<DoubleReal, std::vector<SpectrumType> > > chroms;
				for (typename ExperimentType::ConstIterator it = exp.begin(); it != exp.end(); ++it)
				{
					if (it->getInstrumentSettings().getScanMode() == InstrumentSettings::SRM)
					{
						// exactly one precursor and one product ion
						if (it->getPrecursors().size() == 1 && it->size() == 1)
						{
							chroms[it->getPrecursors().begin()->getMZ()][it->begin()->getMZ()].push_back(*it);
						}
						else
						{
							// TODO
						}
					}
				}

				typename Map<DoubleReal, Map<DoubleReal, std::vector<SpectrumType> > >::const_iterator it1 = chroms.begin();
				for (; it1 != chroms.end(); ++it1)
				{
					typename Map<DoubleReal, std::vector<SpectrumType> >::const_iterator it2 = it1->second.begin();
					for (; it2 != it1->second.end(); ++it2)
					{
						ChromatogramType chrom;
						chrom.setPrecursor(*it2->second.begin()->getPrecursors().begin());
						Product prod;
						prod.setMZ(it2->first);
						chrom.setProduct(prod);
						chrom.setInstrumentSettings(it2->second.begin()->getInstrumentSettings());
						chrom.setAcquisitionInfo(it2->second.begin()->getAcquisitionInfo());
						chrom.setSourceFile(it2->second.begin()->getSourceFile());
						
						typename std::vector<SpectrumType>::const_iterator it3 = it2->second.begin();
						for (; it3 != it2->second.end(); ++it3)
						{
							typename ChromatogramType::PeakType p;
							p.setRT(it3->getRT());
							p.setIntensity(it3->begin()->getIntensity());
							chrom.push_back(p);
						}

						chrom.setNativeID("chromatogram=" + it2->second.begin()->getNativeID()); // TODO native id of spectra?
						chrom.setChromatogramType(ChromatogramSettings::SELECTED_REACTION_MONITORING_CHROMATOGRAM);
						chromatograms.push_back(chrom);
					}
				}

				if (remove_spectra)
				{
					exp.erase(remove_if(exp.begin(), exp.end(), HasScanMode<SpectrumType>(InstrumentSettings::SRM)), exp.end());
				}
      }			
			//@}

	};

} // namespace OpenMS

#endif // OPENMS_KERNEL_CHROMATOGRAMTOOLS_H