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
#include <iostream>
#include "string.h"
#include "fstream"
#include "vector"
#include <assert.h>
#include <sstream>
#include "iostream"
#include "ship.h"

using namespace std;

#ifndef SCENE_H
#define SCENE_H

typedef double ** Board;
typedef vector<Point*> StartingPoints;

class BoardManager
{
public:
    BoardManager(string resourcePath, bool debug);
    ~BoardManager();

    void saveToFile(string);

    int getWidth();
    int getHeight();

    Board getBoard();
    StartingPoints getStartingPoints();

private:
    Board m_board;
    StartingPoints m_startingPoints;
    int width, height;
    bool debugEnabled;
    std::string resourcePath;

    void init();
    void allocateBoard();
    void destroyBoard();
    void readBoardFromFile();
    void findStartingPoints();

    void displayFirstPoints(StartingPoints &startingPoints);
};

#endif // SCENE_H
