
#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>


Actor::Actor(int imageID, double x, double y, int direction, int depth, StudentWorld* studentWorld, bool hitable):
GraphObject(imageID, x, y, direction)
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

void Actor::changeHealth(int health)
{
    if(m_health + health >= 100)
    {
        m_health = 100;
    }
    else if(m_health + health <= 0)
    {
        setAlive(false);
    }
    else
    {
        m_health += health;
    }
}

int Actor::getHealth() const
{
    return m_health;
}

 void Actor::setHealth(int health)
{
    m_health = health;
}


Socrates::Socrates(StudentWorld* studentWorld): Actor(IID_PLAYER, 0, 128, 0, 0, studentWorld, true)
{
    flameCharges = 5;
    sprayCharges = 20;
    setHealth(100);
    positionalAngle = 180;
}

Socrates:: ~Socrates()
{
}

bool Socrates::hasHP()
{
    return true;
}

bool Socrates::canOverlap()
{
    return true;
}

int Socrates::getSprays() const
{
    return sprayCharges;
}

int Socrates::getFlames() const
{
    return flameCharges;
}

void Socrates::changeSprayCharge(int num)
{
    sprayCharges += num;
}
void Socrates::changeFlameCharge(int num)
{
    flameCharges += num;
}



void Socrates::doSomething()
{
    if(!isAlive())
    {
        return;
    }
    
    int ch;
    bool wait = false;
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
                    break;
                    
                case KEY_PRESS_SPACE:
                {
                    if(getSprays()==0)
                    {
                        return;
                    }
                    double dx;
                    double dy;
                    getPositionInThisDirection(getDirection(), 2*SPRITE_RADIUS ,dx,dy);
                    getWorld()->addNewActor(new Spray(getWorld(), dx, dy, getDirection()));
                    
                    changeSprayCharge(-1);
                    getWorld()->playSound(SOUND_PLAYER_SPRAY);
                    wait = !wait;

                }
                    break;
                    
                case KEY_PRESS_ENTER:
                  {
                      
                      if(getFlames()==0)
                        {
                            return;
                        }
                      
                      for(int i = 0; i < 16; i++)
                      {
                          double dx;
                          double dy;
                          getPositionInThisDirection(getDirection() + i*22, 2*SPRITE_RADIUS,dx,dy);
                          
                      getWorld()->addNewActor(new FlameThrower(getWorld(), dx , dy, getDirection()+ i*22));
                      }
                      changeFlameCharge(-1);
                      getWorld()->playSound(SOUND_PLAYER_FIRE);

                  }
                    break;
                    
                default:
                {
                    break;
                }
            }
   }
    if(wait)
         {
             wait = !wait;
         }
         
         else
         {
             if(getSprays()<20)
             {
                 changeSprayCharge(1);
             }
         }
}

Stationary::~Stationary()
{
}

bool Stationary::hasHP()
{
    return false;
}


Stationary::Stationary(int imageID, StudentWorld* studentWorld, double x, double y, bool hitable, int direction): Actor(imageID, x, y, direction, 1, studentWorld, hitable)
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
Dirt::Dirt(StudentWorld* studentWorld, double x, double y): Stationary(IID_DIRT, studentWorld, x, y, true, 0)
{
}

bool Dirt::canOverlap()
{
    return true;
}


Food::~Food()
{

}
Food::Food(StudentWorld* studentWorld, double x, double y): Stationary(IID_FOOD, studentWorld, x, y, false, 90)
{
}

bool Food::canOverlap()
{
    return false;
}

Pit::Pit(StudentWorld* studentWorld, double x, double y): Actor(IID_PIT, x, y, 0, 1, studentWorld, false)
{
    m_salmonella = 5;
    m_aggro_salmonella = 3;
    m_e_coli = 2;

}

bool Pit::canOverlap()
{
    return false;
}

