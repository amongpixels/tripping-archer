/*
 * CNoise.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: dragomir
 */

#include "CNoise.h"

CNoise::CNoise() {
  // TODO Auto-generated constructor stub
  
}

float CNoise::getNoise(int x, int y) {

  //float n = x + y * 57;
  //n = pow(((int) n << 13), n);
  //return ( 1.0f - (float) ( (int) (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);

  //return 2.0f * ( (float)( ( ( ( x * 50 ) % 3000 ) * ( ( y * 50 ) % 3000 ) + 1 ) % 8051 ) / 8051.0f  - 0.5f);
  //int n = x + y * 57;
  //n = pow((float)(n << 13), n);
  //return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);

  m_z = 36969 * (m_z & 65535) + (m_z >> 16) + x;
  m_w = 18000 * (m_w & 65535) + (m_w >> 16) + y;

  return ((m_z << 16) + m_w) * 2.328306435454494e-10;


  //unsigned int n = x + y * 57;
  //n = (int) pow((double) (n << 13), (int) n);
  //return ( 1.0f - (float) ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);

}

void CNoise::fillShitWithBullshit(CImageBuffer * buffer) {

  for (int i = 0 ; i < buffer->getWidth() ; i++) {
    for (int j = 0 ; j < buffer->getHeight() ; j++) {

      float noiseValue = this->getNoise(i, j);

      //noiseValue = (noiseValue + 1.0f) * 0.5f;

      printf("%f\n", noiseValue);

      int color [] = { (int)(255 * noiseValue), (int)(255 * noiseValue), (int)(255 * noiseValue) };
      //int color [] = { 255, 0, 0 };
      buffer->putPixel(i, j, color);
    }
  }

}

CNoise::~CNoise() {
  // TODO Auto-generated destructor stub
}

