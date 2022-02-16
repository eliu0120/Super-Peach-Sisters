#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {
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
        for (int i = 0; i < VIEW_HEIGHT; i += 8)
            for (int j = 0; j < VIEW_WIDTH; j += 8) {
                ge = lev.getContentsOf(j / 8, i / 8);
                switch (ge) {
                case Level::peach:
                    m_actors.push_back(m_actors[0]);
                    m_actors[0] = new Peach(j, i, this);
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
    for (int i = 0; i < m_actors.size(); i++) {
        if (m_actors[i]->isAlive()) {
            m_actors[i]->doSomething();
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    int i = 0;
    // int j = 0; // For testing purposes
    while (i < m_actors.size()) {
        Actor* kill = m_actors[m_actors.size() - 1];
        m_actors.pop_back();
        delete kill;
        // j++; // For testing purposes
    }
    // cerr << "Destructed " << j << " items!" << endl; // For testing purposes
}

bool StudentWorld::collideWall(double x, double y, int& itr) {
    for (int i = 1; i < m_actors.size(); i++) // Change to i = 2 later - flag/mario will become m_actors[1]
        if (m_actors[i]->doesBlock() && (x - 4 <= m_actors[i]->getX() && x + 4 >= m_actors[i]->getX()) && (y - 4 <= m_actors[i]->getY() && y + 4 >= m_actors[i]->getY())) {
            itr = i;
            return true;
        }
    return false;
}

void StudentWorld::bonkActor(int itr) {
    m_actors[itr]->bonk();
}

void StudentWorld::newPowerUp(int powerUp, int startX, int startY) {
    switch (powerUp) {
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

StudentWorld::~StudentWorld() {
    cleanUp();
}
