/*
 * helpers.cpp
 *
 *  Created on: 10 Feb 2013
 *      Author: dragomir
 */

#include "helpers.h"

void archer::helpers::printVector2i(const vector2i & v) {
  printf("vector2i %d %d\n", v[0], v[1]);
}

void archer::helpers::printVector2f(const vector2f & v) {
  printf("vector2f %f %f\n", v[0], v[1]);
}

void archer::helpers::printVector3f(const vector3f & v) {
  printf("vector3f %f %f %f\n", v[0], v[1], v[2]);
}
