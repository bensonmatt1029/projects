/***********************************************************************
 * Source File:
 *    GROUND
 * Author:
 *    Matt Benson
 * Summary:
 *    Where the ground is located and where the runway is
 ************************************************************************/

#include "ground.h"   // for the Ground class definition
#include "uiDraw.h"   // for random() and drawLine()
#include <cassert>
#include "airplane.h"

const long   LZ_SIZE   = 30;    // size of the landing zone
const double MAX_SLOPE = 0.0;   // steapness of the features. Smaller number is flatter
const double LUMPINESS = 100000.0;   // size of the hills. Smaller number is bigger features
const double TEXTURE   = 0.0;   // size of the small features such as rocks

 /************************************************************************
  * GROUND constructor
  * Create a new ground object
  ************************************************************************/
Ground::Ground(const Position & posUpperRight) : posUpperRight(posUpperRight)
{
   assert(posUpperRight.getMetersX() > 0.0);
   assert(posUpperRight.getMetersY() > 0.0);

   // allocate the array
   ground = new double[(int)posUpperRight.getMetersX()];
   runwayWidth = (long)posUpperRight.getMetersX(); // store width for later use

   reset();
}

/************************************************************************
 * RESET
 * Create a new ground
 ************************************************************************/
 void Ground :: reset()
 {
    iLZ = 0;
    // use runwayWidth member variable
    ground[0] = posUpperRight.getMetersY() / 4.0;

    double dy = 0.0;

    for (int i = 1; i < runwayWidth; i++)
    {
       if (i >= iLZ && i < iLZ + runwayWidth)
       {
          // whole runway flat
          ground[i] = ground[i - 1];
       }
      // otherwise, vary the slope
      else
      {
         // 0% is the bottom - favor sloping up to avoid the bottom
         // 100% is the top - favor sloping down to avoid the top
         double percent = ground[i - 1] / (posUpperRight.getMetersY() / 2.0) * 0.5;

         // dy is the slope. positive is up, negative is down
         dy += random(LUMPINESS * (0.25 - percent),
                      LUMPINESS * (0.75 - percent));

         // make sure the slop is not too steep
         if (dy > MAX_SLOPE)
            dy = MAX_SLOPE;
         if (dy < -MAX_SLOPE)
            dy = -MAX_SLOPE;

         // determine the elevation according to the slope
         ground[i] = ground[i - 1] + dy + random(-TEXTURE, TEXTURE);
      }
   }
}

/***********************************************************
 * HTI GROUND
 * Did the lander hit the ground?
 **********************************************************/
bool Ground::hitGround(const Position& pos, int landerWidth) const
{
   // find the extent of the lander
   int xMin = (int)(pos.getMetersX() - (double)landerWidth / 2.0);
   int xMax = (int)(pos.getMetersX() + (double)landerWidth / 2.0);

   // determine the extent
   xMin = (xMin < 0 ? 0 : xMin);
   xMax = (xMax > (int)posUpperRight.getMetersX() - 1 ? (int)posUpperRight.getMetersX() - 1 :  xMax);

   // find the max elevation for the width of the lander
   double maxElevation = ground[xMin];
   for (int i = xMin + 1; i <= xMax; i++)
      if (ground[i] > maxElevation)
         maxElevation = ground[i];

   // return whether we hit the ground
   return pos.getMetersY() < maxElevation;
}

/************************************************************************
 * ON PLATFORM
 * Have we landed on the platform?
 ************************************************************************/
bool Ground :: onPlatform(const Position & pos, int landerWidth) const
{
   // not on the platform if we are too high
   if (getElevation(pos) > 1.0)
      return false;

   // not on the platform if we hit the ground
   if (getElevation(pos) < 0.0)
      return false;

   // not on the platform if we are too far left
   if (pos.getMetersX() + landerWidth / 2.0 < (double)iLZ)
      return false;

   // not on the platform if we are too far right
   if (pos.getMetersX() - landerWidth / 2.0 > (double)(iLZ + runwayWidth))
      return false;

   return true;
}

/*****************************************************************
 * DRAW
 * Draw the ground on the screen
 ****************************************************************/
void Ground::draw(ogstream & gout) const
{
   // Draw the ground as dark gray (runway)
   for (long i = 0; i < (int)posUpperRight.getMetersX(); i++)
   {
      double red = 0.2;
      double green = 0.2;
      double blue = 0.2;

      gout.drawRectangle(Position((double)i, 0.0),
         Position((double)(i + 1), ground[i]),
         red, green, blue);
   }

   // draw the landing pad
   gout.drawRectangle(Position((double)iLZ, ground[iLZ]),
                      Position((double)(iLZ + runwayWidth), ground[iLZ] - 2.0),
                      255.0 /*red*/, 255.0 /*green*/, 0.0 /*blue*/);

}

Position Ground::getPlatformCenter() const
{
   Position center;
   center.setPixelsX((int)(iLZ + PLANE_WIDTH / 2.0));
   center.setPixelsY(0); // we'll set Y properly later
   return center;
}
