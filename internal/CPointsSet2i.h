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

using namespace cml;
using namespace cimg_library;

namespace archer
{
//  enum EMaskType {
//    LINEAR,
//  };
  
  class CPointsSet2i : public TPointsSet<vector2i>
  {
    public:

      int getWidth();
      int getHeight();
      //CHeightmap & getHeightmap();
      void saveAsPNG(char * path);

      void createMask(CHeightmap * h);

      //virtual ~CPointsSet2i();
  };

} /* namespace archer */
#endif /* CPOINTSSET2I_H_ */
