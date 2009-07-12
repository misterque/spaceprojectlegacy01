
#include <vector>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

using namespace std;

struct cam{
  float pitch, yaw, roll;
  double x, y, z, v;
};

struct skymap{
  string name;
  GLuint front,
      left,
      back,
      right,
      top,
      bottom;
};

struct skystar{
  float pitch, yaw;
  GLuint sprite;
  float size, lum;
};

class CSky{
private:

public:
  string name;
  skymap Skymap;
  float r,g,b;
  vector<skystar> v_Stars;
  void Draw();
};


class Object{
private:

public:
  string name;
  double x,y,z;
  double distance_to_eye;
  virtual void Draw();
  class System *mother;
}; 

class Planet : public Object{
private:

public:  

  GLuint map;
  
  float rot;
  
  double radius;
  double a_r, a_g, a_b;
  float alpha, atmosphere_thickness, atmosphere_falloff, rotation;
  void Draw();
};

class Sun : public Object{
private:

public:
  GLuint sprite, sprite2;
  
  double radius;
  double r, g, b;
  void Draw();
};

struct Point{
  GLfloat co[3];
};

struct Triangle{
  int vert[3];
};

class Modell{
private:

public:
  int Load(string filename);
  void Draw();
  
  vector<Point>     v_Vertices;
  vector<Triangle>  v_Triangles;
  class System *mother;
};


/*


GLuint LoadBMP(string filename);
int LoadSkymap ( skymap *sky );


  
int Init();
*/
class System
{
private:

public:
  
  CSky Sky;

  void Init();
  void Draw();
  int xml_parse_system(xmlNode *node);
  int xml_parse_planet(xmlNode *node);
  int xml_parse_sun(xmlNode *node);
  int xml_parse_sky(xmlNode *node);
  int xml_parse_starfield(xmlNode *node);
  
  vector<Planet> v_Planets;
  vector<Sun>    v_Suns;
  skymap Skymap;
//  ~System();

}; 

