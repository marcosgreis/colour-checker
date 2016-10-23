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

#ifndef __FILELINE_H__
#define __FILELINE_H__

#include <string>
#include <vector>
#include <sstream>

namespace colourcheck
{
class Fileline
{
public:
    typedef std::vector<std::string> value_type;

    std::string const &operator[](std::size_t index) const
    {
        return _data[index];
    }

    value_type const &get_data()
    {
        return _data;
    }

    Fileline::value_type::size_type size() const
    {
        return _data.size();
    }

    void read_next_line(std::istream &str)
    {
        std::string line;
        std::getline(str, line);

        std::stringstream lineStream(line);
        std::string value;
        _data.clear();

        std::string::size_type hash_pos = line.find_first_of("#");
        std::string::size_type notspace_pos = line.find_first_not_of(" ");
        if (hash_pos != std::string::npos &&
                hash_pos == notspace_pos) {
            return;
        }

        while (std::getline(lineStream, value, ',')) {

            _data.emplace_back(value);
        }
    }
private:
    Fileline::value_type _data;
};
std::istream &operator>>(std::istream &instr, Fileline &obj);

}


#endif // __FILELINE_H__

