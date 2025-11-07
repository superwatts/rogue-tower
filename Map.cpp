// Map.cpp

#include "Map.h"
#include<iomanip>

constexpr float SCALE = 0.15f; // 0.15f looks good. so does 0.25 w p_passable = 0.5
constexpr bool DEBUG_PRINT = false;
constexpr bool DEBUG_COLOR = false;
float DEBUG_COLOR_V[3] = { 1.0f, 0.0f, 1.0f };
constexpr bool DRAW_P = true;
constexpr unsigned int DEFAULT_SEED = 123456789;

Map::Map()
{
  // Do something.
}

Map::Map(int num_x_tiles, int num_y_tiles)
{
  x_tiles = num_x_tiles;
  y_tiles = num_y_tiles;
  tile_map = new Tile * [num_x_tiles];

  populateMap(num_x_tiles, num_y_tiles, seed_int);
  buildColorMap();
  buildElevationProportions();
}

Map::Map(int num_x_tiles, int num_y_tiles, unsigned int seed)
{
  x_tiles = num_x_tiles;
  y_tiles = num_y_tiles;
  tile_map = new Tile * [num_x_tiles];
  seed_int = seed;

  populateMap(num_x_tiles, num_y_tiles, seed_int);
  buildColorMap();
  buildElevationProportions();
}

Map::Map(int num_x_tiles, int num_y_tiles, std::string seed)
{
  x_tiles = num_x_tiles;
  y_tiles = num_y_tiles;
  tile_map = new Tile * [num_x_tiles];
  seed_int = fnv1a_hash(seed);

  populateMap(num_x_tiles, num_y_tiles, seed_int);
  buildColorMap();
  buildElevationProportions();
}

Map::~Map()
{
  // Destructor.
  clearMap();
  clearColorMap();
  clearElevationValues();
}

void Map::drawMap(float x, float y, float z, bool do_outline)
{
  Tile* current_tile = NULL;
  for (int tx = 0; tx < x_tiles; tx++)
  {
    for (int ty = 0; ty < y_tiles; ty++)
    {
      current_tile = &tile_map[tx][ty];
      if (current_tile != NULL)
      {
        float* color = color_map[current_tile->elevation];
        
        if (DEBUG_COLOR)
        {
          color = DEBUG_COLOR_V;
        }
        /*
        else
        {
          if (tx == selected_tile[0] and ty == selected_tile[1])
          {
            color = selected_color;
            //glColor3fv(selected_color);
          }
          else if (current_tile->passable) { color = low_passable_color; }//glColor3fv(passable_color); }
          else if (current_tile->path) { color = low_passable_color; }//glColor3fv(path_color); }
          else { color = low_impassable_color; }//glColor3fv(impassable_color); }
        }
        */
        if (color == NULL) { continue; }
        if (DRAW_P)
        {
          glColor3f(color[0] * current_tile->p_value, color[1] * current_tile->p_value, color[2] * current_tile->p_value);
        }
        else
        {
          glColor3fv(color);
        }
        current_tile->drawTile(x + (tx * tile_w), y + (ty * tile_w), z, tile_w, do_outline);
        //std::cout << '(' << x << ", " << y << ") is not null\n";
      }
      else
      {
        //std::cout << '(' << x << ", " << y << ") is NULL\n";
      }
    }
  }
}

void Map::drawMapCentered(float x, float y, float z, bool do_outline)
{
  float cx = x - ((tile_w * x_tiles) / 2.0f);
  float cy = y - ((tile_w * y_tiles) / 2.0f);
  drawMap(cx, cy, z, do_outline);
}

void Map::setTileW(float tile_width)
{
  tile_w = tile_width;
}

void Map::setCurrentTile(int x, int y)
{
  if      (x < 0)         { selected_tile[0] = 0; }
  else if (x >= x_tiles)  { selected_tile[0] = x_tiles - 1; }
  else                    { selected_tile[0] = x; }
  
  if      (y < 0)         { selected_tile[1] = 0; }
  else if (y >= y_tiles)  { selected_tile[1] = y_tiles - 1; }
  else                    { selected_tile[1] = y; }
}

void Map::moveCurrentTile(int dx, int dy)
{
  setCurrentTile(selected_tile[0] + dx, selected_tile[1] + dy);
}

const void Map::clearMap()
{
  for (int x = 0; x < x_tiles; x++)
  {
    if (tile_map[x] != NULL)
    {
      delete[] tile_map[x];
    }
  }
  if (tile_map != NULL)
  {
  delete[] tile_map;
  }
}

// Seeded map population.
void Map::populateMap(int num_x_tiles, int num_y_tiles, unsigned int seed)
{
  if (elevation_proportions == NULL) { buildElevationProportions(); }
  //float p_passable = 0.5;
  for (int x = 0; x < num_x_tiles; x++)
  {
    tile_map[x] = new Tile[num_y_tiles];
    for (int y = 0; y < num_y_tiles; y++)
    {
      //std::cout << float(x) << ", " << float(y) << '\n';
      //float p = perlin(float(x) / float(num_x_tiles), float(y) / float(num_y_tiles));
      float p = perlin(float(x) * SCALE, float(y) * SCALE, seed);
      p = (p + 1.0f) / 2.0f;
      tile_map[x][y].p_value = p;
      if (DEBUG_PRINT) { std::cout << '(' << float(x) << ", " << float(y) << ") = " << p << '\n'; }
      // if (p > p_passable { tile_map[x][y].passable = false; }

      // Calculate elevation
      if      (p < elevation_proportions[0]) { tile_map[x][y].elevation = 0; }
      else if (p >= elevation_proportions[elevation_levels-2]) { tile_map[x][y].elevation = elevation_levels-1; }
      else
      {
        for (int i = 1; i < elevation_levels - 1; i++)
        {
          if (p >= elevation_proportions[i - 1] and p < elevation_proportions[i + 1])
          {
            tile_map[x][y].elevation = i;
          }
        }
      }
    }
  }
}

