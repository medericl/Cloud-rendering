#include "ui.hh"
#include "../scene/config.hh"
#include "imgui.h"

static void color_edit(const char* label, Color& c)
{
    float col[3] = { c.r / 255.f, c.g / 255.f, c.b / 255.f };
    if (ImGui::ColorEdit3(label, col)) {
        c.r = col[0] * 255.f;
        c.g = col[1] * 255.f;
        c.b = col[2] * 255.f;
    }
}

static void translate_camera(Scene& scene, const Vector3& delta)
{
    scene.camera.origin = scene.camera.origin + delta;
    scene.camera.p = scene.camera.p + delta;
}

static void rebuild_camera_plan(Scene& scene)
{
    scene.camera.plan_image = Plan_image(
        scene.camera.p,
        scene.camera.p - scene.camera.origin,
        scene.camera.direction_up,
        scene.camera.plan_image.p_image_width,
        scene.camera.plan_image.p_image_height);
}

static void update_camera_keyboard(Scene& scene)
{
    static float camera_speed = 300.0f;
    static float target_speed = 80.0f;

    Vector3 forward = scene.camera.p - scene.camera.origin;
    forward = forward / forward.norm();

    Vector3 right = scene.camera.direction_up.p_v(forward);
    right = right / right.norm();
    Vector3 up = forward.p_v(right);
    up = up / up.norm();

    Vector3 movement(0, 0, 0);
    if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_Z))
        movement = movement + forward;
    if (ImGui::IsKeyDown(ImGuiKey_S))
        movement = movement - forward;
    if (ImGui::IsKeyDown(ImGuiKey_A))
        movement = movement - right;
    if (ImGui::IsKeyDown(ImGuiKey_D))
        movement = movement + right;

    if (movement.norm() > 0.0f) {
        movement = movement / movement.norm();
        translate_camera(scene, movement * camera_speed * ImGui::GetIO().DeltaTime);
        rebuild_camera_plan(scene);
    }

    Vector3 target_movement(0, 0, 0);
    if (ImGui::IsKeyDown(ImGuiKey_UpArrow))
        target_movement = target_movement + up;
    if (ImGui::IsKeyDown(ImGuiKey_DownArrow))
        target_movement = target_movement - up;
    if (ImGui::IsKeyDown(ImGuiKey_LeftArrow))
        target_movement = target_movement - right;
    if (ImGui::IsKeyDown(ImGuiKey_RightArrow))
        target_movement = target_movement + right;

    if (target_movement.norm() > 0.0f) {
        target_movement = target_movement / target_movement.norm();
        scene.camera.p = scene.camera.p + target_movement * target_speed * ImGui::GetIO().DeltaTime;
        rebuild_camera_plan(scene);
    }

    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::SliderFloat("Camera speed", &camera_speed, 10.0f, 2000.0f);
        ImGui::SliderFloat("Target speed", &target_speed, 1.0f, 500.0f);
        ImGui::Text("Position: %.1f %.1f %.1f", scene.camera.origin.x, scene.camera.origin.y, scene.camera.origin.z);
        ImGui::Text("Target: %.1f %.1f %.1f", scene.camera.p.x, scene.camera.p.y, scene.camera.p.z);
    }
}

