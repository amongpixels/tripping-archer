/*
 * CGradientNoise.h
 *
 *  Created on: Jan 19, 2013
 *      Author: dragomir
 *      Based on a paper by Stefan Gustavson
 *
 */

#ifndef CGRADIENTNOISE_H_
#define CGRADIENTNOISE_H_

#include <cmath>
#include <string>

using namespace std;

#include "IHeightmapProcessor.h"
#include "CNoise.h"

namespace archer
{
  class CGradientNoise : IHeightmapProcessor
  {
    private:

      int octaves; // Number of octaves used to build the noise
      double x1, y1, x2, y2; // Bounds of the noise (on 2d graph)
      double persistence; // high frequency = high persistence

    public:

      CGradientNoise();

      void setBounds(double x1, double y1, double x2, double y2);
      void setOctaves(int octaves);
      void setPersistence(double persistence);

      void apply(CHeightmap * h);

      ~CGradientNoise();
  };
}

#endif /* CGRADIENTNOISE_H_ */
