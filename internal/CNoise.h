/*
 * CNoise.h
 *
 *  Created on: Jan 18, 2013
 *      Author: dragomir
 */

#ifndef CNOISE_H_
#define CNOISE_H_

#include <cmath>

#include "CImageBuffer.h"

class CNoise
{
  private:

    unsigned int m_z = 17398247348;
    unsigned int m_w = 29847238948;

    /**
     * This returns a random noise value for given 2d coordinates
     */
    float getNoise(int x, int y);

  public:
    CNoise();
    void fillShitWithBullshit(CImageBuffer * buffer);
    virtual ~CNoise();
};

#endif /* CNOISE_H_ */
