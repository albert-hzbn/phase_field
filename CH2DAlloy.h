#include <map>
#include <string>
#include <fftw3.h>

class CH2DAlloy
{
private:
    double coff_a, coff_b, coff_c, coff_d;
    double fluctuation;
    double avg_comp;
    int size_x;
    int size_y;
    double mobility;
    double delta_t;
    double kappa;
    double delta_x;
    double delta_y;
    int time_interval;
    int total_time;
    int resume;
    char *filename;
    std::string output_format;

    fftw_complex *comp;
    fftw_complex *comp_tilda;
    fftw_complex *energy;
    fftw_complex *energy_tilda;
    fftw_plan plan1;
    fftw_plan plan2;
    fftw_plan plan3;

    std::array<int, 3> pfDimension;

    void loadProfile(std::string);

public:
    CH2DAlloy(std::map<std::string, std::string>);
    void performCalculation();
    ~CH2DAlloy();
};
