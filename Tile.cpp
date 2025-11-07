// Tile.cpp

#include "Tile.h"

Tile::Tile()
{
  passable  = true;
  path      = false;
}

Tile::Tile(bool is_passable, bool is_path)
{
  passable  = is_passable;
  path      = is_path;
}

Tile::Tile(int elevation_level)
{
  elevation = elevation_level;
}

Tile::~Tile()
{

}

void Tile::drawTile(float x, float y, float z, float w, bool do_outline)
{
  // Color is set in Map::drawMap()
  glBegin(GL_TRIANGLES);
    glVertex3f(x,     y,      z);
    glVertex3f(x + w, y,      z);
    glVertex3f(x + w, y + w,  z);
  glEnd();
  glBegin(GL_TRIANGLES);
    glVertex3f(x + w, y + w,  z);
    glVertex3f(x    , y + w,  z);
    glVertex3f(x,     y,      z);
  glEnd();

  if (do_outline)
  {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
      glVertex3f(x,     y,      z);
      glVertex3f(x + w, y,      z);
      glVertex3f(x + w, y + w,  z);
      glVertex3f(x    , y + w,  z);
    glEnd();
  }
}

void Tile::drawTileBorder(float x, float y, float z, float w, DIRECTION direction)
{
  if (direction == D_UP)
  {
    glBegin(GL_LINES);
      glVertex3f(x + w, y + w,  z);
      glVertex3f(x,     y + w,  z);
    glEnd();
  }
  else if (direction == D_DOWN)
  {
    glBegin(GL_LINES);
      glVertex3f(x,     y,      z);
      glVertex3f(x + w, y,      z);
    glEnd();
  }
  else if (direction == D_LEFT)
  {
    glBegin(GL_LINES);
      glVertex3f(x    , y + w,  z);
      glVertex3f(x,     y,      z);
    glEnd();
  }
  else if (direction == D_RIGHT)
  {
    glBegin(GL_LINES);
      glVertex3f(x + w, y,      z);
      glVertex3f(x + w, y + w,  z);
    glEnd();
  }
}