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
#include "CGradient.h"
#include "CNoise.h"
#include "CPointsSet2i.h"

using namespace cimg_library;
using namespace std;
//using namespace cml;

typedef cml::vector2i vector2i;
typedef cml::vector3f vector3f;

namespace archer
{
  /*
   * Heightfield representation where height differs from -1 to 1 inclusive
   */
  class CHeightmap
  {
      friend class CHeightmapRenderer;

    private:

      std::vector < std::vector<float> > values;
      std::vector < std::vector<vector3f> > normals;

      int width;
      int height;

      float maxValue;
      float heightScale;

      bool hasBeenModified;

      void init (int w, int h);
      void calculateNormals();

    public:

      //static float maxHeight;
      //static float minHeight;

      float maxHeight;

      int getWidth() const;
      int getHeight() const;

      float getValue(int x, int y) const;
      vector3f getNormal(int x, int y);
      void setValue(int x, int y, float v);
      void setHeightScale(float h);
      float getHeightScale();
      float getMaxHeight();
      void setMaxHeight(float h);
      void zero();
      void levelOut(const CPointsSet2i & p);

      // This generates a simple test heightmap
      void test();

      void normalize();
      void flip();

      void saveAsPNG(char * path);
      //void saveColorMapAsPNG(char * path, bool useShadows = false);

      void loadFromPNG(char * path);
      //void loadRenderConfig(char * path);

      CHeightmap & operator = (float);

      CHeightmap & operator += (CHeightmap &);
      CHeightmap & operator -= (CHeightmap &);
      CHeightmap & operator *= (const float &);
      CHeightmap & operator *= (const CHeightmap &);
      const CHeightmap operator * (const CHeightmap &);

      CHeightmap();
      CHeightmap(int w, int h);


  };
}

#endif /* CHEIGHTMAP_H_ */
