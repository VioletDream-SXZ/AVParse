main:main.o x264_structure.o analysis.o
	g++ -o main main.o x264_structure.o analysis.o
main.o:
	g++ -o main.o -c AnalysisH264.cpp
x264_structure.o:x264_structure.h types.h
	g++ -c x264_structure.cpp -o x264_structure.o
analysis.o:analysis.h types.h
	g++ -c analysis.cpp -o analysis.o
clean:
	rm -rf *.o