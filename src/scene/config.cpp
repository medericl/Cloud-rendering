#include "config.hh"
#include "scene.hh"

static bool res_mode_was_enabled = false;
static bool big_was_enabled = false;
static bool pov_was_enabled = false;

static int saved_fog_steps = FOG_STEPS;

static float saved_box_min_x = BOX_MIN_X;
static float saved_box_min_y = BOX_MIN_Y;
static float saved_box_min_z = BOX_MIN_Z;
static float saved_box_max_x = BOX_MAX_X;
static float saved_box_max_y = BOX_MAX_Y;
static float saved_box_max_z = BOX_MAX_Z;
static Point3 saved_camera_origin;
static Point3 saved_camera_target;
static Vector3 saved_camera_up;

static void select_cloud_type(CloudPreset preset)
{
    CUMULUS = preset == CloudPreset::Cumulus;
    CLOUD2 = preset == CloudPreset::Cloud2;
    TOWERING_CUMULUS = preset == CloudPreset::ToweringCumulus;
    STRATUS = preset == CloudPreset::Stratus;
    CIRRUS = preset == CloudPreset::Cirrus;
    CIRROCUMULUS = preset == CloudPreset::Cirrocumulus;
    NIMBUS = preset == CloudPreset::Nimbus;
    FOG = preset == CloudPreset::Fog;
    STORM = false;
}

static void apply_cumulus_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.05f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_AMPLITUDE = 0.4f;
    INITIAL_FREQUENCY = 0.0096f;
    FREQ_SHAPE = 0.7577f;
    FREQ_DETAIL = 4.0f;
    OCTAVE = 4;
    LACUNARITY = 1.8f;
    GAIN = 0.3f;
    K_DETAIL = 0.7f;
    CLOUD_COVERAGE = 1.5f;
    FALL_OFF_BOTTOM = 0.1f;
    FALL_OFF_TOP = 0.6f;
    FREQ_WORLEY = 0.008f;
    K_WORLEY = 0.2f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = true;
    W1 = false;
    WORLEY_MIX = false;
    COVERAGE = false;
    COVERAGE_OCT = 2;
    COVERAGE_FREQ = 0.0015f;
    K_COVERAGE = 1.0f;
    WIND_SPEED = 200.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}

static void apply_towering_cumulus_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.05f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_AMPLITUDE = 0.4f;
    INITIAL_FREQUENCY = 0.0096f;
    FREQ_SHAPE = 0.7577f;
    FREQ_DETAIL = 3.5154f;
    OCTAVE = 4;
    LACUNARITY = 0.6766f;
    GAIN = 0.25f;
    K_DETAIL = 0.2419f;
    CLOUD_COVERAGE = 0.9714f;
    FALL_OFF_BOTTOM = 0.9326f;
    FALL_OFF_TOP = 0.2388f;
    FREQ_WORLEY = 0.0f;
    K_WORLEY = 0.0f;
    WIND_SPEED = 60.0f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = false;
    W1 = false;
    WORLEY_MIX = false;
    COVERAGE = false;
    COVERAGE_OCT = 2;
    COVERAGE_FREQ = 0.0015f;
    K_COVERAGE = 1.0f;
    WIND_SPEED = 120.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}

static void apply_cloud2_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.07f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_FREQUENCY = 0.0066f;
    INITIAL_AMPLITUDE = 0.926f;
    FREQ_SHAPE = 0.4714f;
    FREQ_DETAIL = 4.0f;
    OCTAVE = 9;
    LACUNARITY = 0.6766f;
    GAIN = 0.0f;
    K_DETAIL = 0.2419f;
    CLOUD_COVERAGE = 0.9714f;
    FALL_OFF_BOTTOM = 0.5626f;
    FALL_OFF_TOP = 0.503f;
    FREQ_WORLEY = 0.0f;
    K_WORLEY = 0.2f;
    WIND_SPEED = 160.0f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = false;
    W1 = false;
    WORLEY_MIX = false;
    COVERAGE = false;
    COVERAGE_OCT = 2;
    COVERAGE_FREQ = 0.0015f;
    K_COVERAGE = 1.0f;
    WIND_SPEED = 120.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}




static void apply_stratus_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.05f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_AMPLITUDE = 0.3736f;
    INITIAL_FREQUENCY = 0.02f;
    FREQ_SHAPE = 0.7577f;
    FREQ_DETAIL = 4.0f;
    OCTAVE = 4;
    LACUNARITY = 1.2273f;
    GAIN = 0.25f;
    K_DETAIL = 0.4445f;
    CLOUD_COVERAGE = 0.9714f;
    FALL_OFF_BOTTOM = 0.6066f;
    FALL_OFF_TOP = 0.6f;
    FREQ_WORLEY = 0.0f;
    K_WORLEY = 0.0f;
    WIND_SPEED = 60.0f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = false;
    W1 = false;
    WORLEY_MIX = false;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}

static void apply_cirrus_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.018f;
    FOG_STEPS = 32;
    SUN_STEPS = 4;
    INITIAL_AMPLITUDE = 0.22f;
    INITIAL_FREQUENCY = 0.014f;
    FREQ_SHAPE = 1.25f;
    FREQ_DETAIL = 6.5f;
    OCTAVE = 5;
    LACUNARITY = 2.15f;
    GAIN = 0.42f;
    K_DETAIL = 0.2f;
    CLOUD_COVERAGE = 0.9f;
    FALL_OFF_BOTTOM = 0.45f;
    FALL_OFF_TOP = 0.95f;
    FREQ_WORLEY = 0.018f;
    K_WORLEY = 0.08f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = false;
    W1 = false;
    WORLEY_MIX = true;
    COVERAGE = true;
    COVERAGE_OCT = 3;
    COVERAGE_FREQ = 0.0025f;
    K_COVERAGE = 1.2f;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}

