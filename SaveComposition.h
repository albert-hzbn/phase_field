#include <fftw3.h>

class SaveComposition
{
private:
    /* data */
    void fftwComplexToVti(fftw_complex *data, int dims[3], const char *filename);
    void fftwComplexToPng(fftw_complex *data, int dims[3], const char *filename);

public:
    SaveComposition(/* args */);
    ~SaveComposition();
    void fftwComplexToFile(fftw_complex *data, int dims[3], unsigned int, std::string output_format);
};