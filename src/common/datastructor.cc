#include "DataStructor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace GY
{

static void initSource(DataSource* source);

DataSource* allocDataSource(int op)
{
  DataSource* source = static_cast<DataSource*>(malloc(sizeof(DataSource)));
  initSource(source);

  setDataSourceOp(source, op);

  return source;
}

DataSource* allocDataSource(int op, int dataLength, const char* data)
{
  DataSource* source = allocDataSource(op);

  allocDataSourceData(source, dataLength);
  copyToDataSource(source, data, dataLength);

  return source;
}

DataSource* allocDataSource(int op, int dataLength, const char* data, int argc, char** argv)
{
  DataSource* source = allocDataSource(op, dataLength, data);

  setDataSourceArgc(source, argc, argv);

  return source;
}

void freeDataSource(DataSource** source)
{
  if( (*source) == NULL )
    return;

  if( (*source)->_data != NULL )
    freeDataSourceData( *source );

  (*source)->_data = NULL;
  (*source)->_data_length = 0;

  free( *source );
  *source = NULL;

  return;
}

void allocDataSourceData(DataSource* source, int len)
{
  if(source == NULL)
    return;

  if(source->_data != NULL)
    freeDataSourceData( source );

  source->_data = NULL;
  source->_data_length = len;
  source->_data = (char*) malloc (sizeof(char) * len);
}

void freeDataSourceData(DataSource* source)
{
  if(source == NULL)
    return;

  if(source->_data == NULL)
    return;

  free(source->_data);
  source->_data = NULL;
  source->_data_length = 0;
}

bool copyToDataSource(DataSource* source, const char* data, int len)
{
  if( source == NULL )
    return false;

  if( source->_data_length < len )
    return false;

  memcpy( source->_data, data, len );
  return true;
}

void setDataSourceOp(DataSource* source, int op)
{
  if( source == NULL )
    return;

  source->_function_op = op;
}

int getDataSourceOp(DataSource* source)
{
  if(source != NULL)
    return source->_function_op;

  return -1;
}

int getDataSourceDataLength(DataSource* source)
{
  if( source == NULL )
    return -1;

  return source->_data_length;
}

void setDataSourceArgc(DataSource* source, int argc, char** argv)
{
  source->_argc = argc;
  source->_argv = argv;
}

void initSource(DataSource* source)
{
  if(source == NULL)
    return;

  source->_function_op = 0;
  source->_data_length = 0;
  source->_data = NULL;
}

}