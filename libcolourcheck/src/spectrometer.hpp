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

#ifndef __SPECTROMETER_H__
#define __SPECTROMETER_H__

#include <string>
#include <vector>
#include <sstream>

namespace colourcheck
{
class spectrometer
{
public:
    typedef std::vector<double> sample_vector;
    typedef std::vector<spectrometer::sample_vector> matrix_type;


    spectrometer(std::string wavelenght_file, std::string intensities_file)
        : _wavelength_file(wavelenght_file),
          _intensities_file(intensities_file),
          _ready(false),
          _it_timestamp(nullptr) {};

    void parse_data();
    bool is_ready()
    {
        return _ready;
    };

    spectrometer::matrix_type::iterator begin() noexcept
    {
        return _ints_data.begin();
    }

    spectrometer::matrix_type::iterator end() noexcept
    {
        return _ints_data.end();
    }

    double get_wavelength(std::size_t index)
    {
        return _wave_data[index];
    }

private:
    std::string _wavelength_file;
    std::string _intensities_file;
    bool _ready;
    spectrometer::sample_vector _wave_data;
    spectrometer::matrix_type _ints_data;
    spectrometer::matrix_type::iterator _it_timestamp;
};
}


#endif // __SPECTROMETER_H__

