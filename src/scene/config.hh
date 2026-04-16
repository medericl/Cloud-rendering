#pragma once
#include "../image/color.hh"
#include "../vector/vector.hh"

// RAY
inline float kd = 0.9f;
inline float ks = 0.33f;
inline float kr = 0.1f;
inline float ns = 1;
inline int rec_ray = 5;
inline float k_close = 0.001f;
inline float k_far = 1000.0f;

// SCENE
inline Color ambient_color(30, 30, 30);
inline Color background_color(18, 18, 18);

// LIGHT
inline Color COLOR_LIGHT1(21,230,230);
inline Color COLOR_LIGHT2(0,255,202);
inline Color COLOR_LIGHT3(255,0,233);
inline Color COLOR_LIGHT4(94,229,0);
inline Color COLOR_FLOOR(21,21,21);

// bounding box du volume
inline float BOX_MIN_X = -325.0f, BOX_MIN_Y = 0.0f, BOX_MIN_Z = -325.0f;
inline float BOX_MAX_X =  325.0f, BOX_MAX_Y = 250.0f,  BOX_MAX_Z =  325.0f;

// couleur du brouillard
inline Color fog_color(167, 157, 170);

// nombre de steps du raymarching volumique
inline int FOG_STEPS = 60;
inline float INITIAL_FREQUENCY = 0.0096f;
inline float INITIAL_AMPLITUDE = 0.4f;

// Cloud
inline float DENSITY = 0.02f;
inline float WIND_SPEED = 200;
inline Vector3 WIND_DIRECTION = Vector3(-1,0,0);

