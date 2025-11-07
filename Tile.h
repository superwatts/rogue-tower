// Tile.h

#include<iostream>
#include<string>
#include <C:/OpenGLwrappers/glew-1.10.0-win32/glew-1.10.0/include/GL/glew.h>
#include <C:/OpenGLwrappers/freeglut-MSVC-2.8.1-1.mp/freeglut/include/GL/freeglut.h> 
#include "utils.h"

#pragma once

class Tile
{
public:
  bool passable  = false;
  bool path      = false;
  float p_value = 0.0f;
  int elevation = 1;

  Tile();
  Tile(bool is_passable, bool is_path);
  Tile(int elevation_level);
  ~Tile();
  void drawTile(float x, float y, float z, float w, bool do_outline);
  void drawTileBorder(float x, float y, float w, DIRECTION direction);
};


