#include <fftw3.h>
#include <array>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "CH2D.h"
#include "LoadVTK.h"
#include "SaveComposition.h"

void CH2D::loadProfile(std::string filename)
{
    LoadVTK loader;
    std::pair<fftw_complex *, std::array<int, 3>> data = loader.vtkToFFTArray("output.vti");

    // Access the FFTW array and pfDimension using the first and second properties
    comp = data.first;
    pfDimension = data.second;
    if (pfDimension[0] != size_x || pfDimension[1] != size_y || pfDimension[2] != 1)
    {
        std::cerr << "Input file dimensions are not matching, exiting program\n";
        exit(0);
    }
}

CH2D::CH2D(std::map<std::string, std::string> input)
{
    // Define an array of strings to use as a filter
    std::string filter[] = {"FLUCTUATION",
                            "AVG_COMP",
                            "SIZE_X",
                            "SIZE_Y",
                            "MOBILITY",
                            "DELTA_T",
                            "KAPPA",
                            "DELTA_X",
                            "DELTA_Y",
                            "OUT_TIME_INTERVAL",
                            "TOTAL_TIME",
                            "RESUME",
                            "FILE",
                            "OUTPUT_FORMAT"};

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

    fluctuation = std::stod(input["FLUCTUATION"]);
    avg_comp = std::stod(input["AVG_COMP"]);
    size_x = std::stoi(input["SIZE_X"]);
    size_y = std::stoi(input["SIZE_Y"]);
    mobility = std::stod(input["MOBILITY"]);
    delta_t = std::stod(input["DELTA_T"]);
    kappa = std::stod(input["KAPPA"]);
    delta_x = std::stod(input["DELTA_X"]);
    delta_y = std::stod(input["DELTA_Y"]);
    time_interval = std::stoi(input["OUT_TIME_INTERVAL"]);
    total_time = std::stoi(input["TOTAL_TIME"]);
    resume = std::stoi(input["RESUME"]);
    output_format = input["OUTPUT_FORMAT"];

    char charArray[input["FILE"].length() + 1]; // create a char array with the same size as the string
    strcpy(charArray, input["FILE"].c_str());   // copy the string into the char arr

    // Print input values
    std::cout << "Variables considered for calculation\n\n";
    for (const auto &kv : input)
    {
        std::cout << kv.first << " = " << kv.second << std::endl;
    }
    std::cout << "\n\n";

    comp = fftw_alloc_complex(size_x * size_y);
    comp_tilda = fftw_alloc_complex(size_x * size_y);
    energy = fftw_alloc_complex(size_x * size_y);
    energy_tilda = fftw_alloc_complex(size_x * size_y);

    plan1 = fftw_plan_dft_2d(size_x, size_y, comp, comp_tilda, FFTW_FORWARD, FFTW_ESTIMATE);
    plan2 = fftw_plan_dft_2d(size_x, size_y, energy, energy_tilda, FFTW_FORWARD, FFTW_ESTIMATE);
    plan3 = fftw_plan_dft_2d(size_x, size_y, comp_tilda, comp, FFTW_BACKWARD, FFTW_ESTIMATE);

    if (resume == 0)
    {
        for (unsigned int x = 0; x < size_x; ++x)
        {
            for (unsigned int y = 0; y < size_y; ++y)
            {
                comp[y + size_y * x][0] = avg_comp + (-1 + 2 * ((double)rand() / (double)RAND_MAX)) * (fluctuation * 10);
                comp[y + size_y * x][1] = 0;
            }
        }
    }
    else if (resume == 1)
    {
        loadProfile(filename);
    }
    else
    {
        std::cout << "RESUME command takes input values either 0 or 1\n\n";
    }
}

void CH2D::performCalculation()
{
    unsigned int total_time_steps = (int)(total_time / delta_t);
    unsigned int time;
    double temp1, temp2;
    const int half_size_x = size_x / 2;
    const int half_size_y = size_y / 2;

    SaveComposition save_comp;
    int dims[3] = {size_x, size_y, 1};

    for (int iter = 0; iter <= total_time_steps; iter++)
    {
        // Saving data to files
        time = iter * delta_t;
        if (std::fmod(time, time_interval) == 0)
        {
            save_comp.fftwComplexToFile(comp, dims, time, output_format);
        }

        /* Defining free energy */

        for (int x = 0; x < size_x; ++x)
        {
            for (int y = 0; y < size_y; ++y)
            {

                energy[y + size_y * x][0] = -2 * (comp[y + size_y * x][0]) * (1 + 2 * (comp[y + size_y * x][0]) * (comp[y + size_y * x][0]) - 3 * (comp[y + size_y * x][0]));
                energy[y + size_y * x][1] = -2 * (comp[y + size_y * x][1]) * (1 + 2 * (comp[y + size_y * x][1]) * (comp[y + size_y * x][1]) - 3 * (comp[y + size_y * x][1]));
            }
        }

        /* FFT of composition and  free energy */
        fftw_execute(plan1);
        fftw_execute(plan2);

        for (int i = 0; i < size_x; ++i)
        {
            if (i < half_size_x)
            {
                temp1 = i * delta_x;
            }

            else
            {
                temp1 = (i - size_x) * delta_x;
            }

            for (int j = 0; j < size_y; ++j)
            {
                if (j < half_size_y)
                {
                    temp2 = j * delta_y;
                }

                else
                {
                    temp2 = (j - size_y) * delta_y;
                }

                comp_tilda[j + size_y * i][0] = 1.0 * (comp_tilda[j + size_y * i][0] + delta_t * (pow(temp1, 2) + pow(temp2, 2)) * energy_tilda[j + size_y * i][0]) / (1.0 + delta_t * pow((pow(temp1, 2) + pow(temp2, 2)), 2));
                comp_tilda[j + size_y * i][1] = 1.0 * (comp_tilda[j + size_y * i][1] + delta_t * (pow(temp1, 2) + pow(temp2, 2)) * energy_tilda[j + size_y * i][1]) / (1.0 + delta_t * pow((pow(temp1, 2) + pow(temp2, 2)), 2));
            }
        }

        // IFFT of final composition
        fftw_execute(plan3);

        // Normalization of composition
        for (int x = 0; x < size_x; ++x)
        {
            for (int y = 0; y < size_y; ++y)
            {
                comp[y + size_y * x][0] = 1.0 * comp[y + size_y * x][0] / (size_x * size_y);
                comp[y + size_y * x][1] = 1.0 * comp[y + size_y * x][1] / (size_x * size_y);
            }
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

CH2D::~CH2D()
{
}
