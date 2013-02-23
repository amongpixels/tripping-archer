/*
 * CHydraulicErosion.h
 *
 *  Created on: 22 Feb 2013
 *      Author: dragomir
 */

#ifndef CHYDRAULICEROSION_H_
#define CHYDRAULICEROSION_H_

#include <boost/tuple/tuple.hpp>

#include "IHeightmapProcessor.h"

using namespace boost::tuples;

namespace archer
{
  
  class CHydraulicErosion : public IHeightmapProcessor
  {
    public:
      CHydraulicErosion();

      void apply(CHeightmap * h);

      virtual ~CHydraulicErosion();
  };

} /* namespace archer */
#endif /* CHYDRAULICEROSION_H_ */
