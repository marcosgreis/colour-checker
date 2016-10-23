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

#ifndef __FILTER_H__
#define __FILTER_H__

#include <algorithm>
#include <vector>
#include <cmath>

namespace colourcheck
{

template <typename T>
class Filter
{
public:
    Filter() {};
    void add_measure(T v)
    {
        _data.emplace_back(v);
    }

    typename std::vector<T>::iterator begin() noexcept
    {
        return _data.begin();
    }
    typename std::vector<T>::iterator end() noexcept
    {
        return _data.end();
    }
    typename std::vector<T>::size_type size() const noexcept
    {
        return _data.size();
    }

    virtual T get_value() = 0;

    Filter<T> &operator<<(T v)
    {
        add_measure(v);
        return *this;
    }

private:
    std::vector<T> _data;

};

template <typename T>
class AverageFilter : public Filter<T>
{
public:
    AverageFilter() {};

    virtual T get_value()
    {
        T sum = 0;
        std::for_each(this->begin(), this->end(), [&sum](T n) {
            sum += n;
        });
        return sum / ((T) this->size());
    }

    T get_standard_deviation()
    {
        T avg = get_value();
        T sum = 0;
        std::for_each(this->begin(), this->end(), [&sum, avg](T n) {
            sum += (n - avg) * (n - avg);
        });
        return std::sqrt(sum / ((T) this->size()));
    }
};

}
#endif // __FILTER_H__

