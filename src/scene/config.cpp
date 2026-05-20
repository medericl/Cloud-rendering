#include "config.hh"
#include "scene.hh"

static bool debug_was_enabled = false;
static bool big_was_enabled = false;
static bool pov_was_enabled = false;
static bool sunset_was_enabled = false;


static Color sunset_saved_sky_horizon = SKY_HORIZON;
static Color sunset_saved_sky_zenith  = SKY_ZENITH;
static Color sunset_saved_sun         = SUN;
static Point3 sunset_saved_sun_pos    = SUN_POS;
static Color sunset_saved_fog         = fog_color;

static void apply_sunset_mode()
{
    SKY_HORIZON = Color(255, 90, 25);
    SKY_ZENITH  = Color(20, 10, 60);
    SUN         = Color(255, 180, 60);
    SUN_POS     = Point3(1200.0f, 30.0f, 0.0f);
    fog_color   = Color(255, 140, 70);
}

static int saved_fog_steps = FOG_STEPS;

static float saved_box_min_x = BOX_MIN_X;
static float saved_box_min_y = BOX_MIN_Y;
static float saved_box_min_z = BOX_MIN_Z;
static float saved_box_max_x = BOX_MAX_X;
static float saved_box_max_y = BOX_MAX_Y;
static float saved_box_max_z = BOX_MAX_Z;
static Point3 saved_sun_pos = SUN_POS;
static Point3 saved_camera_origin;
static Point3 saved_camera_target;
static Vector3 saved_camera_up;

void apply_debug_mode()
{
    FOG_STEPS = 5;
}

void apply_big_mode()
{
    BOX_MIN_X = -500.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MIN_Z = -500.0f;
    BOX_MAX_X = 500.0f;
    BOX_MAX_Y = 200.0f;
    BOX_MAX_Z = 500.0f;
    SUN_POS = Point3((BOX_MIN_X + BOX_MAX_X) * 0.5f,
                     BOX_MAX_Y + 200.0f,
                     (BOX_MIN_Z + BOX_MAX_Z) * 0.5f);
}

void apply_pov_mode(Scene& scene)
{
    float center_x = (BOX_MIN_X + BOX_MAX_X) * 0.5f;
    float center_z = (BOX_MIN_Z + BOX_MAX_Z) * 0.5f;
    float width = scene.camera.plan_image.p_image_width;
    float height = scene.camera.plan_image.p_image_height;

    scene.camera.origin = Point3(center_x, BOX_MIN_Y - 80.0f, center_z - 160.0f);
    scene.camera.p = Point3(center_x, BOX_MIN_Y + 120.0f, center_z);
    scene.camera.direction_up = Vector3(0.0f, 0.0f, 1.0f);
    scene.camera.plan_image = Plan_image(scene.camera.p,
                                         scene.camera.p - scene.camera.origin,
                                         scene.camera.direction_up,
                                         width,
                                         height);
}

void update_render_modes(Scene& scene)
{
    if (debug && !debug_was_enabled)
        saved_fog_steps = FOG_STEPS;
    if (!debug && debug_was_enabled)
        FOG_STEPS = saved_fog_steps;
    if (debug)
        apply_debug_mode();
    debug_was_enabled = debug;

    if (big && !big_was_enabled) {
        saved_box_min_x = BOX_MIN_X;
        saved_box_min_y = BOX_MIN_Y;
        saved_box_min_z = BOX_MIN_Z;
        saved_box_max_x = BOX_MAX_X;
        saved_box_max_y = BOX_MAX_Y;
        saved_box_max_z = BOX_MAX_Z;
        saved_sun_pos = SUN_POS;
    }

    if (!big && big_was_enabled) {
        BOX_MIN_X = saved_box_min_x;
        BOX_MIN_Y = saved_box_min_y;
        BOX_MIN_Z = saved_box_min_z;
        BOX_MAX_X = saved_box_max_x;
        BOX_MAX_Y = saved_box_max_y;
        BOX_MAX_Z = saved_box_max_z;
        SUN_POS = saved_sun_pos;
    }

    if (big)
        apply_big_mode();
    big_was_enabled = big;

    if (pov && !pov_was_enabled) {
        saved_camera_origin = scene.camera.origin;
        saved_camera_target = scene.camera.p;
        saved_camera_up = scene.camera.direction_up;
    }

    if (!pov && pov_was_enabled) {
        float width = scene.camera.plan_image.p_image_width;
        float height = scene.camera.plan_image.p_image_height;

        scene.camera.origin = saved_camera_origin;
        scene.camera.p = saved_camera_target;
        scene.camera.direction_up = saved_camera_up;
        scene.camera.plan_image = Plan_image(scene.camera.p,
                                             scene.camera.p - scene.camera.origin,
                                             scene.camera.direction_up,
                                             width,
                                             height);
    }

    if (pov)
        apply_pov_mode(scene);
    pov_was_enabled = pov;

    if (SUNSET && !sunset_was_enabled) {
        sunset_saved_sky_horizon = SKY_HORIZON;
        sunset_saved_sky_zenith  = SKY_ZENITH;
        sunset_saved_sun         = SUN;
        sunset_saved_sun_pos     = SUN_POS;
        sunset_saved_fog         = fog_color;
    }
    if (!SUNSET && sunset_was_enabled) {
        SKY_HORIZON = sunset_saved_sky_horizon;
        SKY_ZENITH  = sunset_saved_sky_zenith;
        SUN         = sunset_saved_sun;
        SUN_POS     = sunset_saved_sun_pos;
        fog_color   = sunset_saved_fog;
    }
    if (SUNSET) apply_sunset_mode();
    sunset_was_enabled = SUNSET;
}
