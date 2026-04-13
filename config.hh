#pragma once
#include "color.hh"

inline float kd = 0.9f;
inline float ks = 0.3f;
inline float kr = 0.1f;
inline float ns = 1;
inline int rec_ray = 5;
inline float k_close = 0.001f;
inline float k_far = 1000.0f;

inline Color ambient_color(30, 30, 30);
inline Color background_color(18, 18, 18);
//inline Color background_color(21, 21, 35);

inline Color COLOR_LIGHT1(21,230,230);
inline Color COLOR_LIGHT2(0,255,202);
inline Color COLOR_LIGHT3(255,0,233);
inline Color COLOR_LIGHT4(94,229,0);
//inline Color COLOR_FLOOR(100,100,100);
inline Color COLOR_FLOOR(21,21,21);

// brouillard
inline float DENSITY = 0.086f;

// bounding box du volume
inline float BOX_MIN_X = -300.0f, BOX_MIN_Y = 20.5f, BOX_MIN_Z = -300.0f;
inline float BOX_MAX_X =  300.0f, BOX_MAX_Y = 90.0f,  BOX_MAX_Z =  300.0f;

// couleur du brouillard
inline Color fog_color(86, 75, 89);
//inline Color fog_color(61, 68, 73);

// nombre de steps du raymarching volumique
inline int FOG_STEPS = 5;
