// main.cpp

#include <iostream>
#include <C:/OpenGLwrappers/glew-1.10.0-win32/glew-1.10.0/include/GL/glew.h>
#include <C:/OpenGLwrappers/freeglut-MSVC-2.8.1-1.mp/freeglut/include/GL/freeglut.h> 
#include "OpenGL445Setup-2025.h"
#include "Map.h"


// Customize below 3 items to make canvas of own size & labeling
#define canvas_Width 800
#define canvas_Height 800
char canvas_Name[] = "Tower Defense Roguelike";
float CENTER_X = canvas_Width   / 2.0f;
float CENTER_Y = canvas_Height  / 2.0f;

std::string SEED = "big bugG";
bool do_draw_map_size = true;

const float CLEAR_COLOR[3] = { 0.0f, 0.0f, 0.0f };

int map_size[2] = { 35, 35 };
Map game_map = Map(map_size[0], map_size[1], SEED);

static void clearScreen()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void specialKeys(int key, int x, int y)
{
  if (key == GLUT_KEY_UP)
  {
    game_map.moveCurrentTile(0, 1);
  }
  if (key == GLUT_KEY_DOWN)
  {
    game_map.moveCurrentTile(0, -1);
  }
  if (key == GLUT_KEY_LEFT)
  {
    game_map.moveCurrentTile(-1, 0);
  }
  if (key == GLUT_KEY_RIGHT)
  {
    game_map.moveCurrentTile(1, 0);
  }
}

static void getKeys(unsigned char key, int x, int y)
{
  if (key == '[')
  {
    if (map_size[0] > 1)
    {
      map_size[0]--;
      game_map.resize(map_size[0], map_size[1]);
    }
  }
  if (key == ']')
  {
    if (map_size[0] > 0)
    {
      map_size[0]++;
      game_map.resize(map_size[0], map_size[1]);
    }
  }
  if (key == '-')
  {
    if (map_size[1] > 1)
    {
      map_size[1]--;
      game_map.resize(map_size[0], map_size[1]);
    }
  }
  if (key == '+' or key == '=')
  {
    if (map_size[1] > 0)
    {
      map_size[1]++;
      game_map.resize(map_size[0], map_size[1]);
    }
  }

  if (key == 'm')
  {
    do_draw_map_size = !do_draw_map_size;
  }
  if (key == 'e')
  {
    game_map.printAllElevations();
  }
  if (key == 'p')
  {
    game_map.printAllPValues();
  }
}

static void drawMapSize()
{
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2f(1.0f, canvas_Height - 14.0f);
  std::string message = '('   + std::to_string(map_size[0])
                      + ", "  + std::to_string(map_size[1])
                      + ')';
  for (char c : message)
  {
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
  }
}

static void displayScene()
{
  clearScreen();
  if (do_draw_map_size) { drawMapSize(); }
  game_map.drawMapCentered(CENTER_X, CENTER_Y, 0.0f, true);

  glFinish();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);
  glutDisplayFunc(displayScene);
  glutKeyboardFunc(getKeys);
  glutSpecialFunc(specialKeys);

  game_map.setTileW(20);
  game_map.setElevationQuantity(0, 5);
  game_map.setElevationQuantity(4, 3);

  glutIdleFunc([]() {
    
    glutPostRedisplay();
    });

  glutMainLoop();

  return 0;
}