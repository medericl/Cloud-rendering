#include "ui.hh"
#include "../config.hh"
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
    ImGui::Begin("Brouillard");

    if (ImGui::CollapsingHeader("Brouillard", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderFloat("Density",    &DENSITY,   0.0f,  1.0f,  "%.4f");
        ImGui::SliderInt("Steps",        &FOG_STEPS, 1,     128);
        color_edit("Fog color",          fog_color);
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
        color_edit("Ambient",    ambient_color);
        color_edit("Background", background_color);
        color_edit("Floor",      COLOR_FLOOR);
    }

    ImGui::Text("Spheres: %zu", scene.list_sphere.size());

    ImGui::End();
}
