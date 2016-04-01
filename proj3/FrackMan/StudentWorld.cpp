#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <cstdlib>
#include <cmath>
#include <queue>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

/**
 Constructor for StudentWorld
 @param string assetDir
 */
StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
, m_noOil(0)
, m_noBoulders(0)
, m_noNuggets(0)
, m_noProtestors(0)
, m_ticksSinceLastProtestor(0)
, m_totalPossibleProtestors(0)
, m_hardcoreProtestorProbability(0)
, m_goodieProbability(0)
{}

/**
 Destructor for StudentWorld
 - Remove all dynamically allocated objects
 - Dirt/Frackman/Actors
 */
StudentWorld::~StudentWorld() {
    for(int x = 0; x < DIRT_C; x++) {
        for(int y = 0; y < DIRT_R; y++) {
            delete m_dirt_arr[x][y];
        }
    }
    delete m_player;
    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
      delete (*it);
    }
}

/**
 Initializes every new level
 Does the following
 1) Repaints the dirt
 2) Distributes required actors
 3) Sets player
 */
int StudentWorld::init()
{
    //TODO: WRAP THIS SOMEWHERE ELSE
    //Seeds the random number
    srand(time(0));

    // Paint dirt
    for(int y = 0; y < DIRT_R; y++) {
        for(int x = 0; x < DIRT_C; x++) {
            m_dirt_arr[x][y] = new Dirt(x,y, this);
            //Mine shaft
            if(y >= SHAFT_RS && y <= SHAFT_RE && x >= SHAFT_CS && x <= SHAFT_CE)
                m_dirt_arr[x][y] -> setDead();
        }
    }

    // Distribute Actors
    m_noBoulders = (getLevel()/2 + 2) > 6 ? 6 : (getLevel()/2 + 2);
    m_noNuggets = (5-getLevel()/2) > 2 ? (5-getLevel()/2) : 2;
    m_noOil = (getLevel() + 2) > 20 ? 20 : (getLevel()/2 + 2);

    m_protestorTickDelay = 25 > (200-getLevel()) ? 25 : (200-getLevel());
    m_ticksSinceLastProtestor = 0;
    m_totalPossibleProtestors = (2 + getLevel()*1.5) > 15 ? 15 : (2 + getLevel()*1.5);
    m_hardcoreProtestorProbability = (getLevel()*10 + 30) > 90 ? 90 : (getLevel()*10 + 30);
    m_goodieProbability = getLevel()*25 + 300;

    // Distribute Boulders
    for(int i=0; i<m_noBoulders; ++i) {
      int x = 0, y = 0;
      possibleDistributedPos(x, y, true);
      Actor* b = new Boulder(x,y,this);
      m_actors.push_back(b);
      removeDirt(x,y,x+3,y+3);
    }

    // Distribute Oil
    for(int i=0; i<m_noOil; ++i) {
      int x = 0, y = 0;
      possibleDistributedPos(x, y, false);
      Actor* o = new Oil(x,y,this);
      m_actors.push_back(o);
    }

    // Distribute Gold
    for(int i=0; i<m_noNuggets ; ++i) {
      int x = 0, y = 0;
      possibleDistributedPos(x, y, false);
      Actor* g = new Gold(x,y,false,this);
      m_actors.push_back(g);
    }

    // New Player
    m_player = new Frackman(this);

    return GWSTATUS_CONTINUE_GAME;
}

/**
 Handles every tick
 Does the following
 1) Updates game text
 1) Checks for human input
    - moves player if yes
 2) Moves others
 3) Updates Text
 4) Cleans up dead actors
 5) Handle game state (i.e. check if player died)
 */
