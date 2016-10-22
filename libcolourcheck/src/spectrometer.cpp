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
#include "spectrometer.hpp"
#include "fileline.hpp"

using namespace std;

namespace colourcheck
{

void spectrometer::parse_data()
{
    ifstream wave_file(_wavelength_file);

    fileline line;
    while (wave_file >> line) {
        if (line.size() > 1) {
            _wave_data.push_back(stod(line[1]));
        }
    }

    ifstream ints_file(_intensities_file);

    while (ints_file >> line) {
        vector<double> ints_row;
        for (auto v : line.get_data()) {
            ints_row.push_back(stod(v));
        }
        _ints_data.push_back(ints_row);
    }

    _ready = (_wave_data.size() > 0) && (_ints_data.size() > 0);
    _it_timestamp = _ints_data.begin();
}


}
