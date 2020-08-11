#ifndef CJR_DATA_STRUCTOR_H
#define CJR_DATA_STRUCTOR_H

namespace GY
{

typedef struct DataSource
{
  int _function_op;
  int _data_length;
  char* _data;

  int    _argc;
  char** _argv;
}DataSource;

DataSource* allocDataSource(int op);
DataSource* allocDataSource(int op, int dataLength, const char* data);
DataSource* allocDataSource(int op, int dataLength, const char* data, int argc, char** argv);
void freeDataSource(DataSource** source);

void allocDataSourceData(DataSource* source, int len);
void freeDataSourceData(DataSource* source);

bool copyToDataSource(DataSource* source, const char* data, int len);

void setDataSourceOp(DataSource* source, int op);
int getDataSourceOp(DataSource* source);

int getDataSourceDataLength(DataSource* source);

void setDataSourceArgc(DataSource* source, int argc, char** argv);

}

#endif // CJR_DATA_STRUCTOR_H