#include <fftw3.h>
#include <stdio.h>
#include <complex>
#include <fstream>
#include <math.h>
#include <map>
#include <iostream>
#include <algorithm>
#include "CH1D.h"
#include "LoadInput.h"

CH1D::CH1D(std::map<std::string, std::string> input)
{

    // Define an array of strings to use as a filter
    std::string filter[] = {"SIZE_X",
                            "DELTA_T",
                            "KAPPA",
                            "DELTA_X",
                            "DIFF_TILDA",
                            "OUT_TIME_INTERVAL",
                            "TOTAL_TIME"};

    // Filter the input collection by keeping only the key-value pairs
    // whose keys match any of the strings in the filter array
    for (auto it = input.begin(); it != input.end();)
    {
        if (std::find(std::begin(filter), std::end(filter), it->first) == std::end(filter))
        {
            it = input.erase(it);
        }
        else
        {
            ++it;
        }
    }

    size_x = std::stoi(input["SIZE_X"]);
    delta_t = std::stod(input["DELTA_T"]);
    kappa = std::stod(input["KAPPA"]);
    delta_x = std::stod(input["DELTA_X"]);
    diff_tilda = std::stod(input["DIFF_TILDA"]);
    time_interval = std::stoi(input["OUT_TIME_INTERVAL"]);
    total_time = std::stoi(input["TOTAL_TIME"]);

    // Print input values
    std::cout << "Variables considered for calculation\n\n";
    for (const auto &kv : input)
    {
        std::cout << kv.first << " = " << kv.second << std::endl;
    }

    comp = fftw_alloc_complex(size_x);
    comp_tilda = fftw_alloc_complex(size_x);
    energy = fftw_alloc_complex(size_x);
    energy_tilda = fftw_alloc_complex(size_x);

    plan1 = fftw_plan_dft_1d(size_x, comp, comp_tilda, FFTW_FORWARD, FFTW_ESTIMATE);
    plan2 = fftw_plan_dft_1d(size_x, energy, energy_tilda, FFTW_FORWARD, FFTW_ESTIMATE);
    plan3 = fftw_plan_dft_1d(size_x, comp_tilda, comp, FFTW_BACKWARD, FFTW_ESTIMATE);

    if (resume == 0)
    {
        loadInitialComposition();
    }
}

void CH1D::loadInitialComposition()
{
    for (int i = 0; i < size_x; i++)
    {
        if ((i >= 0) && (i < size_x / 2))
        {
            comp[i][0] = 1.0; // Assign the real part
            comp[i][1] = 0.0; // Assign the imaginary part
        }
        else
        {
            comp[i][0] = -1.0;
            comp[i][1] = 0.0;
        }
    }
}

void CH1D::performCalculation()
{
    delta_k = 2.0 * PI / (size_x * delta_x);
    half_size_x = size_x / 2;

    total_time_steps = total_time / delta_t;
    unsigned int iter;
    int iter_g;
    for (iter = 1; iter <= total_time_steps; iter++)
    {
        // Defining free energy derivative
        for (iter_g = 0; iter_g < size_x; iter_g++)
        {
            energy[iter_g][0] = -comp[iter_g][0] * (comp[iter_g][0] - 1) * (1 + comp[iter_g][0]);
            energy[iter_g][1] = -comp[iter_g][1] * (comp[iter_g][1] - 1) * (1 + comp[iter_g][1]);
        }
        fftw_execute(plan2);
        fftw_execute(plan1);

        int iter_c;
        double temp;

        for (iter_c = 0; iter_c < size_x; iter_c++)
        {
            if (iter_c < half_size_x)
            {
                temp = iter_c * delta_k;
            }
            else
            {
                temp = (iter_c - size_x) * delta_k;
            }

            comp_tilda[iter_c][0] = 1.0 * (comp_tilda[iter_c][0] + diff_tilda * delta_t * pow(temp, 2) * energy_tilda[iter_c][0]) / (1.0 + kappa * delta_t * pow(temp, 4));
            comp_tilda[iter_c][1] = 1.0 * (comp_tilda[iter_c][1] + diff_tilda * delta_t * pow(temp, 2) * energy_tilda[iter_c][1]) / (1.0 + kappa * delta_t * pow(temp, 4));
        }

        // inverse fourier transform of composition
        fftw_execute(plan3);

        // normalize composition
        for (iter_c = 0; iter_c < size_x; iter_c++)
        {
            comp[iter_c][0] = 1.0 * comp[iter_c][0] / (size_x);
            comp[iter_c][1] = 1.0 * comp[iter_c][1] / (size_x);
        }

        // Saving data to files
        time = iter * delta_t;
        if (std::fmod(time, time_interval) == 0)
        {
            SaveToFile(time);
        }
    }

    fftw_free(comp);
    fftw_free(comp_tilda);
    fftw_free(energy);
    fftw_free(energy_tilda);

    fftw_destroy_plan(plan1);
    fftw_destroy_plan(plan2);
    fftw_destroy_plan(plan3);

    fftw_cleanup();
}

void CH1D::SaveToFile(int time)
{
    char file_name[25];
    sprintf(file_name, "output_%d.dat", time);
    std::ofstream output_file(file_name);

    unsigned int iter_c;
    for (iter_c = 0; iter_c < size_x; iter_c++)
    {
        output_file << iter_c << " " << comp[iter_c][0] << std::endl;
    }
    output_file.close();
}

CH1D::~CH1D()
{
}