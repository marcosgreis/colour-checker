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

#define BOOST_TEST_MODULE ColourFunctionTest
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "colourfunction.hpp"

using namespace std;
using namespace colourcheck;

#define REQUIRE_CLOSE(v1,v2) BOOST_REQUIRE_CLOSE(v1,v2,0.00001)

struct F {
    F() : cf("sample.csv")
    {
        createSampleFile();
    }

    void createSampleFile()
    {
        ofstream file("sample.csv");
        file << "430,0.1,0.20,0.30" << endl
             << "435,0.2,0.40,0.60" << endl
             << "440,0.4,0.16,2.40" << endl;

        cf.read();
    }

    ~F()
    {
        remove("sample.csv");
    }
    colourfunction cf;
};

BOOST_FIXTURE_TEST_CASE(read_file, F)
{
    colourfunction::value_type v;

    v = cf[0];
    REQUIRE_CLOSE(430.0f, get<0>(v));
    REQUIRE_CLOSE(0.1f, get<1>(v));
    REQUIRE_CLOSE(0.20f, get<2>(v));
    REQUIRE_CLOSE(0.30f, get<3>(v));

    v = cf[1];
    REQUIRE_CLOSE(435.0f, get<0>(v));
    REQUIRE_CLOSE(0.2f, get<1>(v));
    REQUIRE_CLOSE(0.40f, get<2>(v));
    REQUIRE_CLOSE(0.60f, get<3>(v));

    v = cf[2];
    REQUIRE_CLOSE(440.0f, get<0>(v));
    REQUIRE_CLOSE(0.40f, get<1>(v));
    REQUIRE_CLOSE(0.16f, get<2>(v));
    REQUIRE_CLOSE(2.40f, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(get_exact_wavelength, F)
{
    colourfunction::value_type v;

    v = cf.get(430.0f);
    REQUIRE_CLOSE(430.0f, get<0>(v));
    REQUIRE_CLOSE(0.1f, get<1>(v));
    REQUIRE_CLOSE(0.20f, get<2>(v));
    REQUIRE_CLOSE(0.30f, get<3>(v));

    v = cf.get(435.0f);
    REQUIRE_CLOSE(435.0f, get<0>(v));
    REQUIRE_CLOSE(0.2f, get<1>(v));
    REQUIRE_CLOSE(0.40f, get<2>(v));
    REQUIRE_CLOSE(0.60f, get<3>(v));

    v = cf.get(440.0f);
    REQUIRE_CLOSE(440.0f, get<0>(v));
    REQUIRE_CLOSE(0.40f, get<1>(v));
    REQUIRE_CLOSE(0.16f, get<2>(v));
    REQUIRE_CLOSE(2.40f, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(build_new_wavelength_middle, F)
{
    colourfunction::value_type v;

    v = cf.get(432.5f);
    REQUIRE_CLOSE(432.5f, get<0>(v));
    REQUIRE_CLOSE(0.15f, get<1>(v));
    REQUIRE_CLOSE(0.30f, get<2>(v));
    REQUIRE_CLOSE(0.45f, get<3>(v));

    v = cf[2];
    REQUIRE_CLOSE(435.0f, get<0>(v));
    REQUIRE_CLOSE(0.2f, get<1>(v));
    REQUIRE_CLOSE(0.40f, get<2>(v));
    REQUIRE_CLOSE(0.60f, get<3>(v));

    v = cf[1];
    REQUIRE_CLOSE(432.5f, get<0>(v));
    REQUIRE_CLOSE(0.15f, get<1>(v));
    REQUIRE_CLOSE(0.30f, get<2>(v));
    REQUIRE_CLOSE(0.45f, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(build_new_wavelength_any, F)
{
    colourfunction::value_type v;

    v = cf.get(431.25f);
    REQUIRE_CLOSE(431.25f, get<0>(v));
    REQUIRE_CLOSE(0.125f, get<1>(v));
    REQUIRE_CLOSE(0.25f, get<2>(v));
    REQUIRE_CLOSE(0.375f, get<3>(v));

    v = cf[2];
    REQUIRE_CLOSE(435.0f, get<0>(v));
    REQUIRE_CLOSE(0.2f, get<1>(v));
    REQUIRE_CLOSE(0.40f, get<2>(v));
    REQUIRE_CLOSE(0.60f, get<3>(v));

    v = cf[1];
    REQUIRE_CLOSE(431.25f, get<0>(v));
    REQUIRE_CLOSE(0.125f, get<1>(v));
    REQUIRE_CLOSE(0.25f, get<2>(v));
    REQUIRE_CLOSE(0.375f, get<3>(v));
}

BOOST_FIXTURE_TEST_CASE(check_boundaries, F)
{ 
   colourfunction::value_type v;

    v = cf.get(0.0f);
    REQUIRE_CLOSE(0.0f, get<0>(v));
    REQUIRE_CLOSE(0.0f, get<1>(v));
    REQUIRE_CLOSE(0.0f, get<2>(v));
    REQUIRE_CLOSE(0.0f, get<3>(v));

    v = cf.get(1000.0f);
    REQUIRE_CLOSE(0.0f, get<0>(v));
    REQUIRE_CLOSE(0.0f, get<1>(v));
    REQUIRE_CLOSE(0.0f, get<2>(v));
    REQUIRE_CLOSE(0.0f, get<3>(v));
}
