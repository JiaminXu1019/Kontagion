
#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
using namespace std;
#include "Actor.h"



GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_actors = 0;
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
       
           if(m_actors == 0) //if this is the first pit generated dont both checking for overlap
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
                    if(euclidean(x, (*it)->getX(), y, (*it)->getY()) < 2*SPRITE_RADIUS || z > 120)
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
    
    for(int i = 0; i < max(5* getLevel(), 25); i++) //RANDOMLY generates coordinates for the food objects such that they are no more than 120 pixels away from the center
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
                      if(euclidean(x, (*it)->getX(), y, (*it)->getY()) < 2*SPRITE_RADIUS || z > 120)
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
                    if( (!(*it)->canOverlap()) && (euclidean(x, (*it)->getX(), y, (*it)->getY()) < 2*SPRITE_RADIUS || z > 120))
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
    
    if (!(m_Socrates->isAlive()))
    {
          decLives();
            playSound(SOUND_PLAYER_DIE);
          return GWSTATUS_PLAYER_DIED;
    }
    
      if (m_bacteria == bacteriaKilled)
      {
          playSound(SOUND_FINISHED_LEVEL);
          return GWSTATUS_FINISHED_LEVEL;
      }
    
    vector<Actor*>::iterator it2; //deletes memory of dead actors
    it2 = actors.begin();
    while (it2 != actors.end())
    {
        if ( ! ((*it2)->isAlive()) )
        {
            delete *it2;
            it2 = actors.erase(it);
            m_actors--;
            
        }
        
        else
        {
            it2++;
        }
    }
    
    //TODO:ADD GOODIES
    
    //decLives();
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


double StudentWorld:: euclidean(int x, int x1, int y, int y1) //finds euclidean distance of 2 pts
{
    return sqrt(pow(x - x1, 2) + pow(y - y1, 2));
}

double StudentWorld::getSocratesX() const
{
    return m_Socrates->getX();
}

double StudentWorld::getSocratesY() const
{
    return m_Socrates->getX();
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

void StudentWorld::restoreHealth()
{
    m_Socrates->setHealth(100);
}

void StudentWorld::addFlames()
{
    m_Socrates->changeFlameCharge(5);
}

   
