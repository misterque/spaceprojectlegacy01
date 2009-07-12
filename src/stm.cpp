#include "stm.h"
#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;

#define vector_length( x, y, z ) ( sqrt ( (x)*(x) + (y)*(y) + (z)*(z) ) )
#define vector_product( a, b)  { ( (a)[1]*(b)[2] ) - ( (a)[2]*(b)[1]) , ((a)[2]*(b)[0]) - ((a)[0]*(b)[2] ) , ((a)[0]*(b)[1]) - ((a)[1]*(b)[0])  }  
#define vector_sum(a, b)  {(a)[0] + (b)[0], (a)[1] + (b)[1], (a)[2] + (b)[2] }
#define vector_dif(a, b)  {(a)[0] - (b)[0], (a)[1] - (b)[1], (a)[2] - (b)[2] } 
#define vertices_normal(a,b,c) vector_product ( vector_dif (a,b) , vector_dif (b,c) ) 
float rot;
cam Camera;

    xmlDoc *doc = NULL;
    


    
int Modell::Load(string filename){
  long filesize;
  ifstream fin;
  fin.open(filename.data(), ios::in | ios::binary);
  
  fin.seekg( 0, ios::end );  //Sprung ans ende
  filesize=fin.tellg();      // Position und damit Filesize ermitteln
  fin.seekg(0,ios::beg);     //Sprung an den Anfang

  cout<<fin<<endl;
  char *pBuffer = new char[filesize]; //Erstellen eines Buffers
  fin.read(pBuffer, filesize); //Einlesen
  fin.close();
  
  const char *pPosPtr=pBuffer;
  // einladen der Anzahl Vertices
  int verts = *(int*)pPosPtr;
    pPosPtr+=sizeof(int);
  
  for(int i=0; i<verts; i++){
    Point nVert;
    nVert.co[2] = *(float*)pPosPtr;
      pPosPtr+=sizeof(float);
    nVert.co[0] = *(float*)pPosPtr;
      pPosPtr+=sizeof(float);
    nVert.co[1] = *(float*)pPosPtr;
      pPosPtr+=sizeof(float);

    v_Vertices.push_back(nVert);
  }  
  int faces = *(int*)pPosPtr;
    pPosPtr+=sizeof(int);
  
  for(int i=0; i<faces; i++){
    Triangle nFace;
    nFace.vert[0] = *(int*)pPosPtr;
      pPosPtr+=sizeof(int);
    nFace.vert[1] = *(int*)pPosPtr;
      pPosPtr+=sizeof(int);
    nFace.vert[2] = *(int*)pPosPtr;
      pPosPtr+=sizeof(int);


    v_Triangles.push_back(nFace);
  }
  
  cout<<"VertCount: "<<v_Vertices.size()<<endl;  
  cout<<"PolyCount: "<<v_Triangles.size()<<endl;  

        
}

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 				// Ambient Light Values ( NEW )

GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };				 // Diffuse Light Values ( NEW )

GLfloat LightPosition[]= { 0.0f, -5.0f, 0.0f, 0.0f };				 // Light Position ( NEW )