bool Pit::hasHP()
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
        //getWorld()->addBacteria(10);
        setAlive(false);
    }
    
   int x = randInt(1, 50); //randomly spawns a bacterium; 1/50 chance per tick
    if(x == 25)
    {
        int a = 0;  //checks which types of bacteria are available to spawn and randomly does so among the possible ones to spawn
        int b = 0;
        int c = 0;

        if(m_e_coli > 0)
        {
            a++;
        }
        if(m_salmonella > 0)
       {
           b++;
       }
        if(m_aggro_salmonella > 0)
          {
              c++;
          }
        if(a >0 && b > 0 && c > 0)
        {
            if(randInt(1, 3) == 1) //randomly spawns a bacterium among those that exist
            {
                //TODO: getWorld()->addNewActor(ECOLI)
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
                decrECOLI();
            }
            
            else if(randInt(1, 3) == 2)
                 {
                     getWorld()->addNewActor(new Salmonella(getWorld(), getX(), getY()));
                     getWorld()->playSound(SOUND_BACTERIUM_BORN);
                     decrSalmonella();
                 }
            
            else if(randInt(1, 3) == 3)
                 {
                     getWorld()->addNewActor(new AggroSalmonella(getWorld(), getX(),getY()));
                     getWorld()->playSound(SOUND_BACTERIUM_BORN);
                     decrAggroSalmonella();
                 }
        }
        
        else if(a > 0 && b > 0)
        {
            if(randInt(1, 2) == 1) //randomly spawns a bacterium among those that exist
            {
                //TODO: getWorld()->addNewActor(ECOLI)
                getWorld()->playSound(SOUND_BACTERIUM_BORN);
                decrECOLI();
            }
            
            else if(randInt(1, 2) == 2)
                 {
                     getWorld()->addNewActor(new Salmonella(getWorld(), getX(), getY()));
                     getWorld()->playSound(SOUND_BACTERIUM_BORN);
                     decrSalmonella();
                 }
        }
            
        else if(a>0 && c > 0)
        {
            if(randInt(1, 2) == 1) //randomly spawns a bacterium among those that exist
               {
                   //TODO: getWorld()->addNewActor(ECOLI)
                   getWorld()->playSound(SOUND_BACTERIUM_BORN);
                   decrECOLI();
               }
               
               else if(randInt(1, 2) == 2)
                    {
                        getWorld()->addNewActor(new AggroSalmonella(getWorld(), getX(),getY()));
                        getWorld()->playSound(SOUND_BACTERIUM_BORN);
                        decrAggroSalmonella();
                    }
        }
            
        else if(b>0 && c > 0)
        {
            if(randInt(1, 2) == 1) //randomly spawns a bacterium among those that exist
                       {
                            getWorld()->addNewActor(new Salmonella(getWorld(), getX(), getY()));
                            getWorld()->playSound(SOUND_BACTERIUM_BORN);
                            decrSalmonella();
                       }
                       
           else if(randInt(1, 2) == 2)
                {
                    getWorld()->addNewActor(new AggroSalmonella(getWorld(), getX(),getY()));
                    getWorld()->playSound(SOUND_BACTERIUM_BORN);
                    decrAggroSalmonella();
                }
        }
            
        else if(a>0)
        {
        //TODO: getWorld()->addNewActor(ECOLI)
             getWorld()->playSound(SOUND_BACTERIUM_BORN);
             decrECOLI();
        }
            
        else if(b>0)
        {
            getWorld()->addNewActor(new Salmonella(getWorld(), getX(), getY()));
           getWorld()->playSound(SOUND_BACTERIUM_BORN);
           decrSalmonella();
        }
                    
        else if(c>0)
        {
            getWorld()->addNewActor(new AggroSalmonella(getWorld(), getX(),getY()));
           getWorld()->playSound(SOUND_BACTERIUM_BORN);
           decrAggroSalmonella();
        }

    }

}

Goodie::~Goodie()
{
    
}

bool Goodie::hasHP()
{
    return false;
}


