#include <gui/FluidWindow.h>
#include <GLFW/glfw3.h>
#include <iostream>

FluidWindow::FluidWindow() { }

bool FluidWindow::innit() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW \n";
        return false;
    }

    window = glfwCreateWindow(900, 900, "Fluid Simulation", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

void FluidWindow::renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        //glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}
