#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, StudentWorld* worldPtr, int dir, int depth, double size);
	virtual void doSomething() = 0;
	virtual void bonk() = 0;
	virtual bool doesBlock() const;
	virtual bool isDamageable() const;
	virtual void dump() const;
	StudentWorld* getWorldPtr() const;
	bool isAlive() const;
	virtual ~Actor();

private:
	bool m_isAlive;
	StudentWorld* m_worldPtr;
};

class Peach : public Actor {
public:
	Peach(int startX, int startY, StudentWorld* worldPtr);
	virtual void doSomething();
	virtual void bonk();
	void decreaseHP();

private:
	int m_hp;
	int m_remaining_jump_distance;
	int m_remaining_time_invincible;
	int m_remaining_time_recharge;
	bool isStarPower;
	bool isShootPower;
	bool isJumpPower;
	bool isInvincible;
};

class Wall : public Actor {
public:
	Wall(int imageID, int startX, int startY, StudentWorld* worldPtr);
	virtual void doSomething();
	virtual bool doesBlock() const;
	virtual bool isDamageable() const;
};

class Block : public Wall {
public:
	Block(int startX, int startY, int goodie, StudentWorld* worldPtr);
	virtual void bonk();

private:
	int m_goodie;
	const int NO_GOODIE = 0;
	const int FLOWER = 1;
	const int MUSHROOM = 2;
	const int STAR = 3;
};

class Pipe : public Wall {
public:
	Pipe(int startX, int startY, StudentWorld* worldPtr);
	virtual void bonk();
};

#endif // ACTOR_H_