int StudentWorld::move()
{
    setDisplayText();
    m_player->doSomething();

    //player has moved, update heat maps
    updateHeatMap(m_player->getX(), m_player->getY(), m_toPlayerHeatMap);
    updateHeatMap(ACTOR_MAXC, ACTOR_MAXR, m_toExitHeatMap);

    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
      (*it)->doSomething();
    }

    if(m_ticksSinceLastProtestor > 0) {
      m_ticksSinceLastProtestor--;
    } else if(m_noProtestors < m_totalPossibleProtestors) {
      m_ticksSinceLastProtestor = m_protestorTickDelay;
      if(randInt(1,m_hardcoreProtestorProbability) == 1) {
        cerr << "Adding a hardcore protestor" << endl;
        Actor* p = new HardcoreProtestor(this);
        m_actors.push_back(p);
      } else {
        cerr << "Adding a regular protestor" << endl;
        Actor* p = new RegularProtestor(this);
        m_actors.push_back(p);
      }
      m_noProtestors++;
    }

    if(randInt(1, m_goodieProbability) == 1) {
      if(randInt(1,5) == 1) {
        cerr << "Creating a new Sonar!" << endl;
        Actor* s = new Sonar(0,60,this);
        m_actors.push_back(s);
      } else {
        cerr << "Creating a new Water Pool!" << endl;
        int x = 0, y = 0, tries = 100;
        for(;;) {
          //This hack prevents this from potentially running forever
          //The game is given 100 tries to find an empty spot, if that fails
          //it'll just dump it somewhere in the shaft.
          if(tries == 0) {
            x = SHAFT_CS;
            y = randInt(0, DISTRIBUTED_ER);
            break;
          }
          possibleDistributedPos(x, y,false);
          if(isDirtAt(x, y, x+3, y+3)) {
            tries--;
            continue;
          }
          else break;
        }
        Actor* w = new Water(x,y,this);
        m_actors.push_back(w);
      }
    }


    for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();) {
        if((*it)->isDead()) {
            if((*it)->getType() == ISOIL) {
                m_noOil--;
            }
            if((*it)->getType() == ISPROTESTOR || (*it)->getType() == ISHARDCOREPROTESTOR){
                m_noProtestors--;
            }
            delete (*it);
            it = m_actors.erase(it);
        }
        else
            it++;
    }

    if(m_player->isDead()) {
      decLives();
      m_noProtestors = 0;
      m_ticksSinceLastProtestor = 0;
      return GWSTATUS_PLAYER_DIED;
    }

    if(m_noOil == 0) {
        m_noProtestors = 0;
        m_ticksSinceLastProtestor = 0;
      cerr << "PLAYER WON" << endl;
      return GWSTATUS_FINISHED_LEVEL;
    }

    return GWSTATUS_CONTINUE_GAME;
}

/**
  Cleans the current level
  (see destructor)
 */
void StudentWorld::cleanUp()
{
  for(int x = 0; x < DIRT_C; x++) {
      for(int y = 0; y < DIRT_R; y++) {
          delete m_dirt_arr[x][y];
      }
  }

  for(list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++) {
    delete (*it);
  }

  m_actors.clear();

  delete m_player;
}

/**
 Adds actor to the world
 @param Actor* a
 @return bool - true if succesfully added
 */
bool StudentWorld::addActor(Actor* a) {
    cerr << "Added actor at x:" << a->getX() << " y:" << a->getY() << endl;
    m_actors.push_back(a);
    return true;
}


/**
 Check if 2 actors are close, as defined by a limit
 @param Actor* a1, Actor* a2, int limit
 @return bool - true if near, false otherwise
 */
bool StudentWorld::actorsAreClose(Actor *a, Actor *b, int limit) const {
    return limit >= calculateDistance(a->getX(), a->getY(), b->getX(), b->getY());
}

/**
 Check if a position and an actor are close, as defined by a limit
 @param int x, int y, Actor* a2, int limit
 @return bool - true if near, false otherwise
 */
bool StudentWorld::actorsAreClose(const int x, const int y, Actor *b, int limit) const {
    return limit >= calculateDistance(x, y, b->getX(), b->getY());
}


/**
 Remove dirt from this position
 @param const int& x, const int& y - x & y coords of dirt
 @return true if dirt has been removed
 */
