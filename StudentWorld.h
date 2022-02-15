#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld {
public:
	StudentWorld(string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool collideWall(double x, double y, int& itr);
	void bonkActor(int itr);
	~StudentWorld();

private:
	vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
