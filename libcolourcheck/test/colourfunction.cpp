/*
 * Copyright (C) 2016  Marcos Goulart Reis. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define BOOST_TEST_MODULE ColourFcnTest
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <fstream>
#include "colourfunction.hpp"

using namespace std;
using namespace colourcheck;

#define REQUIRE_CLOSE(v1,v2) BOOST_REQUIRE_CLOSE(v1,v2,0.0001)

class ColourFcnTest
{
public:
    ColourFcnTest() : cf("sample.csv")
    {
        createSampleFile();
    }

    virtual void createSampleFile()
    {
        ofstream file("sample.csv");
        file << "430,0.1,0.20,0.30" << endl
             << "435,0.2,0.40,0.60" << endl
             << "440,0.4,0.16,2.40" << endl;

        cf.read();
    }

    ~ColourFcnTest()
    {
        remove("sample.csv");
    }
    ColourFunction cf;
};

class ColourFcnTestComment : public ColourFcnTest
{
public:
    ColourFcnTestComment()
    {
        createSampleFile();
    }

    virtual void createSampleFile()
    {
        ofstream file("sample.csv");
        file << "# Wl, X, Y, Z" << endl
             << "430,0.1,0.20,0.30" << endl
             << "435,0.2,0.40,0.60" << endl
             << "440,0.4,0.16,2.40" << endl;

        cf.read();
    }

    ~ColourFcnTestComment()
    {
        remove("sample.csv");
    }
};

typedef boost::mpl::vector<ColourFcnTest, ColourFcnTestComment> Fixtures;

BOOST_FIXTURE_TEST_CASE_TEMPLATE(read_file, T, Fixtures, T)
{
    ColourFunction::value_type v;

    v = T::cf[0];
    REQUIRE_CLOSE(430.0, get<0>(v));
    REQUIRE_CLOSE(0.1, get<1>(v));
    REQUIRE_CLOSE(0.20, get<2>(v));
    REQUIRE_CLOSE(0.30, get<3>(v));

    v = T::cf[1];
    REQUIRE_CLOSE(435.0, get<0>(v));
    REQUIRE_CLOSE(0.2, get<1>(v));
    REQUIRE_CLOSE(0.40, get<2>(v));
    REQUIRE_CLOSE(0.60, get<3>(v));

    v = T::cf[2];
    REQUIRE_CLOSE(440.0, get<0>(v));
    REQUIRE_CLOSE(0.40, get<1>(v));
    REQUIRE_CLOSE(0.16, get<2>(v));
    REQUIRE_CLOSE(2.40, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(get_exact_wavelength, ColourFcnTest)
{
    ColourFunction::value_type v;

    v = cf.get(430.0);
    REQUIRE_CLOSE(430.0, get<0>(v));
    REQUIRE_CLOSE(0.1, get<1>(v));
    REQUIRE_CLOSE(0.20, get<2>(v));
    REQUIRE_CLOSE(0.30, get<3>(v));

    v = cf.get(435.0);
    REQUIRE_CLOSE(435.0, get<0>(v));
    REQUIRE_CLOSE(0.2, get<1>(v));
    REQUIRE_CLOSE(0.40, get<2>(v));
    REQUIRE_CLOSE(0.60, get<3>(v));

    v = cf.get(440.0);
    REQUIRE_CLOSE(440.0, get<0>(v));
    REQUIRE_CLOSE(0.40, get<1>(v));
    REQUIRE_CLOSE(0.16, get<2>(v));
    REQUIRE_CLOSE(2.40, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(build_new_wavelength_middle, ColourFcnTest)
{
    ColourFunction::value_type v;

    v = cf.get(432.5);
    REQUIRE_CLOSE(432.5, get<0>(v));
    REQUIRE_CLOSE(0.15, get<1>(v));
    REQUIRE_CLOSE(0.30, get<2>(v));
    REQUIRE_CLOSE(0.45, get<3>(v));

    v = cf[2];
    REQUIRE_CLOSE(435.0, get<0>(v));
    REQUIRE_CLOSE(0.2, get<1>(v));
    REQUIRE_CLOSE(0.40, get<2>(v));
    REQUIRE_CLOSE(0.60, get<3>(v));

    v = cf[1];
    REQUIRE_CLOSE(432.5, get<0>(v));
    REQUIRE_CLOSE(0.15, get<1>(v));
    REQUIRE_CLOSE(0.30, get<2>(v));
    REQUIRE_CLOSE(0.45, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(build_new_wavelength_any, ColourFcnTest)
{
    ColourFunction::value_type v;

    v = cf.get(431.25);
    REQUIRE_CLOSE(431.25, get<0>(v));
    REQUIRE_CLOSE(0.125, get<1>(v));
    REQUIRE_CLOSE(0.25, get<2>(v));
    REQUIRE_CLOSE(0.375, get<3>(v));

    v = cf[2];
    REQUIRE_CLOSE(435.0, get<0>(v));
    REQUIRE_CLOSE(0.2, get<1>(v));
    REQUIRE_CLOSE(0.40, get<2>(v));
    REQUIRE_CLOSE(0.60, get<3>(v));

    v = cf[1];
    REQUIRE_CLOSE(431.25, get<0>(v));
    REQUIRE_CLOSE(0.125, get<1>(v));
    REQUIRE_CLOSE(0.25, get<2>(v));
    REQUIRE_CLOSE(0.375, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(check_boundaries, ColourFcnTest)
{
    ColourFunction::value_type v;

    v = cf.get(0.0);
    REQUIRE_CLOSE(0.0, get<0>(v));
    REQUIRE_CLOSE(0.0, get<1>(v));
    REQUIRE_CLOSE(0.0, get<2>(v));
    REQUIRE_CLOSE(0.0, get<3>(v));

    v = cf.get(1000.0);
    REQUIRE_CLOSE(0.0, get<0>(v));
    REQUIRE_CLOSE(0.0, get<1>(v));
    REQUIRE_CLOSE(0.0, get<2>(v));
    REQUIRE_CLOSE(0.0, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(single_wave, ColourFcnTest)
{
    vector<tuple<double, double>> input;
    input.push_back(make_tuple(430.0, 1.0));

    auto v = cf.get_colour(input).getXYZ();
    REQUIRE_CLOSE(0.10, get<0>(v));
    REQUIRE_CLOSE(0.20, get<1>(v));
    REQUIRE_CLOSE(0.30, get<2>(v));
}

BOOST_FIXTURE_TEST_CASE(known_waves, ColourFcnTest)
{
    vector<tuple<double, double>> input;
    input.push_back(make_tuple(430.0, 1.0));
    input.push_back(make_tuple(435.0, 1.0));
    input.push_back(make_tuple(440.0, 1.0));

    auto v = cf.get_colour(input).getXYZ();
    REQUIRE_CLOSE(0.70, get<0>(v));
    REQUIRE_CLOSE(0.76, get<1>(v));
    REQUIRE_CLOSE(3.30, get<2>(v));
}

BOOST_FIXTURE_TEST_CASE(waves_mixed, ColourFcnTest)
{
    vector<tuple<double, double>> input;
    input.push_back(make_tuple(430.0, 0.5));
    input.push_back(make_tuple(432.5, 0.2));
    input.push_back(make_tuple(440.0, 0.9));

    auto v = cf.get_colour(input).getXYZ();
    REQUIRE_CLOSE(0.440, get<0>(v));
    REQUIRE_CLOSE(0.304, get<1>(v));
    REQUIRE_CLOSE(2.400, get<2>(v));
}

BOOST_FIXTURE_TEST_CASE(from_XYZ_to_xyz, ColourFcnTest)
{
    Colour c(make_tuple(2.0, 4.0, 10.0));

    auto v = c.getxyz();
    REQUIRE_CLOSE(0.125, get<0>(v));
    REQUIRE_CLOSE(0.250, get<1>(v));
    REQUIRE_CLOSE(0.625, get<2>(v));
}

BOOST_FIXTURE_TEST_CASE(get_xyz, ColourFcnTest)
{
    vector<tuple<double, double>> input;
    input.push_back(make_tuple(430.0, 0.5));
    input.push_back(make_tuple(432.5, 0.2));
    input.push_back(make_tuple(440.0, 0.9));

    auto v = cf.get_colour(input).getxyz();
    REQUIRE_CLOSE(0.1399491, get<0>(v));
    REQUIRE_CLOSE(0.0966921, get<1>(v));
    REQUIRE_CLOSE(0.7633587, get<2>(v));
}

