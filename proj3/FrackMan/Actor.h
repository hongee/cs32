#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

// Type Consts
const int ISBOULDER = 11;
const int ISFRACKMAN = 12;
const int ISSQUIRT = 13;
const int ISPROTESTOR = 14;
const int ISHARDCOREPROTESTOR = 15;
const int ISDIRT = 16;
const int ISOIL = 17;
const int ISGOLD = 18;
const int ISSONAR = 19;
const int ISWATER = 20;

// Constants
const int FRACKMAN_SC = 30;
const int FRACKMAN_SR = 60;
const int FRACKMAN_TH = 10;
const int ACTOR_MAXC = 60;
const int ACTOR_MAXR = 60;
const int BOULDER_HEALTH = 30;
const int SQUIRT_HEALTH = 4;
const int GOLD_HEALTH = 100;
const int PROTESTOR_SHOUT_TIME = 15;

// Base Actor
class Actor : public GraphObject {
public:
    Actor(int x, int y, bool isVisible, int imageID, Direction dir, double size, unsigned int depth, unsigned int health, StudentWorld* world);
    virtual ~Actor();

    virtual void doSomething() = 0;

    virtual bool isStaticObject() const;
    virtual bool isUnpassable() const;
    virtual bool canBePickedUpByPlayer() const;
    virtual bool canBePickedUpByProtestor() const;

    //accessors
    bool isDead() const;
    int  getHealth() const;
    virtual int getType() const = 0;
    StudentWorld* getWorld() const;

    //mutators
    bool setDead();
    bool incrHealthBy(const int n);
    virtual bool decrHealthBy(const int n);
    virtual void pickedUpGold();

private:
    bool          m_isDead;
    int           m_health;
    StudentWorld* m_world;
};

// Frackman!
class Frackman : public Actor {
public:
    Frackman(StudentWorld* world);

    virtual void doSomething();
    virtual bool isStaticObject() const;

    int getWater() const;
    int getSonar() const;
    int getGold() const;
    int getHealthAsPct() const;
    virtual int getType() const;

    void increaseGold();
    void increaseSonar();
    void increaseWater();

    virtual bool decrHealthBy(const int n);

private:
    unsigned int m_water_count;
    unsigned int m_sonar_count;
    unsigned int m_gold_count;
    void handleInput();
    void createNewSquirt(int x, int y, Direction dir);
    void dropGold(int x, int y);
};

// Dirt
class Dirt : public Actor {
public:
    Dirt(int x, int y, StudentWorld* world);
    virtual int getType() const;
    virtual void doSomething();
};

// Boulder
class Boulder : public Actor {
public:
    Boulder(int x, int y, StudentWorld* world);

    virtual int getType() const;
    virtual void doSomething();
    virtual bool isUnpassable() const;
private:
    bool m_isFalling;
    bool m_isWaiting;
};

// Squirt
class Squirt : public Actor {
public:
    Squirt(int x, int y, Direction dir, StudentWorld* world);

    virtual int getType() const;
    virtual void doSomething();
    virtual bool isUnpassable() const;
};

// Pick Uppable
class Pickups : public Actor {
public:
    Pickups(int x, int y, int imageID, bool isVisible, bool isPlayerPickup, bool isTemp, int health, StudentWorld* world);

    virtual bool canBePickedUpByPlayer() const;
    virtual bool canBePickedUpByProtestor() const;
    bool isTemporary() const;

private:
    bool m_player_pickup;
    bool m_protestor_pickup;
    bool m_isTemporary;
};

// Oil
class Oil : public Pickups {
public:
    Oil(int x, int y, StudentWorld* world);
    virtual void doSomething();
    virtual int getType() const;
};

// Gold
class Gold : public Pickups {
public:
    Gold(int x, int y, bool isPlayer, StudentWorld* world);
    virtual void doSomething();
    virtual int getType() const;
};

// Sonar
class Sonar : public Pickups {
public:
    Sonar(int x, int y, StudentWorld* world);
    virtual void doSomething();
    virtual int getType() const;
};

// Water
class Water : public Pickups {
public:
    Water(int x, int y, StudentWorld* world);
    virtual void doSomething();
    virtual int getType() const;
};

