/*
 * CImageBuffer.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: dragomir
 */

#include "CImageBuffer.h"

CImageBuffer::CImageBuffer () : CImageBuffer(64, 64) {

}

CImageBuffer::CImageBuffer(unsigned int width, unsigned int height) {

  this->buffer = new CImg <unsigned char> (width, height, 1, 3, 0);
  this->width = width;
  this->height = height;

}

void CImageBuffer::putPixel(unsigned int x, unsigned int y, int * rgb) {

  this->buffer->draw_point(x, y, rgb);

}

void CImageBuffer::savePNG(string path) {

  this->buffer->save_png(path.c_str());

}

int CImageBuffer::getWidth() {
  return this->width;
}

int CImageBuffer::getHeight() {
  return this->height;
}

CImageBuffer::~CImageBuffer () {

  delete this->buffer;

}