// Unseeded map population. Uses Map's seed_int.
void Map::populateMap(int num_x_tiles, int num_y_tiles)
{
  populateMap(num_x_tiles, num_y_tiles, seed_int);
}

void Map::resize(int num_x_tiles, int num_y_tiles)
{
  clearMap();
  
  x_tiles = num_x_tiles;
  y_tiles = num_y_tiles;
  tile_map = new Tile* [x_tiles];

  populateMap(num_x_tiles, num_y_tiles);

  if (selected_tile[0] >= x_tiles) { selected_tile[0] = x_tiles - 1; }
  if (selected_tile[1] >= y_tiles) { selected_tile[1] = y_tiles - 1; }
}

void Map::setSeed(unsigned int new_seed)
{
  seed_int = new_seed;
}
void Map::setSeed(std::string new_seed)
{
  seed_int = fnv1a_hash(new_seed);
}

float* Map::getInterpolatedColor(int level, int lowest_elevation, int highest_elevation)
{
  //float influence = 1.0f / float(highest_elevation - lowest_elevation);
  level -= lowest_elevation;
  highest_elevation -= lowest_elevation;
  float influence = float(level) / float(highest_elevation);

  float d_color[3]  = { high_passable_color[0] - low_passable_color[0],
                        high_passable_color[1] - low_passable_color[1],
                        high_passable_color[2] - low_passable_color[2]};
  
  float* color = new float[3];
  color[0] = low_passable_color[0] + (influence * d_color[0]);
  color[1] = low_passable_color[1] + (influence * d_color[1]);
  color[2] = low_passable_color[2] + (influence * d_color[2]);

  return color;
}
void Map::buildColorMap()
{
  if (elevation_levels < 2) { return; } // FIX LATER
  color_map = new float*[elevation_levels];
  color_map[0]                    = low_impassable_color;
  color_map[elevation_levels - 1] = high_impassable_color;
  for (int i = 1; i <= elevation_levels - 2; i++)
  {
    color_map[i] = getInterpolatedColor(i, 1, elevation_levels - 2);
  }
}
void Map::clearColorMap()
{
  if (color_map != NULL)
  {
    for (int i = 0; i < elevation_levels; i++)
    {
      delete[] color_map[i];
    }
  }
  delete color_map;
}

void Map::buildElevationQuantities()
{
  if (elevation_quantities == NULL) { elevation_quantities = new float[elevation_levels]; }
  for (int i = 0; i < elevation_levels; i++)
  {
    elevation_quantities[i] = 1.0f;
  }
}
void Map::setElevationQuantity(int level, float quantity)
{
  setElevationQuantity(level, quantity, true);
}
void Map::setElevationQuantity(int level, float quantity, bool do_build)
{
  if (elevation_quantities == NULL) { buildElevationQuantities(); }
  if (level > (elevation_levels - 1) or level < 0) { return; }
  else
  {
    elevation_quantities[level] = quantity;
  }
  if (do_build) { buildElevationProportions(); }
  else          { return; }
}
void Map::buildElevationProportions()
{
  if (elevation_proportions == NULL)  { elevation_proportions = new float[elevation_levels]; }
  if (elevation_quantities  == NULL)  { buildElevationQuantities(); }
  float sum_q = 0.0f;
  for (int i = 0; i < elevation_levels; i++)
  {
    sum_q += elevation_quantities[i];
  }
  float sum_p = 0.0f;
  float this_p = 0.0f;
  for (int i = 0; i < elevation_levels; i++)
  {
    this_p = (elevation_quantities[i] / sum_q);
    elevation_proportions[i] = this_p + sum_p;
    sum_p += this_p;
    // DEBUG MESSAGE:
    std::cout << elevation_proportions[i] << ", ";
  }
  // DEBUG MESSAGE:
  std::cout << '\n';

  // Now remap from [0,1] to [0.2,0.8]
  float min_val = 0.2f;
  float max_val = 0.8f;
  for (int i = 0; i < elevation_levels; i++)
  {
    elevation_proportions[i] = min_val + (max_val - min_val) * elevation_proportions[i];
    std::cout << elevation_proportions[i] << ", ";
  }
  std::cout << '\n';

  populateMap(x_tiles, y_tiles);
}
void Map::clearElevationValues()
{
  if (elevation_proportions != NULL) { delete elevation_proportions; }
  if (elevation_quantities  != NULL) { delete elevation_quantities; }
}

void Map::printAllElevations()
{
  if (tile_map != NULL)
  {
    for (int y = 0; y < y_tiles; y++)
    {
      for (int x = 0; x < x_tiles; x++)
      {
        if (tile_map[x] == NULL) { continue; }
        std::cout << std::setw(3) << std::left << tile_map[x][y].elevation;
      }
      std::cout << '\n';
    }
  }
}
void Map::printAllPValues()
{
  if (tile_map != NULL)
  {
    for (int y = 0; y < y_tiles; y++)
    {
      for (int x = 0; x < x_tiles; x++)
      {
        if (tile_map[x] == NULL) { continue; }
        std::cout << std::setw(4) << std::left << std::setprecision(1) <<(tile_map[x][y].p_value);
      }
      std::cout << '\n';
    }
  }
}
