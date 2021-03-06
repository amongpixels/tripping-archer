/*
 * CGradient.cpp
 *
 *  Created on: 13 Mar 2013
 *      Author: dragomir
 */

#include "CGradient.h"

namespace archer
{
  
  bool sortStops(std::pair<float, color3f> p1, std::pair<float, color3f> p2) {
    return p1.first < p2.first;
  }

  CGradient::CGradient() {
    // TODO Auto-generated constructor stub
  }
  
  void CGradient::addStop(float p, color3f color) {

    if (this->stops.empty()) {
      this->minStop = p;
      this->maxStop = p;
    }

    //assert(p >= 0.0f && p <= 1.0f);

    this->stops.push_back(std::pair<float, color3f>(p, color));

    // Make sure that the stops are ordered from smallest to the biggest
    std::sort(this->stops.begin(), this->stops.end(), sortStops);

    // Keep track of boundary stops
    if (p < this->minStop) {
      this->minStop = p;
    }

    if (p > this->maxStop) {
      this->maxStop = p;
    }

  }
  
  void CGradient::addStop(float p, int r, int g, int b) {
    this->addStop(p, color3f((float)(r) / 255.0f, (float)(g) / 255.0f, (float)(b) / 255.0f));
  }

  color3f CGradient::getColor(float p) {

    assert(p >= this->minStop && p <= this->maxStop);

    // Make sure that the stops range from 0.0 to 1.0
//    if (this->stops.front().first != 0.0f) {
//      this->stops.front().first = 0.0f;
//    }
//
//    if (this->stops.back().first != 1.0f) {
//      this->stops.back().first = 1.0f;
//    }

    std::pair<float, color3f> * start = &this->stops.front();
    std::pair<float, color3f> * end = &this->stops.back();

    // Find between which stops we are
    for (std::vector< std::pair<float, color3f> >::iterator i = this->stops.begin() ; i != this->stops.end() ; i++) {

      if (p == (*i).first) {
        return (*i).second;
      }
      else if ((*i).first < p) {
        start = &(*i);
      }
      else if ((*i).first > p) {
        end = &(*i);
        break;
      }

    }

    assert(start && end);

    return this->getLinearColor(p, (*start), (*end));

  }
  
  color3f CGradient::getLinearColor(float p, const std::pair<float, color3f> & c1, const std::pair<float, color3f> & c2) const {

    color3f r;

    if (p >= this->minStop || p <= this->maxStop) {

      //p = (p - c1.first) / (c2.first - c1.first);

      for (int i = 0 ; i < 3 ; i++) {
        r[i] = this->getLinearGradient(c1.second[i], c2.second[i], c1.first - this->minStop, c2.first - this->minStop, p - this->minStop);
        //r[i] = min(c1[i], c2[i]) + abs(c1[i] - c2[i]) * (c1[i] > c2[i] ? 1.0f - p : p);
      }
    }

    return r;

  }
  
  void CGradient::saveAsPNG(char * path, int w, int h) {

    CImg <unsigned char> img (w, h, 1, 3, 0);

    for (int x = 0 ; x < w ; x++) {

      color3f color = this->getColor((float)(x) / (float)(w) * (std::abs(this->minStop) + std::abs(this->maxStop)) + this->minStop);

      char c [] = { 255 * color[0], 255 * color[1], 255 * color[2] };

      img.draw_line(x, 0, x, h, c);

    }

    img.save_png(path);

  }
  
  float CGradient::getLinearGradient(float start, float stop, float start_offset, float stop_offset, float offset) const {
    //printf("start %f, stop %f, start offset %f, stop offset %f, offset %f\n", start, stop, start_offset, stop_offset, offset);
    return (start + ((offset - start_offset) / (stop_offset - start_offset) * (stop - start)));
  }
  
  void CGradient::reset() {
    this->stops.clear();
  }

  CGradient::~CGradient() {
    // TODO Auto-generated destructor stub
  }

} /* namespace archer */
