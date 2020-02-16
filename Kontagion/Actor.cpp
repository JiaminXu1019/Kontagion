
#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>



Actor::Actor(int imageID, double x, double y, int direction, int depth, StudentWorld* studentWorld, bool hitable):
GraphObject(imageID, x, y)
{
    m_studentWorld = studentWorld;
    m_alive = true;
    m_hitable = hitable;

}

bool Actor::isAlive() const
  {
      return m_alive;
  }

void Actor::setAlive(bool x)
{
    m_alive = x;
}

bool Actor::isHitable() const
{
    return m_hitable;
}

void Actor::changeAliveStatus()
{
    m_alive = false;
}

StudentWorld* Actor::getWorld() const
{
    return m_studentWorld;
}

Actor::~Actor()
{
}

Socrates::Socrates(StudentWorld* studentWorld): Actor(IID_PLAYER, 0, 128, 0, 0, studentWorld, true)
{
    flameCharges = 5;
    sprayCharges = 20;
    m_health = 100;
    positionalAngle = 180;
}

Socrates:: ~Socrates()
{
}

void Socrates::changeSprayCharge(int num)
{
    sprayCharges += num;
}
void Socrates::changeFlameCharge(int num)
{
    flameCharges += num;
}
void Socrates::changeHealth(int health)
{
    m_health += health;
}
 void Socrates::setHealth(int health)
{
    m_health = health;
}

void Socrates::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    int ch;
    if (getWorld()->getKey(ch))
    {
        double PI = 4 * atan(1);

        switch (ch)
            {
                case KEY_PRESS_LEFT: //counterclockwise
                    {
                        positionalAngle += 5;
                        moveTo(128 + 128 * cos(positionalAngle * PI/180), 128 + 128 * sin(positionalAngle * PI/180));
                        setDirection(positionalAngle + 180);
                    }
                break;
                case KEY_PRESS_RIGHT: //clockwise
                    {
                        positionalAngle -= 5;
                        moveTo(128 + 128 * cos(positionalAngle * PI/180), 128 + 128 * sin(positionalAngle * PI/180));
                        setDirection(positionalAngle + 180);
                        
                    }
                default:
                    break;
            
            }
   }
}

Useless::~Useless()
{
}

Useless::Useless(int imageID, StudentWorld* studentWorld, int x, int y, bool hitable): Actor(imageID, x, y, 90, 1, studentWorld, hitable)
{
}

void Useless::doSomething()
{
    
}

Dirt::~Dirt()
{
    
}
Dirt::Dirt(StudentWorld* studentWorld, int x, int y): Useless(IID_DIRT, studentWorld, x, y, true)
{
}

Food::~Food()
{
}
Food::Food(StudentWorld* studentWorld, int x, int y): Useless(IID_FOOD, studentWorld, x, y, false)
{
}






// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