void Modell::Draw(){


  int Gl_Light_Array[] = {
   // GL_LIGHT0,
    GL_LIGHT1,
    GL_LIGHT2,
    GL_LIGHT3,
    GL_LIGHT4,
    GL_LIGHT5,
    GL_LIGHT6,
    GL_LIGHT7 };

  for (int i=0; i < mother->v_Suns.size(); i++) {
    GLfloat SunDiffuse[]= { mother->v_Suns[i].r,
                            mother->v_Suns[i].g,
                            mother->v_Suns[i].b,  1.0f }; 
    glLightfv(Gl_Light_Array[i], GL_DIFFUSE, SunDiffuse);   // setting the diffu

  }
  
  

    
  GLfloat SkyAmbient[]=   { mother->Sky.r, mother->Sky.g, mother->Sky.b, 0.0}; // Light emitted by the Skymap
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, SkyAmbient);





    
  


  
  glEnable(GL_LIGHTING);
  glDisable(GL_BLEND);
  //glDisable(GL_DEPTH);
  glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
  glEnable(GL_CULL_FACE);
  glDisable(GL_TEXTURE_2D);
  
    glColor4f(1.0,1.0,1.0,1.0);
    

  
        glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
        /* Enable Light One */
    glEnable(GL_NORMALIZE);


    glRotatef( Camera.pitch,1,0,0);
    glRotatef( Camera.yaw,0,1,0);
    glTranslatef(-Camera.x,-Camera.y,-Camera.z);
  

  for (int j=0; j < mother->v_Suns.size(); j++) {
    GLfloat LightPosition[] = { mother->v_Suns[j].x,
                                mother->v_Suns[j].y,
                                mother->v_Suns[j].z, 1.0f };
			        
    glLightfv(Gl_Light_Array[j], GL_POSITION,LightPosition);	// position the lightsource
    glEnable(Gl_Light_Array[j]);
  }




    
    glTranslatef(0.,0.,0.); 
  

  for(int i=0; i<v_Triangles.size(); i++){
      //float normal[3] = vertices_normal( v_Vertices[v_Triangles[i].vert[0]].co,  v_Vertices[v_Triangles[i].vert[1]].co, v_Vertices[v_Triangles[i].vert[1]].co );
      float difAB[3] = vector_dif (v_Vertices[v_Triangles[i].vert[0]].co, v_Vertices[v_Triangles[i].vert[1]].co);
      float difBC[3] = vector_dif (v_Vertices[v_Triangles[i].vert[1]].co, v_Vertices[v_Triangles[i].vert[2]].co);

      float normal[3] = vector_product( difAB , difBC);
      /*float b[3] = v_Vertices[v_Triangles[i].vert[1]].co;
      float c[3] = v_Vertices[v_Triangles[i].vert[2]].co;
      float normal[3] =    vector_sum ( (a) , (a))   ;
*/
      glNormal3fv(normal);
    
    glBegin( GL_TRIANGLES );
      glVertex3fv( v_Vertices[v_Triangles[i].vert[0]].co );
      glVertex3fv( v_Vertices[v_Triangles[i].vert[1]].co );
      glVertex3fv( v_Vertices[v_Triangles[i].vert[2]].co );
    glEnd();
  }
}    
    
    
//eine sehr hässliche funktion um für opengl bmps zu laden, möglichst schnell überarbeiten
GLuint LoadBMP(string filename)
{
    
    SDL_Surface *bitmap, *conv;
    unsigned texture;
    GLuint texture_id;
    const char* file = (const char*)filename.data();

    bitmap = IMG_Load(file);
    if (!bitmap) return 0;
    
    conv = SDL_CreateRGBSurface(SDL_SWSURFACE, bitmap->w, bitmap->h, 32,
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    #else
            0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
    #endif
    SDL_BlitSurface(bitmap, 0, conv, 0);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //Wichtig fr skymaps!!!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); //
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  
    glPixelStorei(GL_UNPACK_ROW_LENGTH, conv->pitch / conv->format->BytesPerPixel);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, conv->w, conv->h, 0, GL_RGBA,
        GL_UNSIGNED_BYTE, conv->pixels);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    
    SDL_FreeSurface(bitmap);
    SDL_FreeSurface(conv);

    return texture_id;
}



