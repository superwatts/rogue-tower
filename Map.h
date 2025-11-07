// Map.h
#include <iostream>
#include <string>
#include "Tile.h"
#include "utils.h"

#pragma once
class Map
{
private:
  int x_tiles = 0;
  int y_tiles = 0;
  int selected_tile[2] = { 0, 0 };
  int block_size = 0; // # Tiles
  Tile** tile_map = NULL;
  float tile_w = 5;
  int n_passable  = 0;
  int n_impass    = 0;
  int n_path      = 0;

  // How many elevation levels you want. 1 == hole/water, 5 == walll
  unsigned int elevation_levels = 5;
  float** color_map = NULL;
  float* elevation_quantities  = NULL;
  float* elevation_proportions = NULL;

  unsigned int seed_int = 123456789;

  // rgb(69, 112, 36)
  // .0f / 255.0f, 
  float low_passable_color[3]   = { 69.0f / 255.0f, 112.0f / 255.0f, 36.0f / 255.0f, };
  //rgb(128, 196, 78)
  float high_passable_color[3]  = { 128.0f / 255.0f, 196.0f / 255.0f, 78.0f / 255.0f, };
  // rgb(79, 155, 255)
  float low_impassable_color[3] = { 79.0f / 255.0f, 155.0f / 255.0f, 255.0f / 255.0f, };
  // rgb(138, 143, 135)
  float high_impassable_color[3] = { 138.0f / 255.0f, 143.0f / 255.0f, 135.0f / 255.0f, };

  float path_color[3]       = { 0.5f, 0.5f, 0.5f };
  float selected_color[3]   = { 0.0f, 0.0f, 1.0f };
public:
  Map();
  Map(int num_x_tiles, int num_y_tiles);
  Map(int num_x_tiles, int num_y_tiles, unsigned int seed);
  Map(int num_x_tiles, int num_y_tiles, std::string seed);
  ~Map();

  void drawMap(float x, float y, float z, bool do_outline);
  void drawMapCentered(float x, float y, float z, bool do_outline);
  void setTileW(float tile_width);
  void setCurrentTile(int x, int y);
  void moveCurrentTile(int dx, int dy);
  const void clearMap();
  void populateMap(int num_x_tiles, int num_y_tiles);
  void populateMap(int num_x_tiles, int num_y_tiles, unsigned int seed);
  void resize(int num_x_tiles, int num_y_tiles);
  void setSeed(unsigned int new_seed);
  void setSeed(std::string new_seed);
  float* getInterpolatedColor(int level, int lowest_elevation, int highest_elevation);
  void buildColorMap();
  void clearColorMap();
  void buildElevationQuantities();
  void setElevationQuantity(int level, float quantity);
  void setElevationQuantity(int level, float quantity, bool do_build);
  void buildElevationProportions();
  void clearElevationValues();
  void printAllElevations();
  void printAllPValues();
};

