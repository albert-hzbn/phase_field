#include <vtkFloatArray.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>
#include <vtkStructuredGridWriter.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkPNGWriter.h>
#include <fftw3.h>
#include <string>

#include "SaveComposition.h"

SaveComposition::SaveComposition(/* args */)
{
}

void SaveComposition::fftwComplexToFile(fftw_complex *data, int dims[3], unsigned int time, std::string output_format)
{
    std::string temp_str;
    char *filename;
    if (output_format == "PNG")
    {
        // naming output file
        temp_str = "output_" + std::to_string(time) + ".PNG";
        filename = new char[temp_str.length() + 1];
        strcpy(filename, temp_str.c_str());

        SaveComposition::fftwComplexToPng(data, dims, filename);
    }
    else if (output_format == "VTI")
    {
        // naming output file
        temp_str = "output_" + std::to_string(time) + ".vti";
        filename = new char[temp_str.length() + 1];
        strcpy(filename, temp_str.c_str());
        fftwComplexToVti(data, dims, filename);
    }
    std::cout << "Generating output for " << time << std::endl;
}

void SaveComposition::fftwComplexToVti(fftw_complex *data, int dims[3], const char *filename)
{
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(dims[0], dims[1], dims[2]);
    imageData->AllocateScalars(VTK_DOUBLE, 2);

    for (int k = 0; k < dims[2]; k++)
    {
        for (int j = 0; j < dims[1]; j++)
        {
            for (int i = 0; i < dims[0]; i++)
            {
                int index = i + j * dims[0] + k * dims[0] * dims[1];
                imageData->SetScalarComponentFromDouble(i, j, k, 0, data[index][0]); // real part
                imageData->SetScalarComponentFromDouble(i, j, k, 1, data[index][1]); // imaginary part
            }
        }
    }

    vtkSmartPointer<vtkXMLImageDataWriter> writer = vtkSmartPointer<vtkXMLImageDataWriter>::New();
    writer->SetFileName(filename);
    writer->SetInputData(imageData);
    writer->Write();
}

void SaveComposition::fftwComplexToPng(fftw_complex *data, int dims[3], const char *filename)
{
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetDimensions(dims[0], dims[1], dims[2]);
    imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1); // Use VTK_UNSIGNED_CHAR for grayscale image

    // Find the maximum magnitude
    double maxMagnitude = 0.0;
    for (int k = 0; k < dims[2]; k++)
    {
        for (int j = 0; j < dims[1]; j++)
        {
            for (int i = 0; i < dims[0]; i++)
            {
                int index = i + j * dims[0] + k * dims[0] * dims[1];
                double magnitude = sqrt(data[index][0] * data[index][0] + data[index][1] * data[index][1]);
                if (magnitude > maxMagnitude)
                    maxMagnitude = magnitude;
            }
        }
    }

    // Convert the magnitude to unsigned char and set the scalar values
    for (int k = 0; k < dims[2]; k++)
    {
        for (int j = 0; j < dims[1]; j++)
        {
            for (int i = 0; i < dims[0]; i++)
            {
                int index = i + j * dims[0] + k * dims[0] * dims[1];
                double magnitude = sqrt(data[index][0] * data[index][0] + data[index][1] * data[index][1]);
                unsigned char value = static_cast<unsigned char>((magnitude / maxMagnitude) * 255.0);
                imageData->SetScalarComponentFromDouble(i, j, k, 0, value);
            }
        }
    }

    // Save the image data as PNG
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(filename);
    writer->SetInputData(imageData);
    writer->Write();
}

SaveComposition::~SaveComposition()
{
}
