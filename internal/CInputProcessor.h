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
using namespace cml;
using namespace cimg_library;

namespace archer
{

  struct SPointCluster {
      unsigned char color [3];
      CPointsSet2i points;
  };

  class CInputProcessor
  {
    private:

      std::vector <SPointCluster *> clusters;

      void floodFill(const CImg<unsigned char> & img, const vector2i & point, SPointCluster * cluster);

      SPointCluster * findPointInCluster(const vector2i & point);


    public:
      CInputProcessor();

      void loadFromImage(char * path);
      std::vector <SPointCluster *> * getClusters();

      vector2i getClusterMedian (SPointCluster & cluster);


      virtual ~CInputProcessor();
  };
}

#endif /* CINPUTPROCESSOR_H_ */
