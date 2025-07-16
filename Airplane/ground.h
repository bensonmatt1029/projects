/***********************************************************************
 * Header File:
 *    GROUND
 * Author:
 *    Matt Benson
 * Summary:
 *    Where the ground is located and where the runway is
 ************************************************************************/

#pragma once

#include "position.h"   // for Point
#include "uiDraw.h"  // for ogstream


 /***********************************************************
  * GROUND
  * The ground class
  ***********************************************************/
class Ground
{
public:
   // the constructor generates the ground
   Ground(const Position & posUpperRight);

   // reset the game
   void reset();

   // draw the ground on the screen
   void draw(ogstream & gout) const;

   // determine how high the Point is off the ground
   double getElevation(const Position & pos) const
   {
      if (pos.getMetersX() >= 0.0 && pos.getMetersX() < posUpperRight.getMetersX())
         return pos.getMetersY() - ground[(int)pos.getMetersX()];
      else
         return 0.0;
   }

   Position getPlatformCenter() const;

   // on the platform
   bool onPlatform(const Position& position, int landerWidth) const;

   // hit the ground
   bool hitGround(const Position& position, int landerWidth) const;

private:
   double * ground;               // the ground array, expressed in elevation 
   long    iLZ;                   // the left-most location of the landing zone (LZ)
   long runwayWidth;
   Position posUpperRight;        // the width and height of the world
};
