#include "../stm.h"

extern cam Camera;
    
void Planet::Draw (){

  // inner hull, planetbody, outer hull
  
  // make an array of the the lights so that we can talk to them in a for loop // export this
  int Gl_Light_Array[] = {
  //  GL_LIGHT0,
    GL_LIGHT1,
    GL_LIGHT2,
    GL_LIGHT3,/*
    GL_LIGHT4,
   GL_LIGHT5,
    GL_LIGHT6,
    GL_LIGHT7 */};
    
    //disable all light sources
    for(int i = 0; i< 1; i++) glDisable(Gl_Light_Array[i]);
    
  // Setup a glusphere  //this may be exported 
  GLUquadricObj *sphere;
  sphere = gluNewQuadric();
  
  glBindTexture(GL_TEXTURE_2D, map);
  gluQuadricNormals(sphere, GLU_SMOOTH);	// obsolete this is standard, important for lightning
  gluQuadricTexture(sphere, GL_TRUE);           // jeah we want to have a planet texture 
  

  // define the two different ambient values we will use ( one for the planet itself and one for the atmoshere
  GLfloat SkyAmbient[]=   { mother->Sky.r, mother->Sky.g, mother->Sky.b, 0.0}; // Light emitted by the Skymap
  GLfloat AtmosAmbient[]= { mother->Sky.r / 20. , mother->Sky.g  / 20.  , mother->Sky.b  / 20. , 0.0}; // Light emitted by the Skymap onto the Atmospheres
  
  GLfloat ZeroZeroZero[] = { 0.0, 0.0, 0.0, 1.0 };
  
  
  // this nice one will compute the subdivision number for the sheres so we dont have well rounded planets miles and miles away
  int subdivisions = pow ( 0.8, (int)(distance_to_eye * 70. / radius /1000.)) * 30; //this could be optimized or variable for lower machines
    if (subdivisions < 2) subdivisions = 2;
  
  // 0nd step: setup the camera
  
  glColor4f(1.0,1.0,1.0,1.0);
  
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  
  glRotatef( Camera.pitch,1,0,0);
  glRotatef( Camera.yaw,0,1,0);
  glTranslatef(-Camera.x,-Camera.y,-Camera.z);
  
  for (int j=0; j < mother->v_Suns.size(); j++) {
    GLfloat LightPosition[] = { mother->v_Suns[j].x,
                                mother->v_Suns[j].y,
                                mother->v_Suns[j].z, 1.0f };
        
    glLightfv(Gl_Light_Array[j], GL_AMBIENT, ZeroZeroZero);	// maybe this is obsolete
    glLightfv(Gl_Light_Array[j], GL_POSITION,LightPosition);	// position the lightsource
    glEnable(Gl_Light_Array[j]);
  }
  
  
  
  // move to our destination
  glTranslatef(x,y,z);

  //rotate the planet
  glRotatef( alpha,0,0,1);
  glRotatef( rotation,0,1,0); // * rot
  
  glRotatef( 90,1,0,0);
  
  
  
  
  // begin with the inner hull of the atmosphere ==========================================================================================
  // ======================================================================================================================================
  
  // Set the ambience to the systemsky's ambience color divided by X for giving the anmoshere not too much glow on the dark side
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AtmosAmbient);
  
  // we`re setting the faces to cull to front faces because we want the inner backside of the atmo spheres to be drawn
  // this gives looks nicer :)
  glCullFace(GL_FRONT);
  
  float NUM_ATMO_SPHERES = 10; //this is the number of atmospheres to be drawn the more the nicer // must be float but element aus n
  
  //things to enable and disable
  glDisable(GL_DEPTH_TEST); // we wont need this in the whole function
  glEnable(GL_CULL_FACE); // this is important = half the polys and unnice fx in the atmoshere won`t occurre

  glEnable(GL_BLEND); // without this ...
  glDisable(GL_TEXTURE_2D); // we dont have a texture on the atmosphere
  glEnable(GL_LIGHTING); // very important

  // the blend fuction blends the spheres depending on their color applied by suns
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);   
  
  
  for(int i=0; i<NUM_ATMO_SPHERES/2; i++) {
  //  glRotatef( 24.6,0.34,0.65,-0.2); //some random static rotation, gives pseudo anit aliasing fx
    
    //this fluffy function is the reason the spheres get thinner from inner to outer
     float f = (-1/NUM_ATMO_SPHERES) * i + 1;
    
    //now we setup up to 8 suns to shine on our nice atmoshere
    //the atmospheres color values are multiplied with our factor f and the color off our current sun
    for (int j=0; j < mother->v_Suns.size(); j++) {
      GLfloat AtmosDiffuse[]= { a_r * f * mother->v_Suns[j].r,
                                a_g * f * mother->v_Suns[j].g,
                                a_b * f * mother->v_Suns[j].b,  1.0f };

       
      glLightfv(Gl_Light_Array[j], GL_DIFFUSE, AtmosDiffuse);   // setting the diffu
    }
    
 		
    //Finally we draw one off our inner hull spheres
    gluSphere(sphere, radius + atmosphere_thickness  + (((i+1)/NUM_ATMO_SPHERES)*atmosphere_falloff), subdivisions * 2, subdivisions);
  }
  
  //because we turnt this on and it doesnt make sense for other things, just turn it off
  glCullFace(GL_BACK);
  
  
  //2nd Step: Draw the planet =============================================================================================================
  // ======================================================================================================================================
    
  // Set the ambience to the systemsky's ambience color for giving the planet an individual glow
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, SkyAmbient);
  

  
  //enable disable

  glDisable(GL_BLEND); // this time we don't need this
  glEnable(GL_TEXTURE_2D); // no texturing no texture

  //now we setup up to 8 suns to shine on our nice atmoshere again
  //the atmospheres color values are multiplied with our factor f and the color off our current sun
  for (int i=0; i < mother->v_Suns.size(); i++) {
    GLfloat SunDiffuse[]= { mother->v_Suns[i].r,
                            mother->v_Suns[i].g,
                            mother->v_Suns[i].b,  1.0f }; 
     
    glLightfv(Gl_Light_Array[i], GL_DIFFUSE, SunDiffuse);   // setting the diffu
  }
    
  // uhhhnbelieveable 
  // would be nice if someone invents something against the subdivision LOD popping
  gluSphere(sphere,radius, subdivisions * 2, subdivisions);

  
  // 3rd verse same as the first                 ==========================================================================================
  // ======================================================================================================================================
  
  // Set the ambience to the systemsky's ambience color divided by X for giving the anmoshere not too much glow on the dark side
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AtmosAmbient);
  
 
  //things to enable and disable
  glEnable(GL_BLEND); // without this ...
  glDisable(GL_TEXTURE_2D); // we dont have a texture on the atmosphere
  
  for(int i=NUM_ATMO_SPHERES/2; i<NUM_ATMO_SPHERES; i++) {
    //  glRotatef( 24.6,0.34,0.65,-0.2); //some random static rotation, gives pseudo anit aliasing fx
    
    //this fluffy function is the reason the spheres get thinner from inner to outer
    float f = (-1/NUM_ATMO_SPHERES) * i + 1;
    
    //now we setup up to 8 suns to shine on our nice atmoshere again
    //the atmospheres color values are multiplied with our factor f and the color off our current sun
    for (int j=0; j < mother->v_Suns.size(); j++) {
      GLfloat AtmosDiffuse[]= { a_r * f * mother->v_Suns[j].r ,
                                a_g * f * mother->v_Suns[j].g ,
                                a_b * f * mother->v_Suns[j].b ,  1.0f };
      
      glLightfv(Gl_Light_Array[j], GL_DIFFUSE, AtmosDiffuse);   // setting the diffu
    }

    //Finally we draw one off our inner hull spheres
    gluSphere(sphere, radius + atmosphere_thickness  + (((i+1)/NUM_ATMO_SPHERES)*atmosphere_falloff), subdivisions * 2, subdivisions);
  }
  

   
   
    
  // scrappy base code ::::
  // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  
  // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  
  // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::  
  
 
  /* We don't want to modify the projection matrix. */



/*
  float outer_alpha ;
  
  
  if(((int)(distance_to_eye) % 1000) < 500) {
    outer_alpha = - 1./500. * ((int)(distance_to_eye) % 1000) +1.; 
  } else {
    outer_alpha =   1./500. * ((int)(distance_to_eye) % 1000) -1.; 
  }*/

   //cout<<name<<": "<< outer_alpha <<" | "<<t<<endl;


    //things we (dont) need for the atmosphere
  
  //set the ambient to totally dark

    

  

    
    //draw the planets Axis this is for debugging 
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES); 
      glVertex3f(0.0 , 0.0 ,  2.0 * radius);
      glVertex3f(0.0 , 0.0 , -2.0 * radius);
    glEnd();

 
 //   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); 
 //   glColor4f(1, 1, outer_alpha, outer_alpha);
  //  gluSphere(sphere,radius*1.05, t*4, t*2);
  
 // glPopMatrix();
  
  //things we (dont) need for the atmosphere
  
  //set the ambient to totally dark

}


