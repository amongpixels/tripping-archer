/*
 * CSimplexNoise.cpp
 *
 *  Created on: Jan 19, 2013
 *      Author: dragomir
 */

#include "CGradientNoise.h"

namespace archer {

  CGradientNoise::CGradientNoise() {
    this->x1 = 0.0;
    this->x2 = 1.0;
    this->y1 = 0.0;
    this->y2 = 1.0;
  }

  void CGradientNoise::setBounds(double x1, double y1, double x2, double y2) {
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
  }

  void CGradientNoise::setOctaves(int octaves) {
    this->octaves = octaves;
  }

  void CGradientNoise::setPersistence(double persistence) {
    this->persistence = persistence;
  }

  void CGradientNoise::apply(CHeightmap * h) {

    printf("Generating simplex noise... ");

    CNoise noise;

    double xEnd = x2 - x1;
    double yEnd = y2 - y1;

    double xStep = xEnd / ((double)h->getWidth());
    double yStep = yEnd / ((double)h->getHeight());

    double f, a; // frequency, amplitude

    //printf("xEnd yEnd : %f %f, xStep yStep: %f %f\n", xEnd, yEnd, xStep, yStep);

    for (int o = 0 ; o < this->octaves ; o++) {

      CHeightmap noiseHeightmap (h->getWidth(), h->getHeight());

      f = pow(2.0, o);
      a = pow(this->persistence, o);

      //printf("f: %f, a: %f\n", f, a);

      for (int x = 0 ; x < h->getWidth() ; x++) {
        for (int y = 0 ; y < h->getHeight() ; y++) {

          double noiseX = ((double)(x) * xStep + x1) * f;
          double noiseY = ((double)(y) * yStep + y1) * f;

          float noiseValue = noise.getNoise(noiseX, noiseY) * a;

          noiseHeightmap.setValue(x, y, noiseValue * this->persistence);

        }
      }

      (*h) += noiseHeightmap;

    }

    printf("done.\n");

  }

  CGradientNoise::~CGradientNoise() {
    // TODO Auto-generated destructor stub
  }
}