bool StudentWorld::removeDirt(const int x, const int y) {
    if(x < DIRT_C && y < DIRT_R)
      return m_dirt_arr[x][y] -> setDead();
    return false;
}

/**
 Remove dirt from range, inclusive
 @param int sx, int sy, int ex, int ey
 @return true if dirt has been removed
 */
bool StudentWorld::removeDirt(const int sx, const int sy, const int ex, const int ey) {
  bool hasRemovedDirt = false;
  for(int x = sx; x <= ex; x++) {
    for(int y = sy; y <= ey; y++) {
      if(removeDirt(x,y))
        hasRemovedDirt = true;
    }
  }
  return hasRemovedDirt;
};

/**
 Helper function to check if there's dirt within the provided coordinates(inclusive)
 @param int startx, int endx, int starty, int endy
 @return true if there's any dirt at all in the range specified (inclusive)
 */
bool StudentWorld::isDirtAt(const int sx, const int sy, const int ex, const int ey) const {
    if(sx > ex || sy > ey || sx < 0 || sy < 0 ) {
      return false;
    }

    for(int x = sx; x <= ex; ++x) {
      if(x >= DIRT_C) continue;
      for(int y = sy; y <= ey; ++y) {
        if(y >= DIRT_R) continue;
        if(!m_dirt_arr[x][y]->isDead()) {
          return true;
        }
      }
    }
    //cerr << "No dirt found at: sx:"<< sx << " sy:" << sy << " ex:" << ex << " ey:" << ey << endl;
    return false;
}

/**
  Is this location accessible?
  @param int x, int y, Direction dir
  @return true/false
  */
bool StudentWorld::isAccessible(int x, int y, Actor::Direction dir) const {
  if(x < 0 || y < 0 || x >= VIEW_WIDTH || y >= VIEW_HEIGHT) return false;
  int yEndCheck = y;
  int xEndCheck = x;
  switch (dir) {
    case Actor::right:
    case Actor::left:
      yEndCheck+=3;
      break;
    case Actor::down:
    case Actor::up:
      xEndCheck+=3;
      break;
  }
  //cerr << "Checking from x:" << x << " y:" << y << " | ex:" << xEndCheck << " ey:" << yEndCheck << endl;

  return (!isDirtAt(x, y, xEndCheck, yEndCheck) && !hasBlockingActorAt(x, y));
}


/**
 Check if the player is near the actor, given a certain limit
 @param Actor* me, int limit
 @return true if yes else false
 */
bool StudentWorld::playerNearMe(Actor* me, int limit) const {
  return actorsAreClose(me, m_player, limit);
}

/**
 Reveals all actors within a specified radius
 @param int x, int y, int limit
 */
void StudentWorld::revealActorsAt(int x, int y, int limit) {
  for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it) {
      if(actorsAreClose(x, y, (*it), limit)) {
        if(!(*it)->isVisible() && !(*it)->isDead())
          (*it)->setVisible(true);
      }
  }
}

/**
 Check if there's a protestor near the actor, setting other to the pointer to the protestor if true
 @param Actor* me, Actor* &other int limit
 @return true if yes else false
 */
bool StudentWorld::protestorNearMe(Actor *me, Actor *&other, int limit) const {
  for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); it++) {
    int type = (*it)->getType();
    if(type == ISPROTESTOR || type == ISHARDCOREPROTESTOR) {
      if(actorsAreClose(me, (*it), limit)) {
        other = (*it);
        cerr << "Found protestor!" << endl;
        return true;
      }
    }
  }
  return false;
}

/**
  Helper function to check if there's a blocking item in a certain position
  @param int x, int y
  @return true/false
  */
bool StudentWorld::hasBlockingActorAt(const int x, const int y) const {
  for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it) {
      if(!(*it)->isUnpassable()) continue;
      if(actorsAreClose(x, y, (*it), 3)) return true;
  }
  return false;
}