static void apply_cirrocumulus_preset()
{
    fog_color = Color(220, 220, 220);
    DENSITY = 0.098f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_FREQUENCY = 0.0117f;
    INITIAL_AMPLITUDE = 1.226f;
    FREQ_SHAPE = 3.37f;
    FREQ_DETAIL = 4.0f;
    OCTAVE = 9;
    LACUNARITY = 0.6766f;
    GAIN = 0.0f;
    K_DETAIL = 0.2419f;
    CLOUD_COVERAGE = 0.9714f;
    FALL_OFF_BOTTOM = 1.0f;
    FALL_OFF_TOP = 0.0f;
    FREQ_WORLEY = 0.0f;
    K_WORLEY = 0.2f;
    WIND_SPEED = 160.0f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = false;
    W1 = false;
    WORLEY_MIX = false;
    COVERAGE = false;
    COVERAGE_OCT = 2;
    COVERAGE_FREQ = 0.0015f;
    K_COVERAGE = 1.0f;
    WIND_SPEED = 60.0f;
    BOX_MAX_Y = 238.0f;
    BOX_MIN_Y = 144.0f;
}

static void apply_nimbus_preset()
{
    fog_color = Color(121, 122, 124);
    DENSITY = 0.2541f;
    FOG_STEPS = 25;
    SUN_STEPS = 5;
    INITIAL_AMPLITUDE = 0.4f;
    INITIAL_FREQUENCY = 0.0096f;
    FREQ_SHAPE = 0.7577f;
    FREQ_DETAIL = 4.0f;
    OCTAVE = 4;
    LACUNARITY = 1.8f;
    GAIN = 0.3f;
    K_DETAIL = 0.7f;
    CLOUD_COVERAGE = 1.5f;
    FALL_OFF_BOTTOM = 0.1f;
    FALL_OFF_TOP = 0.6f;
    FREQ_WORLEY = 0.008f;
    K_WORLEY = 0.2f;
    VERTICAL_FALL_OFF = true;
    WORLEY_ = true;
    W1 = false;
    WORLEY_MIX = false;
    COVERAGE = false;
    COVERAGE_OCT = 2;
    COVERAGE_FREQ = 0.0015f;
    K_COVERAGE = 1.0f;
    WIND_SPEED = 200.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MAX_Y = 230.0f;
}

void apply_cloud_preset(CloudPreset preset)
{
    select_cloud_type(preset);

    if (preset == CloudPreset::Cumulus)
        apply_cumulus_preset();
    else if (preset == CloudPreset::Cloud2)
        apply_cloud2_preset();
    else if (preset == CloudPreset::ToweringCumulus)
        apply_towering_cumulus_preset();
    else if (preset == CloudPreset::Stratus)
        apply_stratus_preset();
    else if (preset == CloudPreset::Cirrus)
        apply_cirrus_preset();
    else if (preset == CloudPreset::Cirrocumulus)
        apply_cirrocumulus_preset();
    else if (preset == CloudPreset::Nimbus)
        apply_nimbus_preset();
}

void apply_low_res_mode()
{
    FOG_STEPS = 15;
}

void apply_high_res_mode()
{
    FOG_STEPS = 50;
}

void apply_big_mode()
{
    BOX_MIN_X = -500.0f;
    BOX_MIN_Y = 0.0f;
    BOX_MIN_Z = -500.0f;
    BOX_MAX_X = 500.0f;
    BOX_MAX_Y = 200.0f;
    BOX_MAX_Z = 500.0f;
}

void apply_pov_mode(Scene& scene)
{
    float width = scene.camera.plan_image.p_image_width;
    float height = scene.camera.plan_image.p_image_height;

    scene.camera.origin = Point3(1650.0f, -1050.0f, -1650.0f);
    scene.camera.p = Point3(1215.0f, -770.0f, -1227.0f);
    scene.camera.plan_image = Plan_image(scene.camera.p, scene.camera.p - scene.camera.origin, scene.camera.direction_up, width, height);
}

void update_render_modes(Scene& scene)
{
    bool res_mode_enabled = low_res || high_res;
    if (res_mode_enabled && !res_mode_was_enabled)
        saved_fog_steps = FOG_STEPS;

    if (!res_mode_enabled && res_mode_was_enabled)
        FOG_STEPS = saved_fog_steps;

    if (low_res)
        apply_low_res_mode();
    else if (high_res)
        apply_high_res_mode();

    res_mode_was_enabled = res_mode_enabled;

    if (big && !big_was_enabled) {
        saved_box_min_x = BOX_MIN_X;
        saved_box_min_y = BOX_MIN_Y;
        saved_box_min_z = BOX_MIN_Z;
        saved_box_max_x = BOX_MAX_X;
        saved_box_max_y = BOX_MAX_Y;
        saved_box_max_z = BOX_MAX_Z;
    }

    if (!big && big_was_enabled) {
        BOX_MIN_X = saved_box_min_x;
        BOX_MIN_Y = saved_box_min_y;
        BOX_MIN_Z = saved_box_min_z;
        BOX_MAX_X = saved_box_max_x;
        BOX_MAX_Y = saved_box_max_y;
        BOX_MAX_Z = saved_box_max_z;
    }

    if (big)
        apply_big_mode();
    big_was_enabled = big;

    if (pov && !pov_was_enabled) {
        saved_camera_origin = scene.camera.origin;
        saved_camera_target = scene.camera.p;
        saved_camera_up = scene.camera.direction_up;
        apply_pov_mode(scene);
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

    pov_was_enabled = pov;
}
