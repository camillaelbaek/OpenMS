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
// $Maintainer: $
// $Authors: Hendrik Weisser $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>

///////////////////////////

#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationModel.h>

///////////////////////////

START_TEST(TransformationDescription, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace OpenMS;
using namespace std;

TransformationModel* ptr = 0;
TransformationModel* nullPointer = 0;
START_SECTION((TransformationModel()))
{
	ptr = new TransformationModel();
  TEST_NOT_EQUAL(ptr, nullPointer)
}
END_SECTION

START_SECTION((TransformationModel(const DataPoints&, const Param&)))
{
	ptr = new TransformationModel(TransformationModel::DataPoints(), Param());
  TEST_NOT_EQUAL(ptr, nullPointer)
}
END_SECTION

START_SECTION((~TransformationModel()))
{
	delete ptr;
}
END_SECTION

TransformationModel::DataPoints data, empty;
data.push_back(make_pair(0.0, 1.0));
data.push_back(make_pair(1.0, 2.0));
data.push_back(make_pair(1.0, 4.0));

START_SECTION((virtual DoubleReal evaluate(const DoubleReal value) const))
{
	// null model (identity):
	TransformationModel tm;
	TEST_REAL_SIMILAR(tm.evaluate(-3.14159), -3.14159);
	TEST_REAL_SIMILAR(tm.evaluate(0.0), 0.0);
	TEST_REAL_SIMILAR(tm.evaluate(12345678.9), 12345678.9);

	// linear model:
	TEST_EXCEPTION(Exception::IllegalArgument, TransformationModelLinear 
								 lm(empty, Param())); // need data

	TransformationModelLinear lm(data, Param());
	TEST_REAL_SIMILAR(lm.evaluate(-0.5), 0.0);
	TEST_REAL_SIMILAR(lm.evaluate(0.0), 1.0);
	TEST_REAL_SIMILAR(lm.evaluate(0.5), 2.0);
	TEST_REAL_SIMILAR(lm.evaluate(1.0), 3.0);
	TEST_REAL_SIMILAR(lm.evaluate(1.5), 4.0);
	
	// interpolation model:
	TEST_EXCEPTION(Exception::IllegalArgument, TransformationModelInterpolated
								 im(empty, Param())); // need data
	
  data.push_back(make_pair(2.0, 2.0));
	Param params;
	params.setValue("interpolation_type", "linear");
	TransformationModelInterpolated im(data, params);
	// interpolation:
	TEST_REAL_SIMILAR(im.evaluate(0.0), 1.0);
	TEST_REAL_SIMILAR(im.evaluate(0.5), 2.0);
	TEST_REAL_SIMILAR(im.evaluate(1.0), 3.0);
	TEST_REAL_SIMILAR(im.evaluate(1.5), 2.5);
	TEST_REAL_SIMILAR(im.evaluate(2.0), 2.0);
	// extrapolation:
	TEST_REAL_SIMILAR(im.evaluate(-0.5), 0.75);
	TEST_REAL_SIMILAR(im.evaluate(2.5), 2.25);

	// B-spline model:
	params.clear();
	params.setValue("num_breakpoints", 4);
	TEST_EXCEPTION(Exception::IllegalArgument, TransformationModelBSpline 
								 bm(empty, params)); // need data

	data.clear();
  data.push_back(make_pair(1.2, 5.2));
  data.push_back(make_pair(3.2, 7.3));
  data.push_back(make_pair(2.2, 6.25));
  data.push_back(make_pair(2.2, 3.1));
  data.push_back(make_pair(2.2, 7.25));
  data.push_back(make_pair(3.0, 8.5));
  data.push_back(make_pair(3.1, 4.7));
  data.push_back(make_pair(1.7, 6.0));
  data.push_back(make_pair(2.9, 4.7));
  data.push_back(make_pair(4.2, 5.0));
  data.push_back(make_pair(3.7, -2.4));

	TransformationModelBSpline bm(data, params);

#if 0
	// Since the numbers in this test were verified by manual (in fact, visual)
	// inspection, here is a recipe how this was done:
	//
	// To grep for output, "pairs:" and "spline:", you might use a command line
	// like this:
	// make TransformationDescription_test && ctest -V -R TransformationDescription_test &&  ctest -V -R TransformationDescription_test | grep pairs: > points.dat && ctest -V -R TransformationDescription_test | grep spline: > bla.dat
	// To have a look at the results using gnuplot:
	// gnuplot -  # (to start gnuplot)
	// plot './bla.dat' u 5:6, './points.dat' u 5:6

	for (UInt i = 0; i < data.size(); ++i)
	{
		STATUS("data: " << data[i].first << " " << data[i].second);
	}

	for (Int i = -10; i <= 60; i+=5)
	{
		DoubleReal value = i;
		value /= 10;
		DoubleReal image = bm.evaluate(value);
		STATUS("spline: " << value << " " << image);
	}
#endif

	DoubleReal sample_values[] = {-1, -0.5, 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6};
	DoubleReal sample_images[] = {-14.3519415123977, -9.91557518507088, -5.4792088577441, -1.04284253041731, 3.39352379690948, 6.4561466812738, 5.4858954730427, 6.14659387774751, 6.77299727168147, 0.646024122587505, -1.13062259235381, 18.3842099268184, 40.7826815802615, 63.1811532337045, 85.5796248871476};
	for (Size i = 0; i < sizeof(sample_values)/sizeof(*sample_values); ++i)
	{
		DoubleReal x = bm.evaluate(sample_values[i]);
		TEST_REAL_SIMILAR(x, sample_images[i]);
	}

}
END_SECTION

START_SECTION((void getParameters(Param& params) const))
{
	TransformationModel tm;
	Param p_in, p_out;
	tm.getParameters(p_out);
	TEST_EQUAL(p_out.empty(), true);

	p_in.setValue("symmetric_regression", "true");
	TransformationModelLinear lm(data, p_in);
	lm.getParameters(p_out);
	TEST_EQUAL(p_out, p_in);
	p_in.clear();
	p_in.setValue("slope", 12.3);
	p_in.setValue("intercept", -45.6);
	TransformationModelLinear lm2(empty, p_in);
	lm2.getParameters(p_out);
	TEST_EQUAL(p_out, p_in);
}
END_SECTION

START_SECTION(([EXTRA] void getParameters(DoubleReal&, DoubleReal&)))
{
	Param param;
	param.setValue("slope", 12.3);
	param.setValue("intercept", -45.6);
	TransformationModelLinear lm(empty, param);
	DoubleReal slope, intercept;
	lm.getParameters(slope, intercept);
	TEST_REAL_SIMILAR(param.getValue("slope"), slope);
	TEST_REAL_SIMILAR(param.getValue("intercept"), intercept);
}
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