/**
 Helper function to check if there's a blocking item in a certain position, and making sure its not myself
 @param int x, int y
 @return true/false
 */
bool StudentWorld::hasBlockingActorAt(const int x, const int y, const Actor* a) const {
    for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it) {
        if(!(*it)->isUnpassable() || (*it) == a) continue;
        if(actorsAreClose(x, y, (*it), 3)) return true;
    }
    return false;
}

/**
  Helper function to hit actors at a certain position, with specified damage, and if it affects the player
  @param int x, int y, int damage, bool hitsPlayer
  @return true if hits something, false otherwise
  */
bool StudentWorld::causeDamageToActorsAt(const int x, const int y, int damage, bool hitsPlayer, int damageCausingType) {
  bool hasHitSomething = false;
  for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); ++it) {
      if((*it)->isStaticObject()) continue;
      if(actorsAreClose(x, y, (*it), 3)) {
        cerr << "A protestor is hit!" << endl;
        if(!(*it)->decrHealthBy(damage)) {
          //has killed actor
          //TODO: FIGURE SCORE
          if(damageCausingType == ISBOULDER) {
              increaseScore(500);
          } else if(damageCausingType == ISSQUIRT) {
            if((*it)->getType() == ISPROTESTOR) {
              //normal protestor
              increaseScore(100);
            } else if((*it)->getType() == ISHARDCOREPROTESTOR) {
              //hardcore
              increaseScore(250);
            }
          }
        }
        hasHitSomething = true;
      }
  }

  if(hitsPlayer && actorsAreClose(x, y, m_player, 3)) {
    hasHitSomething = true;
    if(damage >= m_player->getHealth())
        m_player->setDead();
    else
        m_player->decrHealthBy(damage);
  }
  return hasHitSomething;
}

/**
  Helper function to calculate a possible posititon of the actor
  @param int& x, int& y
  @return modified x and y
  */
void StudentWorld::possibleDistributedPos(int& x, int& y, bool isBoulder) const {
  int startX = isBoulder ? BOULDER_SR : 0;
  int sx = 0;
  int sy = 0;

  for(;;) {
    sx = randInt(0, DISTRIBUTED_EC);
    sy = randInt(startX, DISTRIBUTED_ER);

    if(!(sx < SHAFT_CS - 4 || sx > SHAFT_CE)) continue;

    bool isTooClose = false;
    for(list<Actor*>::const_iterator it = m_actors.begin(); it != m_actors.end(); it++) {
      isTooClose = calculateDistance((*it)->getX(), (*it)->getY(), sx, sy) < DISTRIBUTED_LIMIT;
      if(isTooClose) break;
    }

    if(!isTooClose) {
      x = sx;
      y = sy;
      //cerr << "Distributing Actor at x:" <<  x << " | y:" << y << endl;
      return;
    }
  }
}

/**
  Finds out if the player is on the same axis and is unobstructed as the given position
  @param int x, int y, Direction &dir
  @return true/false, dir is the relative direction of the player
  */
bool StudentWorld::isPlayerWithinSight(int x, int y, Actor::Direction &dir) const {
  if(m_player->getX() == x) {
    //UP
    for(int i = y; i <= ACTOR_MAXR; i++) {
      if(!isAccessible(x, i, Actor::up)) return false;
      if(m_player->getY() == i){
        //cerr << "Protestor sees player above" << endl;
        dir = Actor::up;
        return true;
      }
    }
    //DOWN
    for(int i = y; i >= 0; i--) {
      if(!isAccessible(x, i, Actor::down)) return false;
      if(m_player->getY() == i){
        //cerr << "Protestor sees player below" << endl;
        dir = Actor::down;
        return true;
      }
    }
  } else if(m_player->getY() == y) {
    //RIGHT
    for(int i = x; i <= ACTOR_MAXC; i++) {
      if(!isAccessible(i, y, Actor::right)) return false;
      if(m_player->getX() == i){
        //cerr << "Protestor sees player right" << endl;
        dir = Actor::right;
        return true;
      }
    }
    //LEFT
    for(int i = x; i >= 0; i--) {
      if(!isAccessible(i, y, Actor::left)) return false;
      if(m_player->getX() == i){
        //cerr << "Protestor sees player left" << endl;
        dir = Actor::left;
        return true;
      }
    }
  }

  return false;

}

