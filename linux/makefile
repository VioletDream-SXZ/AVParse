## Audio Parse Algorithm
AUDIOINCLUDE = algorithm/Audio/include
AUDIOSRC     = algorithm/Audio/src

## Logger
LOGGER = logger

## Common
COMMON = common

## Example
EXAMPLE = example

## Main Include And Linked File
ALLFILE    = DataStructor.o aExample.o \
             audioTool.o aacParse.o wavParse.o
ALLINCLUDE = -I $(AUDIOINCLUDE) -I $(COMMON)

main:main.o $(ALLFILE)
	gcc -o main main.o $(ALLFILE) $(ALLINCLUDE)
main.o:
	g++ -c $(EXAMPLE)/main.cc -o main.o $(ALLINCLUDE)
DataStructor.o:
	g++ -c $(COMMON)/DataStructor.cc -o DataStructor.o
aExample.o:
	g++ -c $(EXAMPLE)/example.cc -o aExample.o $(ALLINCLUDE)

aacParse.o:
	g++ -c $(AUDIOSRC)/aacParse.cc -o aacParse.o -I $(AUDIOINCLUDE)
wavParse.o:
	g++ -c $(AUDIOSRC)/wavParse.cc -o wavParse.o -I $(AUDIOINCLUDE)
audioTool.o:
	g++ -c $(AUDIOSRC)/audioTool.cc -o audioTool.o -I $(AUDIOINCLUDE)
clean:
	rm -rf *.o