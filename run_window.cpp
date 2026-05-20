#include "run_window.hh"
#include "src/scene/scene_init.hh"
#include "src/image/image.hh"

#ifndef NO_UI
#include "src/ui/ui.hh"
#include "src/ui/imgui.h"
#include "src/ui/imgui_impl_glfw.h"
#include "src/ui/imgui_impl_opengl2.h"
#endif

#include <GLFW/glfw3.h>
#include <cmath>


void render_to_pixels(int width, int height, float x_offset, std::vector<unsigned char>& pixels, Scene& scene)
{
    Image image(width, height);
    scene.ray_tracing(image);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const Color& c = image.getPixel(x, y);
            int idx = ((height - 1 - y) * width + x) * 3; // flip Y for OpenGL
            pixels[idx + 0] = c.r;
            pixels[idx + 1] = c.g;
            pixels[idx + 2] = c.b;
        }
    }
}

void run_window(int width, int height, Camera cam)
{
    if (!glfwInit())
        return;

    GLFWwindow* window = glfwCreateWindow(width * 3, height * 3, "Raymarching", nullptr, nullptr);
    if (!window) { glfwTerminate(); return; }
    glfwMakeContextCurrent(window);

#ifndef NO_UI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
#endif

    std::vector<unsigned char> pixels(width * height * 3);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    Scene scene(cam, vector_light(), {}, vector_cube());

    Point3 origin = cam.origin;
    float yaw   = 135.0f;
    float pitch = -20.6f;
    float speed = 5.0f;
    double last_mx = -1, last_my = -1;

    while (!glfwWindowShouldClose(window)) {
        float x_offset = std::sin(glfwGetTime()) * 300.0f;

        // mouse look (only while left button held)
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);
        bool clicking = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
#ifndef NO_UI
        clicking = clicking && !ImGui::GetIO().WantCaptureMouse;
#endif
        if (clicking && last_mx >= 0) {
            yaw   -= (float)(mx - last_mx) * 0.2f;
            pitch -= (float)(my - last_my) * 0.2f;
            if (pitch >  89.0f) pitch =  89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
        last_mx = clicking ? mx : -1;
        last_my = clicking ? my : -1;

        // direction vectors from yaw/pitch
        float ry = yaw   * (float)M_PI / 180.0f;
        float rp = pitch * (float)M_PI / 180.0f;
        Vector3 forward(std::cos(rp) * std::cos(ry), std::sin(rp), std::cos(rp) * std::sin(ry));
        Vector3 world_up(0, 1, 0);
        Vector3 right = world_up.p_v(forward);
        right = right / right.norm();

        // WASD movement
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) origin = origin + forward * speed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) origin = origin - forward * speed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) origin = origin - right * speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) origin = origin + right * speed;

        scene.camera = Camera(origin, origin + forward * 100.0f, world_up, (float)width, (float)height);

        render_to_pixels(width, height, x_offset, pixels, scene);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1);
        glTexCoord2f(1, 0); glVertex2f( 1, -1);
        glTexCoord2f(1, 1); glVertex2f( 1,  1);
        glTexCoord2f(0, 1); glVertex2f(-1,  1);
        glEnd();
        glDisable(GL_TEXTURE_2D);

#ifndef NO_UI
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render_ui(scene);

        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

#ifndef NO_UI
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif

    glDeleteTextures(1, &tex);
    glfwDestroyWindow(window);
    glfwTerminate();
}
