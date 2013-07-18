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
#include <ImageMagick/Magick++.h>
#include "string.h"
#include "fstream"
#include "vector"

#include "ship.h"

using namespace std;
using namespace Magick;

#ifndef SCENE_H
#define SCENE_H 

class Map
{
  public:
    Map(string fileName,bool debug);
    ~Map();
    
    Point findStartingPoints();
    void writeMapToFile(string);
    
    int getWidth();
    int getHeight();
    int getDepth();
    double** getMap();
    vector<Point*> vec;
    
  private:
    Image *obraz;
    double **mapa;
    int width,height,depth;
    
    bool debugEnabled;
    
    bool allocateMap();
    void revokeMap();
    void mapToArray();
    void init(string fileName);

    Point displayFirstPoints(vector<Point*>&);
};

#endif // SCENE_H
