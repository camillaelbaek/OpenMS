// -*- mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// --------------------------------------------------------------------------
//                   OpenMS Mass Spectrometry Framework
// --------------------------------------------------------------------------
//  Copyright (C) 2003-2008 -- Oliver Kohlbacher, Knut Reinert
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
// $Maintainer: Nico Pfeifer $
// --------------------------------------------------------------------------

#ifndef OPENMS_METADATA_PROTEINIDENTIFICATION_H
#define OPENMS_METADATA_PROTEINIDENTIFICATION_H

#include <OpenMS/METADATA/ProteinHit.h>
#include <OpenMS/METADATA/MetaInfoInterface.h>
#include <OpenMS/DATASTRUCTURES/DateTime.h>

namespace OpenMS
{   	
  /**
    @brief Representation of a peptide/protein ProteinIdentification
    
    This class stores the general information and the protein hits of an ProteinIdentification run.
    
    The actual peptide hits are stored in PeptideIdentification instances that are part of 
    spectra or features. 
    
    In order to be able to connect the ProteinIdentification and the corresponding peptide identifications, both
    classes have a string identifier. We recommend using the search engine name and the date as identifier.
    Setting this identifier is especially important, when there are several ProteinIdentification runs for a map
    i.e. several ProteinIdentification instances.

    @todo Add MetaInfoInterface to modifications => update IdXMLFile and ProteinIdentificationVisualizer (Andreas)
     
		@ingroup Metadata
  */
  class OPENMS_DLLAPI ProteinIdentification
  	: public MetaInfoInterface
  {
	  public:
	 		///Hit type definition
	 		typedef ProteinHit HitType;
			
			/// Peak mass type
			enum PeakMassType
			{
				MONOISOTOPIC,
				AVERAGE,
				SIZE_OF_PEAKMASSTYPE
			};
			/// Names corresponding to peak mass types
			static const std::string NamesOfPeakMassType[SIZE_OF_PEAKMASSTYPE];
			
			
			enum DigestionEnzyme
			{
				TRYPSIN,
				PEPSIN_A,
				PROTEASE_K,
				CHYMOTRYPSIN,
				NO_ENZYME,
				UNKNOWN_ENZYME,
				SIZE_OF_DIGESTIONENZYME
			};
			/// Names corresponding to digestion enzymes
			static const std::string NamesOfDigestionEnzyme[SIZE_OF_DIGESTIONENZYME];
			
			/// Search parameters of the DB search
			struct SearchParameters
				: public MetaInfoInterface
			{
				String db; ///< The used database
				String db_version; ///< The database version
				String taxonomy; ///< The taxonomy restriction
				String charges; ///< The allowed charges for the search
				PeakMassType mass_type; ///< Mass type of the peaks
				std::vector<String> fixed_modifications; ///< Used fixed modifications
				std::vector<String> variable_modifications; ///< Allowed variable modifications
				DigestionEnzyme enzyme; ///< The enzyme used for cleavage
				UInt missed_cleavages; ///< The number of allowed missed cleavages
				DoubleReal peak_mass_tolerance; ///< Mass tolerance of fragment ions (Dalton)
				DoubleReal precursor_tolerance; ///< Mass tolerance of precursor ions (Dalton)
				
				SearchParameters()
					: db(),
						db_version(),
						taxonomy(),
						charges(),
						mass_type(MONOISOTOPIC),
						fixed_modifications(),
						variable_modifications(),
						enzyme(UNKNOWN_ENZYME),
						missed_cleavages(0),
						peak_mass_tolerance(0.0),
						precursor_tolerance(0.0)
				{
				};

				bool operator == (const SearchParameters& rhs) const
				{
					return 	db == rhs.db &&
									db_version == rhs.db_version &&
									taxonomy == taxonomy &&
									charges == charges &&
									mass_type == mass_type &&
									fixed_modifications == fixed_modifications &&
									variable_modifications == variable_modifications &&
									enzyme == enzyme &&
									missed_cleavages == missed_cleavages &&
									peak_mass_tolerance == peak_mass_tolerance &&
									precursor_tolerance == precursor_tolerance;

				}

				bool operator != (const SearchParameters& rhs) const
				{
					return !(*this == rhs);
				}
			};
	  	
	  	
	    /** @name constructors,destructors,assignment operator <br> */
	    //@{
	    /// default constructor
	    ProteinIdentification();
	    /// destructor
	    virtual ~ProteinIdentification();
	    /// copy constructor
	    ProteinIdentification(const ProteinIdentification& source);
	    /// assignment operator
	    ProteinIdentification& operator=(const ProteinIdentification& source);
			/// Equality operator
			bool operator == (const ProteinIdentification& rhs) const;		
			/// Inequality operator
			bool operator != (const ProteinIdentification& rhs) const;
	    //@}	 

	   	///@name Protein hit information
	  	//@{	
	    /// returns the protein hits
	    const std::vector<ProteinHit>& getHits() const;
	    /// returns the protein hits (mutable)
	    std::vector<ProteinHit>& getHits();
			/// Appends a protein hit
	    void insertHit(const ProteinHit& input);
			/// Sets the peptide and protein hits
	    void setHits(const std::vector<ProteinHit>& hits); 
			/// returns the peptide significance threshold value
	    Real getSignificanceThreshold() const;
			/// setting of the peptide significance threshold value
			void setSignificanceThreshold(Real value);
	    /// returns the protein score type
	    const String& getScoreType() const;   
	    /// sets the protein score type
	    void setScoreType(const String& type);
	    /// returns true if a higher score represents a better score
	    bool isHigherScoreBetter() const;   
	    /// sets the orientation of the score (higher is better?)
	    void setHigherScoreBetter(bool higher_is_better);
			///sorts the peptide and protein hits according to their score
			void sort();
			/// sorts the peptide hits and assigns ranks according to the sorting
	    void assignRanks();
			//@}

	   	///@name General information
	  	//@{
			/// returns the date of the ProteinIdentification
	    const DateTime& getDateTime() const;
			/// sets the date of the ProteinIdentification
	    void setDateTime(const DateTime& date);
			/// sets the search engine type
			void setSearchEngine(const String& search_engine);
			/// returns the type of search engine used
			const String& getSearchEngine() const;
			/// sets the search engine version
			void setSearchEngineVersion(const String& search_engine_version);
			/// returns the search engine version
			const String& getSearchEngineVersion() const;
			/// sets the search parameters
			void setSearchParameters(const SearchParameters& search_parameters);
			/// returns the search parameters 
			const SearchParameters& getSearchParameters() const; 
	    /// returns the identifier
	    const String& getIdentifier() const;
	    /// sets the indentifier
	    void setIdentifier(const String& id);
			//@}
			
	  protected:
	  
			///@name General information (search engine, parameters and DB)
	  	//@{
			String id_;
			String search_engine_;
			String search_engine_version_;
			SearchParameters search_parameters_;
	    DateTime date_;
	    //@}
	   	
			///@name Protein hit information
	  	//@{
	    String protein_score_type_;   
			bool higher_score_better_;
		  std::vector<ProteinHit> protein_hits_; 
			Real protein_significance_threshold_;
	    //@}
  };

} //namespace OpenMS
#endif // OPENMS_METADATA_PROTEINIDENTIFICATION_H
