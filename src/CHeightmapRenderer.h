/*
 * CHeightmapRenderer.h
 *
 *  Created on: 25 Mar 2013
 *      Author: dragomir
 */

#ifndef CHEIGHTMAPRENDERER_H_
#define CHEIGHTMAPRENDERER_H_

#include <cml/cml.h>

#include "CHeightmap.h"
#include "CGradient.h"

using namespace cml;

namespace archer
{
  /**
   * Renders a heightmap into a texture. In general it colors the heightmap
   * using linear gradient (depending on heights) which can be loaded from
   * the preset. It can calculate simple lighting and shadows.
   */
  class CHeightmapRenderer
  {
    private:

      bool renderShadows; // Whether to draw shadows
      bool renderLight; // Whether to calculate lighting

      vector3f lightDirection;

      CGradient colors;

    public:
      CHeightmapRenderer();
      void setRenderShadows(bool b);
      void setRenderLight(bool b);
      void renderToPNG(CHeightmap * h, char * path);
      void loadPreset(char * path);
      virtual ~CHeightmapRenderer();
  };

} /* namespace archer */
#endif /* CHEIGHTMAPRENDERER_H_ */
