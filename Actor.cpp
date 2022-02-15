#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h""
#include <iostream>
using namespace std;

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
// Actor class functions
Actor::Actor(int imageID, int startX, int startY, StudentWorld* worldPtr, int dir = 0, int depth = 0, double size = 1.0) : GraphObject(imageID, startX, startY, dir, depth, size) {
	m_isAlive = true;
	m_worldPtr = worldPtr;
}

bool Actor::doesBlock() const {
	return false;
}

bool Actor::isDamageable() const {
	return true;
}

void Actor::dump() const { // Testing only
	cerr << doesBlock() << " " << isDamageable() << endl;
}

StudentWorld* Actor::getWorldPtr() const {
	return m_worldPtr;
}

bool Actor::isAlive() const {
	return m_isAlive;
}

Actor::~Actor() {
	// cerr << "Destructing!" << endl; // For testing purposes only
}

// Peach class functions
Peach::Peach(int startX, int startY, StudentWorld* worldPtr) : Actor(IID_PEACH, startX, startY, worldPtr) {
	m_hp = 1;
	m_remaining_jump_distance = 0;
	m_remaining_time_invincible = 0;
	m_remaining_time_recharge = 0;
	isStarPower = false;
	isShootPower = false;
	isJumpPower = false;
	isInvincible = false;
}

void Peach::doSomething() {
	int ch;
	if (getWorldPtr()->getKey(ch)) {
		int itr;
		switch (ch) {
		case KEY_PRESS_LEFT:
			setDirection(180);
			if (getWorldPtr()->collideWall(getX() - 8, getY(), itr))
				getWorldPtr()->bonkActor(itr);
			else
				moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(0);
			if (getWorldPtr()->collideWall(getX() + 8, getY(), itr))
				getWorldPtr()->bonkActor(itr);
			else
				moveTo(getX() + 4, getY());
			break;
		default:
			break;
		}
	}
}

void Peach::bonk() {
	return;
}

void Peach::decreaseHP() {
	m_hp--;
}

// Wall Class functions
Wall::Wall(int imageID, int startX, int startY, StudentWorld* worldPtr) : Actor(imageID, startX, startY, worldPtr, 0, 2, 1.0) {

}

void Wall::doSomething() {
	return;
}

bool Wall::doesBlock() const {
	return true;
}

bool Wall::isDamageable() const {
	return false;
}

// Block class functions
Block::Block(int startX, int startY, int goodie, StudentWorld* worldPtr) : Wall(IID_BLOCK, startX, startY, worldPtr) {
	m_goodie = goodie;
}

void Block::bonk() {
	getWorldPtr()->playSound(SOUND_PLAYER_BONK);
	// cerr << "Bonk!" << endl; // Testing only
	return;
}

// Pipe class functions
Pipe::Pipe(int startX, int startY, StudentWorld* worldPtr) : Wall(IID_PIPE, startX, startY, worldPtr) {

}

void Pipe::bonk() {
	getWorldPtr()->playSound(SOUND_PLAYER_BONK);
	// cerr << "Bonk!" << endl; // Testing only
	return;
}