#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

class Actor;
class Socrates;
class Bacteria;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    double getSocratesX() const;
    double getSocratesY() const;
    void damageSocrates(int x);
    void restoreHealth();
    void addFlames();
    void increaseBacteriaKilled();
    void addBacteria(int x);
    int typeOfSprayHit(Actor* actor);
    int typeOfFlameHit(Actor* actor);
    bool overlapsFood(Actor* actor);
    bool isMovementBlocked(Actor* actor);
    bool isMovementBlockedByDirt(Actor* actor);
    bool isMovementBlockedByCircle(Actor* actor);


    bool isNearbyFood(Actor* actor);
    bool isNearbySocrates(Actor* actor);




    void addNewActor(Actor* actor);
    ~StudentWorld();
    
    
    double euclidean(int x, int x1, int y, int y1);

    
private:
    
    std::vector<Actor*> actors;
    int m_bacteria;
    int m_actors;
    Socrates* m_Socrates;
    int bacteriaKilled;
};

#endif // STUDENTWORLD_H_
