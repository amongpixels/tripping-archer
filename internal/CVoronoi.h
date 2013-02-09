/*
 * CVoronoi.h
 *
 *  Created on: 8 Feb 2013
 *      Author: dragomir
 */

#ifndef CVORONOI_H_
#define CVORONOI_H_

#include <cstdlib>
#include <ctime>
#include <vector>

#include "IHeightmapProcessor.h"

using namespace std;

struct voronoiDistance {
    int pointIndex;
    float distance;
};

struct voronoiPoint {
    vector2i position;
    std::vector <voronoiDistance> distances;
};

bool compareDistances (voronoiDistance d1, voronoiDistance d2);

class CVoronoi : IHeightmapProcessor
{
  private:

    int pointsCount;
    std::vector <voronoiPoint> points;

  public:

    CVoronoi();

    void apply(CHeightmap * h);

    virtual ~CVoronoi();
};

#endif /* CVORONOI_H_ */
