/*
 * CGradient.h
 *
 *  Created on: 13 Mar 2013
 *      Author: dragomir
 */

#ifndef CGRADIENT_H_
#define CGRADIENT_H_

#include <cml/cml.h>
#include <vector>
#include <cassert>

#include "../include/CImg.h"

using namespace cimg_library;
using namespace std;

namespace archer
{

  typedef cml::vector3f color3f;

  bool sortStops(std::pair<float, color3f> p1, std::pair<float, color3f> p2);

  /**
   * Linear gradient with multiple stops.
   */
  class CGradient
  {
    private:

      std::vector < std::pair<float, color3f> > stops;

      float minStop;
      float maxStop;

      float getLinearGradient(float, float, float, float, float) const;
      color3f getLinearColor(float, const std::pair<float, color3f> &, const std::pair<float, color3f> &) const;

    public:

      CGradient();
      void reset();
      void addStop(float, color3f);
      void addStop(float, int, int, int);
      void saveAsPNG(char * , int, int);
      color3f getColor(float);
      virtual ~CGradient();
  };

} /* namespace archer */
#endif /* CGRADIENT_H_ */