int LoadSkymap ( skymap *sky ){

  sky->front =  LoadBMP("./data/"+sky->name+"_front.jpg");
  sky->left =   LoadBMP("./data/"+sky->name+"_left.jpg");
  sky->back =   LoadBMP("./data/"+sky->name+"_back.jpg");
  sky->right =  LoadBMP("./data/"+sky->name+"_right.jpg");
  sky->top =    LoadBMP("./data/"+sky->name+"_up.jpg");
  sky->bottom = LoadBMP("./data/"+sky->name+"_down.jpg");
  
  if (sky->front  &&
      sky->left   &&
      sky->back   &&
      sky->right  &&
      sky->top    &&
      sky->bottom )      return 1;
  else return -1;
}     

     
    
  static GLfloat t0[] = { 0.0f, 0.0f};
  static GLfloat t1[] = { 1.0f, 0.0f};
  static GLfloat t2[] = { 1.0f, 1.0f};
  static GLfloat t3[] = { 0.0f, 1.0f};

  static GLfloat v0[] = { -1.0f,  1.0f, -1.0f }; 
  static GLfloat v1[] = {  1.0f,  1.0f, -1.0f };
  static GLfloat v2[] = {  1.0f, -1.0f, -1.0f };
  static GLfloat v3[] = { -1.0f, -1.0f, -1.0f };

  static GLfloat v4[] = { -1.0f,  1.0f, 1.0f };
  static GLfloat v5[] = {  1.0f,  1.0f, 1.0f };
  static GLfloat v6[] = {  1.0f, -1.0f, 1.0f };
  static GLfloat v7[] = { -1.0f, -1.0f, 1.0f };
  

  



void CSky::Draw(){



  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  
  glRotatef( Camera.pitch,1,0,0);
  glRotatef( Camera.yaw,0,1,0);
  
  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_CULL_FACE);
  glColor4f(1, 1, 1, 1);


  //Draw the front quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.front);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v0 );
    glTexCoord2fv( t1 ); glVertex3fv( v1 );
    glTexCoord2fv( t2 ); glVertex3fv( v2 );
    glTexCoord2fv( t3 ); glVertex3fv( v3 );
  glEnd();
  
    //Draw the left quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.left);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v4 );
    glTexCoord2fv( t1 ); glVertex3fv( v0 );
    glTexCoord2fv( t2 ); glVertex3fv( v3 );
    glTexCoord2fv( t3 ); glVertex3fv( v7 );
  glEnd();

    //Draw the back quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.back);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v5 );
    glTexCoord2fv( t1 ); glVertex3fv( v4 );
    glTexCoord2fv( t2 ); glVertex3fv( v7 );
    glTexCoord2fv( t3 ); glVertex3fv( v6 );
  glEnd();

    //Draw the right quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.right);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v1 );
    glTexCoord2fv( t1 ); glVertex3fv( v5 );
    glTexCoord2fv( t2 ); glVertex3fv( v6 );
    glTexCoord2fv( t3 ); glVertex3fv( v2 );
  glEnd();
  
    //Draw the top quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.top);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v5 );
    glTexCoord2fv( t1 ); glVertex3fv( v1 );
    glTexCoord2fv( t2 ); glVertex3fv( v0 );
    glTexCoord2fv( t3 ); glVertex3fv( v4 );
  glEnd();
    //Draw the bottom quad of the sky  
  glBindTexture(GL_TEXTURE_2D, Skymap.bottom);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( v6 );
    glTexCoord2fv( t1 ); glVertex3fv( v7 );
    glTexCoord2fv( t2 ); glVertex3fv( v3 );
    glTexCoord2fv( t3 ); glVertex3fv( v2 );
  glEnd();

  
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  for(int i=0; i<v_Stars.size(); i++){
    float size = v_Stars[i].size/1000.;
    glBindTexture(GL_TEXTURE_2D, v_Stars[i].sprite);
    
    glPushMatrix();
      glColor4f(1, 1, 1, v_Stars[i].lum);  
       glRotatef( v_Stars[i].yaw,0,1,0);
            glRotatef( v_Stars[i].pitch,1,0,0);

      
      glBegin( GL_QUADS );
        glTexCoord2fv( t0 ); glVertex3f( -size, size, -1 );
        glTexCoord2fv( t1 ); glVertex3f( size,  size, -1 );
        glTexCoord2fv( t2 ); glVertex3f( size, -size, -1 );
        glTexCoord2fv( t3 ); glVertex3f( -size,-size, -1 );
      glEnd();
    glPopMatrix();
  }
      
}  

