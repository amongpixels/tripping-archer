/*
 * IBufferProcessor.h
 *
 *  Created on: Jan 20, 2013
 *      Author: dragomir
 */

#ifndef IBUFFERPROCESSOR_H_
#define IBUFFERPROCESSOR_H_

//#include "CImageBuffer.h"
#include "../external/CImg.h"

using namespace cimg_library;

class IBufferProcessor
{
  public:
    virtual void apply(CImg <unsigned char> *) = 0;
    //virtual ~IBufferProcessor() = 0;
};

#endif /* IBUFFERPROCESSOR_H_ */
