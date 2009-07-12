#include "../stm.h"

extern cam Camera;


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
  
    
void Sun::Draw (){
  /* We don't want to modify the projection matrix. */
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  
  glRotatef( Camera.pitch,1,0,0);
  glRotatef( Camera.yaw,0,1,0);
  glTranslatef(-Camera.x,-Camera.y,-Camera.z);

  
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  

  glTranslatef(x,y,z);
  //things we (dont) need for the atmosphere

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); 

  float modelview[16];
  int i,j;

  // save the current modelview matrix
  glPushMatrix();

  // get the current modelview matrix
  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

  // undo all rotations
  // beware all scaling is lost as well 
  for( i=0; i<3; i++ ) {
	for( j=0; j<3; j++ ) {
		if ( i==j )
			modelview[i*4+j] = 1.0;
		else
			modelview[i*4+j] = 0.0;
	}
  }
  // set the modelview with no rotations and scaling
  glLoadMatrixf(modelview);

 
  GLfloat u0[] = { -1.0f * radius*2,  1.0f * radius*2, 0.0f }; 
  GLfloat u1[] = {  1.0f * radius*2,  1.0f * radius*2, 0.0f };
  GLfloat u2[] = {  1.0f * radius*2, -1.0f * radius*2, 0.0f };
  GLfloat u3[] = { -1.0f * radius*2, -1.0f * radius*2, 0.0f };

  GLfloat b0[] = { -1.0f * radius,  1.0f * radius, 0.0f }; 
  GLfloat b1[] = {  1.0f * radius,  1.0f * radius, 0.0f };
  GLfloat b2[] = {  1.0f * radius, -1.0f * radius, 0.0f };
  GLfloat b3[] = { -1.0f * radius, -1.0f * radius, 0.0f };

  glBindTexture(GL_TEXTURE_2D, sprite);
  glColor3f(r,g,b);
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( u0 );
    glTexCoord2fv( t1 ); glVertex3fv( u1 );
    glTexCoord2fv( t2 ); glVertex3fv( u2 );
    glTexCoord2fv( t3 ); glVertex3fv( u3 );
  glEnd();
  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); 
  glBindTexture(GL_TEXTURE_2D, sprite2);
  glColor3f(1.,1.,1.);
  
  glBegin( GL_QUADS );
    glTexCoord2fv( t0 ); glVertex3fv( b0 );
    glTexCoord2fv( t1 ); glVertex3fv( b1 );
    glTexCoord2fv( t2 ); glVertex3fv( b2 );
    glTexCoord2fv( t3 ); glVertex3fv( b3 );
  glEnd();
    
  
// restores the modelview matrix
  glPopMatrix();

}


