#include <string>
#include <fstream>
#include <iostream>
#include "LoadInput.h"

LoadInput::LoadInput()
{
    input = {
        {"CAL_TYPE", "0"},
        {"FLUCTUATION", "0.0001"},
        {"AVG_COMP", "0.4"},
        {"SIZE_X", "100"},
        {"SIZE_Y", "100"},
        {"DELTA_X", "0.1"},
        {"DELTA_Y", "0.1"},
        {"MOBILITY", "1"},
        {"DELTA_T", "1"},
        {"KAPPA", "1"},
        {"DIFF_TILDA", "1"},
        {"OUT_TIME_INTERVAL", "10"},
        {"TOTAL_TIME", "200"},
        {"RESUME", "0"},
        {"RESUME_FROM", "0"},
        {"OUTPUT_FORMAT", "VTI"}};
}

std::map<std::string, std::string> LoadInput::ReadFromFile(std::string file_name)
{
    std::ifstream file_stream;
    std::string mystring;
    file_stream.open(file_name);
    if (file_stream.is_open())
    {
        std::string line;
        while (std::getline(file_stream, line))
        {
            // Find the position of the equals sign
            std::size_t pos = line.find("=");

            // Extract the left-hand side and right-hand side substrings
            std::string lhs = line.substr(0, pos);
            std::string rhs = line.substr(pos + 1);

            // Trim whitespace from the substrings
            lhs.erase(lhs.find_last_not_of(" \t") + 1);
            rhs.erase(0, rhs.find_first_not_of(" \t"));

            input[lhs] = rhs;
        }
    }
    return input;
}

LoadInput::~LoadInput()
{
}