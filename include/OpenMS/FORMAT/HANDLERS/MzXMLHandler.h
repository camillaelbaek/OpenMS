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

#ifndef OPENMS_FORMAT_HANDLERS_MZXMLHANDLER_H
#define OPENMS_FORMAT_HANDLERS_MZXMLHANDLER_H

#include <OpenMS/METADATA/MetaInfoInterface.h>
#include <OpenMS/FORMAT/Base64.h>
#include <OpenMS/FORMAT/HANDLERS/SchemaHandler.h>
#include <OpenMS/FORMAT/HANDLERS/XMLSchemes.h>
#include <OpenMS/DATASTRUCTURES/String.h>
#include <OpenMS/KERNEL/MSExperiment.h>

#include <xercesc/sax2/Attributes.hpp>

namespace OpenMS
{
	namespace Internal
	{
  /**
  	@brief XML handlers for MzXMLFile

		MapType has to be a MSExperiment or have the same interface.
  		Do not use this class. It is only needed in MzXMLFile.
  */
	template <typename MapType>
  class MzXMLHandler
		: public SchemaHandler
  {
    public:
      /**@name Constructors and destructor */
      //@{
      /// Constructor for a write-only handler
      MzXMLHandler(MapType& exp)
			: SchemaHandler(TAG_NUM,MAP_NUM), // number of tags, number of maps
				exp_(&exp),	
				cexp_(0),
				peak_(),
				spec_(0),
				analyzer_(0),
				decoder_(),
				peak_count_(0),
				char_rest_(),
				spec_write_counter_(1)
  		{
				fillMaps_(Schemes::MzXML[schema_]);	// fill maps with current schema
			}

      /// Constructor for a read-only handler
      MzXMLHandler(const MapType& exp)
			: SchemaHandler(TAG_NUM,MAP_NUM), // number of tags, number of maps
				exp_(0), 
				cexp_(&exp),
				peak_(),
				spec_(),
				decoder_(),
				peak_count_(0),
				char_rest_(),
				spec_write_counter_(1)
  		{
				fillMaps_(Schemes::MzXML[schema_]);	// fill maps with current schema
			}

  		/// Destructor
      virtual ~MzXMLHandler(){}
      //@}
			
			// Docu in base class
      virtual void endElement( const XMLCh* const uri, const XMLCh* const local_name, const XMLCh* const qname);
			
			// Docu in base class
      virtual void startElement(const XMLCh* const uri, const XMLCh* const local_name, const XMLCh* const qname, const xercesc::Attributes& attributes);
			
			// Docu in base class
      virtual void characters(const XMLCh* const chars, const unsigned int length);

  		///Write the contents to a stream
			void writeTo(std::ostream& os);

    protected:
		/// map pointer for reading
		MapType* exp_;
		/// map pointer for writing
		const MapType* cexp_;

		/** @brief indices for tags used by mzData

			Used to access is_parser_in_tag_.
			If you add tags, also add them to XMLSchemes.h.
			Add no elements to the enum after TAG_NUM.
		*/
		enum Tags { TAGNULL=0, MSRUN, INDEX, OFFSET, SHA1, PARENTFILE, INSTRUMENT, DATAPROCESSING,
								SEPARATION, SPOTTING, SCAN, SCANORIGIN, PRECURSORMZ, MALDI, PEAKS, NAMEVALUE,
								COMMENT, SOFTWARE, INDEXOFFSET, OPERATOR, MANUFACTURER, MODEL, IONISATION,
								ANALYZER, DETECTOR, RESOLUTION, MZXML, PROCESSING, SEPARATIONTECH, TAG_NUM};


		/** @brief indices for attributes used by MzXML

			Used to access enum2str_() with ATTMAP.
			If you add terms, also add them to XMLSchemes.h
		*/
		enum Attributes {ATTNULL, POLARITY, SCANTYPE, CENTROIDED, DEISOTOPED,
										 DECONVOLUTED, RETTIME,IONENERGY, COLLENERGY, PRESSURE,
										 STARTMZ, ENDMZ, LOWMZ, HIGHMZ, BASEPEAKMZ, BASEPEAKINT,
										 TOTIONCURRENT, PEAKSCOUNT, NUM, MSLEVEL, SCANCOUNT,
										 FILENAME, FILETYPE, SOFTWAREVERSION, NAME, TYPE,
										 COMPLETION_TIME, PRECURSOR_INTENSITY, PRECURSOR_CHARGE,
										 FIRST_NAME, LAST_NAME, EMAIL, PHONE, URI, VALUE, CATEGORY,
										 PRECISION, BYTEORDER, PAIRORDER, SCHEMA, SPOT_INTEGRATION,
										 INTENSITY_CUTOFF};

		/** @brief indices for enum2str-maps used by mzXML

			Used to access enum2str_().
			If you add maps, also add them to XMLSchemes.h.
			Add no elements to the enum after MAP_NUM.
			Each map corresponds to a string in XMLSchemes.h.
		*/
		enum MapTypes {	POLARITYMAP=0, IONTYPEMAP, TYPEMAP, ANALYZERTYPEMAP, SCANMODEMAP,
										ATTMAP, TAGMAP, RESMETHODMAP, PEAKPROCMAP, PRECISIONMAP,MAP_NUM};

		/// Possible precisions for Base64 data encoding
		enum Precision { UNKNOWN_PRECISION, REAL, DOUBLE};

		typedef typename MapType::SpectrumType SpectrumType;
		typedef typename SpectrumType::PeakType PeakType;
		typedef typename SpectrumType::Iterator PeakIterator;
		typedef typename SpectrumType::PrecursorPeakType PrecursorPeakType;

		/**@name temporary datastructures to hold parsed data */
    //@{
   	PeakIterator peak_;
		SpectrumType* spec_;
		MassAnalyzer* analyzer_;
		MetaInfoDescription* meta_;
		String meta_id_;
		Base64 decoder_;
		Size peak_count_;
		Precision precision_;
		String char_rest_;
		const xercesc::Attributes* atts_;

		//@}

		/// spectrum counter (spectra without peaks are not written)
		UnsignedInt spec_write_counter_;

		/// Add name, value and description to a given MetaInfo object
		void setAddInfo(MetaInfoInterface& info, const String& name, const String& value, const String& description)
		{
			info.metaRegistry().registerName(name, description);
			info.setMetaValue(name,value);
		}

		/// write metaInfo to xml (usually in nameValue-tag)
		inline void writeUserParam_(std::ostream& os, const MetaInfoInterface& meta,
																int indent=4, String tag="nameValue")
		{
			std::vector<std::string> keys;  // Vector to hold keys to meta info
			meta.getKeys(keys);

			for (std::vector<std::string>::const_iterator it = keys.begin(); it!=keys.end(); ++it)
				if ( (*it)[0] != '#')  // internally used meta info start with '#'
			{
				String name = *it;
				os << String(indent,'\t') << "<" << tag << " name=\"";
				if (tag=="processingOperation" && name.find('#')!=std::string::npos)
				{
					std::vector<String> parts;
					name.split('#',parts);
					os << parts[0] << "\" type=\"" << parts[1];
				}
				else
				{
					os << name;
				}
				os << "\" value=\""
					 << meta.getMetaValue(*it) << "\"/>\n";
			}
		}

		/// check if value of attribute equals the required value, otherwise throw error
		inline void checkAttribute_(UnsignedInt attribute, const String& required, const String& required_alt="")
		{
			//TODO improve performace
			const XMLCh* tmp = xercesc::XMLString::transcode(enum2str_(ATTMAP,attribute).c_str());
			if (tmp==0) return;
			if (atts_->getIndex(tmp)==-1) return;
			String value = xercesc::XMLString::transcode(atts_->getValue(tmp));
			if (value!=required && value!=required_alt)
			{
				const xercesc::Locator* loc;
				setDocumentLocator(loc);
				String tmp = String("Invalid value \"") + value + "\" for attribute \"" + enum2str_(ATTMAP,attribute) + "\" in file " + file_;
				error(xercesc::SAXParseException(xercesc::XMLString::transcode(tmp.c_str()), *loc )); 
			}
		}

		/// return value of attribute as String
		inline String getAttributeAsString(UnsignedInt attribute)
		{
			//TODO improve performace
			const XMLCh* tmp = xercesc::XMLString::transcode(enum2str_(ATTMAP,attribute).c_str());
			if (atts_->getIndex(tmp)==-1) 
			{
				return "";
			}
			return xercesc::XMLString::transcode(atts_->getValue(tmp));
		}
  };







