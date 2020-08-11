#ifndef GY_QUANTIZATION_H
#define GY_QUANTIZATION_H

#include <cmath>
#include "../common/global.h"

namespace GY
{

const int    QP     = 0;
const double QPStep = 0.625;
const int    QPPOW  = (1<<15);
const double QPf    = (double)QPPOW;
const double QPF    = (double)QPPOW/3.0;

const int    DEPOW  = (1<<2);
const int    DEf    = (double)DEPOW;

typedef struct TRANSFORM_QUAN_E
{
  const double A = 0.5;
  const double B = sqrt(0.4);
  
  const double E_4X4[4][4] = 
  {
    {    A*A,  A*B/2.0,      A*A,  A*B/2.0},
    {A*B/2.0,  B*B/4.0,  A*B/2.0,  B*B/4.0},
    {    A*A,  A*B/2.0,      A*A,  A*B/2.0},
    {A*B/2.0,  B*B/4.0,  A*B/2.0,  B*B/4.0}
  };
  
  const double a = 1.0 / (16.0 * sqrt(2));
  const double b = 1.0 / (17.0 * sqrt(2));
  const double c = 1.0 / (8.0  * sqrt(5));
  
  const double E_8X8[8][8] = 
  {
    {a, a, a, a, a, a, a, a},
    {b, b, b, b, b, b, b, b},
    {c, c, c, c, c, c, c, c},
    {b, b, b, b, b, b, b, b},
    {a, a, a, a, a, a, a, a},
    {b, b, b, b, b, b, b, b},
    {c, c, c, c, c, c, c, c},
    {b, b, b, b, b, b, b, b}
  };
}TRANSFORM_QUAN_E;

static TRANSFORM_QUAN_E E;

typedef struct QUANTIZATION_PARAMETER
{
  const double Q_4X4[6][4][4] = 
  {
    {
      {E.E_4X4[0][0]*1.0/QPStep*QPf, E.E_4X4[0][1]*1.0/QPStep*QPf, E.E_4X4[0][2]*1.0/QPStep*QPf, E.E_4X4[0][3]*1.0/QPStep*QPf},
      {E.E_4X4[1][0]*1.0/QPStep*QPf, E.E_4X4[1][1]*1.0/QPStep*QPf, E.E_4X4[1][2]*1.0/QPStep*QPf, E.E_4X4[1][3]*1.0/QPStep*QPf},
      {E.E_4X4[2][0]*1.0/QPStep*QPf, E.E_4X4[2][1]*1.0/QPStep*QPf, E.E_4X4[2][2]*1.0/QPStep*QPf, E.E_4X4[2][3]*1.0/QPStep*QPf},
      {E.E_4X4[3][0]*1.0/QPStep*QPf, E.E_4X4[3][1]*1.0/QPStep*QPf, E.E_4X4[3][2]*1.0/QPStep*QPf, E.E_4X4[3][3]*1.0/QPStep*QPf}
    },
    {0}
  };
  
  const double deQ_4X4[6][4][4] = 
  {
    {
      {10, 13, 10, 13},
      {13, 16, 13, 16},
      {10, 13, 10, 13},
      {13, 16, 13, 16}
    },
    {0}
  };
}QUANTIZATION_PARAMETER;

static QUANTIZATION_PARAMETER QUAN;

void Quantization_4x4(Macroblock4x4& m);
void deQuantization_4x4(Macroblock4x4& m);

}

#endif // GY_QUANTIZATION_H