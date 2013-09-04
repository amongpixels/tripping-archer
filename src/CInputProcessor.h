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

#include "../include/CImg.h"
#include "CPointsSet2i.h"

using namespace std;
using namespace cimg_library;

namespace archer
{
  typedef std::vector< std::vector<int> > Array2i;

  struct SPointCluster {
      unsigned char channel;
      CPointsSet2i points;
  };

  /**
   * Input processor generally reads the input (rgb bitmap) and clusters
   * the data into points sets.
   */
  class CInputProcessor
  {
    private:

      std::vector <SPointCluster *> clusters;

      int inputWidth;
      int inputHeight;

      /**
       * Performs a flood fill on a given channel to find a full continuous cluster
       */
      void floodFillOnChannel (CImg<unsigned char> const & img, vector2i const & point, SPointCluster * cluster, unsigned char channel);

      SPointCluster * findPointInCluster(vector2i const & point);

      //void processClusters();

    public:
      CInputProcessor();
      void loadFromImage(char * path);

      int getInputWidth();
      int getInputHeight();

      std::vector <SPointCluster *> & getClusters();
      vector2i getClusterMedian (SPointCluster & cluster);

      virtual ~CInputProcessor();
  };
}

#endif /* CINPUTPROCESSOR_H_ */
