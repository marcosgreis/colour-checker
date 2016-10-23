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

#ifndef __COLOURFUNCTION_H__
#define __COLOURFUNCTION_H__

#include <string>
#include <tuple>
#include <vector>

namespace colourcheck
{

class Colour
{
public:
    typedef std::tuple<double, double, double> value_type;

    Colour(const Colour::value_type &v) :
        _XYZ(v)
    {};

    const Colour::value_type &getXYZ() const
    {
        return _XYZ;
    }

    const Colour::value_type getxyz() const
    {
        double sum = std::get<0>(_XYZ) +
                     std::get<1>(_XYZ) +
                     std::get<2>(_XYZ);

        return std::make_tuple(std::get<0>(_XYZ) / sum,
                               std::get<1>(_XYZ) / sum,
                               std::get<2>(_XYZ) / sum);
    }


private:
    Colour::value_type _XYZ;
};

class ColourFunction
{
public:
    typedef std::tuple<double, double, double, double> value_type;

    ColourFunction(std::string filename)
        : _filename(filename) {};

    void read();
    bool is_ready()
    {
        return _data.size() > 0 ? true : false;
    }

    ColourFunction::value_type const &operator[](std::size_t index) const
    {
        return _data[index];
    }

    ColourFunction::value_type const get(double wavelength);

    Colour const get_colour(
        const std::vector<std::tuple<double, double>> &input_vector);

private:

    ColourFunction::value_type mean(double value,
                                    ColourFunction::value_type const &v1,
                                    ColourFunction::value_type const &v2);

    std::string _filename;
    std::vector<ColourFunction::value_type> _data;
};

}

#endif // __COLOURFUNCTION_H__