Goodie::Goodie(StudentWorld* studentWorld, double x, double y, int ID): Actor(ID, x , y , 0 , 1 , studentWorld, true)
{
    lifetime = max(rand()%(300 - (10 * studentWorld->getLevel())), 50);
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
    
    if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
    {
        getWorld()->increaseScore(-50);
        setAlive(false);
        getWorld()->damageSocrates(20);
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

Fungus::Fungus(StudentWorld* studentWorld, double x, double y): Goodie(studentWorld, x, y, IID_FUNGUS)
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
    
    if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
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

RestoreHealthItem::RestoreHealthItem(StudentWorld* studentWorld, double x, double y):Goodie(studentWorld, x, y, IID_RESTORE_HEALTH_GOODIE)
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
    
    if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
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

AddLifeItem::AddLifeItem(StudentWorld* studentWorld, double x, double y):Goodie(studentWorld, x, y, IID_EXTRA_LIFE_GOODIE)
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
    
    if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
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

AddFlameThrowerItem::AddFlameThrowerItem(StudentWorld* studentWorld, double x, double y):Goodie(studentWorld, x, y, IID_FLAME_THROWER_GOODIE)
{

}



Projectile::Projectile(StudentWorld* studentWorld, double x, double y, int direction, int ID): Actor(ID, x, y, direction, 1 , studentWorld, false)
{
    distance_traveled = 0;
}

bool Projectile::hasHP()
{
    return false;
}

int Projectile::getDistanceTraveled() const
{
    return distance_traveled;
}

void Projectile::addDistanceTraveled()
{
    distance_traveled += 2*SPRITE_RADIUS;
}
bool Projectile::canOverlap()
{
    return true;
}

Projectile::~Projectile()
{
}

void Projectile::setMaxTravel(int x)
{
    max_travel_distance = x;
}

void Spray::doSomething()
{
    if(!isAlive())
          {
              return;
          }
    if(getWorld()->typeOfSprayHit(this) == 1)
    {
       setAlive(false);
       return;
    }
    
    else if(getWorld()->typeOfSprayHit(this) == 2)
    {
       setAlive(false);

       return;
    }
    
        moveAngle(getDirection(), 2*SPRITE_RADIUS);
    //std::cout<<getDirection()<<endl;
        addDistanceTraveled();
        if(getDistanceTraveled() > 112)
        {
            setAlive(false);
        }
}

Spray::~Spray()
{
    
}

Spray::Spray(StudentWorld* studentWorld, double x, double y, int direction): Projectile(studentWorld, x, y, direction, IID_SPRAY)
{
    setMaxTravel(112);
}


void FlameThrower::doSomething()
{
    if(!isAlive())
            {
                return;
            }
      
    if(getWorld()->typeOfFlameHit(this) == 1)
    {

       setAlive(false);
       return;
    }
    
    else if(getWorld()->typeOfFlameHit(this) == 2)
    {

       setAlive(false);
       return;
    }
         // moveAngle(getDirection(), 2*SPRITE_RADIUS);
    moveForward(2*SPRITE_RADIUS);
          addDistanceTraveled();
    
          if(getDistanceTraveled() > 32)
          {
              setAlive(false);
          }
    
}


FlameThrower::FlameThrower(StudentWorld* studentWorld, double x, double y, int direction): Projectile(studentWorld, x, y, direction, IID_FLAME)
{
    setMaxTravel(32);
}

FlameThrower::~FlameThrower()
{
}

bool Bacteria::hasHP()
{
    return true;
}

Bacteria::~Bacteria()
{
    
}

int Bacteria::getFoodEaten() const
{
    return food_eaten;
}

void Bacteria::setFoodEaten(int x)
{
    food_eaten = x;
}


Bacteria::Bacteria(StudentWorld* studentWorld, double x, double y, int ID):Actor(ID, x, y, 90, 0, studentWorld, true)
{
    movement_planDistance = 0;
    food_eaten = 0;
    setHealth(0);
}

bool Bacteria::canOverlap()
{
    return true;
}


int Bacteria::getMovementPlanDistance() const
{
    return movement_planDistance;
}


void Bacteria::setMovementPlanDistance(int x)
{
    movement_planDistance = x;
}


Salmonella::Salmonella(StudentWorld* studentWorld, double x, double y): Bacteria(studentWorld, x, y, IID_SALMONELLA)
{
    setHealth(4);
}


void Salmonella::doSomething()
{
    if(!isAlive())
     {
         return;
     }
     
    if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
    {
        getWorld()->damageSocrates(1);
    }
     
     else if(getFoodEaten()== 3)
     {
         double newX = getX();
         double newY = getY();
         if(getX() < VIEW_WIDTH/2)
         {
             newX += (SPRITE_WIDTH / 2);
         }
        
         else if(getX() > VIEW_WIDTH/2)
         {
             newX -= (SPRITE_WIDTH / 2);
         }
         
         if(getY() < VIEW_HEIGHT/2)
                {
                    newY += (SPRITE_WIDTH / 2);
                }
         
         else if(getY() > VIEW_HEIGHT/2)
              {
                  newY -= (SPRITE_WIDTH / 2);
              }
         
         getWorld()->addNewActor(new Salmonella(getWorld(), newX, newY));
         setFoodEaten(0);
     }
    
    else
    {
        if(getWorld()->overlapsFood(this))
        {
            setFoodEaten(getFoodEaten() + 1);
        }
    }
    
    if(getMovementPlanDistance() > 0)
    {
        setMovementPlanDistance(getMovementPlanDistance() - 1);
        
        if(!getWorld()->isMovementBlocked(this))
        {
            moveForward(3);
        }
        else
        {
            setDirection(randInt(0, 359));
            setMovementPlanDistance(10);
        }
        return;

    }
    
    else if(!getWorld()->isNearbyFood(this))
    {
        setDirection(randInt(0, 359));
        setMovementPlanDistance(10);
        return;
    }
    
    else if(getWorld()->isNearbyFood(this))
    {
       if(!getWorld()->isMovementBlocked(this))
        {
            moveForward(3);
        }
        else
        {
            setDirection(randInt(0, 359));
            setMovementPlanDistance(10);
        }
        return;
        
    }
}

Salmonella::~Salmonella()
{
}

AggroSalmonella::AggroSalmonella(StudentWorld* studentWorld, double x, double y): Bacteria(studentWorld, x, y, IID_SALMONELLA)
{
    setHealth(10);
}

AggroSalmonella::~AggroSalmonella()
{

}

void AggroSalmonella::doSomething()
{
    //step 1
    
    if(!isAlive())
       {
           return;
       }
    
    if(getWorld()->isNearbySocrates(this)) //step 2: does steps 3 - 5 after, then returns immediately
    {
//step 2 continued
        if(getWorld()->isMovementBlockedByCircle(this))
        {
            setDirection(randInt(0, 359));
            setMovementPlanDistance(10);
        }
        
        else if(!(getWorld()->isMovementBlockedByDirt(this)))
        {
            moveForward(3);
        }
        
        //step 3
        
        if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(),  getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
            {
                getWorld()->damageSocrates(2);
            }
        
        
        if(getFoodEaten()== 3) //step 4
        {
            double newX = getX();
            double newY = getY();
            if(getX() < VIEW_WIDTH/2)
            {
                newX += (SPRITE_WIDTH / 2);
            }
           
            else if(getX() > VIEW_WIDTH/2)
            {
                newX -= (SPRITE_WIDTH / 2);
            }
            
            if(getY() < VIEW_HEIGHT/2)
                   {
                       newY += (SPRITE_WIDTH / 2);
                   }
            
            else if(getY() > VIEW_HEIGHT/2)
                 {
                     newY -= (SPRITE_WIDTH / 2);
                 }
            
            getWorld()->addNewActor(new AggroSalmonella(getWorld(), newX, newY));
            setFoodEaten(0);
        }
        
        if(getWorld()->overlapsFood(this)) //step 5
              {
                  setFoodEaten(getFoodEaten() + 1);
              }
        
        return;
        
    }
     
    else //if step 2 failed, step 3
    {
        //step 3
        if(getWorld()->euclidean(getX(), getWorld()->getSocratesX(), getY(), getWorld()->getSocratesY()) <= 2 * SPRITE_RADIUS)
            {
                getWorld()->damageSocrates(2);
                
        //skips to step 6 if step 3 succeeds
              if(getMovementPlanDistance() > 0)
               {
                   setMovementPlanDistance(getMovementPlanDistance() - 1);
                   
                   if(!getWorld()->isMovementBlocked(this))
                   {
                       moveForward(3);
                   }
                   else
                   {
                       setDirection(randInt(0, 359));
                       setMovementPlanDistance(10);
                   }
                   return;

               }
               
               else if(!getWorld()->isNearbyFood(this))
               {
                   setDirection(randInt(0, 359));
                   setMovementPlanDistance(10);
                   return;
               }
               
               else if(getWorld()->isNearbyFood(this))
               {
                  if(!getWorld()->isMovementBlocked(this))
                   {
                       moveForward(3);
                   }
                   else
                   {
                       setDirection(randInt(0, 359));
                       setMovementPlanDistance(10);
                   }
                   return;
                   
               }
            }
        
        if(getFoodEaten()== 3) //step 4 if step 3 fails
               {
                   double newX = getX();
                   double newY = getY();
                   if(getX() < VIEW_WIDTH/2)
                   {
                       newX += (SPRITE_WIDTH / 2);
                   }
                  
                   else if(getX() > VIEW_WIDTH/2)
                   {
                       newX -= (SPRITE_WIDTH / 2);
                   }
                   
                   if(getY() < VIEW_HEIGHT/2)
                          {
                              newY += (SPRITE_WIDTH / 2);
                          }
                   
                   else if(getY() > VIEW_HEIGHT/2)
                        {
                            newY -= (SPRITE_WIDTH / 2);
                        }
                   
                   getWorld()->addNewActor(new AggroSalmonella(getWorld(), newX, newY));
                   setFoodEaten(0);
                 
                   //skips to step 6 if step 4 suceeds
                     if(getMovementPlanDistance() > 0)
                           {
                               setMovementPlanDistance(getMovementPlanDistance() - 1);
                               
                               if(!getWorld()->isMovementBlocked(this))
                               {
                                   moveForward(3);
                               }
                               else
                               {
                                   setDirection(randInt(0, 359));
                                   setMovementPlanDistance(10);
                               }
                               return;

                           }
                           
                           else if(!getWorld()->isNearbyFood(this))
                           {
                               setDirection(randInt(0, 359));
                               setMovementPlanDistance(10);
                               return;
                           }
                           
                           else if(getWorld()->isNearbyFood(this))
                           {
                              if(!getWorld()->isMovementBlocked(this))
                               {
                                   moveForward(3);
                               }
                               else
                               {
                                   setDirection(randInt(0, 359));
                                   setMovementPlanDistance(10);
                               }
                               return;
                               
                           }

                   
               }
               
        if(getWorld()->overlapsFood(this)) //step 5
             {
                 setFoodEaten(getFoodEaten() + 1);
             }
               
        if(getMovementPlanDistance() > 0) //step 6
          {
              setMovementPlanDistance(getMovementPlanDistance() - 1);
              
              if(!getWorld()->isMovementBlocked(this))
              {
                  moveForward(3);
              }
              else
              {
                  setDirection(randInt(0, 359));
                  setMovementPlanDistance(10);
              }
              return;

          }
      
      else if(!getWorld()->isNearbyFood(this)) //step  7
      {
          setDirection(randInt(0, 359));
          setMovementPlanDistance(10);
          return;
      }
      
      else if(getWorld()->isNearbyFood(this))
      {
         if(!getWorld()->isMovementBlocked(this))
          {
              moveForward(3);
          }
          else
          {
              setDirection(randInt(0, 359));
              setMovementPlanDistance(10);
          }
          return;
          
      }
        
    }

    
    
    
    
}




// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
