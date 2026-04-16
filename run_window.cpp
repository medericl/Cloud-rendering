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

    GLFWwindow* window = glfwCreateWindow(width, height, "Raymarching", nullptr, nullptr);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    Scene scene(cam, vector_light(), {}, vector_cube());

    while (!glfwWindowShouldClose(window)) {
        float x_offset = std::sin(glfwGetTime()) * 300.0f;

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
