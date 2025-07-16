/***********************************************************************
 * Header File:
 * ACCELERATION
 * Author:
 * Matt Benson
 * Summary:
 * Everything we need to know about acceleration
 ************************************************************************/


#pragma once

class TestAcceleration;
class TestVelocity;
class TestPosition;
class Angle;
class Velocity;


/*********************************************
 * Acceleration
 * Let's get moving
 *********************************************/
class Acceleration
{
   friend TestPosition;
   friend TestVelocity;
   friend TestAcceleration;
   friend Velocity;

public:
   // constructors
   Acceleration() : ddx(0.0), ddy(0.0) {}
   Acceleration(double ddx, double ddy) : ddx(ddx), ddy(ddy) {}
   Acceleration(double acc, Angle& angle) : ddx(0.0), ddy(0.0)
   {
      setAccelerationDirection(acc, angle);
   }

   // getters
   double getDDX()   const { return ddx; }
   double getDDY()   const { return ddy; }

   // setters                        
   void setDDX(double ddx) { this->ddx = ddx; }
   void setDDY(double ddy) { this->ddy = ddy; }
   void set(const Angle& a, double magnitude);
   void addDDX(double ddx) { this->ddx += ddx; }
   void addDDY(double ddy) { this->ddy += ddy; }
   void add(const Acceleration& rhs);
   void setAccelerationDirection(double acc, const Angle& angle);

private:
   double ddx;     // horizontal acceleration
   double ddy;     // vertical acceleration
};
