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
    typedef std::tuple<float, float, float, float> value_type;

    colourfunction(std::string filename)
        : _filename(filename) {};

    void read();

    colourfunction::value_type const &operator[](std::size_t index) const
    {
        return _data[index];
    }
    colourfunction::value_type const get(float wavelength);

private:

    colourfunction::value_type mean(float value,
            colourfunction::value_type const &v1,
            colourfunction::value_type const &v2);

    std::string _filename;
    std::vector<colourfunction::value_type> _data;
};

}

#endif // __COLOURFUNCTION_H__