void StudentWorld::exploreMapPos(int x, int y, int origin, Actor::Direction dir, queue<P> &q, int m[][VIEW_HEIGHT]) {
  if(m[x][y] == UNSEEN) {
    switch(dir) {
      case Actor::left:
        if(!isAccessible(x, y, dir)) {
            m[x][y] = BLOCKED;
          return;
        }
        break;
      case Actor::right:
        if(!isAccessible(x+3, y, dir)) {
            m[x][y] = BLOCKED;
          return;
        }
        break;
      case Actor::up:
        if(!isAccessible(x, y+3, dir)) {
            m[x][y] = BLOCKED;
          return;
        }
        break;
      case Actor::down:
        if(!isAccessible(x, y, dir)) {
            m[x][y] = BLOCKED;
          return;
        }
        break;
    }
    m[x][y] = origin;
    q.push(P(x,y,origin));
  }
}


/**
  Updates heat map
  */
void StudentWorld::updateHeatMap(int destx, int desty, int map[][VIEW_HEIGHT]) {

  for(int i=0;i<VIEW_WIDTH;i++) {
    for(int j=0;j<VIEW_HEIGHT;j++)
      map[i][j] = UNSEEN;
  }

  queue<P> q;

  q.push(P(destx, desty,0));
  map[destx][desty] = 0;

  while(!q.empty()) {
    P p = q.front();
    q.pop();

    //up
    exploreMapPos(p.x, p.y+1, p.originNum + 1, Actor::up, q, map);
    //down
    exploreMapPos(p.x, p.y-1, p.originNum + 1, Actor::down, q, map);
    //left
    exploreMapPos(p.x-1, p.y, p.originNum + 1, Actor::left, q, map);
    //right
    exploreMapPos(p.x+1, p.y, p.originNum + 1, Actor::right, q, map);
  }
}

void StudentWorld::moveTowardsPlayer(Actor* a, int& nx, int &ny, Actor::Direction &dir) {
  int x = a->getX(), y = a->getY();

  int bestValue = BLOCKED;
  //up
  if(m_toPlayerHeatMap[x][y+1] < bestValue) {
    bestValue = m_toPlayerHeatMap[x][y+1];
    nx = x;
    ny = y+1;
    dir = Actor::up;
  }
  //down
  if(m_toPlayerHeatMap[x][y-1] < bestValue) {
    bestValue = m_toPlayerHeatMap[x][y-1];
    nx = x;
    ny = y-1;
    dir = Actor::down;
  }
  //left
  if(m_toPlayerHeatMap[x-1][y] < bestValue) {
    bestValue = m_toPlayerHeatMap[x-1][y];
    nx = x-1;
    ny = y;
    dir = Actor::left;
  }
  //right
  if(m_toPlayerHeatMap[x+1][y] < bestValue) {
    bestValue = m_toPlayerHeatMap[x+1][y];
    nx = x+1;
    ny = y;
    dir = Actor::right;
  }

}

void StudentWorld::moveTowardsExit(Actor* a, int& nx, int &ny, Actor::Direction &dir) {
  int x = a->getX(), y = a->getY();

  int bestValue = BLOCKED;
  //up
  if(m_toExitHeatMap[x][y+1] < bestValue) {
    bestValue = m_toExitHeatMap[x][y+1];
    nx = x;
    ny = y+1;
    dir = Actor::up;
  }
  //down
  if(m_toExitHeatMap[x][y-1] < bestValue) {
    bestValue = m_toExitHeatMap[x][y-1];
    nx = x;
    ny = y-1;
    dir = Actor::down;
  }
  //left
  if(m_toExitHeatMap[x-1][y] < bestValue) {
    bestValue = m_toExitHeatMap[x-1][y];
    nx = x-1;
    ny = y;
    dir = Actor::left;
  }
  //right
  if(m_toExitHeatMap[x+1][y] < bestValue) {
    bestValue = m_toExitHeatMap[x+1][y];
    nx = x+1;
    ny = y;
    dir = Actor::right;
  }
}

