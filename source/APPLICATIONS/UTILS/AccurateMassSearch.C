// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2013.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Erhan Kenar $
// $Authors: Erhan Kenar, $
// --------------------------------------------------------------------------

#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/FORMAT/ConsensusXMLFile.h>
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/FORMAT/MzTab.h>
#include <OpenMS/FORMAT/MzTabFile.h>
#include <OpenMS/FORMAT/FileHandler.h>
#include <OpenMS/FORMAT/FileTypes.h>

#include <OpenMS/ANALYSIS/ID/AccurateMassSearchEngine.h>

#include <OpenMS/APPLICATIONS/TOPPBase.h>

using namespace OpenMS;
using namespace std;

//-------------------------------------------------------------
//Doxygen docu
//-------------------------------------------------------------

/**
        @page UTILS_AccurateMassSearch AccurateMassSearch

        @brief AccurateMassSearch assembles metabolite features from singleton mass traces.

        <CENTER>
        <table>
        <tr>
        <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. predecessor tools </td>
        <td VALIGN="middle" ROWSPAN=3> \f$ \longrightarrow \f$ AccurateMassSearch \f$ \longrightarrow \f$</td>
        <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. successor tools </td>
        </tr>
        <tr>
        <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_PeakPickerHiRes </td>
        <td VALIGN="middle" ALIGN = "center" ROWSPAN=2> @ref TOPP_TextExporter</td>
        </tr>
        <tr>
        <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_PeakPickerWavelet </td>
        </tr>
        </table>
        </CENTER>

        Mass traces alone would allow for further analyzes such as metabolite ID or statistical
        evaluation. However, in general, monoisotopic mass traces are accompanied with satellite
        C13 peaks and thus may render the analysis more difficult. @ref AccurateMassSearch fulfills
        a further data reduction step by assembling compatible mass traces to metabolite features
        (that is, mass traces all stemming from one metabolite). To this end, multiple metabolite
        hypotheses are formulated and scored according to how well differences in RT and m/z or
        intensity ratios match to those of theoretical isotope patterns.

        <B>The command line parameters of this tool are:</B>
        @verbinclude UTILS_AccurateMassSearch.cli
*/

// We do not want this class to show up in the docu:
/// @cond TOPPCLASSES

class TOPPAccurateMassSearch :
        public TOPPBase
{
public:
    TOPPAccurateMassSearch() :
        TOPPBase("AccurateMassSearch", "Find potential HMDB ids within the given mass error window.", false)
    {
    }

protected:

    void registerOptionsAndFlags_()
    {
        registerInputFile_("in", "<file>", "", "featureXML or consensusXML file");
        setValidFormats_("in", StringList::create("featureXML,consensusXML"));
        registerOutputFile_("out", "<file>", "", "mzTab file");
        setValidFormats_("out", StringList::create("csv"));

        // addEmptyLine_();
        // addText_("Parameters for the accurate mass search can be given in the 'algorithm' part of INI file.");
        registerSubsection_("algorithm", "Algorithm parameters section");
    }

    Param getSubsectionDefaults_(const String& /*section*/) const
    {
        return AccurateMassSearchEngine().getDefaults();
    }

    ExitCodes main_(int, const char**)
    {

        //-------------------------------------------------------------
        // parameter handling
        //-------------------------------------------------------------

        String in = getStringOption_("in");
        String out = getStringOption_("out");

        //-------------------------------------------------------------
        // loading input
        //-------------------------------------------------------------

        //        MzMLFile mz_data_file;
        //        mz_data_file.setLogType(log_type_);
        //        MSExperiment<Peak1D> ms_peakmap;
        //        std::vector<Int> ms_level(1, 1);
        //        (mz_data_file.getOptions()).setMSLevels(ms_level);
        //        mz_data_file.load(in, ms_peakmap);

        //        if (ms_peakmap.empty())
        //        {
        //            LOG_WARN << "The given file does not contain any conventional peak data, but might"
        //                        " contain chromatograms. This tool currently cannot handle them, sorry.";
        //            return INCOMPATIBLE_INPUT_DATA;
        //        }

        //-------------------------------------------------------------
        // get parameters
        //-------------------------------------------------------------

        Param ams_param = getParam_().copy("algorithm:", true);
        writeDebug_("Parameters passed to AccurateMassSearch", ams_param, 3);


        // mzTAB output datastructure
        MzTab mztab_output;
        MzTabFile mztab_outfile;

        FileTypes::Type filetype = FileHandler::getType(in);

        if (filetype == FileTypes::FEATUREXML)
        {
            FeatureMap<> ms_feat_map;

            FeatureXMLFile().load(in, ms_feat_map);


            //-------------------------------------------------------------
            // do the work
            //-------------------------------------------------------------

            AccurateMassSearchEngine ams;
            ams.setParameters(ams_param);

            ams.run(ms_feat_map, mztab_output);

            //-------------------------------------------------------------
            // writing output
            //-------------------------------------------------------------

            // annotate output with data processing info
            //addDataProcessing_(ms_feat_map, getProcessingInfo_(DataProcessing::IDENTIFICATION_MAPPING));


        }
        else if (filetype == FileTypes::CONSENSUSXML)
        {
            ConsensusMap ms_cons_map;

            ConsensusXMLFile().load(in, ms_cons_map);

            //-------------------------------------------------------------
            // do the work
            //-------------------------------------------------------------

            AccurateMassSearchEngine ams;
            ams.setParameters(ams_param);

            ams.run(ms_cons_map, mztab_output);

            //-------------------------------------------------------------
            // writing output
            //-------------------------------------------------------------

            // annotate output with data processing info
            //addDataProcessing_(ms_feat_map, getProcessingInfo_(DataProcessing::IDENTIFICATION_MAPPING));

        }

        mztab_outfile.store(out, mztab_output);


        return EXECUTION_OK;
    }

};


int main(int argc, const char** argv)
{
    TOPPAccurateMassSearch tool;
    return tool.main(argc, argv);
}

/// @endcond