void render_ui(Scene& scene)
{
    ImGui::Begin("Cloud");

    if (ImGui::CollapsingHeader("Types of Clouds", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::RadioButton("Cumulus", CUMULUS))
            apply_cloud_preset(CloudPreset::Cumulus);
        if (ImGui::RadioButton("Cloud2", CLOUD2))
            apply_cloud_preset(CloudPreset::Cloud2);
        if (ImGui::RadioButton("Towering Cumulus", TOWERING_CUMULUS))
            apply_cloud_preset(CloudPreset::ToweringCumulus);
        if (ImGui::RadioButton("Stratus", STRATUS))
            apply_cloud_preset(CloudPreset::Stratus);
        if (ImGui::RadioButton("Cirrus", CIRRUS))
            apply_cloud_preset(CloudPreset::Cirrus);
        if (ImGui::RadioButton("Cirrocumulus", CIRROCUMULUS))
            apply_cloud_preset(CloudPreset::Cirrocumulus);
        if (ImGui::RadioButton("Nimbus", NIMBUS))
            apply_cloud_preset(CloudPreset::Nimbus);
        ImGui::Checkbox("Storm", &STORM);
        ImGui::Checkbox("Fog", &FOG);
    }

    if (ImGui::CollapsingHeader("Time", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Day", &DAY); // white clouds - less than 2000 meters
        ImGui::Checkbox("Sunset", &SUNSET); // flat clouds - have a lot of impact in the temperature and lights
        ImGui::Checkbox("Sunrise", &SUNRISE); // 7000 - 14000 meters ice crestals
    }

    if (ImGui::CollapsingHeader("Modes", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Debug", &debug);
        ImGui::Checkbox("Wind", &WIND);
        ImGui::Checkbox("Light", &LIGHT);
        ImGui::Checkbox("Low resolution", &low_res);
        ImGui::Checkbox("High resolution", &high_res);
        ImGui::Checkbox("Big cloud zone", &big);
        ImGui::Checkbox("Cloud POV", &pov);
        ImGui::Checkbox("Hg phase", &HG);
        ImGui::Checkbox("Ambiant Cloud", &AMBIENT_CLOUD);
        ImGui::Checkbox("Vertical falloff", &VERTICAL_FALL_OFF);
        ImGui::Checkbox("Jitter", &JITTER);
        ImGui::Checkbox("Worley ", &WORLEY_);
        ImGui::Checkbox("W1", &W1);
    }

    if (ImGui::CollapsingHeader("Coveragee", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Coverage", &COVERAGE);
        ImGui::SliderFloat("K Coverage", &K_COVERAGE, 0.5f, 1.5f, "%.4f");
        ImGui::SliderFloat("Coverage frequency", &COVERAGE_FREQ, 0.0f, 1.0f, "%.4f");
        ImGui::SliderInt("Coverage octave", &COVERAGE_OCT, 0, 15);
    }

    update_render_modes(scene);
    update_camera_keyboard(scene);

    if (ImGui::CollapsingHeader("Cloud", ImGuiTreeNodeFlags_DefaultOpen)) {
        color_edit("Cloud color",          fog_color);
        ImGui::SliderFloat("Density",    &DENSITY,   0.0f,  0.3f,  "%.4f");
        ImGui::SliderInt("Cloud Steps",        &FOG_STEPS, 1,     128);
        ImGui::SliderFloat("Initial frequenciy",    &INITIAL_FREQUENCY,   0.00f,  0.02f,  "%.4f");
        ImGui::SliderFloat("Initial amplitude",    &INITIAL_AMPLITUDE,   0.00f,  2.0f,  "%.4f");
        ImGui::SliderFloat("Shape frequenciy",    &FREQ_SHAPE,   0.00f,  5.0f,  "%.4f");
        ImGui::SliderFloat("Detail frequenciy",    &FREQ_DETAIL,   0.00f,  10.00f,  "%.4f");
        ImGui::SliderFloat("Lacunarity",    &LACUNARITY,   0.00f,  5.00f,  "%.4f");
        ImGui::SliderFloat("Gain",    &GAIN,   0.00f,  1.00f,  "%.4f");
        ImGui::SliderInt("Octave",    &OCTAVE,   0,  15);
        ImGui::SliderFloat("K Detail",    &K_DETAIL,   0.00f,  2.00f,  "%.4f");
        ImGui::SliderFloat("Cloud coverage",    &CLOUD_COVERAGE,   0.00f,  5.00f,  "%.4f");
        ImGui::SliderFloat("fall off bottom",    &FALL_OFF_BOTTOM,   0.00f,  1.00f,  "%.4f");
        ImGui::SliderFloat("fall off top",    &FALL_OFF_TOP,   0.00f,  1.00f,  "%.4f");
        ImGui::SliderFloat("Worley",    &K_WORLEY,   0.0f,  0.9f,  "%.4f");
        ImGui::SliderFloat("Worley frequency",    &FREQ_WORLEY,   0.0f,  0.04f,  "%.4f");
        ImGui::SliderFloat("Wind Speed",    &WIND_SPEED,   0.0f,  1000.0f,  "%.4f");
    }

    if (ImGui::CollapsingHeader("Terrain", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Show terrain", &TERRAIN);
        ImGui::SliderFloat("Texture (earth/green/rock)", &TERRAIN_TEXTURE, 0.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Height scale",               &TERRAIN_SCALE,   0.0f, 3.0f, "%.2f");
    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::SliderFloat("K HG", &K_HG, 0.1f, 0.5f, "%.3f");
        ImGui::SliderFloat("G HG", &G_HG, 0.3f, 0.7f, "%.3f");
        ImGui::SliderFloat("Silver HG", &G_HG_SILVER, 0.6f, 0.9f, "%.3f");

        ImGui::SliderFloat("Powder strength", &POWDER_STRENGTH, 0.0f, 10.0f, "%.3f");
        ImGui::SliderFloat("Powder amount", &POWDER_AMOUNT, 0.0f, 5.0f, "%.3f");
    }

    if (ImGui::CollapsingHeader("Scene")) {
        color_edit("Ambient",    ambient_color);
        color_edit("Background", background_color);
        color_edit("Sky horizon", SKY_HORIZON);
        color_edit("Sky zenith", SKY_ZENITH);
        color_edit("Floor",      COLOR_FLOOR);
        color_edit("Sun",        COLOR_SUN);
        ImGui::SliderFloat("Sun X", &SUN_POS.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Sun Y", &SUN_POS.y, -100.0f, 4000.0f);
        ImGui::SliderFloat("Sun Z", &SUN_POS.z, -1000.0f, 1000.0f);
    }

    if (ImGui::CollapsingHeader("Bounding box")) {
        ImGui::SliderFloat("Box min X",  &BOX_MIN_X, -500.0f,   0.0f);
        ImGui::SliderFloat("Box min Y",  &BOX_MIN_Y, -100.0f, 200.0f);
        ImGui::SliderFloat("Box min Z",  &BOX_MIN_Z, -500.0f,   0.0f);
        ImGui::SliderFloat("Box max X",  &BOX_MAX_X,    0.0f, 500.0f);
        ImGui::SliderFloat("Box max Y",  &BOX_MAX_Y,    0.0f, 500.0f);
        ImGui::SliderFloat("Box max Z",  &BOX_MAX_Z,    0.0f, 500.0f);
    }

    //if (ImGui::CollapsingHeader("Ray")) {
    //    ImGui::SliderFloat("kd",      &kd,      0.0f,   1.0f);
    //    ImGui::SliderFloat("ks",      &ks,      0.0f,   1.0f);
    //    ImGui::SliderFloat("kr",      &kr,      0.0f,   1.0f);
    //    ImGui::SliderFloat("ns",      &ns,      1.0f, 100.0f);
    //    ImGui::SliderFloat("k_close", &k_close, 0.0001f, 1.0f, "%.4f");
    //    ImGui::SliderFloat("k_far",   &k_far,   100.0f, 2000.0f);
    //}

    if (!scene.list_light.empty()) {
        scene.list_light[0].origin = SUN_POS;
        scene.list_light[0].color = COLOR_SUN;
    }

    ImGui::Text("Spheres: %zu", scene.list_sphere.size());

    ImGui::End();
}}
