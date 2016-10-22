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

class colourfunction
{
public:
    typedef std::tuple<double, double, double, double> value_type;
    typedef std::tuple<double, double, double> colour_type; //TODO - create a class indicating the colour space

    colourfunction(std::string filename)
        : _filename(filename) {};

    void read();

    colourfunction::value_type const &operator[](std::size_t index) const
    {
        return _data[index];
    }

    colourfunction::value_type const get(double wavelength);

    colourfunction::colour_type const get_colourXYZ(
        const std::vector<std::tuple<double, double>> &input_vector);

    colourfunction::colour_type const to_xyz(
        const colourfunction::colour_type &colourXYZ);

    colourfunction::colour_type const get_colour_xyz(
        const std::vector<std::tuple<double, double>> &input_vector);

private:

    colourfunction::value_type mean(double value,
                                    colourfunction::value_type const &v1,
                                    colourfunction::value_type const &v2);

    std::string _filename;
    std::vector<colourfunction::value_type> _data;
};

}

#endif // __COLOURFUNCTION_H__

