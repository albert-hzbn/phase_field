#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <utility>

#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPoints.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataReader.h>

#include <fftw3.h>
#include "LoadVTK.h"

LoadVTK::LoadVTK(/* args */)
{
}

LoadVTK::~LoadVTK()
{
}


std::pair<fftw_complex *, std::array<int, 3>> LoadVTK::vtkToFFTArray(std::string inputFilename)
{
    // Read the VTK file
    vtkSmartPointer<vtkXMLImageDataReader> reader = vtkSmartPointer<vtkXMLImageDataReader>::New();
    reader->SetFileName(inputFilename.c_str());
    reader->Update();

    vtkImageData *imageData = reader->GetOutput();

    // Get the dimensions and data from the VTK file
    int dimensions[3];
    imageData->GetDimensions(dimensions);
    double *data = static_cast<double *>(imageData->GetScalarPointer());

    // Create the FFTW array
    fftw_complex *in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * dimensions[0] * dimensions[1] * dimensions[2]);

    // Copy the VTK data to the input FFTW array
    for (int i = 0; i < dimensions[0] * dimensions[1] * dimensions[2]; ++i)
    {
        in[i][0] = data[i]; // real part
        in[i][1] = 0.0;     // imaginary part
    }

    // Create a std::array for dimensions
    std::array<int, 3> dim_array = {dimensions[0], dimensions[1], dimensions[2]};

    // Return the FFTW array and dimensions as a std::pair
    return std::make_pair(in, dim_array);
}
