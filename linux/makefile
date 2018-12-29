main:main.o global.o logger.o transformCode.o quantization.o
	g++ -o main main.o global.o logger.o transformCode.o quantization.o
main.o:
	g++ -c example/loggerexample1.cc -o main.o
logger.o:logger/logger.h
	g++ -c logger/logger.cc -o logger.o
global.o:common/global.h
	g++ -c common/global.cc -o global.o
transformCode.o:algorithm/transformCode.h
	g++ -c algorithm/transformCode.cc -o transformCode.o
quantization.o:algorithm/quantization.h
	g++ -c algorithm/quantization.cc -o quantization.o
clean:
	rm -rf *.o