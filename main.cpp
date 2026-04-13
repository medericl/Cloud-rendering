#include "config.hh"
#include "image.hh"
#include "camera.hh"
#include "light.hh"
#include "sphere.hh"
#include "cube.hh"
#include "scene.hh"
#include "vector.hh"

#ifndef NO_UI
#include "ui/ui.hh"
#include "ui/imgui.h"
#include "ui/imgui_impl_glfw.h"
#include "ui/imgui_impl_opengl2.h"
#endif

#include <GLFW/glfw3.h>
#include <cmath>


std::vector<Sphere> vector_sphere()
{
    std::vector<Sphere> spheres = {

        Sphere(Point3(-200, 55, 0), 50, Color(30,30,30)), // red
        Sphere(Point3(0, 55, 0), 51, Color(30,30,30)), // red
        Sphere(Point3(0, 55, 0), 52, Color(30,30,30)), // red
    };
    return spheres;
}

std::vector<Cube> vector_cube()
{
    return {
        Cube(Point3(0, 0, 0), Vector3(400, 12, 400), Color(30, 30, 30)),
    };
}

std::vector<Light> vector_light()
{
    std::vector<Light> lights = {
        Light(Point3(-400,600,400), COLOR_LIGHT1),
        Light(Point3(400,600,-400), COLOR_LIGHT2),

        Light(Point3(400,600,400), COLOR_LIGHT3),
        Light(Point3(-400,600,-400), COLOR_LIGHT4),


        Light(Point3(400,-60,400), Color(20,20,20)),
        Light(Point3(-400,-60,-400), Color(20,20,20)),
    };
    return lights;
}

static void render_to_pixels(int width, int height, float x_offset, std::vector<unsigned char>& pixels, Scene& scene)
{
    for (auto& s : scene.list_sphere)
    {
        if (s.radius == 51)
            s.center.x = x_offset;
        if (s.radius == 52)
            s.center.z = x_offset;
    }

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

static void run_window(int width, int height)
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


    Camera camera(Point3(800,800,-800), Point3(180,180,-180), Vector3(0,1,0), width, height);

    Scene scene(camera, vector_light(), vector_sphere(), vector_cube());

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

int main() {
    int width = 1200;
    int height = 1200;


    run_window(width, height);
    return 0;
}
