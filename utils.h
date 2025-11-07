#pragma once

// utils.h

#include <iostream>
#include <string>
#include <math.h>

extern enum DIRECTION
{
  D_UP,
  D_DOWN,
  D_LEFT,
  D_RIGHT
};

// Perlin noise from https://www.youtube.com/watch?v=kCIaHqb60Cw
extern struct my_vector2;

extern unsigned int hash(unsigned int x, unsigned int y, unsigned int seed);

// DEsc
extern unsigned int fnv1a_hash(const std::string& text);

extern my_vector2 randomGradient(int ix, int iy, unsigned int seed);
extern my_vector2 randomGradient(int ix, int iy);

// Computes the dot product of the distance and gradient vectors.
extern float dotGridGradient(int ix, int iy, float x, float y, unsigned int seed);

extern float interpolate(float a0, float a1, float w);

// Chat lerp:
extern float lerp(float a0, float a1, float w);


// Chat fade:
// Perlin's original fade curve
extern float chatfade(float t);


// Sample Perlin noise at coordinates x, y
extern float perlin(float x, float y, unsigned int seed);
extern float perlin(float x, float y, std::string seed);
extern float perlin(float x, float y);
