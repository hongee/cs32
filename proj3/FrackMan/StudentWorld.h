#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <vector>
#include <queue>

const int DIRT_R = 60;
const int DIRT_C = 64;
const int SHAFT_RS = 4;
const int SHAFT_RE = 59;
const int SHAFT_CS = 30;
const int SHAFT_CE = 33;

const int BOULDER_SR = 20;
const int DISTRIBUTED_EC = 60;
const int DISTRIBUTED_ER = 56;
const int DISTRIBUTED_LIMIT = 6;

const int BLOCKED = 99999;
const int KNOWN = 99991;
const int SEEN = 99911;
const int UNSEEN = 99111;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();

	virtual int init();
	virtual int move();
	virtual void cleanUp();

	bool removeDirt(const int x, const int y);
	bool removeDirt(const int sx, const int sy, const int ex, const int ey);
	bool isDirtAt(const int sx, const int sy, const int ex, const int ey) const;

	bool hasBlockingActorAt(const int x, const int y) const;
  bool hasBlockingActorAt(const int x, const int y, const Actor* a) const;
	bool causeDamageToActorsAt(const int x, const int y, int damage, bool hitsPlayer, int damageCausingType);

  bool isAccessible(int x, int y, Actor::Direction dir) const;

  bool addActor(Actor* a);

	bool playerNearMe(Actor* me, int limit) const;
	bool protestorNearMe(Actor* me, Actor* &other,int limit) const;

	void revealActorsAt(int x, int y, int limit);
	//void getNextPosition(int cx, int cy, int& nx, int &ny, Actor::Direction &dir);

	void moveTowardsPlayer(Actor* a, int& nx, int &ny, Actor::Direction &dir);
	void moveTowardsExit(Actor* a, int& nx, int &ny, Actor::Direction &dir);

	int stepsToPlayer(int x, int y) const;

	bool isPlayerWithinSight(int x, int y, Actor::Direction &dir) const;

	Frackman* getFrackman();

private:
	struct P {
        int x;
        int y;
        int originNum;
        P(int xx, int yy, int n)
        : x(xx)
        , y(yy)
        , originNum(n)
        {};
    };
		/*
    struct Position {
			int x;
			int y;
			Position* next;
			Position* previous;

			Position(int x, int y)
			: x(x),y(y),next(nullptr),previous(nullptr){};

			Position(int x, int y, Position* prev)
			: x(x),y(y),next(nullptr),previous(prev){};
		};*/

	std::list<Actor*> m_actors;
	//Some sort of tree thing
  //std::vector<Position*> m_points;

	int m_toPlayerHeatMap[VIEW_WIDTH][VIEW_HEIGHT];
	int m_toExitHeatMap[VIEW_WIDTH][VIEW_HEIGHT];

	Dirt* m_dirt_arr[DIRT_C][DIRT_R];
	Frackman* m_player;

	int m_noOil;
	int m_noBoulders;
	int m_noNuggets;
	int m_noProtestors;

	int m_protestorTickDelay;
	int m_ticksSinceLastProtestor;
	int m_totalPossibleProtestors;
	int m_hardcoreProtestorProbability;
	int m_goodieProbability;

  bool actorsAreClose(Actor* a, Actor* b, int limit) const;
  bool actorsAreClose(const int x, const int y, Actor *b, int limit) const;
	void possibleDistributedPos(int& x, int& y, bool isBoulder) const;
  //void explorePosition(int x, int y, Position* prev, Actor::Direction dir, std::vector<Position*> &pL, std::queue<Position*> &tS, int ct[][VIEW_HEIGHT]);
  void exploreMapPos(int x, int y, int origin,Actor::Direction dir, std::queue<P> &q, int m[][VIEW_HEIGHT]);
	void setDisplayText();
	void updateHeatMap(int destx, int desty, int map[][VIEW_HEIGHT]);
	std::string formatToString(int num, unsigned int finalLength, char filler);
	int getOil() const;

};

// Inline Fuctions
inline
int StudentWorld::getOil() const {
	return m_noOil;
}

inline
Frackman* StudentWorld::getFrackman() {
	return m_player;
}

// Utility Functions
int randInt(int begin, int end);
double calculateDistance(int sx, int sy, int ex, int ey);

#endif // STUDENTWORLD_H_
