/*
 * IHeightmapProcessor.h
 *
 *  Created on: 8 Feb 2013
 *      Author: dragomir
 */

#ifndef IHEIGHTMAPPROCESSOR_H_
#define IHEIGHTMAPPROCESSOR_H_

#include <cml/cml.h>

#include "CHeightmap.h"

using namespace cml;

class IHeightmapProcessor
{
  public:
    virtual void apply(CHeightmap * h) = 0;
};


#endif /* IHEIGHTMAPPROCESSOR_H_ */
