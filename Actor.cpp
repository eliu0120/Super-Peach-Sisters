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

void Actor::bonk() {
	return;
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

void Actor::kill() {
	m_isAlive = false;
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
	m_remaining_time_starPower = 0;
	isStarPower = false;
	isShootPower = false;
	isJumpPower = false;
	isInvincible = false;
}

void Peach::doSomething() {
	if (!isAlive())
		return;
	if (m_remaining_time_starPower > 0) {
		m_remaining_time_starPower--;
		if (m_remaining_time_starPower == 0)
			setPower(false, STAR);
	}
	if (m_remaining_time_recharge > 0)
		m_remaining_time_recharge--;
	if (m_remaining_jump_distance > 0)
		if (getWorldPtr()->collideWall(getX(), getY() + 4, true))
			m_remaining_jump_distance = 0;
		else {
			moveTo(getX(), getY() + 4);
			m_remaining_jump_distance--;
		}
	else if (!getWorldPtr()->collideWall(getX(), getY() - 4, false))
		moveTo(getX(), getY() - 4);
	getWorldPtr()->isOverlap(getX(), getY(), true);
	int ch;
	if (getWorldPtr()->getKey(ch)) {
		switch (ch) {
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (!getWorldPtr()->collideWall(getX() - 4, getY(), true))
				moveTo(getX() - 4, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!getWorldPtr()->collideWall(getX() + 4, getY(), true))
				moveTo(getX() + 4, getY());
			break;
		case KEY_PRESS_UP:
			if (getWorldPtr()->collideWall(getX(), getY() - 4, false)) {
				if (!isJumpPower)
					m_remaining_jump_distance = 8;
				else
					m_remaining_jump_distance = 12;
				getWorldPtr()->playSound(SOUND_PLAYER_JUMP);
			}
			break;
		case KEY_PRESS_SPACE:
			if (!isShootPower || m_remaining_time_recharge > 0)
				break;
			getWorldPtr()->playSound(SOUND_PLAYER_FIRE);
			m_remaining_time_recharge = 8;
			if (getDirection() == left)
				getWorldPtr()->newPeachFireball(getX() - 4, getY(), left);
			else
				getWorldPtr()->newPeachFireball(getX() + 4, getY(), right);
			break;
		default:
			break;
		}
	}
}

void Peach::bonk() {
	cerr << "Ouch!" << endl;
}

void Peach::setHP(int hp) {
	m_hp = hp;
}

bool Peach::getStarPower() const {
	return isStarPower;
}

bool Peach::getShootPower() const {
	return isShootPower;
}

bool Peach::getJumpPower() const {
	return isJumpPower;
}

void Peach::setPower(bool activated, int powerUp) {
	switch (powerUp) {
	case FLOWER:
		isShootPower = activated;
		// cerr << "Shoot Power!" << endl; // Testing only
		break;
	case MUSHROOM:
		isJumpPower = activated;
		// cerr << "Jump Power!" << endl; // Testing only
		break;
	case STAR:
		isStarPower = activated;
		if (activated)
			m_remaining_time_starPower = 150;
		// cerr << "Star power!" << endl; // Testing only
		break;
	}
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
	if (m_goodie == NO_GOODIE)
		getWorldPtr()->playSound(SOUND_PLAYER_BONK);
	else {
		getWorldPtr()->playSound(SOUND_POWERUP_APPEARS);
		getWorldPtr()->newPowerUp(m_goodie, getX(), getY() + 8); // Are we allowed to identify different power ups like this?
		m_goodie = NO_GOODIE;
	}
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

// PowerUp class functions
PowerUp::PowerUp(int imageID, int startX, int startY, StudentWorld* worldPtr) : Actor(imageID, startX, startY, worldPtr, 0, 1, 1.0) {

}

void PowerUp::doSomething() {
	if (getWorldPtr()->overlapPeach(getX(), getY())) {
		getWorldPtr()->playSound(SOUND_PLAYER_POWERUP);
		getWorldPtr()->applyPeachPowerUp(getPowerUp());
		getWorldPtr()->increaseScore(25 + 25 * getPowerUp());
		kill();
		return;
	}
	if (!getWorldPtr()->collideWall(getX(), getY() - 2, false))
		moveTo(getX(), getY() - 2);
	switch (getDirection()) {
	case left:
		if (getWorldPtr()->collideWall(getX() - 2, getY(), false)) {
			setDirection(right);
			return;
		} else
			moveTo(getX() - 2, getY());
		break;
	case right:
		if (getWorldPtr()->collideWall(getX() + 2, getY(), false)) {
			setDirection(left);
			return;
		}
		else
			moveTo(getX() + 2, getY());
		break;
	}
}

bool PowerUp::isDamageable() const {
	return false;
}

// Flower class functions
Flower::Flower(int startX, int startY, StudentWorld* worldPtr) : PowerUp(IID_FLOWER, startX, startY, worldPtr) {

}

int Flower::getPowerUp() const {
	return FLOWER;
}

// Mushroom class functions
Mushroom::Mushroom(int startX, int startY, StudentWorld* worldPtr) : PowerUp(IID_MUSHROOM, startX, startY, worldPtr) {

}

int Mushroom::getPowerUp() const {
	return MUSHROOM;
}

// Star class functions
Star::Star(int startX, int startY, StudentWorld* worldPtr) : PowerUp(IID_STAR, startX, startY, worldPtr) {

}

int Star::getPowerUp() const {
	return STAR;
}

// Projectile class functions
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* worldPtr, int dir = 0) : Actor(imageID, startX, startY, worldPtr, dir, 1, 1.0) {

}

void Projectile::doSomething() {
	if (damage())
		return;
	if (!getWorldPtr()->collideWall(getX(), getY() - 2, false))
		moveTo(getX(), getY() - 2);
	switch (getDirection()) {
	case left:
		if (getWorldPtr()->collideWall(getX() - 2, getY(), false)) {
			kill();
			return;
		}
		else
			moveTo(getX() - 2, getY());
		break;
	case right:
		if (getWorldPtr()->collideWall(getX() + 2, getY(), false)) {
			kill();
			return;
		}
		else
			moveTo(getX() + 2, getY());
		break;
	}
}

bool Projectile::isDamageable() const {
	return false;
}

bool Projectile::damage() {
	if (getWorldPtr()->isOverlap(getX(), getY(), false)) {
		kill();
		return true;
	}
	return false;
}

// PeachFireball class functions
PeachFireball::PeachFireball(int startX, int startY, StudentWorld* worldPtr, int dir) : Projectile(IID_PEACH_FIRE, startX, startY, worldPtr, dir) {

}

void PeachFireball::isAbstract() {
	return;
}

// Enemy class functions
Enemy::Enemy(int imageID, int startX, int startY, StudentWorld* worldPtr, int dir) : Actor(imageID, startX, startY, worldPtr, dir) {

}

void Enemy::doSomething() {
	if (!isAlive())
		return;
	if (getWorldPtr()->overlapPeach(getX(), getY())) {
		getWorldPtr()->bonkPeach();
		return;
	}
	switch (getDirection()) {
	case left:
		if (getWorldPtr()->collideWall(getX() - 1, getY(), false) || !getWorldPtr()->collideWall(getX() - 8, getY() - 1, false))
			setDirection(right);
		break;
	case right:
		if (getWorldPtr()->collideWall(getX() + 1, getY(), false) || !getWorldPtr()->collideWall(getX() + 8, getY() - 1, false))
			setDirection(left);
		break;
	}
	switch (getDirection()) {
	case left:
		if (getWorldPtr()->collideWall(getX() - 1, getY(), false) || !getWorldPtr()->collideWall(getX() - 8, getY() - 1, false))
			return;
		moveTo(getX() - 1, getY());
		break;
	case right:
		if (getWorldPtr()->collideWall(getX() + 1, getY(), false) || !getWorldPtr()->collideWall(getX() + 8, getY() - 1, false))
			return;
		moveTo(getX() + 1, getY());
		break;
	}
}

void Enemy::bonk() {
	if (getWorldPtr()->isPeachStarPower()) {
		getWorldPtr()->playSound(SOUND_PLAYER_KICK);
		kill();
	}
}

void Enemy::kill() {
	getWorldPtr()->increaseScore(100);
	Actor::kill();
}

// Goomba class functions
Goomba::Goomba(int startX, int startY, StudentWorld* worldPtr, int dir) : Enemy(IID_GOOMBA, startX, startY, worldPtr, dir) {

}

void Goomba::isAbstract() {
	return;
}