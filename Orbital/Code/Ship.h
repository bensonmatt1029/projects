/***********************************************************************
 * Header File:
 *    Ship
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about the Ship
 ************************************************************************/

#pragma once

#include "satellite.h"

 /**************************************************
  * Ship
  * The whole Ship
  ***************************************************/
class Ship : public Whole
{
public:
   Ship();

   const Position& getPos() const { return pos; }
   const Velocity& getVel() const { return velocity; }

   double getAngle() const { return angle.getRadians(); }

   void input(const Interface& ui, std::list <Satellite*>& satellites);

   void draw(ogstream& gout)
   {
      gout.drawShip(this->pos, this->angle.getRadians(), thrust);
   }

   void destroy(std::list <Satellite*>& satellites);

   bool isThrust() const { return thrust; }

   void move(double timeDilation, const Interface& ui, std::list<Satellite*>& satellites);

private:
   bool thrust;
};
