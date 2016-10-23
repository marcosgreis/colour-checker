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

#include <iostream>
#include <fstream>
#include <tuple>
#include <boost/program_options.hpp>
#include "fileline.hpp"
#include "spectrometer.hpp"
#include "colourfunction.hpp"

using namespace std;
using namespace colourcheck;
namespace po = boost::program_options;

int calculate_data(colourfunction &cf, spectrometer &s, const string &output);

int main(int argc, char **argv)
{
    try {
        po::options_description generic("Generic");
        generic.add_options()
        ("help,h", "This help message");

        po::options_description config("Configurations");
        config.add_options()
        ("wavelength,w", po::value<string>(), "Wavelengths input file")
        ("intensity,i", po::value<string>(), "Intensities input file")
        ("ciexyz,c", po::value<string>(), "Colour matching functions input file")
        ("output,o", po::value<string>()->default_value("output.csv"), "Output file");

        po::options_description desc("Allowed options");
        desc.add(generic).add(config);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help") || !vm.count("wavelength") || !vm.count("intensity") || !vm.count("ciexyz")) {
            cout << desc << "\n";
            return 0;
        }

//    spectrometer s("/tmp/waves.csv", "/tmp/int.csv");
//    spectrometer s("/tmp/wavelengths.csv", "/tmp/intensities.csv");
        spectrometer s(vm["wavelength"].as<string>(), vm["intensity"].as<string>());
//    colourfunction cf("/tmp/ciexyz31.csv");
        colourfunction cf(vm["ciexyz"].as<string>());
        s.parse_data();
        cf.read();

        if (!s.is_ready()) {
            cout << "Error reading spectrometer files!" << endl;
            return 1;
        }

        if (!cf.is_ready()) {
            cout << "Error reading colour matching function file!" << endl;
            return 1;
        }

        if (calculate_data(cf, s, vm["output"].as<string>()) != 0) {
            return 1;
        }
    }
    catch (exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "Unknown internal error! =(" << endl;
    }

    return 0;
}

class csvoutput
{
public:
    csvoutput(std::string filename) :
        _output(filename),
        _comma(false) {}

    bool is_open() const
    {
        return _output.is_open();
    }

    void create_header(std::string header)
    {
        _output << "#" << header << endl;
    };

    void new_line()
    {
        _comma = false;
        _output << endl;
    }

    template <typename T>
    void new_cell(T cell)
    {
        if (_comma) {
            _output << ",";
        }
        _comma = true;
        _output << cell;
    }

private:
    ofstream _output;
    bool _comma;
};

int calculate_data(colourfunction &cf, spectrometer &s, const string &output)
{
    csvoutput out(output);
    if (!out.is_open()) {
        cout << "Could not open output file!" << endl;
        return 1;
    }

    out.create_header("Timestamp,x,y");

    int temp_aux = 1;
    for (auto it = s.begin(); it != s.end(); ++it) {
        vector<tuple<double, double>> data;
        out.new_cell(temp_aux++);

        size_t index = 0;
        for (auto intensity : *it) {
            data.push_back(make_tuple(s.get_wavelength(index), intensity));
            index++;
        }

        auto v = cf.get_colour_xyz(data);

        out.new_cell(get<0>(v));
        out.new_cell(get<1>(v));
        out.new_line();
    }

    out.create_header("Final_x,Final_y");
    out.new_cell(0);
    out.new_cell(0);
    out.new_line();

    return 0;
}