int StudentWorld::stepsToPlayer(int x, int y) const {
  return m_toPlayerHeatMap[x][y];
}


/**
  NOT USED ANY MORE

void StudentWorld::explorePosition(int x, int y, Position* prev, Actor::Direction dir, vector<Position*> &pL, queue<Position*> &tS, int ct[][VIEW_HEIGHT]) {
  if(ct[x][y] == UNSEEN) {
    switch(dir) {
      case Actor::left:
        if(!isAccessible(x, y, dir)) {
          for(int i = y; i <= y+3; i++) {
            ct[x][i] = BLOCKED;
          }
          return;
        }
        break;
      case Actor::right:
        if(!isAccessible(x+3, y, dir)) {
          for(int i = y; i <= y+3; i++) {
            ct[x][i] = BLOCKED;
          }
          return;
        }
        break;
      case Actor::up:
        if(!isAccessible(x, y+3, dir)) {
          for(int i = x; i <= x+3; i++) {
            ct[i][y] = BLOCKED;
          }
          return;
        }
        break;
      case Actor::down:
        if(!isAccessible(x, y, dir)) {
          for(int i = x; i <= x+3; i++) {
            ct[i][y] = BLOCKED;
          }
          return;
        }
        break;
    }
    Position* n = new Position(x,y, prev);
    tS.push(n);
    pL.push_back(n);
    ct[x][y] = SEEN;
  } else if(ct[x][y] == KNOWN) {
    Position* n = new Position(x,y, prev);
    tS.push(n);
    pL.push_back(n);
  }
}
*/

