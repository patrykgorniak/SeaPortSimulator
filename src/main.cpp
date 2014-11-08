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


#include "demo.h"

bool help()
{
    char answer;
    cout<<"You have run application wihtout any arguments."<<endl;
    cout<<"You can use following parameters:"<<endl;
    cout<<"Ships count (max 5) - default = 5"<<endl;
    cout<<"Delay [10 - 100] - default = 80"<<endl;
    cout<<"Debug [ 0 / 1 ] - default = 0"<<endl;

    cout<<"Do you want to run app with default parameters? [Y/n]"<<endl;

    answer = getchar();
    if(answer == 'n' || answer == 'N')
        return false;
    else
        return true;
}

std::string getResourcePath()
{
    const int length=128;
    char buffer[length];
    readlink("/proc/self/exe", buffer, length);
    std::string path = buffer;
    std::string::size_type end = path.find_last_of("/");
    return path.substr(0,end) + "/../share/SeaPortSimulator/";
}


int main(int argc, char **argv)
{
    int ships,speed;
    bool debug;

    if(argc == 4) {
        ships = atoi(argv[1]);
        speed = atoi(argv[2]);
        debug = static_cast<bool>(atoi(argv[3]));
    } else {
        if(help()) {
            ships = 5;
            speed = 80;
            debug = false;
        } else
            exit(0);
    }

    srand(time(0));
    Demo *m_demo = new Demo(getResourcePath() , ships, speed, debug);
    m_demo->play();
    exit(0);
}

