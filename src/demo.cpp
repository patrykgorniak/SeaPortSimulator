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

demo::demo(string fileName, int maxShips,int delay,bool debug)
{
    // Initializing variables
    this->maxShips = maxShips;
    this->delay = delay;
    this->curShips = 0;

    // enable/disable debug mode
    this->debugEnabled = debug;

    ships = new Ship*[maxShips];
    thread = new pthread_t[maxShips];

    // map initi        alizing
    initObjects("maps/mapa.bmp");

    //  map->writeMapToFile("mapa.txt");
    map->findStartingPoints();

    // take the list of the existing roads
    existingRoad = map->vec;

    // initialize free roads
    for(unsigned int i=0; i<existingRoad.size(); i++)
        freeRoad.push_back(i);

    // initializing SDL
    initSDL(fileName);

    // load ships avatars from files
    loadAvatarShip("ship/","");
}


// take the first free road
int demo::getRoad()
{
    int road = -1;
    startPoint.x = -1;
    startPoint.y = -1;
    if(freeRoad.size()>0) 
    {
        startPoint = *(existingRoad[freeRoad[0]]);
        road = freeRoad[0];
        freeRoad.erase(freeRoad.begin());
        return road;
    }


    return road;
}

// destructor
demo::~demo() {}

// random value from [ min , max ]
int demo::customDelay(int min,int max)
{
    int time;
    time = rand()%(max - min);
    time += min;
    return time;
}

// Initializing map
void demo::initObjects(string path)
{
    if(debugEnabled)
        cout<<"Initializing map"<<endl;

    pthread_mutex_init(&mutex,NULL);
    map = new Map(path,debugEnabled);
}

// initializing SDL
void demo::initSDL(string fileName)
{
    if(debugEnabled)
        cout<<"Initializing SDL"<<endl;

    // initializing SDL
    SDL_Init( SDL_INIT_EVERYTHING );

    // Set the title of the main window
    SDL_WM_SetCaption("Port demo",NULL);

    // Configure the screen
    screen = SDL_SetVideoMode( 1024, 719, 32, SDL_SWSURFACE );

    // load background
    image = loadImage( fileName );

    // set background
    SDL_BlitSurface( image, NULL, screen, NULL );

    // refresh screen
    SDL_Flip( screen );
}

void demo::loadAvatarShip(string path,string name = "")
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
SDL_Surface *demo::loadImage(string filename )
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
    Ship* stat=(Ship*)obj;
    stat->move();

    pthread_exit(NULL);
}

// main function of the app. Is responsible for keyboard events, refresing the screen,
// creating new ships, setting new starting points, etc.
void demo::play()
{
    while(true) {
        while( SDL_PollEvent( &event ) ) {
            if(event.type == SDL_QUIT)
                return;
        }

        if( curShips < maxShips ) {
            for(int i=0; i<maxShips; i++) {
                if(ships[i]==NULL) {
                    int road = getRoad();
                    if(road > -1) {
                        if(debugEnabled) {
                            cout<<"Created ship nb "<<i<<endl;
                            cout<<"Ship "<<i<<" took the road: "<<road<<"  ";
                            cout<<"Starting point X: "<<startPoint.x<<" Y: "<<startPoint.y<<endl;
                        }

                        ships[i] = new Ship(startPoint.x,startPoint.y,road,map->getMap(),map->getWidth(),map->getHeight(),customDelay(20,70),mutex,debugEnabled);
                        ships[i]->setAvatarType(randType(2));
                        curShips++;

                        pthread_create(&thread[i],NULL,threadF,(void*)ships[i]);
                    }
                }
            }
        }

        //Apply image to screen
        SDL_BlitSurface( image, NULL, screen, NULL );

        pthread_mutex_lock (&mutex);
        for(int i=0; i<maxShips; i++) {
            if(ships[i]!=NULL && ships[i]->isAlive()) {
                applySurface(ships[i]->get_x(),ships[i]->get_y(),AvatarShips[ships[i]->getAvatarType()][ships[i]->getAvatar()],screen,NULL);
                if(debugEnabled)
                    cout<<"Ship nb "<<i<<" position: X = "<<ships[i]->get_x()<<" Y = "<<ships[i]->get_y()<<endl;
            } else {
                curShips--;
                freeRoad.push_back(ships[i]->getRoad());

                if(debugEnabled)
                    cout<<"Ship "<<i<<" frees the road: "<<ships[i]->getRoad()<<endl;

                delete ships[i];
                ships[i]=NULL;
                if(debugEnabled)
                    cout<<"Destroy ship "<<i<<endl;
            }
        }
        pthread_mutex_unlock(&mutex);

        SDL_Flip( screen );
        SDL_Delay(delay);
    }
}

// random value from [ 0 , max ]
int demo::randType(int max)
{
    return rand()%max;
}

//  apply ship`s avatar on the background
void demo::applySurface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
