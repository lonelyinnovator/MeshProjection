#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "render.h"

int main(int argc, char *argv[]) {

//    if (std::filesystem::exists(image_dir)) {
//        std::filesystem::remove_all(image_dir);
//    }

    Render render;

    render.OneCameraRender();

    // render shaded
//    render.MeshProjectionRender(true);
    // render depth map
//    render.mesh_projection_render(false);

//    shaded_seg_render(window);

    glfwTerminate();
    return 0;
}

