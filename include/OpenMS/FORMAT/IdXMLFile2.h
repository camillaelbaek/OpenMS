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
// $Maintainer: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_FORMAT_IDXMLFILE2_H
#define OPENMS_FORMAT_IDXMLFILE2_H

#include <OpenMS/METADATA/Identification.h>
#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/FORMAT/HANDLERS/XMLHandler.h>

#include <vector>

namespace OpenMS 
{
  /**
    @brief Used to load and store IdXML files
    
    This class is used to load and store documents that implement 
    the schema of IdXML files.
		
		A documented schema for this format can be found at http://open-ms.sourceforge.net/schemas/. 
		
		One file can contain several identification runs. Each run consists of peptide hits stored in 
		PeptideIdentification and (optional) protein hits stored in Identification. Peptide and protein
		hits are connected via a string identifier. We use the searche engine and the date as identifier.
		
  	@todo Document and upload schema (Marc)
  	
  	@note This format will only be used until the HUPO-PSI AnalysisXML format is finished!
  	
  	@ingroup FileIO
  */
  class IdXMLFile2
  	: protected Internal::XMLHandler
  {
		public:
		
			/// Constructor
			IdXMLFile2();
			
			/**
			@brief Loads the identifications of an IdXML file
			
			The information is read in and the information is stored in the
			corresponding variables
			*/
			void load(const String& filename, std::vector<Identification>& protein_ids, std::vector<PeptideIdentification>& peptide_ids) throw (Exception::FileNotFound, Exception::ParseError);
			 			 
			/**
			@brief Stores the data in an IdXML file
			
			The data is read in and stored in the file 'filename'.
			*/
			void store(String filename, const std::vector<Identification>& protein_ids, const std::vector<PeptideIdentification>& peptide_ids) throw (Exception::UnableToCreateFile); 
  	
  		protected:
			/// Writes the xml file to the ostream 'os'
			void writeTo(std::ostream& os);
			
			// Docu in base class
			virtual void endElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname);
			
			// Docu in base class
			virtual void startElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname, const xercesc::Attributes& attributes);
			
			/// writes user parameters
			void writeUserParam_(std::ostream& os, const MetaInfoInterface& meta, UInt indent) const;
			
  			/// @name members for loading data
  			//@{
  			/// Pointer to fill in protein identifications
  			std::vector<Identification>* prot_ids_;
  			/// Pointer to fill in peptide identifications
  			std::vector<PeptideIdentification>* pep_ids_;
  			/// Pointer to last read object with MetaInfoInterface
  			MetaInfoInterface* last_meta_;
  			/// Search parameters map (key is the "id")
  			std::map<String,Identification::SearchParameters> parameters_;
  			/// Temporary search parameters variable
  			Identification::SearchParameters param_;
  			/// Temporary id
  			String id_;
  			/// Temporary protein identification
  			Identification prot_id_;
  			/// Temporary peptide identification
  			PeptideIdentification pep_id_;
  			/// Temporary protein hit
  			ProteinHit prot_hit_;
  			/// Temporary peptide hit
  			PeptideHit pep_hit_;
  			/// 
  			std::map<String,String> proteinid_to_accession_;
  			
  			//@}
  };
 
} // namespace OpenMS

#endif // OPENMS_FORMAT_IDXMLFILE2_H
