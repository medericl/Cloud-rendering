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

void render_ui(Scene& scene)
{
    ImGui::Begin("Cloud");

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
        color_edit("Floor",      COLOR_FLOOR);
    }

    ImGui::Text("Spheres: %zu", scene.list_sphere.size());

    ImGui::End();
}
