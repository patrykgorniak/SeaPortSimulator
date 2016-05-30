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

BoardManager::BoardManager(string resourcePath, bool debug):
    m_board(NULL),
    debugEnabled(debug),
    resourcePath(resourcePath)
{
    init();
}

BoardManager::~BoardManager()
{
    destroyBoard();
}

void BoardManager::init()
{
    this->height = 719;
    this->width = 1024;

    if(debugEnabled) {
        cout << "Height: " << this->height << endl;
        cout << "Width: " << this->width << endl;
    }

    allocateBoard();
    readBoardFromFile();
    findStartingPoints();
}

// take height of the map
int BoardManager::getHeight()
{
    return height;
}

// take the width
int BoardManager::getWidth()
{
    return width;
}

// take virtual map of the roads
Board BoardManager::getBoard()
{
    return m_board;
}

// allocate memory for the map
void BoardManager::allocateBoard()
{
    m_board = new double*[height];
    for(int i=0; i<height; i++)
        m_board[i] = new double[width]();
}

// free memory
void BoardManager::destroyBoard()
{
    if(m_board!=NULL)
    {
        for(int i=0; i<height; i++)
            delete[] m_board[i];
        delete[] m_board;
    }
}

StartingPoints BoardManager::getStartingPoints()
{
    return m_startingPoints;
}

// find starting points for ships
void BoardManager::findStartingPoints()
{
    Point* pkt;
    int counter = 0;

    //vertical searching on the edges
    for(int i = 0; i < height; i++)
    {
        if(m_board[i][0] == 1)
        {
            counter++;
            pkt = new Point(0,i);
            m_startingPoints.push_back(pkt);
            if(debugEnabled)
                cout<<"Found point: x="<<pkt->x<<" y="<<pkt->y<<endl;
        }

        if(m_board[i][width - 1] == 1)
        {
            counter++;
            pkt = new Point(width - 1,i);
            m_startingPoints.push_back(pkt);
            if(debugEnabled)
                cout<<"Found point: x="<<pkt->x<<" y="<<pkt->y<<endl;
        }
    }
    //horizontal searching on the edges
    for(int i=0; i<width; i++)
    {
        if(m_board[0][i] == 1)
        {
            counter++;
            pkt = new Point(i,0);
            m_startingPoints.push_back(pkt);
            if(debugEnabled)
                cout<<"Found point: x="<<pkt->x<<" y="<<pkt->y<<endl;

        }
        if(m_board[height - 1][i] == 1)
        {
            counter++;
            pkt = new Point(i,height - 1);
            m_startingPoints.push_back(pkt);
            if(debugEnabled)
                cout<<"Found point: x="<<pkt->x<<" y="<<pkt->y<<endl;
        }
    }

    if(debugEnabled)
        cout<<"Found "<<counter<<" points"<<endl;
}

// display found points
void BoardManager::displayFirstPoints(StartingPoints& startingPoints)
{
    for(unsigned int iter=0; iter < startingPoints.size(); iter++)
    {
        cout<<"Starting points X: "<<startingPoints[iter]->x<<" Y: "<<startingPoints[iter]->y<<endl;
    }
}

// write virtual map to the file
void BoardManager::saveToFile(string fileName)
{
    int ret=-1;
    fstream str;

    if(debugEnabled)
        cout<<"Writing to the file: "<<fileName<<endl;

    string command = "rm " + fileName;
    ret = system(command.c_str());

    if(debugEnabled) {
        if(!ret)
            cout<<"System: deleting file "<<fileName<<endl;
        else
            cout<<"System: Error during deleting file "<<fileName<<endl;
    }

    str.open(fileName.c_str(),ios_base::out);

    for(int i=0; i<height; i++) {
        for(int j=0; j<width; j++) {
            if(m_board[i][j])
                str <<"o";
            else
                str <<'.';
        }
        str<<endl;
    }
}

// convert bitmap to the virtual map with roads for the ships
void BoardManager::readBoardFromFile()
{
    ifstream file;
    string line;
    cout<< resourcePath<<endl;
    file.open(resourcePath + "maps/map_1.bin");
    int x,y;
    if(file.is_open())
    {
        while(getline(file, line, ';'))
        {
            std::stringstream   linestream(line);
            linestream >> x >> y;
            m_board[y][x] = 1;
        }
        file.close();
    }
    else
    {
        cout<<"Error: Cannot read board from file. Exiting..."<<endl;
        exit(1);
    }
}