void Object::Draw(){

};


string xml_get_string(xmlNode *cur_node){
  xmlChar *key;  
  key = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
  string Str =  string(reinterpret_cast<char*> (key));	    
  xmlFree(key);
  return Str;
}

float xml_get_float(xmlNode *cur_node){
  xmlChar *key;  
  key = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
  float Str =  atof(reinterpret_cast<char*> (key));	    
  xmlFree(key);
  return Str;
}



int System::xml_parse_planet(xmlNode *node){
  xmlNode *cur_node = NULL;
  Planet nPlanet;
  for (cur_node = node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"name"))) 		 nPlanet.name = xml_get_string(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"map"))) 		 nPlanet.map = LoadBMP(xml_get_string(cur_node));
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"x"))) 			 nPlanet.x = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"y"))) 			 nPlanet.y = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"z"))) 			 nPlanet.z = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"radius"))) 		 nPlanet.radius = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"a_r"))) 		 nPlanet.a_r = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"a_g"))) 		 nPlanet.a_g = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"a_b"))) 		 nPlanet.a_b = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"alpha"))) 		 nPlanet.alpha = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"atmosphere_thickness"))) nPlanet.atmosphere_thickness = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"atmosphere_falloff"))) 	 nPlanet.atmosphere_falloff = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"rotation"))) 		 nPlanet.rotation = xml_get_float(cur_node);
    }
  }
  nPlanet.mother = this;
  v_Planets.push_back(nPlanet);
}

int System::xml_parse_sun(xmlNode *node){
  xmlNode *cur_node = NULL;
  Sun nPlanet;
  for (cur_node = node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"name"))) 		 nPlanet.name = xml_get_string(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"sprite"))) 		 nPlanet.sprite = LoadBMP(xml_get_string(cur_node));
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"x"))) 			 nPlanet.x = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"y"))) 			 nPlanet.y = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"z"))) 			 nPlanet.z = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"radius"))) 		 nPlanet.radius = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"r"))) 		 nPlanet.r = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"g"))) 		 nPlanet.g = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"b"))) 		 nPlanet.b = xml_get_float(cur_node);
    }
  }

  nPlanet.sprite2 = LoadBMP("./data/sun2.jpg");
  nPlanet.mother = this;
  v_Suns.push_back(nPlanet);

}



int System::xml_parse_starfield(xmlNode *node){
  xmlNode *cur_node = NULL;
  GLuint Star;
  float minsize = 1.5, maxsize = 5.;
  float minpitch = -90., maxpitch = 90.;

  float pitch = 0., yaw = 0.;
  float minlum = 0.3, maxlum = 1.;
  int n=100;
  int seed = rand();

  for (cur_node = node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"file"))) 	 Star = LoadBMP(xml_get_string(cur_node));
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"maxsize"))) 	 maxsize = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"minsize"))) 	 minsize = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"maxpitch"))) 	 maxpitch = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"minpitch"))) 	 minpitch = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"pitch"))) 	 pitch = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"yaw"))) 	 yaw = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"minlum"))) 	 minlum = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"maxlum"))) 	 maxlum = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"number"))) 	 n = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"seed"))) 	 seed = xml_get_float(cur_node);
    
    }


  }
  srand(seed);
  for(int i=0; i<n; i++){
    skystar nStar;
    nStar.size = ((float(rand()%10000) / 10000. * ( maxsize - minsize)) + minsize) ;
    nStar.lum = (float(rand()%10000) / 10000.  * ( maxlum - minlum)) + minlum;
    nStar.pitch = float(rand()%10000) / 10000. * ( maxpitch - minpitch) + minpitch + pitch;
    nStar.yaw = float(rand()%10000) / 10000. * 360. + yaw;
    nStar.sprite = Star;

    Sky.v_Stars.push_back(nStar);
  } 
}

