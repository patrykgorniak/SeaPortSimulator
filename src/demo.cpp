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

Demo::Demo(string fileName, int maxShips,int delay,bool debug):
    ships(0)
{
    // Initializing variables
    this->maxShips = maxShips;
    this->delay = delay;
    this->curShips = 0;

    // enable/disable debug mode
    this->debugEnabled = debug;

    ships = new Ship*[maxShips];
    for(int i=0; i<maxShips;i++)
        ships[i]=NULL;

    thread = new pthread_t[maxShips];

    // map initializing
    initObjects("maps/maps.jpeg");

    //    map->saveToFile("mapa.txt");

    // initialize free roads
    for(unsigned int i=0; i < map->getStartingPoints().size(); i++)
        freeRoads.push_back(i);

    // initializing SDL
    initSDL(fileName);

    // load ships avatars from files
    loadAvatarShip("ship/","");
}


// take the first free road
int Demo::getRoad()
{
    int road = -1;

    if(freeRoads.size() > 0)
    {
        road = freeRoads.at(0);
        freeRoads.erase(freeRoads.begin());
        return road;
    }
    return road;
}

// destructor
Demo::~Demo()
{
    pthread_exit(NULL);
}

// random value from [ min , max ]
int Demo::customDelay(int min,int max)
{
    int time;
    time = rand()%(max - min);
    time += min;
    return time;
}

// Initializing map
void Demo::initObjects(string path)
{
    if(debugEnabled)
        cout<<"Initializing map"<<endl;

    pthread_mutex_init(&mutex,NULL);
    map = new BoardManager(path, debugEnabled);
}

// initializing SDL
void Demo::initSDL(string fileName)
{
    if(debugEnabled)
        cout<<"Initializing SDL"<<endl;

    // initializing SDL
    SDL_Init( SDL_INIT_EVERYTHING );

    // Set the title of the main window
    SDL_WM_SetCaption("Port demo",NULL);

    // Configure the screen
    screen = SDL_SetVideoMode( 1024, 720, 32, SDL_SWSURFACE );

    // load background
    image = loadImage( fileName );

    // set background
    SDL_BlitSurface( image, NULL, screen, NULL );

    // refresh screen
    SDL_Flip( screen );
}

void Demo::loadAvatarShip(string path,string name = "")
{
    if(debugEnabled)
        cout<<"Following avatars have been read: "<<endl;

    for(int type=0; type<2; type++) {
        ostringstream kat;
        kat<<"type_"<<type<<"/";
        for(int i=0; i<8; i++) {
            ostringstream ostr;
            ostr<<path<<kat.str()<<name<<i+1<<".bmp";

            if(debugEnabled)
                cout<< ostr.str()<<endl;

            AvatarShips[type][i] = loadImage(ostr.str());

            if(AvatarShips[type][i]==NULL) {
                if(debugEnabled)
                    cout<<"Error during loading "<<i<<" ship"<<endl;
            }
        }
    }
}

// load BMP (bitmap)
SDL_Surface *Demo::loadImage(string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );

    // If image loaded properly
    if( loadedImage != NULL ) {
        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
        if( optimizedImage != NULL ) {
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0, 0xFF ) );
        }
    }
    return optimizedImage;
}

// thread function
void* threadF(void* obj)
{
    Ship* m_ship=(Ship*)obj;
    m_ship->run();
    pthread_exit(NULL);
}

// main function of the app. Is responsible for keyboard events, refresing the screen,
// creating new ships, setting new starting points, etc.
void Demo::createNewShips()
{
    for(int i=0; i<maxShips; i++)
    {
        if(ships[i]==NULL)
        {
            int road;
            if( ( road = getRoad()) > -1)
            {
                if(debugEnabled)
                {
                    cout<<"Created ship nb "<<i<<endl;
                    cout<<"Ship "<<i<<" took the road: "<<road<<"  ";
                    cout<<"Starting point X: "<<startPoint.x<<" Y: "<<startPoint.y<<endl;
                }

                Point p((map->getStartingPoints()[road])->x, (map->getStartingPoints()[road])->y);

                ships[i] = new Ship(p.x,p.y, road, map->getBoard(), map->getWidth(), map->getHeight(), customDelay(20,70), mutex, debugEnabled);
                ships[i]->setAvatarType(randType(2));
                curShips++;

                pthread_create(&thread[i], NULL, threadF, (void*)ships[i]);
            }
        }
    }
}

void Demo::cleanupShips()
{
    for(int i=0; i<maxShips; i++)
    {
        if(ships[i]!=NULL && !ships[i]->isAlive() && ships[i]->isFinished())
        {
            freeRoads.push_back(ships[i]->getRoad());
            delete ships[i];
            ships[i]=NULL;

            if(debugEnabled)
                cout<<"Destroyed ship: "<<i<<endl;

            curShips--;
        }
    }
}

void Demo::repaint()
{
    pthread_mutex_lock (&mutex);
    for(int i=0; i < maxShips; i++)
    {
        if(ships[i]!=NULL && ships[i]->isAlive())
        {
            applySurface(ships[i]->getX(), ships[i]->getY(), AvatarShips[ships[i]->getAvatarType()][ships[i]->getAvatar()], screen, NULL);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void Demo::play()
{
    while(true)
    {
        while( SDL_PollEvent( &event ) )
        {
            if(event.type == SDL_QUIT)
                return;
        }

        cleanupShips();
        if( curShips < maxShips )
        {
            createNewShips();
        }
        //Apply image to screen
        SDL_BlitSurface( image, NULL, screen, NULL );
        repaint();
        SDL_Flip( screen );
        SDL_Delay(delay);
    }
}

// random value from [ 0 , max ]
int Demo::randType(int max)
{
    return rand()%max;
}

//  apply ship`s avatar on the background
void Demo::applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
