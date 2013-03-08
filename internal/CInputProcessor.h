/*
 * CInputProcessor.h
 *
 *  Created on: 9 Feb 2013
 *      Author: dragomir
 */

#ifndef CINPUTPROCESSOR_H_
#define CINPUTPROCESSOR_H_

#include <vector>
#include <cml/cml.h>
#include <cmath>

#include "../external/CImg.h"
#include "CPointsSet2i.h"

using namespace std;
using namespace cimg_library;

namespace archer
{
  typedef std::vector< std::vector<int> > Array2i;

  struct SPointCluster {
      unsigned char color [3];
      CPointsSet2i points;
  };

  class CInputProcessor
  {
    private:

      std::vector <SPointCluster *> clusters;

      void floodFill (CImg<unsigned char> const & img, vector2i const & point, SPointCluster * cluster);

      SPointCluster * findPointInCluster(vector2i const & point);

      int getConnectivity(Array2i const & array, int x, int y);

      /*
       * Checks if any of the neighbours is present (neighbours passed as
       * indices of indices ie P1, P2, P3 etc.)
       */
      bool passesBackgroundTest(Array2i const & array, int x, int y, int neighbours [3]);

      /*
       * Generates a linear list of 9 indices P1, P2, P3 ... P9 defining x and y
       * coordinates of every cell in 3X3 cell starting at the origin, then
       * going up and counter clockwise.
       */
      void createNeghbourIndices(std::vector< std::pair<int, int> > * pairs, int x, int y);


    public:
      CInputProcessor();

      void loadFromImage(char * path);
      std::vector <SPointCluster *> * getClusters();

      void getSkeleton(char * path);

      vector2i getClusterMedian (SPointCluster & cluster);


      virtual ~CInputProcessor();
  };
}

#endif /* CINPUTPROCESSOR_H_ */
