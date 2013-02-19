/*
 * CHeightmap.h
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#ifndef CHEIGHTMAP_H_
#define CHEIGHTMAP_H_

#include <cml/cml.h>
#include <vector>
#include <cassert>

#include "../external/CImg.h"
#include "helpers.h"

using namespace cimg_library;
using namespace std;
using namespace cml;

namespace archer
{
  class CHeightmap
  {
    private:

      std::vector < std::vector<float> > values;
      std::vector < std::vector<vector3f> > normals;

      int width;
      int height;

      float maxValue;
      float heightScale;

      void calculateNormals();

    public:

      void init (int w, int h);

      int getWidth();
      int getHeight();

      float getValue(int x, int y);
      void setValue(int x, int y, float v);
      void setHeightScale(float h);
      void zero();
      void normalize();

      void saveAsPNG(char * path);
      void saveColorMapAsPNG(char * path);

      CHeightmap & operator += (CHeightmap &);
      CHeightmap & operator *= (const float &);

      CHeightmap();
      CHeightmap(int w, int h);


  };
}

#endif /* CHEIGHTMAP_H_ */
