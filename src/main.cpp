/***************************************************************************
 *   Copyright (C) 2005 by Mister Que                                      *
 *   que@linux                                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#include "stm.h"

#define W 1024
#define H 768
#define D 32

#define PI 3.14159

using namespace std;

extern float rot;
extern cam Camera;


int main(int argc, char *argv[])
{
  srand(time(0));
  


  bool done = false;
  SDL_Event event; 
  SDL_Surface *screen;
  const SDL_VideoInfo* info = NULL;
  
  
  SDL_Init ( SDL_INIT_VIDEO);
  
  info = SDL_GetVideoInfo( );
  
  cout<<info->vfmt->BitsPerPixel<<endl;
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  
 
  screen = SDL_SetVideoMode(W, H, 32, SDL_OPENGL | SDL_FULLSCREEN );
  //perlin(128,128);
  
  glShadeModel( GL_SMOOTH );
  
    glClearColor( 0, 0, 0, 0 );


    glViewport( 0, 0, W, H );


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    gluPerspective( 60.0, 4./3., 0.1, 8000.0 );
  glEnable(GL_TEXTURE_2D);
  

  System SolSystem;
  SolSystem.Init();
  

  
Camera.x = 0.0;
Camera.y = 0.0;
Camera.z = 0.0;
Camera.pitch = 0.0;
Camera.yaw = 0.0;
Camera.roll = 0.0;
Camera.v = 0;

  
   while(SDL_PollEvent(&event));  
    
  Uint32 lasttick, ticktack,  tick, count = 0;
  lasttick = SDL_GetTicks();
  ticktack = lasttick;
  while (!done) {
  tick = SDL_GetTicks() - ticktack;
  ticktack = SDL_GetTicks();
  if ( SDL_GetTicks() - lasttick > 1000){
    cout<<count<<endl;
    count=0;
    lasttick = SDL_GetTicks();
  }
  count++;
  
  // #####################  INPUT ######################  
    while(SDL_PollEvent(&event))
    switch(event.type)
    {
    case SDL_KEYDOWN:
      if (event.key.keysym.sym==SDLK_ESCAPE)
        done=true;
      if (event.key.keysym.sym==SDLK_LEFT)
        Camera.v = 0;
      if (event.key.keysym.sym==SDLK_RIGHT)
        Camera.v += 200000;
      if (event.key.keysym.sym==SDLK_UP)
        Camera.v += 1000;
      if (event.key.keysym.sym==SDLK_DOWN)
        Camera.v -= 1000;
      break;
    case SDL_MOUSEMOTION:
      Camera.pitch += event.motion.yrel;
      Camera.yaw +=   event.motion.xrel;
      break;
    
    }
  // #####################  COMPUTE ######################  

  
  // #####################  OUTPUT ######################  
      
   // Camera.z += event.motion.yrel*3;
  //cout<<gluErrorString( glGetError ())<<endl;
    // in m / s
   Camera.x += sin(Camera.yaw / 180. * PI) * Camera.v / 1000. * tick / 1000.;
   Camera.z -= cos(Camera.yaw / 180. * PI) * Camera.v / 1000. * tick / 1000.;
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    SolSystem.Draw();
    SDL_GL_SwapBuffers( );
  }  


  SDL_Quit();
  return 0;
}


