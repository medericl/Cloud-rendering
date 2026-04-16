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
inline float BOX_MAX_X =  325.0f, BOX_MAX_Y = 30.0f,  BOX_MAX_Z =  325.0f;

// couleur du brouillard
inline Color fog_color(167, 157, 170);

// nombre de steps du raymarching volumique
//inline int FOG_STEPS = 60;
// Cloud
inline float DENSITY = 0.02f;
//inline int FOG_STEPS = 40; // précision dans le nuage
//inline int FOG_STEPS = 10; // précision dans le nuage
inline int FOG_STEPS = 5; // précision dans le nuage
inline float INITIAL_AMPLITUDE = 0.4f;
inline float INITIAL_FREQUENCY = 0.0096f; // taille nuage
inline float FREQ_SHAPE = 0.7577f;
inline float FREQ_DETAIL = 4.0f;
inline int OCTAVE = 4; // bruit -> change forme du nuage -> ajoute des détails
inline float LACUNARITY = 1.8; // vitesse de croissance de fréquence
inline float GAIN = 0.5; // poids de chaque octave
inline float K_DETAIL = 0.6;
inline float CLOUD_COVERAGE = 1.5;
inline float FALL_OFF_BOTTOM = 0.1f;
inline float FALL_OFF_TOP = 0.6f;
inline float K_WORLEY = 0.0073f;

// Wind
inline float WIND_SPEED = 200;
inline Vector3 WIND_DIRECTION = Vector3(-1,0,0);

