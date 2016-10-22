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

#include <fstream>
#include "colourfunction.hpp"
#include "filereader.hpp"


#include <iostream>


using namespace std;

namespace colourcheck
{

void colourfunction::read()
{
    ifstream file(_filename);
    fileline line;

    while (file >> line) {
        auto parse = [line](size_t i) {
            return stod(line[i]);
        };
        auto content = make_tuple(parse(0), parse(1), parse(2), parse(3));
        _data.push_back(content);
    }
}

colourfunction::value_type colourfunction::mean(double value,
        colourfunction::value_type const &v1,
        colourfunction::value_type const &v2)
{
    double factor = (value - std::get<0>(v1)) / (std::get<0>(v2) - std::get<0>(v1));

    auto adjust = [factor](double f1, double f2) {
        return f1 * (1 - factor) + f2 * (factor);
    };

    return make_tuple(adjust(std::get<0>(v1), std::get<0>(v2)),
                      adjust(std::get<1>(v1), std::get<1>(v2)),
                      adjust(std::get<2>(v1), std::get<2>(v2)),
                      adjust(std::get<3>(v1), std::get<3>(v2)));
}

colourfunction::value_type const colourfunction::get(double wavelength)
{
    double last_value;
    last_value = std::get<0>(*_data.begin());
    for (auto it = _data.begin(); it != _data.end(); ++it) {

        double value = std::get<0>(*it);
        if (wavelength == value) {
            return *it;
        }
        else if (wavelength > last_value && wavelength < value) {
            auto v = mean(wavelength, *(it - 1), *it);
            _data.insert(it, v);
            return v;
        }

    }
    return make_tuple(0.0, 0.0, 0.0, 0.0);
}

colourfunction::colour_type const colourfunction::get_colourXYZ(
    const vector<tuple<double, double>> &input_vector)
{
    colourfunction::colour_type output = make_tuple(0.0, 0.0, 0.0);

    for (auto input : input_vector) {
        double wavelength = std::get<0>(input);
        double intensity = std::get<1>(input);

        auto v = get(wavelength);

        std::get<0>(output) += std::get<1>(v) * intensity;
        std::get<1>(output) += std::get<2>(v) * intensity;
        std::get<2>(output) += std::get<3>(v) * intensity;
    }

    return output;
}

colourfunction::colour_type const colourfunction::to_xyz(
    const colourfunction::colour_type &colourXYZ)
{
    double sum = std::get<0>(colourXYZ) +
                std::get<1>(colourXYZ) +
                std::get<2>(colourXYZ);

    return make_tuple(std::get<0>(colourXYZ) / sum,
                      std::get<1>(colourXYZ) / sum,
                      std::get<2>(colourXYZ) / sum);
}


colourfunction::colour_type const colourfunction::get_colour_xyz(
    const std::vector<std::tuple<double, double>> &input_vector)
{
    return to_xyz(get_colourXYZ(input_vector));
}



}
