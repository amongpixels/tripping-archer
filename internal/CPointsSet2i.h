/*
 * CPointsSet2i.h
 *
 *  Created on: 19 Feb 2013
 *      Author: dragomir
 */

#ifndef CPOINTSSET2I_H_
#define CPOINTSSET2I_H_

#include <cml/cml.h>
#include <cstdlib>

#include "../external/CImg.h"
#include "CHeightmap.h"
#include "TPointsSet.h"
#include "helpers.h"

//using namespace cml;
using namespace cimg_library;

namespace archer
{
//  enum EMaskType {
//    LINEAR,
//  };
  
  typedef std::vector< std::vector<unsigned char> > Array2b;

  class CPointsSet2i : public TPointsSet<vector2i>
  {
    protected:

      /*
       * This keeps the track of where points are, it's useful for quickly
       * telling if the point is there or building skeleton out of points
       * using thinning. Binary map is always up to date.
       */
      Array2b binaryMap;

    private:

      int getConnectivity(Array2b const & array, int x, int y);

      /*
       * Checks if any of the neighbours is present (neighbours passed as
       * indices of indices ie P1, P2, P3 etc.)
       */
      bool passesBackgroundTest(Array2b const & array, int x, int y, int neighbours [3]);

      /*
       * Generates a linear list of 9 indices P1, P2, P3 ... P9 defining x and y
       * coordinates of every cell in 3X3 cell starting at the origin, then
       * going up and counter clockwise.
       */
      void createNeghbourIndices(std::vector< std::pair<int, int> > * pairs, int x, int y);

    public:

      CPointsSet2i();

      int getWidth();
      int getHeight();

      /**
       * Without with and height specified this will save the image
       * with the size just enough to fit all the points. Specifying the
       * dimensions keeps all the points in original positions.
       */
      void saveAsPNG(char * path, int width = -1, int height = -1);

      //using TPointsSet<vector2i>::addPoint;
      void addPoint(const vector2i & v);

      /**
       * Sped up the point look-up from base class by using the binary map
       * (which is specific to integer points)
       */
      bool isPointInSet(const vector2i & v);

      //using TPointsSet<vector2i>::clear;
      void clear();

      void generateSkeleton(CPointsSet2i * skeleton);

      void createMask(CHeightmap * h, float blur = 0.0f);

      CPointsSet2i & operator += (const CPointsSet2i & p);
      const CPointsSet2i operator + (const CPointsSet2i & );

  };

} /* namespace archer */
#endif /* CPOINTSSET2I_H_ */