	//--------------------------------------------------------------------------------



	template <typename MapType>
  void MzXMLHandler<MapType>::characters(const XMLCh* const chars, const unsigned int /*length*/)
  {
  	//std::cout << " -- Chars -- "<< xercesc::XMLString::transcode(chars) << " -- " << std::endl;
  		
		if(is_parser_in_tag_[PEAKS])
		{
			//chars may be split to several chunks => concatenate them
			char_rest_ += xercesc::XMLString::transcode(chars);
		}
		else if (	is_parser_in_tag_[OFFSET] ||
							is_parser_in_tag_[INDEXOFFSET] ||
							is_parser_in_tag_[SHA1])
		{
			
		}
		else if (	is_parser_in_tag_[PRECURSORMZ])
		{
			spec_->getPrecursorPeak().getPosition()[0] = asFloat_(xercesc::XMLString::transcode(chars));
		}
		else if (	is_parser_in_tag_[COMMENT])
		{
			if (is_parser_in_tag_[INSTRUMENT])
			{
				 setAddInfo(exp_->getInstrument(),"#Comment" , xercesc::XMLString::transcode(chars), "Instrument.Comment");
			}
			else if (is_parser_in_tag_[DATAPROCESSING])
			{
				setAddInfo(exp_->getProcessingMethod(),"#Comment", xercesc::XMLString::transcode(chars),"DataProcessing.Comment");
			}
			else if (is_parser_in_tag_[SCAN])
			{
				spec_->setComment( xercesc::XMLString::transcode(chars) );
			}
			else if (String(xercesc::XMLString::transcode(chars)).trim()!="")
			{
				std::cout << "Unhandled characters: \"" << xercesc::XMLString::transcode(chars) << "\"" << std::endl;
			}
		}
		else if (String(xercesc::XMLString::transcode(chars)).trim()!="")
		{
				std::cout << "Unhandled characters: \"" << xercesc::XMLString::transcode(chars) << "\"" << std::endl;
		}
  	//std::cout << " -- !Chars -- " << std::endl;
  }

