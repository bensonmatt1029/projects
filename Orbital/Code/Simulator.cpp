/***********************************************************************
 * Source File:
 *    Simulator : A class to run the orbit simulator
 * Author:
 *    Matt Benson
 * Summary:
 *    Provides a convenient interface to run the orbital simulator
 ************************************************************************/

#include "simulator.h"     // for SIMULATOR

 /***********************************************************************
  * CONSTRUCTOR
  * Initializes all the member variables of the orbital
  * simulator: Stars, Satellites, ptUpperRight
  ************************************************************************/
Simulator::Simulator(Position ptUpperRight)
{
   double frameRate = 30.0;
   double hoursPerDay = 24.0;
   double minutesPerHour = 60.0;
   double secondsPerMinute = 60.0;
   double secondsPerDay = hoursPerDay * minutesPerHour * secondsPerMinute;
   double timeDilation = hoursPerDay * minutesPerHour;

   // initialize the stars
   for (int i = 0; i < 200; i++)
   {
      stars[i].reset(ptUpperRight.getPixelsX(), ptUpperRight.getPixelsX());
   }

   // ship is in the upper right corner
   satellites.push_back(new Ship);

   // rotate the earth
   double radiansInADay = -3.14159 * 2.0;
   double radiansPerFrame = (radiansInADay / frameRate) * (timeDilation / secondsPerDay);
   angleEarth += radiansPerFrame;

   // satellites
   for (int i = 0; i < 6; i++)
      satellites.push_back(new GPS(i));
   satellites.push_back(new Sputnik());
   satellites.push_back(new Hubble());
   satellites.push_back(new Dragon());
   satellites.push_back(new Starlink());
   return;
}

/*************************************************************************
 * DESTRUCTOR
 * 
 *************************************************************************/
Simulator::~Simulator()
{
   for (list <Satellite*> ::iterator it = satellites.begin();
      it != satellites.end();
      ++it)
      delete * it;
}

/*************************************************************************
 * INPUT
 * Handles all the input of the simulator
 *************************************************************************/
void Simulator::input(const Interface& pUI)
{
   for (list <Satellite*> ::iterator it = satellites.begin();
      it != satellites.end();
      ++it)
      (*it)->input(pUI, satellites);
}

/*************************************************************************
 * Move
 * Moves all the satellites of the simulator
 *************************************************************************/
void Simulator::move()
{
   double frameRate = 30.0;
   double hoursPerDay = 24.0;
   double minutesPerHour = 60.0;
   double secondsPerMinute = 60.0;
   double secondsPerDay = hoursPerDay * minutesPerHour * secondsPerMinute;
   double timeDilation = hoursPerDay * minutesPerHour;
   double timeStep = timeDilation / frameRate;

   list <Satellite*> ::iterator it1;
   list <Satellite*> ::iterator it2;

   // advance everything
   for (auto satellite : satellites)
      satellite->move(timeStep);

   // look for collisions
   for (it1 = satellites.begin(); it1 != satellites.end(); ++it1)
      for ((it2 = it1)++; it2 != satellites.end(); ++it2)

         // are we alive and well?
         if (!(*it1)->isDead() && !(*it2)->isDead() &&
            !(*it1)->isInvisible() && !(*it2)->isInvisible())
         {
            // we should never compare the same satellite!
            assert(it1 != it2);
            double satelliteDistance = computeDistance((*it1)->getPosition(), 
                                                      ( *it2)->getPosition());
            
            // kill the satellite(s) if they collide
            if (satelliteDistance < (*it1)->getRadius() + (*it2)->getRadius())
            {
               (*it1)->kill();
               (*it2)->kill();
            }
         }

   // destroy anything marked as dead
   for (it1 = satellites.begin(); it1 != satellites.end();)
      if ((*it1)->isDead())
      {
         (*it1)->destroy(satellites);
         it1 = satellites.erase(it1);
      }
      else
         ++it1;
}

/*************************************************************************
 * DRAW
 * Draws all the satellites in the simulator to the screen
 *************************************************************************/
void Simulator::draw(ogstream& gout)
{
   
   // first draw the stars
   for (auto& star : stars)
      star.draw(gout);

   // then the satellites
   list <Satellite*> ::iterator it;
   for (it = satellites.begin(); it != satellites.end(); ++it)
      (*it)->draw(gout);

   // then the earth
   gout.drawEarth(ptEarth, angleEarth);
}

/*************************************************************************
 * CALLBACK
 * Handles the game logic
 *************************************************************************/
void callBack(const Interface* pUI, void* p)
{
   Simulator* pSim = (Simulator*)p;
   ogstream gout;
   
   // move the ship
   pSim->input(*pUI);

   // update the satellites
   pSim->move();

   // draw everything
   pSim->draw(gout);
}

double Position::metersFromPixels = 40.0;

/*********************************
 * Initialize the simulation and set it in motion
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   // Test cases
   testRunner();

   // Initialize OpenGL
   Position ptUpperRight;
   ptUpperRight.setZoom(128000.0 /* 128km equals 1 pixel */);
   ptUpperRight.setPixelsX(1000.0);
   ptUpperRight.setPixelsY(1000.0);
   Interface ui(0, NULL,
      "Demo",   /* name on the window */
      ptUpperRight);

   // Initialize the demo
   Simulator sim(ptUpperRight);

   // set everything into action
   ui.run(callBack, &sim);

   return 0;
}
