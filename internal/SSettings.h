/*
 * SSettings.h
 *
 *  Created on: 24 Mar 2013
 *      Author: dragomir
 */

#ifndef SSETTINGS_H_
#define SSETTINGS_H_

namespace archer
{
  struct SSettings {
      bool useVoronoi;
      bool renderShadows;
      bool useErosion;
      unsigned char terrainType;
  };

  static SSettings globalSettings = { false, false, false, 3 };

}


#endif /* SSETTINGS_H_ */
