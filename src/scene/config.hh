#pragma once
#include "../image/color.hh"
#include "../vector/vector.hh"

inline bool SUNSET = false;

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
//inline Color background_color(78, 120, 238);
inline Color background_color(67, 137, 210);
inline Color SKY_HORIZON(180, 215, 245);
inline Color SKY_ZENITH(65, 135, 220);

// LIGHT
inline Color COLOR_LIGHT1(21,230,230);
inline Color COLOR_LIGHT2(0,255,202);
inline Color COLOR_LIGHT3(255,0,233);
inline Color COLOR_LIGHT4(94,229,0);
inline Color COLOR_FLOOR(21,21,21);
inline Color COLOR_SUN(255, 235, 200);
inline Color SUN(255, 235, 200);
inline Point3 SUN_POS(500.0f, 2000.0f, 500.0f);

// bounding box du volume
inline float BOX_MIN_X = -325.0f, BOX_MIN_Y = 0.0f, BOX_MIN_Z = -325.0f;
inline float BOX_MAX_X =  325.0f, BOX_MAX_Y = 230.0f,  BOX_MAX_Z =  325.0f;


// couleur du brouillard
//inline Color fog_color(167, 157, 170);
//inline Color fog_color(200, 200, 200);
//inline Color fog_color(185, 205, 225);
inline Color fog_color(220, 220, 220);
inline Color CLOUD_SHADOW_COLOR(70, 95, 145);

// nombre de steps du raymarching volumique
// Cloud
//inline float DENSITY = 0.1f;
inline float DENSITY = 0.05f;
inline int FOG_STEPS = 25; // précision dans le nuage
inline int SUN_STEPS = 5;
inline float INITIAL_AMPLITUDE = 0.4f;
inline float INITIAL_FREQUENCY = 0.0096f; // taille nuage
inline float FREQ_SHAPE = 0.7577f;
inline float FREQ_DETAIL = 4.0f;
inline int OCTAVE = 4; // bruit -> change forme du nuage -> ajoute des détails
inline float LACUNARITY = 1.8; // vitesse de croissance de fréquence
//inline float GAIN = 0.5; // poids de chaque octave
inline float GAIN = 0.3; // poids de chaque octave
inline float K_DETAIL = 0.7;
inline float CLOUD_COVERAGE = 1.5;
inline float FALL_OFF_BOTTOM = 0.1f;
inline float FALL_OFF_TOP = 0.6f;
//inline float FREQ_WORLEY = 0.0073f;
inline float K_HG=0.3f;
inline float G_HG=0.5f;
inline float G_HG_SILVER=0.9f;
inline float POWDER_STRENGTH = 6.0f;
inline float POWDER_AMOUNT = 1.0f;
inline float K_AMBIENT_CLOUD= 0.4f; // pas dans ui

// WORLEY
inline float FREQ_WORLEY = 0.008f;
inline float K_WORLEY = 0.2f;

// Terrain
inline float TERRAIN_TEXTURE = 0.75f; // 0=dark earth, 0.5=green, 1=rock
inline float TERRAIN_SCALE   = 1.0f; // vertical height multiplier

// Wind
inline float WIND_SPEED = 200;
inline Vector3 WIND_DIRECTION = Vector3(-1,0,0);

// setup
inline bool debug = false; // -> inline int FOG_STEPS = 5; // précision dans le nuage
inline bool high_res = false;
inline bool low_res = false;
inline bool big = false;
inline bool pov = false;
inline bool JITTER = false;
inline bool VERTICAL_FALL_OFF = true;
inline bool HG = true;
inline bool AMBIENT_CLOUD = true;
inline bool LIGHT = true;
inline bool WIND = true;
inline bool WORLEY_ = true;
inline bool W1 = false;
inline bool WORLEY_MIX = false;

// Coverage
inline bool COVERAGE = false;
inline int COVERAGE_OCT = 2;
inline float COVERAGE_FREQ = 0.0015f;
inline float K_COVERAGE = 1.0f;

inline bool CUMULUS = true;
inline bool TOWERING_CUMULUS = false;
inline bool STRATUS = false;
inline bool CIRRUS = false;
inline bool CIRROCUMULUS = false;
inline bool NIMBUS = false;
inline bool STORM = false;
inline bool FOG = false;
inline bool CLOUD2 = false;

inline bool DAY = false;
inline bool SUNRISE = false;

enum class CloudPreset {
    Cumulus,
    Cloud2,
    ToweringCumulus,
    Stratus,
    Cirrus,
    Cirrocumulus,
    Nimbus
};

class Scene;
void apply_cloud_preset(CloudPreset preset);
void apply_high_res_mode();
void apply_low_res_mode();
void apply_big_mode();
void apply_pov_mode(Scene& scene);
void update_render_modes(Scene& scene);
