#include <fftw3.h>
#include <string.h>
#include <utility>
#include <array>

class LoadVTK
{
private:
    /* data */
public:
    LoadVTK(/* args */);
    std::pair<fftw_complex *, std::array<int, 3>> vtkToFFTArray(std::string);
    ~LoadVTK();
};

