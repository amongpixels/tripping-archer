/*
 * helpers.h
 *
 *  Created on: 10 Feb 2013
 *      Author: dragomir
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <cstdio>
#include <cml/cml.h>

using namespace cml;

namespace archer {

  namespace helpers {

    void printVector2i (const vector2i & v);
    void printVector2f (const vector2f & v);
    void printVector3f (const vector3f & v);

  }

}


#endif /* HELPERS_H_ */
