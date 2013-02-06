/*
 * CHeightmap.h
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#ifndef CHEIGHTMAP_H_
#define CHEIGHTMAP_H_

#include <vector>

#include "../external/CImg.h"

using namespace cimg_library;
using namespace std;

class CHeightmap
{
  private:

    vector < vector<float> > values;

    int width;
    int height;

    float maxValue;

  public:

    void init (int w, int h);

    int getWidth();
    int getHeight();

    float getValue(int x, int y);
    void setValue(int x, int y, float v);

    void saveAsPNG(char * path);

    CHeightmap();
    CHeightmap(int w, int h);


};

#endif /* CHEIGHTMAP_H_ */
