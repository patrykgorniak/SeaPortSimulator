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

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "stdlib.h"
#include "sstream"

#include "map.h"
#include "ship.h"

#ifndef DEMO_H
#define DEMO_H

class demo
{
public:
    demo(string fileName, int maxShips, int delay,bool debug);
    ~demo();
    void play();

private:
    SDL_Surface* image;
    SDL_Surface* screen;
    SDL_Event event;
    SDL_Surface* AvatarShips[2][8];

    Map *map;
    Ship** ships;

    int maxShips,curShips,delay;
    static int max;

    bool debugEnabled;

    pthread_mutex_t mutex;
    pthread_t *thread;

    Point startPoint;
    vector<int> freeRoad;
    vector<Point*> existingRoad;

    SDL_Surface * loadImage(string);

    int randType(int);
    void initSDL(string);
    void initObjects(string);
    void applySurface( int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
    void loadAvatarShip(string path, string name);
    int getRoad();
    int customDelay(int min,int max);
};

#endif // DEMO_H
