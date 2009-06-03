// -*- mode: C++; tab-width: 2; -*-
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
// $Maintainer: Johannes Junker $
// $Authors: Johannes Junker $
// --------------------------------------------------------------------------

#ifndef OPENMS_VISUAL_TOPPASTOOLVERTEX_H
#define OPENMS_VISUAL_TOPPASTOOLVERTEX_H

#include <OpenMS/VISUAL/TOPPASVertex.h>
#include <OpenMS/DATASTRUCTURES/Param.h>

#include <QtCore/QVector>

namespace OpenMS
{
	class OPENMS_DLLAPI TOPPASToolVertex
		: public TOPPASVertex
	{
		Q_OBJECT
		
		public:
			
			/// Stores the information for input/output files/lists
			struct IOInfo
			{
				enum IOType
				{
					IOT_FILE,
					IOT_LIST
				};
				
				IOType type;
				String param_name;
				StringList valid_types;
			};
			
			/// Default constructor
			TOPPASToolVertex();
			/// Constructor
			TOPPASToolVertex(const String& name, const String& type = "");
			/// Copy constructor
			TOPPASToolVertex(const TOPPASToolVertex& rhs);
			/// Destructor
			virtual ~TOPPASToolVertex();
			/// Assignment operator
			TOPPASToolVertex& operator= (const TOPPASToolVertex& rhs);
			
			/// Fills @p input_infos with the required input files/lists together with their valid types.
			void getRequiredInputFiles(QVector<IOInfo>& input_infos);
			/// Fills @p output_infos with the required output files/lists together with their valid types.
			void getRequiredOutputFiles(QVector<IOInfo>& output_infos);
			
			/// The static instance counter (for unique instance numbers)
			static UInt id_counter;
			
		protected:
		
			///@name reimplemented Qt events
      //@{
      void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
			//@}
			/// Initializes the parameters with standard values (from -write_ini)
			void initParam_();
			/// Fills @p io_infos with the required input/output files/lists. If @p input_files is true, input files are returned, otherwise output files.
			void getRequiredFiles_(QVector<IOInfo>& io_infos, bool input_files);
			
			/// The parameters of the tool
			Param param_;
			/// The unique ID
			UInt id_;
	};
}

#endif
