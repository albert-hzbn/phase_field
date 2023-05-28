#include <iostream>
#include <string>
#include "LoadInput.h"
#include "CH1D.h"
#include "CH2D.h"

int main(int argc, char **argv)
{
    if (!argv[1])
    {
        std::cerr << "File name should not be empty\n";
        return 1;
    }
    std::string file_name = argv[1];
    LoadInput *load_input = new LoadInput();
    std::map<std::string, std::string> input = load_input->ReadFromFile(file_name);

    if (input["CAL_TYPE"] == "0")
    {
        std::cout << "Cahn Hillard 1D selected\n\n";
        CH1D ch1d(input);
        ch1d.performCalculation();
    }

    else if (input["CAL_TYPE"] == "1")
    {
        std::cout << "Cahn Hillard 2D selected\n\n";
        CH2D ch2d(input);
        ch2d.performCalculation();
    }
    else if(input["CAL_TYPE"] == "2")
    {
        
    }

    return 0;
}