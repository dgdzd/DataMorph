#ifndef DM_FLUIDWINDOW_H
#define DM_FLUIDWINDOW_H

#include <GLFW/glfw3.h>

class FluidWindow {
public:
    GLFWwindow* window;

    FluidWindow();
    bool innit();
    void renderLoop();
};

#endif