	template <typename MapType>
  void MzXMLHandler<MapType>::startElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname, const xercesc::Attributes& attributes)
  {
  	//std::cout << " -- Start -- "<< xercesc::XMLString::transcode(qname) << " -- " << std::endl;
  	
  	String tmp_str;
  	int tag = str2enum_(TAGMAP,xercesc::XMLString::transcode(qname),"opening tag");	// index of current tag
		is_parser_in_tag_[tag] = true;
		atts_ = &attributes;
		
		switch(tag)
		{
			case MSRUN:
				tmp_str = getAttributeAsString(SCANCOUNT);
				if (tmp_str!="") // optional attribute
				{  
					exp_->reserve( asUnsignedInt_(tmp_str) );
				}
			case MZXML:
				// look for schema information
				if (atts_->getIndex(xercesc::XMLString::transcode(enum2str_(ATTMAP,SCHEMA).c_str()))!=-1)
				{
					tmp_str = getAttributeAsString(SCHEMA);
					//std::cout << "SCHEMA: " << tmp_str << std::endl;
					if (tmp_str!="")
					{
						for (UnsignedInt index=0; index<Schemes::MzXML_num; ++index)
						{
							if (tmp_str.hasSubstring(Schemes::MzXML[index][0]))
							{
								schema_ = index;
								// refill maps with older schema
								for (Size i=0; i<str2enum_array_.size(); i++)	str2enum_array_[i].clear();
								for (Size i=0; i<enum2str_array_.size(); i++)	enum2str_array_[i].clear();
								fillMaps_(Schemes::MzXML[schema_]);
								break;
							}
						}
					}
				}
				// Additional attributes: startTime, endTime
				break;
			case PARENTFILE:
				exp_->getSourceFile().setNameOfFile( getAttributeAsString(FILENAME).c_str() );
				exp_->getSourceFile().setFileType( getAttributeAsString(FILETYPE) );
				// Additional attributes: fileSha1
				break;
			case INSTRUMENT:
				{
					if (attributes.getLength()==0) break;  // attributes only in mzXML 1.0
					exp_->getInstrument().setModel( xercesc::XMLString::transcode(attributes.getValue(xercesc::XMLString::transcode(enum2str_(TAGMAP,MODEL).c_str()))));
					exp_->getInstrument().setVendor( xercesc::XMLString::transcode(attributes.getValue(xercesc::XMLString::transcode(enum2str_(TAGMAP,MANUFACTURER).c_str()))));
					MassAnalyzer analyzer;
					String str = enum2str_(TAGMAP,ANALYZER);
					analyzer.setType((MassAnalyzer::AnalyzerType)str2enum_(ANALYZERTYPEMAP,xercesc::XMLString::transcode(atts_->getValue(xercesc::XMLString::transcode(str.c_str()))),str.c_str()));
					exp_->getInstrument().getMassAnalyzers().push_back(analyzer);
					str = enum2str_(TAGMAP,IONISATION);
					exp_->getInstrument().getIonSource().setIonizationMethod((IonSource::IonizationMethod)str2enum_(IONTYPEMAP,xercesc::XMLString::transcode(atts_->getValue(xercesc::XMLString::transcode(str.c_str()))),str.c_str()));
				}
				break;
			case SOFTWARE:
				if (is_parser_in_tag_[DATAPROCESSING])
				{
					exp_->getSoftware().setVersion( getAttributeAsString(SOFTWAREVERSION) );
					exp_->getSoftware().setName( getAttributeAsString(NAME) );
					exp_->getSoftware().setComment( getAttributeAsString(TYPE) );
					exp_->getSoftware().setCompletionTime( asDateTime_(getAttributeAsString(COMPLETION_TIME)) );
				}
				else if (is_parser_in_tag_[INSTRUMENT])
				{
					// not part of METADATA -> putting it into MetaInfo
					std::string swn = "#InstSoftware", swn_d = "Instrument software name",
						swv = "#InstSoftwareVersion", swv_d = "Instrument software version",
						swt = "#InstSoftwareType", swt_d = "Instrument software type",
						cmpl = "#InstSoftwareTime", cmpl_d = "Instrument software completion time";
					MetaInfoRegistry& registry =	MetaInfo().registry();
					registry.registerName(swn,swn_d);
					registry.registerName(swv,swv_d);
					registry.registerName(swt,swt_d);
					registry.registerName(cmpl,cmpl_d);
					exp_->getInstrument().setMetaValue(swn,getAttributeAsString(NAME));
					exp_->getInstrument().setMetaValue(swv,getAttributeAsString(SOFTWAREVERSION));
					exp_->getInstrument().setMetaValue(swt,getAttributeAsString(TYPE));
					tmp_str = getAttributeAsString(COMPLETION_TIME);
					if  (tmp_str!="")
					{
						DateTime time = asDateTime_(tmp_str);
						time.get(tmp_str);
						exp_->getInstrument().setMetaValue(cmpl,tmp_str);
					}
				}
				break;
			case PEAKS:
				{
					checkAttribute_(PRECISION,enum2str_(PRECISIONMAP,REAL),
																		enum2str_(PRECISIONMAP,DOUBLE));
					const String str = enum2str_(ATTMAP,PRECISION);
					precision_ = (Precision) str2enum_(PRECISIONMAP,xercesc::XMLString::transcode(atts_->getValue(xercesc::XMLString::transcode(str.c_str()))),str.c_str());
					checkAttribute_(BYTEORDER,"network");
					checkAttribute_(PAIRORDER,"m/z-int");
				}
				break;
			case PRECURSORMZ:
				{
					PrecursorPeakType& peak
						= spec_->getPrecursorPeak();
					peak.setIntensity( asFloat_(getAttributeAsString(PRECURSOR_INTENSITY)) );
					// optional attribute
					if (!(getAttributeAsString(PRECURSOR_CHARGE)==""))
						peak.setCharge(asSignedInt_(getAttributeAsString(PRECURSOR_CHARGE)));
					// Unhandled: windowWideness, precursorScanNum (optinal)
				}
				break;
			case SCAN:
				{
					//exp_->insert(exp_->end(), SpectrumType() );
					exp_->push_back(SpectrumType() );
					spec_ = &(exp_->back());
					
					// required attributes
					spec_->setMSLevel( asSignedInt_(getAttributeAsString(MSLEVEL)) );
					peak_count_ = asSignedInt_( getAttributeAsString(PEAKSCOUNT) );
					spec_->resize(peak_count_);
					peak_ = spec_->begin();

					//optinal attributes
					for (UnsignedInt i=0; i<attributes.getLength(); i++)
					{
						int att = str2enum_(ATTMAP,xercesc::XMLString::transcode(attributes.getQName(i)),"scan attribute");
						String value = xercesc::XMLString::transcode(attributes.getValue(i));
						InstrumentSettings& sett = spec_->getInstrumentSettings();
						switch (att)
							{
							case POLARITY:
								sett.setPolarity( (IonSource::Polarity) str2enum_(POLARITYMAP,value,"polarity") );
								break;
							case SCANTYPE:
								sett.setScanMode( (InstrumentSettings::ScanMode) str2enum_(SCANMODEMAP,value,"scan mode") );
								break;
							case RETTIME:
								value.trim();
								spec_->setRetentionTime( asFloat_(value.substr(2,value.size()-3)));
								//std::cout << spec_->getRetentionTime() << std::endl;
								break;
							case STARTMZ:      sett.setMzRangeStart( asDouble_(value)); break;
							case ENDMZ:				 sett.setMzRangeStop( asDouble_(value)); break;
							case DEISOTOPED:
							  exp_->getProcessingMethod().setDeisotoping(asBool_(value));	break;
							case DECONVOLUTED:
								exp_->getProcessingMethod().setChargeDeconvolution(asBool_(value));	break;
							//	case CENTROIDED: case IONENERGY:	case COLLENERGY: 	case PRESSURE: case LOWMZ:
							//	case HIGHMZ: case BASEPEAKMZ:	case BASEPEAKINT:	case TOTIONCURRENT
							}
					}
				}
				break;
			case OPERATOR:
				{
					exp_->getContacts().insert(exp_->getContacts().end(), ContactPerson());
					exp_->getContacts().back().setName( getAttributeAsString(FIRST_NAME) + " " +getAttributeAsString(LAST_NAME));
					if (!(getAttributeAsString(EMAIL)==""))
					{
						exp_->getContacts().back().setEmail(getAttributeAsString(EMAIL));
					}
					exp_->getContacts().back().setContactInfo( String("PHONE: ")+getAttributeAsString(PHONE)+" URI: " + getAttributeAsString(URI));
				}
				break;
			case MANUFACTURER:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,MANUFACTURER))
					exp_->getInstrument().setVendor( getAttributeAsString(VALUE) );
				break;
			case MODEL:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,MODEL))
					exp_->getInstrument().setModel( getAttributeAsString(VALUE) );
				break;
			case IONISATION:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,IONISATION))
					exp_->getInstrument().getIonSource().setIonizationMethod(
						(IonSource::IonizationMethod)
						str2enum_(IONTYPEMAP,getAttributeAsString(VALUE),"ionization type")
					);
				break;
			case ANALYZER:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,ANALYZER))
				{
					exp_->getInstrument().getMassAnalyzers().insert(exp_->getInstrument().getMassAnalyzers().end(), MassAnalyzer());
					analyzer_ = &(exp_->getInstrument().getMassAnalyzers().back());
					analyzer_->setType( (MassAnalyzer::AnalyzerType)
						str2enum_(ANALYZERTYPEMAP,getAttributeAsString(VALUE),"analyzer type")
					);
				}
				break;
			case DETECTOR:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,DETECTOR))
				{
					IonDetector& ion_d = exp_->getInstrument().getIonDetector();
					ion_d.setType( (IonDetector::Type) str2enum_(TYPEMAP,getAttributeAsString(VALUE),"detector type") );
				}
				break;
			case RESOLUTION:
				if (getAttributeAsString(CATEGORY)==enum2str_(TAGMAP,RESOLUTION))
				{
					if (analyzer_ == 0) break;
					analyzer_->setResolutionMethod(
						(MassAnalyzer::ResolutionMethod)
						str2enum_(RESMETHODMAP,getAttributeAsString(VALUE),"resolution method"));
				}
				break;
			case DATAPROCESSING:
					//optinal attributes
					for (UnsignedInt i=0; i<attributes.getLength(); i++)
					{
						int att = str2enum_(ATTMAP,xercesc::XMLString::transcode(attributes.getQName(i)),"dataprocessing attribute");
						String value = xercesc::XMLString::transcode(attributes.getValue(i));
						switch (att)
							{
								case DEISOTOPED:
								  exp_->getProcessingMethod().setDeisotoping(asBool_(value));
									break;
								case DECONVOLUTED:
									exp_->getProcessingMethod().setChargeDeconvolution(asBool_(value));
									break;
								case CENTROIDED:
									exp_->getProcessingMethod().setSpectrumType((SpectrumSettings::SpectrumType)
										str2enum_(PEAKPROCMAP,value,"peak processing"));
									break;
								//UNHANDLED: spotIntegration, intensityCutoff
							}
					}
					break;
			case NAMEVALUE:
				if (is_parser_in_tag_[INSTRUMENT])
				{
					setAddInfo(	exp_->getInstrument(), getAttributeAsString(NAME), getAttributeAsString(VALUE), "Instrument.Comment");
				}
				else if (is_parser_in_tag_[SCAN])
				{
					setAddInfo(	*spec_, getAttributeAsString(NAME), getAttributeAsString(VALUE), "Instrument.Comment");
				}
				else
				{
					std::cout << " Warning Unhandled tag: \"" << enum2str_(TAGMAP,NAMEVALUE) << "\"" << std::endl;
				}
				break;
			case PROCESSING:
				setAddInfo(exp_->getProcessingMethod(), getAttributeAsString(NAME) + "#" + getAttributeAsString(TYPE),
						getAttributeAsString(VALUE), "Processing.Comment");

		}
		
		//std::cout << " -- !Start -- " << std::endl;
	}


	template <typename MapType>
	void MzXMLHandler<MapType>::endElement(const XMLCh* const /*uri*/, const XMLCh* const /*local_name*/, const XMLCh* const qname)
  {
  	//std::cout << " -- Ende -- "<< xercesc::XMLString::transcode(qname) << " -- " << std::endl;
  	
		int tag = str2enum_(TAGMAP,xercesc::XMLString::transcode(qname),"closing tag"); // index of current tag
		is_parser_in_tag_[tag] = false;

		if (tag==INSTRUMENT && analyzer_)
		{
			analyzer_ = 0;
		}
		
		if (tag==PEAKS)
		{
			if (precision_==DOUBLE)		//precision 64
			{
				double* data = decoder_.decodeDoubleCorrected(char_rest_.c_str(), char_rest_.size());
				char_rest_ = "";
				//push_back the peaks into the container
				for (Size n = 0 ; n < ( 2 * peak_count_) ; n += 2)
				{
					
					peak_->getPosition()[0] = data[n];
					peak_->setIntensity(data[n+1]);
 					++peak_;
 				}
			}
			else											//precision 32
			{
				float* data = decoder_.decodeFloatCorrected(char_rest_.c_str(), char_rest_.size());
				char_rest_ = "";
				//push_back the peaks into the container
				for (Size n = 0 ; n < (2 * peak_count_) ; n += 2)
				{
					//std::cout << "n  : " << n  << " -> " << data[n] << std::endl;
					//std::cout << "n+1: " << n+1  << " -> " << data[n+1] << std::endl;
					
					peak_->getPosition()[0] = data[n];
					peak_->setIntensity(data[n+1]);
     			++peak_;
				}
			}
		}
		//std::cout << " -- Ende -- " << std::endl;
  }

	template <typename MapType>
	void MzXMLHandler<MapType>::writeTo(std::ostream& os)
	{
		//determine how many spectra there are (count only those with peaks)
		UnsignedInt count_tmp_  = 0;
		for (UnsignedInt s=0; s<cexp_->size(); s++)
		{
			const SpectrumType& spec = (*cexp_)[s];
			if (spec.size()!=0) ++count_tmp_;
		}

		os << "<!-- -*- Mode: XML; tab-width: 2; -*- -->\n"
			 << "<mzXML xmlns=\"http://sashimi.sourceforge.net/schema_revision/mzXML_2.0\" "
		   << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
			 << "xsi:schemaLocation=\"http://sashimi.sourceforge.net/schema_revision/mzXML_2.0 "
			 << "http://sashimi.sourceforge.net/schema_revision/mzXML_2.0/mzXML_idx_2.0.xsd\">\n"
			 << "\t<msRun scanCount=\"" << count_tmp_ << "\">\n"
			 << "\t\t<parentFile fileName=\"" << cexp_->getSourceFile().getNameOfFile()
			 << "\" fileType=\"" << cexp_->getSourceFile().getFileType()
			 << "\" fileSha1=\"0000000000000000000000000000000000000000\"/>\n";

		if (cexp_->getInstrument() != Instrument())
		{
			const Instrument& inst = cexp_->getInstrument();
			os << "\t\t<msInstrument>\n"
				 << "\t\t\t<msManufacturer category=\"msManufacturer\" value=\""
				 <<	inst.getVendor() << "\"/>\n"
				 << "\t\t\t<msModel category=\"msModel\" value=\""
				 << inst.getModel() << "\"/>\n"
				 << "\t\t\t<msIonisation category=\"msIonisation\" value=\""
				 << enum2str_(IONTYPEMAP,inst.getIonSource().getIonizationMethod())
				 << "\"/>\n";

			const std::vector<MassAnalyzer>& analyzers = inst.getMassAnalyzers();
			if ( analyzers.size()>0 )
			{
				os << "\t\t\t<msMassAnalyzer category=\"msMassAnalyzer\" value=\""
					 << enum2str_(ANALYZERTYPEMAP,analyzers[0].getType())  << "\"/>\n";
			}
			else
			{
				std::cout << " Warning: mzXML supports only one analyzer! Skipping the other " << (analyzers.size()-1) << "mass analyzers." << std::endl;
			}
			os << "\t\t\t<msDetector category=\"msDetector\" value=\""
				 << enum2str_(TYPEMAP,inst.getIonDetector().getType()) << "\"/>\n";
			try
			{
				std::string type = inst.getMetaValue("#InstSoftwareType").toString(),
				name = inst.getMetaValue("#InstSoftware").toString(),
				version = inst.getMetaValue("#InstSoftwareVersion").toString();
				String str = inst.getMetaValue("#InstSoftwareTime").toString();
				String time(str);
				time.replace(' ', 'T');
				os << "\t\t\t<software type=\"" << type
					 << "\" name=\"" << name
					 << "\" version=\"" << version
				 	 << "\" completionTime=\"" << time << "\"/>\n";
			}
			catch(Exception::InvalidValue exception)
			{

			}
			
			if ( analyzers.size()>0 )
			{
				if (analyzers[0].getResolutionMethod())
					os << "\t\t\t<msResolution category=\"msResolution\" value=\""
				 		 << enum2str_(RESMETHODMAP,analyzers[0].getResolutionMethod()) << "\"/>\n";
			}
			else
			{
				std::cout << "Warning: mzXML supports only one analyzer! Skipping the other " << (analyzers.size()-1) << "mass analyzers." << std::endl;
			}
			
			if ( cexp_->getContacts().size()>0 )
			{
				const ContactPerson& cont = cexp_->getContacts()[0];
				
				os << "\t\t\t<operator first=\"";
				std::vector<String> tmp;
				if (cont.getName().split(',',tmp))
				{
					os << tmp[1] << "\" last=\"" << tmp[0];
				}
				else
				{
					if (cont.getName().split(' ',tmp))
					{
						os << tmp[0] << "\" last=\"" << tmp[1];
					}
					else
					{
						os << "\" last=\"" << cont.getName();
					}
				}
				String info = cont.getContactInfo();
				if (info.find("PHONE:")!=std::string::npos && info.find("URI:")!=std::string::npos)
				{
					UnsignedInt uri = info.find("URI:");
					os << "\" phone=\"" << info.substr(6,uri-6).trim()
						 << "\" email=\"" << cont.getEmail()
						 << "\" URI=\"" << info.substr(uri+4).trim() << "\"/>\n";
				}
				else
				{
					os << "\" phone=\"\" email=\"" << cont.getEmail() << "\" URI=\"" << info << "\"/>\n";
				}
			}
			writeUserParam_(os,inst,3);
			try
			{
				DataValue com = inst.getMetaValue("#Comment");
				if (!com.isEmpty()) os << "\t\t\t<comment>" << com << "</comment>\n";
			}
			catch(Exception::InvalidValue exception)
			{

			}
			os << "\t\t</msInstrument>\n";
		}

		const Software& software = cexp_->getSoftware();
		os << "\t\t<dataProcessing deisotoped=\""
			 << cexp_->getProcessingMethod().getDeisotoping()
			 << "\" chargeDeconvoluted=\""
			 << cexp_->getProcessingMethod().getChargeDeconvolution()
			 << "\" centroided=\""
			 << enum2str_(PEAKPROCMAP,cexp_->getProcessingMethod().getSpectrumType())
			 << "\">\n"
			 << "\t\t\t<software type=\"" << software.getComment()
			 << "\" name=\"" << software.getName()
			 << "\" version=\"" << software.getVersion();

		if (software.getCompletionTime() != DateTime())
		{
			String tmp;
			software.getCompletionTime().get(tmp);
			String qtmp(tmp);
			qtmp.replace(' ', 'T');
			os << "\" completionTime=\"" << qtmp;
		}
		os << "\"/>\n";
		writeUserParam_(os,cexp_->getProcessingMethod(),3,"processingOperation");

		try
		{
			DataValue com = cexp_->getProcessingMethod().getMetaValue("#Comment");
			if (!com.isEmpty()) os << "\t\t\t<comment>" << com << "</comment>\n";
		}
		catch(Exception::InvalidValue exception)
		{

		}

		os << "\t\t</dataProcessing>\n";

		// write scans
		for (UnsignedInt s=0; s<cexp_->size(); s++)
		{
			const SpectrumType& spec = (*cexp_)[s];

			//do not write empty spectra
			if (spec.size()==0) continue;
			
			int MSLevel = spec.getMSLevel();

			if (MSLevel==1 && s!=0)
				os << String(MSLevel+1,'\t') << "</scan>\n";

			os << String(MSLevel+1,'\t')
				 << "<scan num=\"" << spec_write_counter_++ << "\" msLevel=\""
				 << spec.getMSLevel() << "\" peaksCount=\""
				 << spec.size() << "\" polarity=\""
				 << enum2str_(POLARITYMAP,spec.getInstrumentSettings().getPolarity());

			if (spec.getInstrumentSettings().getScanMode())
			{
				os << "\" scanType=\""
					 << enum2str_(SCANMODEMAP,spec.getInstrumentSettings().getScanMode());
			}
			os << "\" retentionTime=\"PT"
				 << spec.getRetentionTime() << "S\"";
			if (spec.getInstrumentSettings().getMzRangeStart()!=0)
				os << " startMz=\"" << spec.getInstrumentSettings().getMzRangeStart() << "\"";
			if (spec.getInstrumentSettings().getMzRangeStop()!=0)
				os << " endMz=\"" << spec.getInstrumentSettings().getMzRangeStop() << "\"";
			os << ">\n";

			const PrecursorPeakType& peak = spec.getPrecursorPeak();
			if (peak!= PrecursorPeakType())
			{
				os << String(MSLevel+2,'\t') << "<precursorMz precursorIntensity=\""
					 << peak.getIntensity();
				if (peak.getCharge()!=0)
					os << "\" precursorCharge=\"" << peak.getCharge();
				os << "\">"
				 	 << peak.getPosition()[0] << "</precursorMz>\n";
			}

			os << String(MSLevel+2,'\t') << "<peaks precision=\"32\""
				 << " byteOrder=\"network\" pairOrder=\"m/z-int\">";

			float* tmp = decoder_.getFloatBuffer(spec.size()*2);
			for (UnsignedInt i=0; i<spec.size(); i++)
			{
				tmp[2*i]   = spec.getContainer()[i].getPosition()[0];
				tmp[2*i+1] = spec.getContainer()[i].getIntensity();
			}
			os << decoder_.encodeFloatCorrected() << "</peaks>\n";

			writeUserParam_(os,spec,MSLevel+2);
			if (spec.getComment() != "")
				os << String(MSLevel+2,'\t') << "<comment>" << spec.getComment() << "</comment>\n";

			if (MSLevel==2)
				os << String(MSLevel+1,'\t') << "</scan>\n";
		}

		if (cexp_->size()) os << "\t\t</scan>\n";
		os << "\t</msRun>\n"
			 << "\t<indexOffset>0</indexOffset>\n"
			 << "</mzXML>\n";
	}

	} // namespace Internal

} // namespace OpenMS

#endif
