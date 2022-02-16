#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Peach;

class StudentWorld : public GameWorld {
public:
	StudentWorld(string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool collideWall(double x, double y, bool bonk);
	bool overlapPeach(double x, double y);
	void isOverlap(double x, double y, bool bonk);
	void newPowerUp(int powerUp, int startX, int startY);
	void applyPeachPowerUp(int powerUp);
	void newPeachFireball(int startX, int startY, int dir);
	~StudentWorld();

private:
	vector<Actor*> m_actors;
	Peach* m_peach;
};

#endif // STUDENTWORLD_H_
