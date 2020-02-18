
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

bool Socrates::canOverlap()
{
    return true;
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

int Socrates::getHealth() const
{
    return m_health;
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
                case KEY_PRESS_LEFT: //counterclockwise movement, changes the positional angle and the corresponding x,y, coordinates and dir.
                    {
                        positionalAngle += 5;
                        moveTo(128 + 128 * cos(positionalAngle * PI/180), 128 + 128 * sin(positionalAngle * PI/180));
                        setDirection(positionalAngle + 180);
                    }
                break;
                case KEY_PRESS_RIGHT: //clockwise movement, changes the positional angle and the corresponding x,y, coordinates and dir.
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

Stationary::~Stationary()
{
}

Stationary::Stationary(int imageID, StudentWorld* studentWorld, int x, int y, bool hitable): Actor(imageID, x, y, 90, 1, studentWorld, hitable)
{
}

void Stationary::doSomething()
{
    
}
bool Stationary::canOverlap()
{
    return false;
}

Dirt::~Dirt()
{
    
}
Dirt::Dirt(StudentWorld* studentWorld, int x, int y): Stationary(IID_DIRT, studentWorld, x, y, true)
{
}

bool Dirt::canOverlap()
{
    return true;
}


Food::~Food()
{

}
Food::Food(StudentWorld* studentWorld, int x, int y): Stationary(IID_FOOD, studentWorld, x, y, false)
{
}

bool Food::canOverlap()
{
    return false;
}

Pit::Pit(StudentWorld* studentWorld, int x, int y): Actor(IID_PIT, x, y, 0, 1, studentWorld, false)
{
    m_salmonella = 5;
    m_aggro_salmonella = 3;
    m_e_coli = 2;

}

bool Pit::canOverlap()
{
    return false;
}

Pit::~Pit()
{
}

void Pit::decrSalmonella()
{
    m_salmonella--;
}

void Pit::decrAggroSalmonella()
{
    m_aggro_salmonella--;
}
void Pit::decrECOLI()
{
    m_e_coli--;
}

bool Pit::isEmpty() const
{
    if(m_e_coli + m_aggro_salmonella + m_salmonella == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Pit::doSomething()
{
    if(isEmpty())
    {
        setAlive(false);
    }
    
   int x = randInt(1, 50); //randomly spawns a bacterium; 1/50 chance per tick
    if(x == 25)
    {
        int y = 0;
        if(m_e_coli > 0)
        {
            y++;
        }
        if(m_aggro_salmonella > 0)
          {
              y++;
          }
        if(m_salmonella > 0)
          {
              y++;
          }
        
        if(randInt(1, y) == 1) //randomly spawns a bacterium among those that exist
        {
            //TODO: getWorld()->addNewActor(ECOLI)
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
            decrECOLI();
        }
        else if(randInt(1, y) == 2)
             {
                //TODO: getWorld()->addNewActor()
                 getWorld()->playSound(SOUND_BACTERIUM_BORN);
                 decrSalmonella();
             }
        else if(randInt(1, y) == 3)
             {
                 //TODO: getWorld()->addNewActor()
                 getWorld()->playSound(SOUND_BACTERIUM_BORN);
                 decrAggroSalmonella();
             }
    }

}

Goodie::~Goodie()
{
    
}

Goodie::Goodie(StudentWorld* studentWorld, int x, int y, int ID): Actor(ID, x , y , 0 , 1 , studentWorld, true)
{
    lifetime = max(rand() % (300 - (10 * studentWorld->getLevel())), 50);
}


bool Goodie::canOverlap()
{
    return true;
}

void Goodie::decrLifetime()
{
    lifetime--;
    
}
int Goodie::getLifetime() const
{
    return lifetime;
}


void Fungus::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    if(getX() == getWorld()->getSocratesX() && getY() == getWorld()->getSocratesY())
    {
        getWorld()->increaseScore(-50);
        setAlive(false);
        getWorld()->damageSocrates(-20);
        return;
    }
    
    decrLifetime();
    
    if(getLifetime() == 0)
    {
        setAlive(false);
    }
    
}
    
Fungus::~Fungus()
{
    
}

Fungus::Fungus(StudentWorld* studentWorld, int x, int y): Goodie(studentWorld, x, y, IID_FUNGUS)
{
    
}

RestoreHealthItem::~RestoreHealthItem()
{
}

void RestoreHealthItem::doSomething()
{
    if(!isAlive())
       {
           return;
       }
    
    if(getX() == getWorld()->getSocratesX() && getY() == getWorld()->getSocratesY())
    {
        getWorld()->increaseScore(250);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->restoreHealth();
        return;
    }
    
    decrLifetime();
    
    if(getLifetime() == 0)
    {
        setAlive(false);
    }
}

RestoreHealthItem::RestoreHealthItem(StudentWorld* studentWorld, int x, int y):Goodie(studentWorld, x, y, IID_RESTORE_HEALTH_GOODIE)
{

}


AddLifeItem::~AddLifeItem()
{
}

void AddLifeItem::doSomething()
{
    if(!isAlive())
       {
           return;
       }
    
    if(getX() == getWorld()->getSocratesX() && getY() == getWorld()->getSocratesY())
    {
        getWorld()->increaseScore(500);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
        return;
    }
    
    decrLifetime();
    
    if(getLifetime() == 0)
    {
        setAlive(false);
    }
}

AddLifeItem::AddLifeItem(StudentWorld* studentWorld, int x, int y):Goodie(studentWorld, x, y, IID_EXTRA_LIFE_GOODIE)
{
}



AddFlameThrowerItem::~AddFlameThrowerItem()
{
}

void AddFlameThrowerItem::doSomething()
{
    if(!isAlive())
       {
           return;
       }
    
    if(getX() == getWorld()->getSocratesX() && getY() == getWorld()->getSocratesY())
    {
        getWorld()->increaseScore(300);
        setAlive(false);
        getWorld()->addFlames();
        getWorld()->incLives();
        return;
    }
    
    decrLifetime();
    
    if(getLifetime() == 0)
    {
        setAlive(false);
    }
}

AddFlameThrowerItem::AddFlameThrowerItem(StudentWorld* studentWorld, int x, int y):Goodie(studentWorld, x, y, IID_FLAME_THROWER_GOODIE)
{

}





// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
