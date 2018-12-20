main:main.o x264_structure.o analysis.o
	g++ -o main main.o x264_structure.o analysis.o
main.o:
	g++ -o main.o -c ./example/AnalysisH264.cpp
x264_structure.o:./base/x264_structure.h ./base/types.h
	g++ -c ./base/x264_structure.cpp -o x264_structure.o
analysis.o:./base/analysis.h ./base/types.h
	g++ -c ./base/analysis.cpp -o analysis.o
clean:
	rm -rf *.o