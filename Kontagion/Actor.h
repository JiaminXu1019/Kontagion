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
        virtual bool canOverlap() = 0;

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
            int getHealth() const;
            virtual bool canOverlap();

    
        private:
            int m_health;
            int sprayCharges;
            int flameCharges;
            int positionalAngle;
};


class Stationary: public Actor //dirt and food objects
{
    public:
        Stationary(int imageID, StudentWorld* studentWorld, int x, int y, bool hitable);
        virtual ~Stationary();
        virtual void doSomething();
        virtual bool canOverlap();

    private:

};

class Dirt: public Stationary
{
    public:
        Dirt(StudentWorld* studentWorld, int x, int y);
        virtual ~Dirt();
        virtual bool canOverlap();
        
    private:

};

class Food: public Stationary
{
    public:
        Food(StudentWorld* studentWorld, int x, int y);
        virtual ~Food();
        virtual bool canOverlap();

    private:


};


class Pit: public Actor
{
    public:
        Pit(StudentWorld* studentWorld, int x, int y);
        virtual ~Pit();
        virtual void doSomething();
        void decrSalmonella();
        void decrAggroSalmonella();
        void decrECOLI();
        bool isEmpty() const;
        virtual bool canOverlap();
    
    private:
        int m_salmonella;
        int m_aggro_salmonella;
        int m_e_coli;
};




class Goodie: public Actor
{
    public:
    Goodie(StudentWorld* studentWorld, int x, int y, int ID);

        virtual void doSomething() = 0;
        virtual bool canOverlap();
        virtual ~Goodie();
        void decrLifetime();
        int getLifetime() const;

    private:
        int lifetime;
};

class Fungus: public Goodie
{
    public:
        Fungus(StudentWorld* studentWorld, int x, int y);
        virtual void doSomething();
        virtual ~Fungus();

    private:
     
};

class RestoreHealthItem: public Goodie
{
    public:
        RestoreHealthItem(StudentWorld* studentWorld, int x, int y);
         virtual void doSomething();
         virtual ~RestoreHealthItem();
        
    private:
};

class AddLifeItem: public Goodie
{
    public:
        AddLifeItem(StudentWorld* studentWorld, int x, int y);
            virtual void doSomething();
            virtual ~AddLifeItem();
        
    private:
};

class AddFlameThrowerItem: public Goodie
{
    public:
        AddFlameThrowerItem(StudentWorld* studentWorld, int x, int y);
              virtual void doSomething();
              virtual ~AddFlameThrowerItem();
        
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







// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
