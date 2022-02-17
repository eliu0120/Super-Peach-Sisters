#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {
    m_peach = nullptr;
    /* // This section is for testing only
    m_actors.push_back(new Peach(0, 0, this));
    m_actors.push_back(new Block(0, 8, 0, this));
    m_actors.push_back(new Block(0, 16, 0, this));
    m_actors.push_back(new Block(0, 25, 0, this));
    for (int i = 0; i < m_actors.size(); i++)
        m_actors[i]->dump(); */
}

int StudentWorld::init() {
    Level lev(assetPath());
    string level_file = "level01.txt";
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_success) {
        // cerr << "Success" << endl; // Testing purposes only - delete
        Level::GridEntry ge;
        int dir;
        for (int i = 0; i < VIEW_HEIGHT; i += 8)
            for (int j = 0; j < VIEW_WIDTH; j += 8) {
                ge = lev.getContentsOf(j / 8, i / 8);
                switch (ge) {
                case Level::peach:
                    m_peach = new Peach(j, i, this);
                    break;
                case Level::block:
                    m_actors.push_back(new Block(j, i, 0, this));
                    break;
                case Level::flower_goodie_block:
                    m_actors.push_back(new Block(j, i, 1, this));
                    break;
                case Level::mushroom_goodie_block:
                    m_actors.push_back(new Block(j, i, 2, this));
                    break;
                case Level::star_goodie_block:
                    m_actors.push_back(new Block(j, i, 3, this));
                    break;
                case Level::pipe:
                    m_actors.push_back(new Pipe(j, i, this));
                    break;
                case Level::goomba:
                    dir = randInt(0, 1);
                    m_actors.push_back(new Goomba(j, i, this, dir * 180));
                    break;
                case Level::koopa:
                    dir = randInt(0, 1);
                    m_actors.push_back(new Koopa(j, i, this, dir * 180));
                    break;
                case Level::empty:
                default:
                    break;
                }
            }
    } else {
        cerr << "Level not found" << endl;
        return GWSTATUS_LEVEL_ERROR;
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (m_peach->isAlive())
        m_peach->doSomething();
    for (int i = 0; i < m_actors.size(); i++)
        if (m_actors[i]->isAlive()) {
            m_actors[i]->doSomething();
            if (!m_peach->isAlive()) {
                playSound(SOUND_PLAYER_DIE);
                return GWSTATUS_PLAYER_DIED;
            }
        }
    for (int i = 0; i < m_actors.size(); i++)
        if (!m_actors[i]->isAlive()) {
            delete m_actors[i];
            m_actors.erase(m_actors.begin() + i);
            i--;
        }
    ostringstream oss;
    oss << "Lives: " << getLives() << " Level: " << getLevel() << " Points: " << getScore();
    if (m_peach->getShootPower())
        oss << " ShootPower!";
    if (m_peach->getJumpPower())
        oss << " JumpPower!";
    if (m_peach->getStarPower())
        oss << " StarPower!";
    string statText = oss.str();
    setGameStatText(statText);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    int i = 0;
    // int j = 0; // For testing purposes
    delete m_peach;
    while (i < m_actors.size()) {
        Actor* kill = m_actors[m_actors.size() - 1];
        m_actors.pop_back();
        delete kill;
        // j++; // For testing purposes
    }
    // cerr << "Destructed " << j << " items!" << endl; // For testing purposes
}

bool StudentWorld::collideWall(double x, double y, bool bonk) {
    for (int i = 0; i < m_actors.size(); i++) // Change to i = 1 later - flag/mario will become m_actors[0]
        if (m_actors[i]->doesBlock() && (x - 7 <= m_actors[i]->getX() && x + 7 >= m_actors[i]->getX()) && (y - 7 <= m_actors[i]->getY() && y + 7 >= m_actors[i]->getY())) {
            if (bonk)
                m_actors[i]->bonk();
            return true;
        }
    return false;
}

bool StudentWorld::overlapPeach(double x, double y) {
    return (x - 7 <= m_peach->getX() && x + 7 >= m_peach->getX()) && (y - 7 <= m_peach->getY() && y + 7 >= m_peach->getY());
}

bool StudentWorld::isOverlap(double x, double y, bool bonk) {
    for (int i = 0; i < m_actors.size(); i++) // Change to i = 1 later - flag/mario will become m_actors[1]
        if ((x - 7 <= m_actors[i]->getX() && x + 7 >= m_actors[i]->getX()) && (y - 7 <= m_actors[i]->getY() && y + 7 >= m_actors[i]->getY()))
            if (bonk) {
                m_actors[i]->bonk();
                return true;
            } else if (m_actors[i]->isDamageable()) {
                m_actors[i]->kill();
                return true;
            }
    return false;
}

void StudentWorld::newPowerUp(int powerUp, int startX, int startY) {
    switch (powerUp) { // Are we allowed to identify different power ups like this?
    case FLOWER:
        m_actors.push_back(new Flower(startX, startY, this));
        break;
    case MUSHROOM:
        m_actors.push_back(new Mushroom(startX, startY, this));
        break;
    case STAR:
        m_actors.push_back(new Star(startX, startY, this));
        break;
    }
}

void StudentWorld::applyPeachPowerUp(int powerUp) {
    m_peach->setPower(powerUp);
}

void StudentWorld::newPeachFireball(int startX, int startY, int dir) {
    m_actors.push_back(new PeachFireball(startX, startY, this, dir));
}

bool StudentWorld::isPeachStarPower() const {
    return m_peach->getStarPower();
}

void StudentWorld::bonkPeach() {
    m_peach->bonk();
}

StudentWorld::~StudentWorld() {
    cleanUp();
}
