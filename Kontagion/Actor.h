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
        virtual bool hasHP() = 0;
        void changeHealth(int health);
           void setHealth(int health);
           int getHealth() const;


private:
        int m_health;
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
            virtual bool canOverlap();
            int getSprays() const;
            int getFlames() const;
            virtual bool hasHP();

        private:
            int sprayCharges;
            int flameCharges;
            int positionalAngle;
};


class Stationary: public Actor //dirt and food objects
{
    public:
        Stationary(int imageID, StudentWorld* studentWorld, double x, double y, bool hitable, int direction);
            virtual ~Stationary();
            virtual void doSomething();
            virtual bool canOverlap();
            virtual bool hasHP();


    private:

};

class Dirt: public Stationary
{
    public:
        Dirt(StudentWorld* studentWorld, double x, double y);
        virtual ~Dirt();
        virtual bool canOverlap();

        
    private:

};

class Food: public Stationary
{
    public:
        Food(StudentWorld* studentWorld, double x, double y);
        virtual ~Food();
        virtual bool canOverlap();


    private:

};


class Pit: public Actor
{
    public:
        Pit(StudentWorld* studentWorld, double x, double y);
        virtual ~Pit();
        virtual void doSomething();
        void decrSalmonella();
        void decrAggroSalmonella();
        void decrECOLI();
        bool isEmpty() const;
        virtual bool canOverlap();
        virtual bool hasHP();

    
    private:
        int m_salmonella;
        int m_aggro_salmonella;
        int m_e_coli;
};




class Goodie: public Actor
{
    public:
    Goodie(StudentWorld* studentWorld, double x, double y, int ID);

        virtual void doSomething()=0;
        virtual bool canOverlap();
        virtual ~Goodie();
        void decrLifetime();
        int getLifetime() const;
        virtual bool hasHP();


    private:
        int lifetime;
};

class Fungus: public Goodie
{
    public:
        Fungus(StudentWorld* studentWorld, double x, double y);
        virtual void doSomething();
        virtual ~Fungus();

    private:
     
};

class RestoreHealthItem: public Goodie
{
    public:
        RestoreHealthItem(StudentWorld* studentWorld, double x, double y);
         virtual void doSomething();
         virtual ~RestoreHealthItem();
        
    private:
};

class AddLifeItem: public Goodie
{
    public:
        AddLifeItem(StudentWorld* studentWorld, double x, double y);
            virtual void doSomething();
            virtual ~AddLifeItem();
            
    private:
};

class AddFlameThrowerItem: public Goodie
{
    public:
        AddFlameThrowerItem(StudentWorld* studentWorld, double x, double y);
              virtual void doSomething();
              virtual ~AddFlameThrowerItem();
        
    private:
};


class Projectile: public Actor
{
    public:
            Projectile(StudentWorld* studentWorld, double x, double y, int direction, int ID);
           virtual void doSomething()=0;
           virtual bool canOverlap();
           virtual ~Projectile();
            void setMaxTravel(int x);
            virtual bool hasHP();
            int getDistanceTraveled() const;
            void addDistanceTraveled();

        
    private:
        int max_travel_distance;
        int distance_traveled;

};

class Spray: public Projectile
{
    public:
            Spray(StudentWorld* studentWorld, double x, double y, int direction);
            virtual void doSomething();
            virtual ~Spray();
           
    private:
    
};

class FlameThrower: public Projectile
{
    public:
     FlameThrower(StudentWorld* studentWorld, double x, double y, int direction);
        virtual void doSomething();
             virtual ~FlameThrower();
           
    private:
    
};

class Bacteria: public Actor
{
    public:
        Bacteria(StudentWorld* studentWorld, double x, double y, int ID);
        virtual void doSomething() = 0;
        virtual bool hasHP();
        virtual ~Bacteria();
        void setFoodEaten(int x);
        int getFoodEaten() const;
        virtual bool canOverlap();
        int getMovementPlanDistance() const;
        void setMovementPlanDistance(int x);



        
    private:
        int food_eaten;
        int movement_planDistance;

};

class Salmonella: public Bacteria
{
    public:
        Salmonella(StudentWorld* studentWorld, double x, double y);
        virtual void doSomething();
        virtual ~Salmonella();

    
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









// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
