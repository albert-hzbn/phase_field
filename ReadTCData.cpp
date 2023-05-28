#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include "ReadTCData.h"


std::vector<double> ReadTCData::findInflectionPoints(const Polynomial &poly, double start, double end, double step)
{
    std::vector<double> inflectionPoints;
    double prevSlope = poly.derivative(start);

    for (double x = start + step; x < end; x += step)
    {
        double currentSlope = poly.derivative(x);

        if (prevSlope * currentSlope < 0)
        {
            inflectionPoints.push_back(x);
        }

        prevSlope = currentSlope;
    }

    return inflectionPoints;
}

std::vector<double> ReadTCData::findBinodalPoints(const Polynomial &poly, double start, double end, double step)
{
    std::vector<double> binodalPoints;
    double prevSlope = poly.derivative(start);

    for (double x = start + step; x < end; x += step)
    {
        double currentSlope = poly.derivative(x);

        if (prevSlope * currentSlope <= 0)
        {
            binodalPoints.push_back(x);
        }

        prevSlope = currentSlope;
    }

    return binodalPoints;
}

std::vector<Polynomial> ReadTCData::readPolynomialsFromCSV(const std::string &filename)
{
    std::vector<Polynomial> polynomials;
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return polynomials;
    }

    std::string line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Polynomial poly;

        if (!(iss >> poly.e >> poly.a >> poly.b >> poly.c >> poly.d))
        {
            std::cerr << "Failed to read polynomial data from line: " << line << std::endl;
            continue;
        }

        polynomials.push_back(poly);
    }

    file.close();
    return polynomials;
}



// int main()
// {
//     std::vector<Polynomial> polynomials = readPolynomialsFromCSV("temperature_data.csv");

//     for (const Polynomial &poly : polynomials)
//     {
//         std::cout << "Polynomial: a*x^4 + b*x^3 + c*x^2 + d*x + e" << std::endl;
//         std::cout << "a = " << poly.a << ", b = " << poly.b << ", c = " << poly.c << ", d = " << poly.d << ", e = " << poly.e << std::endl;

//         std::vector<double> inflectionPoints = findInflectionPoints(poly, 1100, 1420, 20);
//         std::vector<double> binodalPoints = findBinodalPoints(poly, 1100, 1420, 20);

//         std::cout << "Inflection Points: ";
//         for (double point : inflectionPoints)
//         {
//             std::cout << point << " ";
//         }
//         std::cout << std::endl;

//         std::cout << "Binodal Points: ";
//         for (double point : binodalPoints)
//         {
//             std::cout << point << " ";
//         }
//         std::cout << std::endl;

//         std::cout << std::endl;
//     }

//     return 0;
// }

