## Audio Parse Algorithm
AUDIOINCLUDE = algorithm/Audio/include
AUDIOSRC     = algorithm/Audio/src

AUDIODECODERINC = algorithm/Audio/include/decoder
AUDIODECODERSRC = algorithm/Audio/src/decoder

## Logger
LOGGER = logger

## Common
COMMON = common

## Example
EXAMPLE = example

## Main Include And Linked File
ALLFILE    = DataStructor.o \
             aExample.o decoderExample.o \
             audioTool.o aacParse.o wavParse.o oggParse.o \
             pcmEncoder.o \
             aacAdtsDecoder.o
ALLINCLUDE = -I $(AUDIOINCLUDE) -I $(COMMON) -I $(AUDIODECODERINC)
ALLCPPLIB  = -l fdk-aac

main:main.o $(ALLFILE)
	g++ -o main main.o $(ALLFILE) $(ALLINCLUDE) $(ALLCPPLIB)
main.o:
	g++ -c $(EXAMPLE)/main.cc -o main.o $(ALLINCLUDE)
DataStructor.o:
	g++ -c $(COMMON)/DataStructor.cc -o DataStructor.o

aExample.o:
	g++ -c $(EXAMPLE)/example.cc -o aExample.o $(ALLINCLUDE)
decoderExample.o:
	g++ -c $(EXAMPLE)/decoderExample.cc -o decoderExample.o $(ALLINCLUDE)

audioTool.o:
	g++ -c $(AUDIOSRC)/audioTool.cc -o audioTool.o -I $(AUDIOINCLUDE)

aacParse.o:
	g++ -c $(AUDIOSRC)/aacParse.cc -o aacParse.o -I $(AUDIOINCLUDE)
wavParse.o:
	g++ -c $(AUDIOSRC)/wavParse.cc -o wavParse.o -I $(AUDIOINCLUDE)
oggParse.o:
	g++ -c $(AUDIOSRC)/oggParse.cc -o oggParse.o -I $(AUDIOINCLUDE)

pcmEncoder.o:
	g++ -c $(AUDIOSRC)/pcmEncoder.cc -o pcmEncoder.o -I $(AUDIOINCLUDE)
aacAdtsDecoder.o:
	g++ -c $(AUDIODECODERSRC)/aacAdtsDecoder.cc -o aacAdtsDecoder.o -I $(AUDIODECODERINC)

clean:
	rm -rf *.o