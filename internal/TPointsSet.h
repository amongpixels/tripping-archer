/*
 * CPoints.h
 *
 *  Created on: 18 Feb 2013
 *      Author: dragomir
 */

#ifndef TPOINTSSET_H_
#define TPOINTSSET_H_

#include <vector>
#include <algorithm>
#include <cml/cml.h>

using namespace cml;

namespace archer
{
  
  template <class T> bool compareVectors(T v1, T v2) {
      return v1.length_squared() < v2.length_squared();
  }

  template <class T> class TPointsSet
  {
    protected:

      std::vector <T> points;

      T topRight;
      T bottomLeft;

    public:
//      TPointsSet() {
//
//      }

      void addPoint(const T & v) {

        if (this->points.empty()) {
          this->topRight = v;
          this->bottomLeft = v;
        }
        else {
          this->topRight.set(std::max(this->topRight[0], v[0]), std::max(this->topRight[1], v[1]));
          this->bottomLeft.set(std::min(this->bottomLeft[0], v[0]), std::min(this->bottomLeft[1], v[1]));
        }

        this->points.push_back(v);

      }

      bool isPointInSet(const T & v) {

        // this is a complete bruteforce now, something like kdtree would be apprioproate
        for (typename std::vector<T>::iterator p = this->points.begin() ; p < this->points.end() ; p++) {
          if (v == (*p)) {
            return true;
          }
        }

        return false;
      }

      bool isEmpty() {
        return this->points.empty();
      }

      TPointsSet<T> & intersetc(TPointsSet<T> & ps) {

        TPointsSet<T> intersection;

        for (typename std::vector<T>::iterator p = this->points.begin() ; p < this->points.end() ; p++) {
          if (ps.isPointInSet(*p)) {
            intersection.addPoint(*p);
          }
        }

        return intersection;

      }

      TPointsSet<T> & translate(T & v) {

        TPointsSet<T> translated;

        for (typename std::vector<T>::iterator p = this->points.begin() ; p < this->points.end() ; p++) {
          translated.addPoint(p + v);
        }

        return translated;
      }

      T & getMedianPoint() {

        std::vector<T> pointsCopy = this->points;

        // sort points in cluster
        std::sort(pointsCopy.begin(), pointsCopy.end(), compareVectors<T>);

        return pointsCopy[std::floor(pointsCopy.size() * 0.5)];

      }

      std::vector <T> * getPoints() {
        return &this->points;
      }

      int getCount() {
        return this->points.size();
      }

//      ~TPointsSet() {
//
//      }

  };

  template <class T> bool compareVectors(T v1, T v2);

} /* namespace arrow */
#endif /* TPOINTSSET_H_ */
