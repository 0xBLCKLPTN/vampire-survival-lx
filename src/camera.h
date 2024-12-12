#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct Camera {
  int viewport_width, viewport_height;
  int map_width, map_height;
  int x,y;
  int target_x;
  int target_y;
  float easing_duration;
  float zoom_level;
  float target_zoom_level;
} Camera;

#endif