int System::xml_parse_sky(xmlNode *node){
  xmlNode *cur_node = NULL;
  for (cur_node = node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"name"))) 	 Sky.Skymap.name = xml_get_string(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"r"))) 		 Sky.r = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"g"))) 		 Sky.g = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"b"))) 		 Sky.b = xml_get_float(cur_node);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"starfield")))    xml_parse_starfield(cur_node->children);
    }


  }
    LoadSkymap (&Sky.Skymap);

  /*
  GLuint Star = LoadBMP("./data/star.bmp");
  GLuint Star2 = LoadBMP("./data/star2.bmp");
  LoadSkymap (&Sky.Skymap);
  for(int i=0; i<350; i++){
    skystar nStar;
    nStar.size = rand()%4 +1.5;
    nStar.lum = float(rand()%80 +21) / 100.;
    nStar.pitch = float(rand())/32768. * 180. - 90;
    nStar.yaw = float(rand())/32768. * 360;

    switch(rand()%2){
      case 0: nStar.sprite = Star; break;
      case 1: nStar.sprite = Star2; break;
    }
    Sky.v_Stars.push_back(nStar);
  } */
}




int System::xml_parse_system(xmlNode *node){
  xmlNode *cur_node = NULL;
  for (cur_node = node; cur_node; cur_node = cur_node->next) {
    if (cur_node->type == XML_ELEMENT_NODE) {
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"planet"))) xml_parse_planet(cur_node->children);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"sun"))) xml_parse_sun(cur_node->children);
      if ((!xmlStrcmp(cur_node->name, (const xmlChar *)"sky"))) xml_parse_sky(cur_node->children);
      

    }
  }
}
/*
                    xmlChar *key;
		    key = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
		    printf("keyword: %s\n", key);
		    xmlFree(key);*/




   

Modell TestM;

void System::Init() {
  xmlNode *root_element = NULL;


    /*parse the file and get the DOM */
    doc = xmlReadFile("./omega.xml", NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file");
    }

    /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    xml_parse_system(root_element->children);

    /*free the document */
    xmlFreeDoc(doc);

    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    TestM.Load("./data/hornet.fsm");
    TestM.mother = this;
  
  

}

void System::Draw() {
  
  Sky.Draw();
      
  // Draw all Objects in System
  
  // Zunächst wird ein array mit pointern auf alle Systemobjecte angelegt
  Object *Liste[v_Planets.size() + v_Suns.size()];
  Object *Platzhalter;
  
  //Dieses wird als nächstes gefüllt und die Distanzen zum Blickpunkt errechnet
  int n=0;

  for( int i = 0; i < v_Suns.size(); i++) {
    Liste[n] = &v_Suns[n];
    Liste[n]->distance_to_eye = vector_length ( Camera.x - Liste[n]->x, Camera.y - Liste[n]->y, Camera.z - Liste[n]->z ); 
    n++;
  }
    
  for( int i = 0; i < v_Planets.size(); i++) {
    Liste[n] = &v_Planets[i];
    Liste[n]->distance_to_eye = vector_length ( Camera.x - Liste[n]->x, Camera.y - Liste[n]->y, Camera.z - Liste[n]->z ); 
    n++;
  }

  
  // Nun wird das Array nach Distanzen sortiert
  // hier wird das Bubblesort verfahren angewandt, schneller können später implementiert werden.
  for(int k = 0; k < v_Planets.size() + v_Suns.size(); k++) {
    for( int i = 0; i < v_Planets.size() + v_Suns.size() - 1; i++) {
      if (Liste[i]->distance_to_eye > Liste[i+1]->distance_to_eye) {
        Platzhalter = Liste[i];
        Liste[i] = Liste[i+1];
        Liste[i+1] = Platzhalter;
      }
    }
  }
  //Abschließend werden die Systemobjekte gezeichnet
 
  for( int i = v_Planets.size() + v_Suns.size() - 1; i >=0; i--) {
    Liste[i]->Draw();
    //cout<<i<<" "<<Liste[i]->name<<": "<<Liste[i]->distance_to_eye<<endl;
  }
   
    TestM.Draw();
  

  
}