// Protestor
class GenericProtestor : public Actor {
public:
    GenericProtestor(int image, int squaresToMove, int health, StudentWorld* world);
    virtual void doSomething();
    bool isLeaving() const;
    virtual int getType() const = 0;
    virtual bool doPathfinding() = 0;
    virtual bool decrHealthBy(const int n);
    virtual bool isStaticObject() const;
    void setLeaving();
    void setTicksToNextMove(int n);

private:
    bool shouldIMove();
    bool m_isLeavingField;
    const int m_tickDelay;
    int m_ticksToNextMove;
    int m_ticksSinceLastShout;
    int m_ticksSinceLastPerpendicular;
    int m_noSquaresToMoveInCurrentDir;
};

// Regular Protestor
class RegularProtestor : public GenericProtestor {
public:
    RegularProtestor(StudentWorld* world);
    virtual int getType() const;
    virtual bool doPathfinding();
    virtual void pickedUpGold();
};

// Hardcore Protestor
class HardcoreProtestor : public GenericProtestor {
public:
    HardcoreProtestor(StudentWorld* world);
    virtual int getType() const;
    virtual bool doPathfinding();
    virtual void pickedUpGold();
};

/* Inlined Functions */
inline
bool Actor::isDead() const {
    return m_isDead;
}

inline
int Actor::getHealth() const {
    return m_health;
}

inline
bool Actor::incrHealthBy (const int n) {
    m_health += n;
    return true;
}

inline
bool Actor::isStaticObject() const {
    return true;
}

inline
bool Actor::isUnpassable() const {
    return false;
}

inline
bool Actor::canBePickedUpByPlayer() const {
    return false;
}

inline
bool Actor::canBePickedUpByProtestor() const {
    return false;
}

inline
void Actor::pickedUpGold() {
    return;
}

inline
StudentWorld* Actor::getWorld() const {
    return m_world;
}

inline
int Frackman::getHealthAsPct() const {
    return (static_cast<double>(getHealth()) / FRACKMAN_TH)*100;
}

inline
int Frackman::getWater() const {
    return m_water_count;
}

inline
int Frackman::getSonar() const {
    return m_sonar_count;
}

inline
int Frackman::getGold() const {
    return m_gold_count;
}

inline
bool Frackman::isStaticObject() const {
    return false;
}

inline
int Frackman::getType() const {
    return ISFRACKMAN;
}

inline
void Frackman::increaseGold() {
    m_gold_count++;
}

inline
void Frackman::increaseSonar() {
    m_sonar_count++;
}

inline
void Frackman::increaseWater() {
    m_water_count+=5;
}

inline
int Dirt::getType() const {
    return ISDIRT;
}

inline
int Boulder::getType() const {
    return ISBOULDER;
}

inline
int Oil::getType() const {
    return ISOIL;
}

inline
bool Boulder::isUnpassable() const {
    return true;
}

inline
int Squirt::getType() const {
    return ISSQUIRT;
}

inline
int Gold::getType() const {
    return ISGOLD;
}

inline
int Sonar::getType() const {
    return ISSONAR;
}

inline
int Water::getType() const {
    return ISWATER;
}

inline
bool Squirt::isUnpassable() const {
    return false;
}

inline
bool Pickups::canBePickedUpByPlayer() const {
    return m_player_pickup;
}

inline
bool Pickups::canBePickedUpByProtestor() const {
    return m_protestor_pickup;
}

inline
bool Pickups::isTemporary() const {
  return m_isTemporary;
}

inline
bool GenericProtestor::isLeaving() const {
  return m_isLeavingField;
}

inline
bool GenericProtestor::isStaticObject() const {
  return false;
}

inline
void GenericProtestor::setLeaving() {
  m_isLeavingField = true;
}

inline
void GenericProtestor::setTicksToNextMove(int n) {
  m_ticksToNextMove = n;
}

inline
int RegularProtestor::getType() const {
  return ISPROTESTOR;
}

inline
bool RegularProtestor::doPathfinding() {
  return false;
}

inline
int HardcoreProtestor::getType() const {
  return ISHARDCOREPROTESTOR;
}

Actor::Direction randomDirection();
#endif // ACTOR_H_
