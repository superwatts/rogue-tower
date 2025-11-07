// utils.cpp

#include <iostream>
#include <string>
#include <math.h>
#include "utils.h"

constexpr bool CHAT = true;
constexpr unsigned int DEFAULT_SEED = 123456789;

// Perlin noise from https://www.youtube.com/watch?v=kCIaHqb60Cw
struct my_vector2 {
  float x;
  float y;
  my_vector2() { x = 0; y = 0; }
  my_vector2(float ix, float iy) { x = ix; y = iy; }
};

unsigned int hash(unsigned int x, unsigned int y, unsigned int seed) {
  //unsigned int h = x * 374761393u + y * 668265263u; // Prime multipliers
  unsigned int h = x * 374761393u + y * 668265263u + seed * 1442695040888963407u;
  h = (h ^ (h >> 13)) * 1274126177u;
  return h ^ (h >> 16);
}

// String hash to unsigned int. Taken from ChatGPT.
unsigned int fnv1a_hash(const std::string& text) {
  const unsigned int FNV_PRIME = 16777619u;
  unsigned int hash = 2166136261u;
  for (char c : text) {
    hash ^= static_cast<unsigned char>(c);
    hash *= FNV_PRIME;
  }
  return hash;
}

my_vector2 randomGradient(int ix, int iy, unsigned int seed) {
  // No precomputed gradients mean this works for any number of grid coordinates
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;
  unsigned a = ix, b = iy;
  a *= 3284157443;

  b ^= a << s | a >> w - s;
  b *= 1911520717;

  a ^= b << s | b >> w - s;
  a *= 2048419325;
  float random;
  if (not CHAT) { random = a * (3.14159265 / ~(~0u >> 1)); } // in [0, 2*Pi]
  else          { random = fmod(sin(a) * 43758.5453f, 1.0f) * 2.0f * 3.14159265f; }

  // Create the vector from the angle
  if (not CHAT)
  {
    my_vector2 v;
    v.x = sin(random);
    v.y = cos(random);
    return v;
  }
  else
  {
    unsigned int h = hash(ix, iy, seed);
    float angle = (h % 360) * 3.14159265f / 180.0f;
    return my_vector2(cos(angle), sin(angle));
  }
}

my_vector2 randomGradient(int ix, int iy) {
  return randomGradient(ix, iy, DEFAULT_SEED);
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y, unsigned int seed) {
  // Get gradient from integer coordinates
  my_vector2 gradient = randomGradient(ix, iy, seed);

  // Compute the distance vector
  float dx = x - (float)ix;
  float dy = y - (float)iy;

  // Compute the dot-product
  return (dx * gradient.x + dy * gradient.y);
}

float dotGridGradient(int ix, int iy, float x, float y) {
  return dotGridGradient(ix, iy, x, y, DEFAULT_SEED);
}

static float interpolate(float a0, float a1, float w)
{
  return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

// Chat lerp:
float lerp(float a0, float a1, float w)
{
  return a0 + w * (a1 - a0);
}


// Chat fade:
// Perlin's original fade curve
float chatfade(float t)
{
  return t * t * t * (t * (t * 6 - 15) + 10);
}


// Sample uint-seeded Perlin noise at coordinates x, y
float perlin(float x, float y, unsigned int seed) {

  // Determine grid cell corner coordinates
  int x0 = (int)x;
  int y0 = (int)y;
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  // Compute Interpolation weights
  float sx, sy;
  if (not CHAT)
  {
    sx = x - (float)x0;
    sy = y - (float)y0;
  }
  else
  {
    sx = chatfade(x - x0);
    sy = chatfade(y - y0);
  }

  // Compute and interpolate top two corners
  float n0 = dotGridGradient(x0, y0, x, y, seed);
  float n1 = dotGridGradient(x1, y0, x, y, seed);
  float ix0;
  if (not CHAT) { ix0 = interpolate(n0, n1, sx); }
  else          { ix0 = lerp(n0, n1, sx); }

  // Compute and interpolate bottom two corners
  n0 = dotGridGradient(x0, y1, x, y, seed);
  n1 = dotGridGradient(x1, y1, x, y, seed);
  float ix1;
  if (not CHAT) { ix1 = interpolate(n0, n1, sx); }
  else          { ix1 = lerp(n0, n1, sx); }

  // Final step: interpolate between the two previously interpolated values, now in y
  float value;
  if (not CHAT) { value = interpolate(ix0, ix1, sy); }
  else          { value = lerp(ix0, ix1, sy); }

  return value;
}

// Sample string-seeded Perlin noise at coordinates x, y
float perlin(float x, float y, std::string seed)
{
  return perlin(x, y, fnv1a_hash(seed));
}

// Sample UNSEEDED perlin noise. Uses DEFAULT_SEED.
float perlin(float x, float y)
{
  return perlin(x, y, DEFAULT_SEED);
}