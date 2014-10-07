/*
    "Port Demo" is an application which simulate seaport.
    Copyright (C) 2010  Patryk Górniak <patryk.lukasz.gorniak@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/



#include "pthread.h"
#include "iostream"
#include "stdlib.h"
#include <unistd.h>
using namespace std;


#ifndef SHIP_H
#define SHIP_H

class Point {
public:
    int x;
    int y;

    Point() {
    }

    Point(int x_,int y_) {
        this->x = x_;
        this->y = y_;
    }
};

class Ship {
public:
    Ship(int x, int y,int road, double** map,int mapWidth,int mapHeight, int delay, pthread_mutex_t& mutex_,bool debug);
    ~Ship();
    int get_x();
    int get_y();
    void move();
    void setAvatar(int);
    void setAvatarType(int);
    int getAvatarType();
    int getAvatar();
    bool isAlive();
    int getRoad();
    bool isFinished();

private:
    Point previous,current,next;
    bool alive;
    int avatar,delay,mapWidth,mapHeight,road,avatarType;
    double** map;
    bool isInPort;
    bool debugEnabled;
    bool finihed;

    pthread_mutex_t& mutex;

    void calculateNextPoint();
    bool findNextPoint(char);
    bool findOnLeft();
    bool findOnRight();
    bool findAbove();
    bool findBelow();
    void changePoints();
    int timeInPort(int min,int max);
};

#endif // SHIP_H
