## Audio Parse Algorithm
AUDIOINCLUDE = algorithm/audio/include
AUDIOSRC     = algorithm/audio/src

AUDIODECODERINC = algorithm/audio/include/decoder
AUDIODECODERSRC = algorithm/audio/src/decoder

## Video Parse Algorithm
VIDEOINC = algorithm/video

## Logger
LOGGER = logger

## Common
COMMON = common

## Example
EXAMPLE = example

## common file o
COMMONFILE = getbitcontext.o \
			 gymath.o

## Main Include And Linked File
ALLFILE    = logger.o    \
             hevc_ps.o   \
			 $(COMMONFILE)

ALLINCLUDE = -I $(AUDIOINCLUDE) -I $(COMMON) -I $(AUDIODECODERINC) -I $(VIDEOINC)
ALLCPPLIB  =

main:main.o $(ALLFILE)
	g++ -o main main.o $(ALLFILE) $(ALLINCLUDE) $(ALLCPPLIB)
main.o:
	g++ -c $(EXAMPLE)/main.cc -o main.o $(ALLINCLUDE)
logger.o:
	g++ -c $(LOGGER)/logger.cc -o logger.o
hevc_ps.o:
	g++ -c $(VIDEOINC)/hevc_ps.cc -o hevc_ps.o

getbitcontext.o:
	g++ -c $(COMMON)/getbitcontext.cc -o getbitcontext.o
gymath.o:
	g++ -c $(COMMON)/gymath.cc -o gymath.o

clean:
	rm -rf *.o