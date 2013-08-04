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
#include "ship.h"


// Constructor
Ship::Ship(int x, int y, int road, double** map, int mapWidth, int mapHeight, int delay, pthread_mutex_t& mutex_,bool debug): mutex(mutex_)
{
    // Initializing variables
    this->debugEnabled = debug;

    this->road = road;

    this->next.x = -1;
    this->next.y = -1;

    this->isInPort = false;

    this->alive = true;
    this->delay = delay;

    this->current.x = x;
    this->current.y = y;

    this->previous.x = x;
    this->previous.y = y;

    this->mapHeight = mapHeight;
    this->mapWidth = mapWidth;

    this->map = map;
}

// take the road for ship
int Ship::getRoad()
{
    return road;
}

// take current x position of the ship
int Ship::get_x()
{
    return current.x;
}

// take current y position of the ship
int Ship::get_y()
{
    return current.y;
}

// calculate next point for move
void Ship::move()
{
    while(alive)
    {
        pthread_mutex_lock (&mutex);
        calculateNextPoint();
        pthread_mutex_unlock(&mutex);

        // We are in sea port
        if(isInPort)
        {
            // turn around the ship
            swap(current,previous);

            if(debugEnabled)
                cout<<"We are in sea port"<<endl;

            usleep(timeInPort(2,10)*900000);
        }
        isInPort = false;

        usleep(delay*500);
    }
}

// check if the ship is on or off the map
bool Ship::isAlive()
{
    return alive;
}

// take the avatar type
int Ship::getAvatarType()
{
    return avatarType;
}

// set avatar type ( model of ship ex. small/big)
void Ship::setAvatarType(int avatarType)
{
    this->avatarType = avatarType;
}

// remember next move
void Ship::changePoints()
{
    previous = current;
    current = next;
}


// calculate next move
void Ship::calculateNextPoint()
{
    // go up
    if( (previous.y - current.y) > 0)
    {
        if(findNextPoint('u'))
        {
            changePoints();
            return;
        }

        if(findNextPoint('l'))
        {
            changePoints();
            return;
        }

        if(findNextPoint('r'))
        {
            changePoints();
            return;
        }

        if(debugEnabled)
            cout<<"Top part of the port"<<endl;

        isInPort = true;
    }
    // go down
    else if((previous.y - current.y) < 0)
    {
        if(findNextPoint('d'))
        {
            changePoints();
            return;
        }

        if(findNextPoint('l'))
        {
            changePoints();
            return;
        }

        if(findNextPoint('r'))
        {
            changePoints();
            return;
        }

        if(debugEnabled)
            cout<<"Bottom part of the map"<<endl;

        isInPort = true;
    }
    // horizontal move
    else if( (previous.y - current.y) == 0)
    {
        // start or end of the trip
        if( (previous.x - current.x) == 0)
        {
            if(findNextPoint('u'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('l'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('r'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('d'))
            {
                changePoints();
                return;
            }

            isInPort = true;
        }
        // go left
        else if( (previous.x - current.x) > 0)
        {
            if(findNextPoint('l'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('u'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('d'))
            {
                changePoints();
                return;
            }

            isInPort = true;
        }
        // go right
        else if( (previous.x - current.x) < 0 )
        {
            if(findNextPoint('r'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('u'))
            {
                changePoints();
                return;
            }

            if(findNextPoint('d'))
            {
                changePoints();
                return;
            }

            isInPort = true;
        }
    }
}

// random value from [min,max]
int Ship::timeInPort(int min,int max)
{
    int time;
    time = rand()%(max - min);
    time += min;
    return time;
}

bool Ship::findNextPoint(char direction)
{
    switch(direction)
    {
    case 'l':
        if(findOnLeft())
            return true;
        break;

    case 'r':
        if(findOnRight())
            return true;
        break;

    case 'u':
        if(findAbove())
            return true;
        break;

    case 'd':
        if(findBelow())
            return true;
        break;

    default:
        break;
    }

    return false;
}


// find next free point above current position
bool Ship::findAbove()
{

    //  o q o
    // 	x
    //  	x
    // find next point above
    if ( ( current.y >= 1) && (map[current.y - 1][current.x] < 0.7) )
    {
        avatar = 6;
        next.x = current.x;
        next.y = current.y - 1;
        return true;
    }
    //  q o o
    // 	x
    //  	x
    // find next point above and on the left
    else if( ( current.y >= 1) && (current.x - 1) >= 0 && map[current.y - 1][current.x - 1] < 0.7 )
    {
        avatar = 5;
        next.x = current.x - 1;
        next.y = current.y - 1;
        return true;
    }
    //  o o q
    // 	x
    //  	x
    // find next point uabove and on the right
    else if( ( current.y >= 1) && (current.x + 1) < mapWidth && map[current.y - 1][current.x + 1] < 0.7 )
    {
        avatar = 7;
        next.x = current.x + 1;
        next.y = current.y - 1;
        return true;
    }

    // ship above the upper edge of the map
    if(next.y == 0 || next.x == 0)
    {
        if(debugEnabled)
            cout<<"Ship above the upper edge of the map"<<endl;

        alive = false;
    }

    return false;
}

// find next free point to the right of current position
bool Ship::findOnRight()
{
    if ( ( current.y >= 1) && (current.x < (mapWidth - 1) ) && map[current.y][current.x + 1] < 0.7 )
    {
        avatar = 0;
        next.x = current.x + 1;
        next.y = current.y;
        return true;
    }

    // ship to the right of the right edge of the map
    if(next.x == mapWidth - 1)
    {
        if(debugEnabled)
            cout<<"Ship to the right of the right edge of the map"<<endl;

        alive = false;
    }

    return false;
}

// find next free point below current position
bool Ship::findBelow()
{
    // find next free point below
    if ( (current.x >= 0 && current.x < mapWidth && current.y < (mapHeight - 1)) && map[current.y + 1][current.x] < 0.7 )
    {
        avatar = 2;
        next.x = current.x;
        next.y = current.y + 1;
        return true;
    }

    // find next free point below and on the left
    else if ( (current.x > 0 && current.x < mapWidth && current.y < (mapHeight - 1)) && map[current.y + 1][current.x - 1] < 0.7 )
    {
        avatar = 3;
        next.x = current.x - 1;
        next.y = current.y + 1;
        return true;
    }
    // find next free point below and on the right
    else if ( (current.x >= 0  && current.x < mapWidth && current.y < (mapHeight - 1)) && map[current.y + 1][current.x + 1] < 0.7 )
    {
        avatar = 1;
        next.x = current.x + 1;
        next.y = current.y + 1;
        return true;
    }

    // Ship below the bottom edge of the map
    if(next.y == mapHeight-1 || next.x == mapWidth-1)
    {
        if(debugEnabled)
            cout<<"Ship below the bottom edge of the map"<<endl;

        alive = false;
    }

    return false;
}


// find next free point on the left of the current position
bool Ship::findOnLeft()
{
    if ( current.x > 0 && map[current.y][current.x - 1] < 0.7 )
    {
        avatar = 4;
        next.x = current.x - 1;
        next.y = current.y;
        return true;
    }

    //  Ship on the left of the left edge of the map
    if(next.x == 0)
    {
        if(debugEnabled)
            cout<<"Ship on the left of the left edge of the map"<<endl;

        alive = false;
    }

    return false;
}

// set avatar for the ship
void Ship::setAvatar(int avatar)
{
    this->avatar = avatar;
}

// destructor
Ship::~Ship()
{

}

// take avatar for the ship
int Ship::getAvatar()
{
    return avatar;
}
