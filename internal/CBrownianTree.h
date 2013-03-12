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
    private:
      CPointsSet2i * boundingPoints;
    public:
      CBrownianTree();
      CBrownianTree(unsigned int ite, unsigned int w, unsigned int h);
      void createBrownian(unsigned int ite, unsigned int w, unsigned int h);
      void setBoundingPoints(CPointsSet2i *);
      virtual ~CBrownianTree();
  };

} /* namespace arrow */
#endif /* CBROWNIANTREE_H_ */
