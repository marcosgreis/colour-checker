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
#include "fileline.hpp"


using namespace std;

namespace colourcheck
{

void ColourFunction::read()
{
    ifstream file(_filename);
    Fileline line;

    while (file >> line) {
        if (line.size() > 3) {
            auto parse = [line](size_t i) {
                return stod(line[i]);
            };
            auto content = make_tuple(parse(0), parse(1), parse(2), parse(3));
            _data.push_back(content);
        }
    }
}

ColourFunction::value_type ColourFunction::mean(double value,
        ColourFunction::value_type const &v1,
        ColourFunction::value_type const &v2)
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

ColourFunction::value_type const ColourFunction::get(double wavelength)
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

Colour const ColourFunction::get_colour(
    const vector<tuple<double, double>> &input_vector)
{
    Colour::value_type output = make_tuple(0.0, 0.0, 0.0);

    for (auto input : input_vector) {
        double wavelength = std::get<0>(input);
        double intensity = std::get<1>(input);

        auto v = get(wavelength);

        std::get<0>(output) += std::get<1>(v) * intensity;
        std::get<1>(output) += std::get<2>(v) * intensity;
        std::get<2>(output) += std::get<3>(v) * intensity;
    }

    return Colour(output);
}

}
