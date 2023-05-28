#include <fftw3.h>
#include <map>

#define PI 3.14159265

class CH1D
{
private:
    int size_x;
    int half_size_x;
    int total_time_steps;
    double delta_t;
    double delta_x;
    double kappa;
    double delta_k;
    double diff_tilda;
    int time;
    int time_interval;
    int total_time;
    int resume;
    int resume_from;

    fftw_complex *comp;
    fftw_complex *comp_tilda;
    fftw_complex *energy;
    fftw_complex *energy_tilda;
    fftw_plan plan1;
    fftw_plan plan2;
    fftw_plan plan3;

    void SaveToFile(int);
    void loadInitialComposition();

public:
    CH1D(std::map<std::string, std::string>);
    void performCalculation();
    ~CH1D();
};

