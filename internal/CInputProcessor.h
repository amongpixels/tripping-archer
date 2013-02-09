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

#include "../external/CImg.h"

using namespace std;
using namespace cml;
using namespace cimg_library;

struct SPointCluster {
    unsigned char color [3];
    std::vector <vector2i> points;
};

class CInputProcessor
{
  private:

    void floodFill(CImg<unsigned char> * img, vector2i * point, SPointCluster * cluster);
    bool isPointInCluster(CImg<unsigned char> * img, vector2i point);

  public:
    CInputProcessor();

    void loadFromImage(char * path);

    virtual ~CInputProcessor();
};

#endif /* CINPUTPROCESSOR_H_ */
