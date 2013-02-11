/*
 * CFault.h
 *
 *  Created on: 5 Feb 2013
 *      Author: dragomir
 */

#ifndef CFAULT_H_
#define CFAULT_H_

#include <cstdlib>
#include <cml/cml.h>

#include "CHeightmap.h"

using namespace cml;
using namespace archer;

class CFault
{
  private:

    int iterations;

  public:

    void apply(CHeightmap * h);
};

#endif /* CFAULT_H_ */
