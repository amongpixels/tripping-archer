/*
 * CImageBuffer.h
 *
 *  Created on: Jan 18, 2013
 *      Author: dragomir
 */

#ifndef CIMAGEBUFFER_H_
#define CIMAGEBUFFER_H_

#include <fstream>
#include <vector>
#include <string>

#include "../external/CImg.h"

using namespace cimg_library;
using namespace std;

class CImageBuffer
{
  private:

    CImg <unsigned char> * buffer;
    int width;
    int height;

  public:
    CImageBuffer();
    CImageBuffer(unsigned int width, unsigned int height);

    //void init(unsigned int width, unsigned int height);
    void putPixel(unsigned int x, unsigned int y, int * rgb);
    void savePNG(string path);
    int getWidth();
    int getHeight();

    virtual ~CImageBuffer();
};

#endif /* CIMAGEBUFFER_H_ */
