// main.cpp

#include <iostream>
#include <C:/OpenGLwrappers/glew-1.10.0-win32/glew-1.10.0/include/GL/glew.h>
#include <C:/OpenGLwrappers/freeglut-MSVC-2.8.1-1.mp/freeglut/include/GL/freeglut.h> 
#include "OpenGL445Setup-2025.h"
#include "Map.h"


// Customize below 3 items to make canvas of own size & labeling
#define canvas_Width 1200
#define canvas_Height 1200
char canvas_Name[] = "Tower Defense Roguelike";
float CENTER_X = canvas_Width   / 2.0f;
float CENTER_Y = canvas_Height  / 2.0f;

std::string SEED = "gorbus";
bool do_draw_map_params = true;
bool draw_map_grid = false;

const float CLEAR_COLOR[3] = { 0.0f, 0.0f, 0.0f };

int map_size[2] = { 200, 200 };
Map game_map = Map(map_size[0], map_size[1], SEED);

constexpr float D_PERLIN = 0.0125;
int map_movement_speed = 5;

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
  if (key == 'w') { game_map.moveOffset(0,                    map_movement_speed); }
  if (key == 'a') { game_map.moveOffset(-map_movement_speed,  0); }
  if (key == 's') { game_map.moveOffset(0,                    -map_movement_speed); }
  if (key == 'd') { game_map.moveOffset(map_movement_speed,   0); }
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
  if (key == '+' || key == '=')
  {
    if (map_size[1] > 0)
    {
      map_size[1]++;
      game_map.resize(map_size[0], map_size[1]);
    }
  }

  if (key == '\'')
  {
    game_map.perlin_scale += D_PERLIN;
    game_map.populateMap();
  }
  if (key == ';')
  {
    game_map.perlin_scale -= D_PERLIN;
    game_map.populateMap();
  }

  if (key == 'm') { do_draw_map_params    = !do_draw_map_params; }
  if (key == 'p') { game_map.draw_p       = !game_map.draw_p; }
  if (key == 'o') { draw_map_grid      = !draw_map_grid; }
  if (key == 'b') { game_map.draw_borders = !game_map.draw_borders; }

  if (key == 'E') { game_map.printAllElevations(); }
  if (key == 'V') { game_map.printAllPValues(); }
}

static void drawMapParams()
{
  glColor3f(1.0f, 1.0f, 1.0f);
  glRasterPos2f(1.0f, canvas_Height - 14.0f);
  std::string message = "Size: (" + std::to_string(map_size[0])
                      + ", "  + std::to_string(map_size[1])
                      + ')';
  for (char c : message) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); }

  glRasterPos2f(1.0f, canvas_Height - 27.0f);
  message = "Scale: " + std::to_string(game_map.perlin_scale);
  for (char c : message) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); }
}

static void displayScene()
{
  clearScreen();
  glLoadIdentity();
  //glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
  if (do_draw_map_params) { drawMapParams(); }
  //game_map.drawMapCentered(CENTER_X, CENTER_Y, 0.0f, 20.0f, true);
  game_map.drawMapCenteredFit(CENTER_X, CENTER_Y, 0.0f, draw_map_grid, canvas_Width - 70, canvas_Height - 70);

  glFinish();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);
  glutDisplayFunc(displayScene);
  glutKeyboardFunc(getKeys);
  glutSpecialFunc(specialKeys);

  game_map.setElevationLevels(15);
  game_map.setElevationQuantity(0, 14);
  //game_map.setElevationQuantity(0, 2);
  //game_map.setElevationQuantity(2, 4);
  //game_map.setElevationQuantity(4, 3);

  glutIdleFunc([]() {
    
    glutPostRedisplay();
    });

  glutMainLoop();

  return 0;
}