
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;
#include "Actor.h"
#include <iostream> // defines the overloads of the << operator
#include <sstream>
#include <cmath>
#include <math.h>
#include <stdio.h> // defines the type std::ostringstream
#include <iomanip>


GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_actors = 0;
    m_bacteria = 0;
    bacteriaKilled = 0;

}

int StudentWorld::init()
{
    
    for(int i = 0; i < getLevel(); i++) //RANDOMLY generates coordinates for pit objects such that they are no more than 120 pixels away from the center
       {
           int x = randInt(-120, 120);
           int y = randInt(-120, 120);
           double z = sqrt(pow(x, 2) + pow(y,2));
           
           x += VIEW_WIDTH/2;
           y += VIEW_HEIGHT/2;
       
           if(m_actors == 0) //if this is the first pit generated dont bother checking for overlap
           {
            
               while(z > 120)
                  {
                      x = randInt(-120, 120);
                      y = randInt(-120, 120);
                      z = sqrt(pow(x, 2) + pow(y,2));
                      x += VIEW_WIDTH/2;
                      y += VIEW_HEIGHT/2;
                      
                  }
           }
           
           else{
               vector<Actor*>::iterator it;
                it = actors.begin();
                while (it != actors.end()) //iterates through all the past pit objects to see if these coordinates overlap
                {
                    if(euclidean(x, (*it)->getX(), y, (*it)->getY()) <= 2*SPRITE_RADIUS || z > 120)
                    {
                        x = randInt(-120, 120);
                        y = randInt(-120, 120);
                        z = sqrt(pow(x, 2) + pow(y,2));
                        x += VIEW_WIDTH/2;
                        y += VIEW_HEIGHT/2;
                        
                        it = actors.begin();
                    }
                    else
                    {
                        it++;
                    }
                }
             }
              
           addNewActor(new Pit(this, x, y));
       }
    
    for(int i = 0; i < min(5* getLevel(), 25); i++) //RANDOMLY generates coordinates for the food objects such that they are no more than 120 pixels away from the center
      {
          int x = randInt(-120, 120);
          int y = randInt(-120, 120);
          double z = sqrt(pow(x, 2) + pow(y,2));
          
          x += VIEW_WIDTH/2;
          y += VIEW_HEIGHT/2;
      
              vector<Actor*>::iterator it;
                  it = actors.begin();
                  while (it != actors.end()) //iterates through all the past pit and food objects to see if these coordinates overlap
                  {
                      if(euclidean(x, (*it)->getX(), y, (*it)->getY()) <= 2*SPRITE_RADIUS || z > 120)
                      {
                          x = randInt(-120, 120);
                          y = randInt(-120, 120);
                          z = sqrt(pow(x, 2) + pow(y,2));
                          x += VIEW_WIDTH/2;
                          y += VIEW_HEIGHT/2;
                          
                          it = actors.begin();

                      }
                      else
                      {
                          it++;
                      }
                  }
          addNewActor(new Food(this, x, y));
      }
    
    for(int i = 0; i < max(180 - (getLevel() * 20), 20); i++) //RANDOMLY generates coordinates for the dirt objects such that they are no more than 120 pixels away from the center
    {
        int x = randInt(-120, 120);
        int y = randInt(-120, 120);
        double z = sqrt(pow(x, 2) + pow(y,2));
        
        x += VIEW_WIDTH/2;
        y += VIEW_HEIGHT/2;
    
            vector<Actor*>::iterator it;
                it = actors.begin();
                while (it != actors.end()) //iterates through all the past food and pit objects to see if these coordinates overlap
                { //ensures these are excluding dirt objects
                    if( (!(*it)->canOverlap()) && (euclidean(x, (*it)->getX(), y, (*it)->getY()) <= 2*SPRITE_RADIUS || z > 120))
                    {
                        x = randInt(-120, 120);
                        y = randInt(-120, 120);
                        z = sqrt(pow(x, 2) + pow(y,2));
                        x += VIEW_WIDTH/2;
                        y += VIEW_HEIGHT/2;
                        
                        it = actors.begin();
                    
                    }
                    else
                    {
                        it++;
                    }
                }
        addNewActor(new Dirt(this, x, y));
    }
    
    m_Socrates = new Socrates(this);

    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_Socrates->doSomething(); //socrates does something
    
    vector<Actor*>::iterator it; //tells every actor to do something
    it = actors.begin();
     while(it!= actors.end())
     {
         (*it) ->doSomething();
         it++;
     }
    
    if (!(m_Socrates->isAlive())) //is socrates alive
    {
          decLives();
            playSound(SOUND_PLAYER_DIE);
          return GWSTATUS_PLAYER_DIED;
    }
    
    
      if (m_bacteria == bacteriaKilled) //has the level been complete
      {
          playSound(SOUND_FINISHED_LEVEL);
          return GWSTATUS_FINISHED_LEVEL;
      }
     
    
    vector<Actor*>::iterator it1; //deletes memory of dead actors
    it1 = actors.begin();
    while (it1 != actors.end())
    {
        if ( ! ((*it1)->isAlive()) )
        {
            delete *it1;
            it1 = actors.erase(it1);
            m_actors--;
        }
        
        else
        {
            it1++;
        }
    }
    
    //possibly add Fungus
    int chanceFungus = max(510-(getLevel() * 10), 200);
    int random = randInt(0, chanceFungus - 1);
    if(random == 0)
    {
        double x = randInt(-VIEW_RADIUS, VIEW_RADIUS);
        double y = sqrt(pow(VIEW_RADIUS,2) - pow(x, 2));
        x += VIEW_WIDTH/2;
        y += VIEW_HEIGHT/2;
        
      vector<Actor*>::iterator it2;
      it2 = actors.begin();
      while (it2 != actors.end())
      { //
          if( (*it2)->getX() == x && y == (*it2)->getY())
          {
              double x = randInt(-VIEW_RADIUS, VIEW_RADIUS);
             double y = sqrt(pow(VIEW_RADIUS,2) - pow(x, 2));
             x += VIEW_WIDTH/2;
             y += VIEW_HEIGHT/2;
              
              it2 = actors.begin();
          
          }
          else
          {
              it2++;
          }
      }
        addNewActor(new Fungus(this, x, y));
    }
    
    //possibly add Goodies
    int chanceGoodie = max(510-(getLevel() * 10), 250);
      int random2 = randInt(0, chanceGoodie - 1);
      if(random2 == 0)
      {
          double x2 = randInt(-VIEW_RADIUS, VIEW_RADIUS);
          double y2 = sqrt(pow(VIEW_RADIUS,2) - pow(x2, 2));
          x2 += VIEW_WIDTH/2;
          y2 += VIEW_HEIGHT/2;
          int z = randInt(1,10);
          {
              if(z <= 6) //60% for a restorehealth
              {
                  vector<Actor*>::iterator it3; //ensures no fungus spawns on another fungus
                  it3 = actors.begin();
                  while (it3 != actors.end())
                  { //
                      if( (*it3)->getX() == x2 && y2 == (*it3)->getY())
                      {
                          double x2 = randInt(-VIEW_RADIUS, VIEW_RADIUS);
                         double y2 = sqrt(pow(VIEW_RADIUS,2) - pow(x2, 2));
                         x2 += VIEW_WIDTH/2;
                         y2 += VIEW_HEIGHT/2;
                          it3 = actors.begin();
                      
                      }
                      else
                      {
                          it3++;
                      }
                  }
                  addNewActor(new RestoreHealthItem(this, x2, y2));

              }
              
              else if(z > 6 && z <= 9) //30% chance for an addflames
              {
                  vector<Actor*>::iterator it3;
                  it3 = actors.begin();
                  while (it3 != actors.end())
                  { //
                      if( (*it3)->getX() == x2 && y2 == (*it3)->getY())
                      {
                          double x2 = randInt(-VIEW_RADIUS, VIEW_RADIUS);
                         double y2 = sqrt(pow(VIEW_RADIUS,2) - pow(x2, 2));
                         x2 += VIEW_WIDTH/2;
                         y2 += VIEW_HEIGHT/2;
                          
                          it3 = actors.begin();
                      
                      }
                      else
                      {
                          it3++;
                      }
                  }
                  addNewActor(new AddFlameThrowerItem(this, x2, y2));

              }
              
              else //10% chance for an add life item
              {
                  vector<Actor*>::iterator it3;
                  it3 = actors.begin();
                  while (it3 != actors.end())
                  { //
                      if( (*it3)->getX() == x2 && y2 == (*it3)->getY())
                      {
                          double x2 = randInt(-VIEW_RADIUS, VIEW_RADIUS);
                         double y2 = sqrt(pow(VIEW_RADIUS,2) - pow(x2, 2));
                         x2 += VIEW_WIDTH/2;
                         y2 += VIEW_HEIGHT/2;
                          
                          it3 = actors.begin();
                      
                      }
                      else
                      {
                          it3++;
                      }
                  }
                    addNewActor(new AddLifeItem(this, x2, y2));
              }
                
          }
      }
    //updates the header stats of the game
      ostringstream oss;
      oss.fill('0');
      oss.setf(ios::fixed);
      oss.precision(0);
      if(getScore() >= 0 )
      {
          oss << "Score: " << setw(6) << getScore() << "  ";
      }
    
      else
      {
          oss << "Score: -" << setw(5) << getScore() * -1
          << "  ";
      }
      oss << "Level: " << getLevel() << "  ";
      oss << "Lives: " << getLives() << "  ";
      oss << "Health: " << setw(2) << (m_Socrates->getHealth())<< "  ";;
      oss << "Sprays: " << setw(2) << m_Socrates->getSprays()<< "  ";;
      oss << "Flames: " << m_Socrates->getFlames();
      setGameStatText(oss.str());

    return GWSTATUS_CONTINUE_GAME ;
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::cleanUp()
{
    delete m_Socrates; //deletes Socrates
    m_Socrates = nullptr;
    
    vector<Actor*>::iterator it;
      it = actors.begin();
       while(it!= actors.end()) //deletes all actors
       {
            delete *it;
            it = actors.erase(it);
       }
    
    m_actors = 0;
    m_bacteria = 0;
    bacteriaKilled = 0;
    
}

void StudentWorld::addNewActor(Actor* actor) //adds a new actor
{
    actors.push_back(actor);
    m_actors++;
}


double StudentWorld::euclidean(int x, int x1, int y, int y1) //finds euclidean distance of 2 pts
{
    return sqrt(pow(x - x1, 2) + pow(y - y1, 2));
}

double StudentWorld::getSocratesX() const
{
    return m_Socrates->getX();
}

double StudentWorld::getSocratesY() const
{
    return m_Socrates->getY();
}

void StudentWorld::damageSocrates(int x)
{
    m_Socrates->changeHealth(-x);
    playSound(SOUND_PLAYER_HURT);
    
    if(m_Socrates->getHealth() <= 0)
    {
        m_Socrates->setAlive(false);
    }
}

void StudentWorld::increaseBacteriaKilled()
{
    bacteriaKilled++;
}

void StudentWorld::addBacteria(int x)
{
    m_bacteria += x;
}


void StudentWorld::restoreHealth()
{
    m_Socrates->setHealth(100);
}

void StudentWorld::addFlames()
{
    m_Socrates->changeFlameCharge(5);
}


int StudentWorld::typeOfSprayHit(Actor* actor)
{
    vector<Actor*>::iterator it; //iteratest through all actors
      it = actors.begin();
    int z = 0;
      while (it!= actors.end())
      {
          //did the  spray hit a goodie or dirt, if so instant  kill it
          if((*it)->isHitable() && (!(*it)->hasHP()) && euclidean(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= 2 * SPRITE_RADIUS)
          {
              (*it)->setAlive(false);
              return 1;
          }
          
          //did the spray hit a  bacteria, if  so decrement its health and check to see if  it should die
          else if ((*it)->isBacteria()&& (*it)->hasHP() && (*it)->isAlive() && (*it)->getHealth() > 0 && euclidean(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= 2 * SPRITE_RADIUS)
          {
              int x =  (*it)->getX();
              int y = (*it)->getY();
              (*it)->changeHealth(-2);
        
              if(!((*it)->isAlive()))
              {
                  increaseBacteriaKilled();
                  if(!(*it)->isEcoli())
                     {
                         playSound(SOUND_SALMONELLA_DIE);
                     }
                  
                    else
                     {
                         playSound(SOUND_ECOLI_DIE);
                     }
       
                  
                  increaseScore(100);
                  z = randInt(1, 2);
                  if(z==1)
                    {
                        //50% chance of a new food object
                        addNewActor(new Food(this, x, y));
                        return 2;
                    }
              }
              else
              {
                  playSound(SOUND_SALMONELLA_HURT);
              }

              return 2;
          }
          
          else
          {
              it++;
          }
          
      }
    return 3;
          
}

int StudentWorld::typeOfFlameHit(Actor* actor)
{
    vector<Actor*>::iterator it;
      it = actors.begin();

      while (it!= actors.end())
      {//if dirt, goodies, etc... hit
          int z = 0;
          if((*it)->isHitable() && (!(*it)->hasHP()) && euclidean(actor->getX(),  (*it)->getX(), actor->getY(), (*it)->getY()) <= 2 * SPRITE_RADIUS)
          {
              (*it)->setAlive(false);
              return 1;
          }
          
          // if salmonella or aggro salmonella hit,decrement health
          else if ((*it)->isBacteria() && (*it)->hasHP() && (*it)->isAlive() && (*it)->getHealth() > 0 && euclidean(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= 2 * SPRITE_RADIUS)
          {
              int x =   (*it)->getX();
              int y =   (*it)->getY();
              (*it)->changeHealth(-5);
          
              if(!((*it)->isAlive()))
               {
                   increaseBacteriaKilled();
                   if(!(*it)->isEcoli())
                   {
                       playSound(SOUND_SALMONELLA_DIE);
                   }
                   else
                   {
                       playSound(SOUND_ECOLI_DIE);
                   }
               
                   increaseScore(100);
                   z = randInt(1, 2);
                   if(z==1)
                    {
                        //50% chance of a new food object
                        addNewActor(new Food(this, x, y));
                        return 2;
                    }
                 
               }
               
               else
               {
                   playSound(SOUND_SALMONELLA_HURT);
               }

              return 2;
          }
          
          
          //did not hit that actor
          else
          {
              it++;
          }
          
      }
    return 3;
          
}

bool StudentWorld::overlapsFood(Actor *actor)
{
    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it!= actors.end())
    {//if food  is overlapping with this actor
        if(!((*it)->isHitable()) && (*it)->getDirection() == 90 && euclidean(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= 2 * SPRITE_RADIUS)
        {
            (*it)->setAlive(false);
            return true;
            
        }
            it++;
    }
    return false;
}


bool StudentWorld::isMovementBlocked(Actor *actor)
{

    for(int i = 1; i < 4; i ++) //checks if  movement blocked by the outside of the circle
    {
        double x;
          double y;
          actor->getPositionInThisDirection(actor->getDirection(), i, x, y);
        if(euclidean(x, VIEW_WIDTH/2, y, VIEW_HEIGHT/2) >= VIEW_RADIUS)
               {
                   return true;
               }
    }
    
    for(int i = 1; i < 4; i ++)  //checks  if movement is blocked  by dirt
      {
           double x;
            double y;
          actor->getPositionInThisDirection(actor->getDirection(), i, x, y);
        vector<Actor*>::iterator it;
        it = actors.begin();
        while (it!= actors.end())
        {
            //if its dirt in the way
            if(((*it)->isHitable()) && !((*it)->isGoodie()) && !((*it)->isBacteria()) && (*it)->getDirection() == 0 && euclidean(x, (*it)->getX(), y, (*it)->getY()) <= SPRITE_WIDTH/2)
            {
                return true;
                
            }
                it++;
        }
    }
    return false;
}

bool StudentWorld::isMovementBlockedByDirt(Actor *actor)
{
    
    for(int i = 1; i < 4; i ++) //checks  if movement is blocked  by dirt
      {
           double x;
            double y;
          actor->getPositionInThisDirection(actor->getDirection(), i, x, y);
        vector<Actor*>::iterator it;
        it = actors.begin();
        while (it!= actors.end())
        {
            //if its dirt in the way
            if(((*it)->isHitable()) && !((*it)->isGoodie()) && !((*it)->isBacteria()) && (*it)->getDirection() == 0 && euclidean(x, (*it)->getX(), y, (*it)->getY()) <= SPRITE_WIDTH/2)
            {
                return true;
                
            }
                it++;
        }
    }
    return false;
}

bool StudentWorld::isMovementBlockedByCircle(Actor *actor)
{

    for(int i = 1; i < 4; i ++) //is movement blocked by the outside of the circle
    {
        double x;
          double y;
          actor->getPositionInThisDirection(actor->getDirection(), i, x, y);
        if(euclidean(x, VIEW_WIDTH/2, y, VIEW_HEIGHT/2) >= VIEW_RADIUS)
               {
                   return true;
               }
    }
    
    return false;
}

bool StudentWorld::isNearbyFood(Actor *actor)
{
    const double PI = 4 * atan(1);

    vector<Actor*>::iterator it;
    it = actors.begin();
    while (it!= actors.end())
    {//if food within 128 pixels
        if((*it)->isFood() && !((*it)->isHitable()) && (*it)->getDirection() == 90 && euclidean(actor->getX(), (*it)->getX(), actor->getY(), (*it)->getY()) <= 128)
        {
            //sets the new direction of the bacteria to chase the food
            //TODO: ADJUST ARCSIN new direction
            actor->setDirection(int(0.5 + (180/PI) * atan2((*it)->getY() - actor->getY(), (*it)->getX() - actor->getX())));
            return true;
        }
        it++;
    }
    return false;
}


bool StudentWorld::isNearbySocrates(Actor *actor)
{
 const double PI = 4 * atan(1);

    //if Socrates within 72 pixels

        if(euclidean(actor->getX(), getSocratesY(), actor->getY(), getSocratesX()) <= 72)
        {
            //sets the new direction of the bacteria to chase Socrates
              actor->setDirection(int(0.5 + (180/PI) * atan2(getSocratesY() - actor->getY(), getSocratesX() - actor->getX())));
        }
    
    return false;
}

bool StudentWorld::isEcoliNearbySocrates(Actor *actor)
{
 const double PI = 4 * atan(1);

    //if Socrates within 72 pixels

        if(euclidean(actor->getX(), getSocratesY(), actor->getY(), getSocratesX()) <= 256)
        {
            //sets the new direction of the bacteria to chase Socrates
              actor->setDirection(int(0.5 + (180/PI) * atan2(getSocratesY() - actor->getY(), getSocratesX() - actor->getX())));
            return true;
        }
    
    return false;
}




 
