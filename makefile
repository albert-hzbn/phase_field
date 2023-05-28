CC = g++
CFLAGS = -fPIC -I/usr/local/include/vtk-9.2 -g
LDFLAGS =  -L/usr/local/fftw/lib -L/usr/local/lib
LDLIBS =  -lfftw3 -lvtkCommonCore-9.2 -lvtkCommonDataModel-9.2 \
         -lvtkIOCore-9.2 -lvtkIOImage-9.2 -lvtkIOLegacy-9.2 -lvtksys-9.2 -lvtkIOXML-9.2 -lvtkCommonExecutionModel-9.2 \
          -lm -debug

OBJS = main.o LoadInput.o LoadVTK.o CH1D.o CH2D.o SaveComposition.o

.PHONY: all clean

all: phase_field

phase_field: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) phase_field
