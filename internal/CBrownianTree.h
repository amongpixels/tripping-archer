/*
 * CBrownianTree.h
 *
 *  Created on: 18 Feb 2013
 *      Author: dragomir
 */

#ifndef CBROWNIANTREE_H_
#define CBROWNIANTREE_H_

#include <vector>
#include <cml/cml.h>
#include <cstdlib>
#include <ctime>

#include "CPointsSet2i.h"


using namespace cml;

namespace archer
{
  
  class CBrownianTree : public CPointsSet2i
  {
    public:
      CBrownianTree();
      CBrownianTree(unsigned int w, unsigned int h, unsigned int ite);
      void init(unsigned int w, unsigned int h, unsigned int ite);
      virtual ~CBrownianTree();
  };

} /* namespace arrow */
#endif /* CBROWNIANTREE_H_ */