/**
  NOT USED ANYMORE!!!
  @param int cx, int cy, int& nx, int& ny
  @return the values of the next move

void StudentWorld::getNextPosition(int cx, int cy, int &nx, int &ny, Actor::Direction &dir) {
  //Does this position already have a known path?
  //If yes I'm done!
  //Also update the position in the check table
  int mazeCheckTable[VIEW_WIDTH][VIEW_HEIGHT];
  //Initialize my check table
  for(int i=0;i<VIEW_WIDTH;i++) {
    for(int j=0;j<VIEW_HEIGHT;j++)
      mazeCheckTable[i][j] = UNSEEN;
  }

  for(vector<Position*>::iterator it = m_points.begin(); it != m_points.end(); it++) {
    //so i won't have to run through this array every time
    mazeCheckTable[(*it)->x][(*it)->y] = KNOWN;
    if((*it)->x == cx && (*it)->y == cy) {
      //great success!
      Position* nn = (*it)->next;
      nx = nn->x;
      ny = nn->y;
      if(nx > cx) dir = Actor::right;
      if(nx < cx) dir = Actor::left;
      if(ny > cy) dir = Actor::up;
      if(ny < cy) dir = Actor::down;

      return;
    }
  }

  //Ok this is a new position with unknown values. Now what?
  //Time to solve this!

  //Queue of stuff to see
  queue<Position*> toSee;
  vector<Position*> pointsList;

  //Add my current element to the queue
  Position* p = new Position(cx,cy);
  mazeCheckTable[cx][cy] = SEEN;
  toSee.push(p);
  pointsList.push_back(p);

  Position* c = nullptr;

  while(!toSee.empty()) {
    c = toSee.front();
    toSee.pop();

    //exit if i found a known point or im at the end
    if(mazeCheckTable[c->x][c->y] == KNOWN) {
      //c is the known point
      cerr << "found known point" << endl;
      for(vector<Position*>::iterator it = m_points.begin(); it != m_points.end(); it++) {
        if((*it)->x == c->x && (*it)->y == c->y) {
          //set c's previous point's next to the known point
          c->previous->next = *it;
          //set c as the last point
          c = c->previous;
          break;
        }
      }
      break;
    };
    //this is the first run of this algorithm - add the final point to the vector
    if(c->x == ACTOR_MAXC && c->y == ACTOR_MAXR) {
      m_points.push_back(c);
      c->previous->next = c;
      c = c->previous;
      break;
    }

    //UP
    explorePosition(c->x, c->y+1, c, Actor::up, pointsList, toSee, mazeCheckTable);
    //DOWN
    explorePosition(c->x, c->y-1, c, Actor::down, pointsList, toSee, mazeCheckTable);
    //LEFT
    explorePosition(c->x-1, c->y, c, Actor::left, pointsList, toSee, mazeCheckTable);
    //RIGHT
    explorePosition(c->x+1 , c->y, c, Actor::right, pointsList, toSee, mazeCheckTable);
  }

  //I have either reached the end, or found a known point.
  //This entire path needs to be pushed into our points vector
  for(;c->previous != nullptr;c = c->previous) {
    if(c->next == nullptr)
      cerr << "SOMETHING BAD HAS HAPPENED" << endl;
    c->previous->next = c;
    m_points.push_back(c);
  }
  //my original item
  m_points.push_back(c);
  nx = c->next->x;
  ny = c->next->y;
  if(nx > cx) dir = Actor::right;
  if(nx < cx) dir = Actor::left;
  if(ny > cy) dir = Actor::up;
  if(ny < cy) dir = Actor::down;

  //...and we'll need to clean up the unused points
  for(vector<Position*>::iterator plit = pointsList.begin(); plit != pointsList.end();plit++) {
    bool isMatch = false;
    for(vector<Position*>::iterator it = m_points.begin(); it != m_points.end(); it++) {
      if((*it) == (*plit)) {
          isMatch = true;
          //cerr << "MATCH" << endl;

      }
    }
      if(!isMatch) {
          //cerr << "Deleting x:" << (*plit)->x << " y:" << (*plit)->y << endl;
          delete *plit;
      }
  }
}
 */

/**
 Formats ints to strings for output
 @param int num, unsigned int finalLength, char filler
  - takes the int, the desired fixed length of the string and a filler character as arguments
 @return string
  - returns the properly formatted string
 */
string StudentWorld::formatToString(int num, unsigned int finalLength, char filler) {
  string s = to_string(num);
  while(s.length() != finalLength) {
    s.insert(0, 1, filler);
  }
  return s;
}

/**
 Set display text
 */
void StudentWorld::setDisplayText() {
  //"Scr: 0321000 Lvl: 52 Lives: 3 Hlth: 80% Water: 20 Gld: 3 Sonar: 1 Oil Left: 2"
  //The below formatting follows the demo
  string s = "";
  s += "Scr: " + formatToString(getScore(), 6, '0') + "  ";
  s += "Lvl: " + formatToString(getLevel(), 2, ' ') + "  ";
  s += "Lives: " + to_string(getLives()) + "  ";
  s += "Hlth: " + formatToString(m_player->getHealthAsPct(), 3, ' ') + "%  ";
  s += "Wtr: " + formatToString(m_player->getWater(), 2, ' ') + "  ";
  s += "Gld: " + formatToString(m_player->getGold(), 2, ' ') + "  ";
  s += "Sonar: " + formatToString(m_player->getSonar(), 2, ' ') + "  ";
  s += "Oil Left: " + formatToString(getOil(),2, ' ');

  setGameStatText(s);
}

/** Utility Functions **/
int randInt(int begin, int end) {
  end++;
  return begin + rand() % (end - begin);
}

double calculateDistance(int sx, int sy, int ex, int ey) {
    return sqrt((ex-sx)*(ex-sx) + (ey-sy)*(ey-sy));
}
