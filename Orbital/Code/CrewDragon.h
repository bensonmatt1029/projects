/***********************************************************************
 * Header File:
 *    Crew Dragon
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about the crew dragon
 ************************************************************************/

#pragma once

#include "satellite.h"

 /***********************************************************************
  * DRAGON Left
  * Part of the crew Dragon
  ************************************************************************/
class DragonLeft : public Part
{
public:
   DragonLeft(const Satellite& parent, const Angle& dirKick) : Part(parent, dirKick)
   {
      this->radius = 6.0 * this->pos.getZoom();
   }
   virtual void draw(ogstream& gout)
   {
      gout.drawCrewDragonLeft(this->pos, this->angle.getRadians(), Position());
   }
   virtual void destroy(std::list <Satellite*>& satellites)
   {
      satellites.push_back(new Fragment(*this, Angle(0.0)));
      satellites.push_back(new Fragment(*this, Angle(180.0)));
   }
};

/***********************************************************************
 * DRAGON Center
 * Part of the crew Dragon
 ************************************************************************/
class DragonCenter : public Part
{
public:
   DragonCenter(const Satellite& parent, const Angle& dirKick) : Part(parent, dirKick)
   {
      this->radius = 6.0 * this->pos.getZoom();
   }
   virtual void draw(ogstream& gout)
   {
      gout.drawCrewDragonCenter(this->pos, this->angle.getRadians());
   }
   virtual void destroy(std::list <Satellite*>& satellites)
   {
      for (double degrees = 0.0; degrees <= 360.0; degrees += 90.0)
         satellites.push_back(new Fragment(*this, Angle(degrees)));
   }
};

/***********************************************************************
 * DRAGON Right
 * Part of the crew Dragon
 ************************************************************************/
class DragonRight : public Part
{
public:
   DragonRight(const Satellite& parent, const Angle& dirKick) : Part(parent, dirKick)
   {
      this->radius = 6.0 * this->pos.getZoom();
   }
   virtual void draw(ogstream& gout)
   {
      gout.drawCrewDragonCenter(this->pos, this->angle.getRadians());
   }
   virtual void destroy(std::list <Satellite*>& satellites)
   {
      satellites.push_back(new Fragment(*this, Angle(90.0)));
      satellites.push_back(new Fragment(*this, Angle(270.0)));
   }
};

/***********************************************************************
 * DRAGON
 * The Space-X crew Dragon
 ************************************************************************/
class Dragon : public Whole
{
public:
   Dragon() : Whole(4000)
   {
      this->pos = Position(0.0, 8000000.0);
      this->velocity.setDX(-7900.0);
      this->velocity.setDY(0.0);
      this->angularVelocity = 0.0;
      this->radius = 7.0 * this->pos.getZoom();
   }
   virtual void draw(ogstream& gout)
   {
      gout.drawCrewDragon(this->pos, this->angle.getRadians());
   }
   virtual void destroy(std::list <Satellite*>& satellites)
   {
      satellites.push_back(new DragonCenter(*this, Angle(90.0)));
      satellites.push_back(new DragonLeft(*this, Angle(   0.0)));
      satellites.push_back(new DragonRight(*this, Angle(180.0)));
      satellites.push_back(new Fragment(*this, Angle(   330.0)));
      satellites.push_back(new Fragment(*this, Angle(   250.0)));
   }
};
