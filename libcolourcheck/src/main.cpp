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
#include "filter.hpp"

using namespace std;
using namespace colourcheck;
namespace po = boost::program_options;

int calculate_data(ColourFunction &cf, Spectrometer &s, const string &output, int complete);

int main(int argc, char **argv)
{
    try {
        po::options_description generic("Generic");
        generic.add_options()
        ("help,h", "This help message")
        ("license,l", "License and copyright");

        po::options_description config("Configurations");
        config.add_options()
        ("wavelength,w", po::value<string>(), "Wavelengths input file")
        ("intensity,i", po::value<string>(), "Intensities input file")
        ("ciexyz,c", po::value<string>(), "Colour matching functions input file")
        ("output,o", po::value<string>()->default_value("output.csv"), "Output file")
        ("complete,t", po::value<int>()->default_value(0), "Output each timestamp in file");

        po::options_description desc("Allowed options");
        desc.add(generic).add(config);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("version")) {
            cout <<
                 "Copyright (C) 2016  Marcos Goulart Reis. All rights reserved." << endl << endl <<
                 "This program is free software: you can redistribute it and/or modify" << endl <<
                 "it under the terms of the GNU General Public License as published by" << endl <<
                 "the Free Software Foundation, either version 3 of the License, or" << endl <<
                 "(at your option) any later version." << endl << endl <<
                 "This program is distributed in the hope that it will be useful," << endl <<
                 "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl <<
                 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl <<
                 "GNU General Public License for more details." << endl << endl <<
                 "You should have received a copy of the GNU General Public License" << endl <<
                 "along with this program.  If not, see <http://www.gnu.org/licenses/>." << endl;

            return 0;
        }

        if (vm.count("help") || !vm.count("wavelength") || !vm.count("intensity") || !vm.count("ciexyz")) {
            cout << "This program was designed to solve the C++ Challenge from Thalmic Labs (VanHackathon)." << endl;
            cout << desc << endl;
            return 0;
        }

        cout << "Wavelength file: " << vm["wavelength"].as<string>() << endl;
        cout << "Intensity file: " << vm["intensity"].as<string>() << endl;
        cout << "Colour matching functions file: " << vm["ciexyz"].as<string>() << endl;
        cout << "Output file: " << vm["output"].as<string>() << endl;
        cout << "Complete output: " << vm["complete"].as<int>() << endl;

        Spectrometer s(vm["wavelength"].as<string>(), vm["intensity"].as<string>());
        ColourFunction cf(vm["ciexyz"].as<string>());
        s.parse_data();
        cf.read();

        if (!s.is_ready()) {
            cout << "Error reading Spectrometer files!" << endl;
            return 1;
        }

        if (!cf.is_ready()) {
            cout << "Error reading colour matching function file!" << endl;
            return 1;
        }

        if (calculate_data(cf, s, vm["output"].as<string>(), vm["complete"].as<int>()) != 0) {
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

int calculate_data(ColourFunction &cf, Spectrometer &s, const string &output, int complete)
{
    AverageFilter<double> x_filter;
    AverageFilter<double> y_filter;
    AverageFilter<double> uv_filter;
    csvoutput out(output);
    if (!out.is_open()) {
        cout << "Could not open output file!" << endl;
        return 1;
    }

    if (complete) {
        out.create_header("Timestamp,x,y");
    }

    int timestamp_proc = 0;
    for (auto it = s.begin(); it != s.end(); ++it) {
        vector<tuple<double, double>> data;
        cout << "Processing timestamp #" << ++timestamp_proc << endl;

        size_t index = 0;
        for (auto intensity : *it) {
            data.push_back(make_tuple(s.get_wavelength(index), intensity));
            index++;
        }

        Colour colour = cf.get_colour(data);
        auto xyz = colour.getxyz();

        x_filter << get<0>(xyz);
        y_filter << get<1>(xyz);

        if (complete) {
            out.new_cell(timestamp_proc);
            out.new_cell(get<0>(xyz));
            out.new_cell(get<1>(xyz));
            out.new_line();
        }

        auto uv = colour.getuv();
        uv_filter << get<0>(uv) * get<1>(uv);
    }

    out.create_header("Average x,Average y,SDCM");
    out.new_cell(x_filter.get_value());
    out.new_cell(y_filter.get_value());
    out.new_cell(uv_filter.get_standard_deviation());

    out.new_line();

    return 0;
}


