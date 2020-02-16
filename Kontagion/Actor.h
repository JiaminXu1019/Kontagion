#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>
using namespace std;

class StudentWorld;

class Actor: public GraphObject
{
    
public:
    
    Actor(int imageID, double x, double y, int direction, int depth, StudentWorld* studentWorld, bool hitable);
    
      virtual void doSomething() = 0;
      StudentWorld* getWorld() const;
       bool isAlive() const;
        void setAlive(bool x);
       void changeAliveStatus();
       virtual ~Actor();
        bool isHitable() const;
    

private:
        bool m_alive;
        bool m_hitable;
        StudentWorld* m_studentWorld;
       
};

class Socrates: public Actor
{

        public:
            Socrates(StudentWorld* studentWorld);
            virtual ~Socrates();
            virtual void doSomething();
            void changeSprayCharge(int num);
            void changeFlameCharge(int num);
            void changeHealth(int health);
            void setHealth(int health);

    
        private:
            int m_health;
            int sprayCharges;
            int flameCharges;
            int positionalAngle;
};


class Useless: public Actor
{
    public:
        Useless(int imageID, StudentWorld* studentWorld, int x, int y, bool hitable);
        virtual ~Useless();
        virtual void doSomething();

    private:

};

class Dirt: public Useless
{
    public:
        Dirt(StudentWorld* studentWorld, int x, int y);
        virtual ~Dirt();
        
    private:

};

class Food: public Useless
{
    public:
        Food(StudentWorld* studentWorld, int x, int y);
        virtual ~Food();
        
    private:

};


class Pits: public Actor
{
    public:
        
    private:
};

class Fungi: public Actor{
    public:
        
    private:
};






class Bacteria: public Actor
{
    public:
        
    private:
      int m_health;
    
};

class Salmonella: public Bacteria
{
    public:
        
    private:
};


class AggroSalmonella: public Salmonella
{
    public:
        
    private:

};

class Ecoli: public Bacteria
{
    public:
        
    private:
    
};

class Projectile: public Actor
{
    public:
        
    private:
};

class Spray: public Projectile
{
    public:
           
    private:
    
};

class FlameThrower: public Projectile
{
    public:
           
    private:
    
};



class Goodie: public Actor
{
    public:
        
    private:
};

class RestoreHealthItem: public Goodie
{
    public:
        
    private:
};

class AddLifeItem: public Goodie
{
    public:
        
    private:
};

class AddFlameThrowerItem: public Goodie
{
    public:
        
    private:
};





// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
