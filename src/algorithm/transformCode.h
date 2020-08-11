#ifndef GY_TRANSFORM_CODE_H
#define GY_TRANSFORM_CODE_H

#include <cmath>
#include "../common/global.h"

namespace GY
{

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

void HadamardTransform4x4(Macroblock4x4& m);
void HadamardTransform16x16(Macroblock16x16& m);

void DctTransform4x4(Macroblock4x4& m);
void DctTransform8x8(Macroblock16x16& m);

}

#endif // GY_TRANSFORM_CODE_H