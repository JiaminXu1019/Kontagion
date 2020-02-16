
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
    m_Socrates = new Socrates(this);

    for(int i = 0; i < max(180 - (getLevel() * 20), 20); i++)
    {
        int x = randInt(-120, 120);
        int y = randInt(-120, 120);
        double z = sqrt(pow(x, 2) + pow(y,2));
        
        x += VIEW_WIDTH/2;
        y += VIEW_HEIGHT/2;
    
        while(z > 120)
        {
            x = randInt(-120, 120);
            y = randInt(-120, 120);
            z = sqrt(pow(x, 2) + pow(y,2));
            
            x += VIEW_WIDTH/2;
            y += VIEW_HEIGHT/2;
         
        }
        addNewActor(new Dirt(this, x, y));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    m_Socrates->doSomething();
    
    vector<Actor*>::iterator it;
    it = actors.begin();
     while(it!= actors.end())
     {
         (*it) ->doSomething();
         it++;
     }
    
    //decLives();
    return GWSTATUS_CONTINUE_GAME ;
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::cleanUp()
{
    delete m_Socrates;
    m_Socrates = nullptr;
    
    vector<Actor*>::iterator it;
      it = actors.begin();
       while(it!= actors.end())
       {
            delete *it;
            it = actors.erase(it);
       }
    
}

void StudentWorld::addNewActor(Actor* actor)
{
    actors.push_back(actor);
    m_actors++;
}


double euclidean(int x, int x1, int y, int y1)
{

    return sqrt(pow(x - x1, 2) + pow(y - y1, 2));
}
//bool isOverlap(int otherX, int otherY)

   
