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

#include "map.h"

// constructor
Map::Map(string fileName,bool debug)
{
  this->debugEnabled = debug;
  
  mapa = NULL;
  if(debugEnabled)
    cout<<"Wywolanie konstruktora mapy."<<endl;
  
  init(fileName);
  allocateMap();
  mapToArray();
}

// destructor
Map::~Map()
{
  revokeMap();
}

// find starting points for ships
Point Map::findStartingPoints()
{
  Point* pkt;

  //vertical searching on the edges
  for(int i = 0;i < height;i++)
  {
    if(mapa[i][0] < 0.7)
    {
      pkt = new Point(0,i);
      vec.push_back(pkt);
    }
    
    if(mapa[i][width - 1] < 0.7)
    {
      pkt = new Point(width - 1,i);
      vec.push_back(pkt);
    }
  }
  //horizontal searching on the edges
  for(int i=0;i<width;i++)
  {
    if(mapa[0][i] < 0.7)
    {
      pkt = new Point(i,0);
      vec.push_back(pkt);
    }
    if(mapa[height - 1][i] < 0.7)
    {
      pkt = new Point(i,height - 1);
      vec.push_back(pkt);
    }
  }
  
  return displayFirstPoints(vec);
}

// display found points
Point Map::displayFirstPoints(vector< Point* >& vec)
{
  Point pkt(0,0);
    
  if(!debugEnabled)
    return pkt;

  if(vec.size() < 1)
    return pkt;
  
  for(unsigned int iter=0;iter < vec.size();iter++)
  {
    cout<<"Starting points X: "<<vec[iter]->x<<" Y: "<<vec[iter]->y<<endl;
  }
  pkt = *(vec[0]);
  
  return pkt;
}

void Map::init(string fileName)
{
  obraz = new Image();
  obraz->read(fileName.c_str());
  
  height = obraz->rows();
  width = obraz->columns();
  depth = obraz->depth();
  
  if(debugEnabled)
  {
    cout << "Height: " << height << endl;
    cout << "Width: " << width << endl;
    cout << "Depth: " << depth << endl;
  }
}

// write virtual map to the file
void Map::writeMapToFile(string fileName)
{
  if(debugEnabled)
    cout<<"Writing to the file: "<<fileName<<endl;
  
  int ret=-1;
  string command = "rm " + fileName;
  fstream str;
  
  ret = system(command.c_str());
  
  if(debugEnabled)
  {
    if(!ret)
      cout<<"System: deleting file "<<fileName<<endl;
    else
      cout<<"System: Error during deleting file "<<fileName<<endl;
  }
  
  str.open(fileName.c_str(),ios_base::out);

  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      if(mapa[i][j] < 0.5)
	str <<"X";
      else
	str <<" ";
    }
    str<<endl;
  }
}

// convert bitmap to the virtual map with roads for the ships
void Map::mapToArray()
{
  for(int i=0;i<height;i++)
  {
    for(int j=0;j<width;j++)
    {
      ColorGray c = obraz->pixelColor(j,i);
      mapa[i][j] = c.shade();
    }
  }
}

// allocate memory for the map
bool Map::allocateMap()
{
  mapa = new double*[height];
  for(int i=0;i<height;i++)
    mapa[i] = new double[width];
  return true;
}


// free memory
void Map::revokeMap()
{
  if(debugEnabled)
    cout<< "Destructor."<<endl;
  
  if(mapa!=NULL)
  {
    for(int i=0;i<width;i++)
      delete[] mapa[i];
    delete[] mapa;
    
    delete obraz;
  }
}

// take color depth
int Map::getDepth()
{
  return depth;
}

// take height of the map
int Map::getHeight()
{
  return height;
}

// take virtual map of the roads
double** Map::getMap()
{
  return mapa;
}

// take the width
int Map::getWidth()
{
  return width;
}
