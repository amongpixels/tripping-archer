/*
 * CSimplexNoise.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: dragomir
 */

#include "CSimplexNoise.h"

CSimplexNoise::CSimplexNoise() {
  
  for(int i = 0 ; i < 512 ; i++) {
    perm[i] = p[i & 255];
    permMod12[i] = (short)(perm[i] % 12);
  }

  this->F2 = 0.5 * (sqrt(3.0) - 1.0);
  this->G2 = (3.0 - sqrt(3.0)) / 6.0;

}

double CSimplexNoise::getNoise(double xin, double yin) {

  double n0, n1, n2; // Noise contributions from the three corners

  // Skew the input space to determine which simplex cell we're in
  double s = (xin+yin)*F2; // Hairy factor for 2D

  int i = floor(xin+s);
  int j = floor(yin+s);

  double t = (i+j)*G2;

  double X0 = i-t; // Unskew the cell origin back to (x,y) space
  double Y0 = j-t;

  double x0 = xin-X0; // The x,y distances from the cell origin
  double y0 = yin-Y0;

  // For the 2D case, the simplex shape is an equilateral triangle.
  // Determine which simplex we are in.

  int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords

  if (x0 > y0) { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    i1=1;
    j1=0;
  }
  else { // upper triangle, YX order: (0,0)->(0,1)->(1,1)
    i1=0;
    j1=1;
  }

  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6
  double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
  double y1 = y0 - j1 + G2;
  double x2 = x0 - 1.0 + 2.0 * G2; // Offsets for last corner in (x,y) unskewed coords
  double y2 = y0 - 1.0 + 2.0 * G2;

  // Work out the hashed gradient indices of the three simplex corners
  int ii = i & 255;
  int jj = j & 255;
  int gi0 = permMod12[ii+perm[jj]];
  int gi1 = permMod12[ii+i1+perm[jj+j1]];
  int gi2 = permMod12[ii+1+perm[jj+1]];

  // Calculate the contribution from the three corners
  double t0 = 0.5 - x0*x0-y0*y0;
  if(t0<0) n0 = 0.0;
  else {
    t0 *= t0;
    n0 = t0 * t0 * dot(grad[gi0], x0, y0);  // (x,y) of grad3 used for 2D gradient
  }
  double t1 = 0.5 - x1*x1-y1*y1;
  if(t1<0) n1 = 0.0;
  else {
    t1 *= t1;
    n1 = t1 * t1 * dot(grad[gi1], x1, y1);
  }
  double t2 = 0.5 - x2*x2-y2*y2;
  if(t2<0) n2 = 0.0;
  else {
    t2 *= t2;
    n2 = t2 * t2 * dot(grad[gi2], x2, y2);
  }

  // Add contributions from each corner to get the final noise value.
  // The result is scaled to return values in the interval [-1,1].
  return 70.0 * (n0 + n1 + n2);
}

void CSimplexNoise::setBounds(double x1, double y1, double x2, double y2) {
  this->x1 = x1;
  this->x2 = x2;
  this->y1 = y1;
  this->y2 = y2;
}

void CSimplexNoise::setOctaves(int octaves) {
  this->octaves = octaves;
}

void CSimplexNoise::setPersistence(double persistence) {
  this->persistence = persistence;
}

void CSimplexNoise::apply(CImg <unsigned char> * buffer) {

  double xEnd = x2 - x1;
  double yEnd = y2 - y1;

  double xStep = buffer->width() / xEnd;
  double yStep = buffer->height() / yEnd;

  float f, a; // frequency, amplitude

  printf("%d %d ; %f %f\n", xEnd, yEnd, xStep, yStep);

  //buffer->clear();

  //CImg<unsigned char> tempBuffer(*buffer);

  double noise [buffer->width()] [buffer->height()];
  for (int i = 0 ; i < buffer->width() ; i++) {
    for (int j = 0 ; j < buffer->height() ; j++) {
      noise[i][j] = 0.0;
    }
  }

  double maxNoiseValue = 0.0;

  for (int o = 0 ; o < this->octaves ; o++) {

    f = pow(2.0, o);
    a = pow(this->persistence, o);

    printf("f: %f, a: %f\n", f, a);

    CImg<unsigned char> chujfer ((*buffer));



    for (int i = 0 ; i < buffer->width() ; i++) {
      for (int j = 0 ; j < buffer->height() ; j++) {

        float noiseValue = this->getNoise((double)(i) * f * 0.01, (double)(j) * f * 0.01) * a;
        //float noiseValue = this->getNoise(i, j);

        noiseValue = ((noiseValue + 1.0f) * 0.5f);

        noise[i][j] += noiseValue;

        if (noise[i][j] > maxNoiseValue) {
          maxNoiseValue = noise[i][j];
        }

        //printf("%f\n", noiseValue);

        int color [] = { (int)(255 * noiseValue), (int)(255 * noiseValue), (int)(255 * noiseValue) };
        chujfer.draw_point(i, j, color);
      }
    }

    string s = "chujfer";
    s += o;
    s += ".png";

    chujfer.save_png(s.c_str());

    //(*buffer) += tempBuffer;
    //tempBuffer.clear();

  }

  for (int i = 0 ; i < buffer->width() ; i++) {
    for (int j = 0 ; j < buffer->height() ; j++) {
      int n = (int)(255.0 * (noise[i][j] / maxNoiseValue));
      int color [] = { n, n, n };
      buffer->draw_point(i, j, color);
    }
  }

}

CSimplexNoise::~CSimplexNoise() {
  // TODO Auto-generated destructor stub
}

