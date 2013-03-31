/*
 * CThermalErosion.h
 *
 *  Created on: 21 Feb 2013
 *      Author: dragomir
 */

#ifndef CTHERMALEROSION_H_
#define CTHERMALEROSION_H_

#include <boost/tuple/tuple.hpp>

#include "IHeightmapProcessor.h"

using namespace boost::tuples;

namespace archer
{
  
  class CThermalErosion : public IHeightmapProcessor
  {
    private:

      float talus;
      int iterations;

    public:
      CThermalErosion();

      void setTalus(float f);

      /**
       * Number of iterations
       */
      void setStrength(int i);


      void apply(CHeightmap * h);

      virtual ~CThermalErosion();
  };

} /* namespace archer */
#endif /* CTHERMALEROSION_H_ */
