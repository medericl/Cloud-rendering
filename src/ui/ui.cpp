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
    if (ImGui::IsKeyDown(ImGuiKey_W))
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

    update_camera_keyboard(scene);

    if (ImGui::CollapsingHeader("Modes", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Debug", &debug);
        ImGui::Checkbox("Big cloud zone", &big);
        ImGui::Checkbox("Cloud POV", &pov);
        ImGui::Checkbox("Jitter", &JITTER);
        ImGui::Checkbox("Vertical falloff", &VERTICAL_FALL_OFF);
        ImGui::Checkbox("Hg phase", &HG);
        ImGui::Checkbox("Ambiant Cloud", &AMBIENT_CLOUD);
    }

    if (ImGui::CollapsingHeader("Coveragee", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Checkbox("Coverage", &COVERAGE);
        ImGui::SliderFloat("Coverage frequency", &COVERAGE_FREQ, 0.0f, 1.0f, "%.4f");
        ImGui::SliderInt("Coverage octave", &COVERAGE_OCT, 0, 15);
    }



    update_render_modes(scene);

    if (ImGui::CollapsingHeader("Cloud", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Density",    &DENSITY,   0.0f,  0.1f,  "%.4f");
        ImGui::SliderInt("Steps",        &FOG_STEPS, 1,     128);
        color_edit("Cloud color",          fog_color);
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

    if (ImGui::CollapsingHeader("Bounding box")) {
        ImGui::SliderFloat("Box min X",  &BOX_MIN_X, -500.0f,   0.0f);
        ImGui::SliderFloat("Box min Y",  &BOX_MIN_Y, -100.0f, 200.0f);
        ImGui::SliderFloat("Box min Z",  &BOX_MIN_Z, -500.0f,   0.0f);
        ImGui::SliderFloat("Box max X",  &BOX_MAX_X,    0.0f, 500.0f);
        ImGui::SliderFloat("Box max Y",  &BOX_MAX_Y,    0.0f, 500.0f);
        ImGui::SliderFloat("Box max Z",  &BOX_MAX_Z,    0.0f, 500.0f);
    }

    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::SliderFloat("Sun X", &SUN_POS.x, -1000.0f, 1000.0f);
        ImGui::SliderFloat("Sun Y", &SUN_POS.y, -100.0f, 1000.0f);
        ImGui::SliderFloat("Sun Z", &SUN_POS.z, -1000.0f, 1000.0f);
        ImGui::SliderFloat("G HG", &G_HG, -0.99f, 0.99f, "%.3f");
        ImGui::SliderFloat("Powder strength", &POWDER_STRENGTH, 0.0f, 10.0f, "%.3f");
        ImGui::SliderFloat("Powder amount", &POWDER_AMOUNT, 0.0f, 5.0f, "%.3f");

        for (size_t i = 0; i < scene.list_light.size(); i++) {
            char label[32];
            snprintf(label, sizeof(label), "Light %zu", i + 1);
            color_edit(label, scene.list_light[i].color);
        }
    }

    if (ImGui::CollapsingHeader("Ray")) {
        ImGui::SliderFloat("kd",      &kd,      0.0f,   1.0f);
        ImGui::SliderFloat("ks",      &ks,      0.0f,   1.0f);
        ImGui::SliderFloat("kr",      &kr,      0.0f,   1.0f);
        ImGui::SliderFloat("ns",      &ns,      1.0f, 100.0f);
        ImGui::SliderFloat("k_close", &k_close, 0.0001f, 1.0f, "%.4f");
        ImGui::SliderFloat("k_far",   &k_far,   100.0f, 2000.0f);
    }

    if (ImGui::CollapsingHeader("Scene")) {
        color_edit("Ambient",    ambient_color);
        color_edit("Background", background_color);
        color_edit("Sky horizon", SKY_HORIZON);
        color_edit("Sky zenith", SKY_ZENITH);
        color_edit("Floor",      COLOR_FLOOR);
        color_edit("Sun",        SUN);
    }

    if (!scene.list_light.empty()) {
        scene.list_light[0].origin = SUN_POS;
        scene.list_light[0].color = SUN;
    }

    ImGui::Text("Spheres: %zu", scene.list_sphere.size());

    ImGui::End();
}
