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

#define BOOST_TEST_MODULE AverageFilterTest
#include <boost/test/unit_test.hpp>
#include <fstream>
#include "filter.hpp"

using namespace std;
using namespace colourcheck;

#define REQUIRE_CLOSE(v1,v2) BOOST_REQUIRE_CLOSE(v1,v2,0.0001)

BOOST_AUTO_TEST_CASE(one_sample)
{
    AverageFilter<double> f;
    f.add_measure(10.0);

    REQUIRE_CLOSE(10.0, f.get_value());
}

BOOST_AUTO_TEST_CASE(two_samples)
{
    AverageFilter<double> f;
    f.add_measure(10.0);
    f.add_measure(20.2);

    REQUIRE_CLOSE(15.1, f.get_value());
}

BOOST_AUTO_TEST_CASE(many_samples)
{
    AverageFilter<double> f;
    f << 10.0;
    f << 20.0;
    f << 30.9;

    REQUIRE_CLOSE(20.3, f.get_value());
}

BOOST_AUTO_TEST_CASE(many_samples_base)
{
    AverageFilter<double> avg;
    Filter<double> *f = &avg;
    *f << 7.0;
    *f << 20.0;
    *f << 30.9;

    REQUIRE_CLOSE(19.3, f->get_value());
}

BOOST_AUTO_TEST_CASE(standard_deviation)
{
    AverageFilter<double> f;
    f << 2;
    f << 4;
    f << 4;
    f << 4;
    f << 5;
    f << 5;
    f << 7;
    f << 9;

    REQUIRE_CLOSE(5.0, f.get_value());
    REQUIRE_CLOSE(2.0, f.get_standard_deviation());
}


