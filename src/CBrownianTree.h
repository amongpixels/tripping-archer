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

  /**
   * This class can generate a Brownian tree. Since it implements CPointsSet2i
   * the generated tree can be used as a bounding points set
   * for particle depositor and other heightmap processors.
   */
  class CBrownianTree : public CPointsSet2i
  {
    private:
      CPointsSet2i * boundingPoints;
    public:
      CBrownianTree();

      CBrownianTree(unsigned int ite, unsigned int w, unsigned int h);

      /**
       * Generates Brownian tree.
       * @param ite number of iterations to run
       * @param w width of the tree
       * @param h height
       */
      void createBrownian(unsigned int ite, unsigned int w, unsigned int h);

      /**
       * The area where the tree is created can be itself limited by
       * a points set
       */
      void setBoundingPoints(CPointsSet2i *);

      virtual ~CBrownianTree();
  };

} /* namespace arrow */
#endif /* CBROWNIANTREE_H_ */
