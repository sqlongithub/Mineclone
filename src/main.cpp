#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"

const unsigned int WIDTH = 1280;
const unsigned int HEIGHT = 720;

int main()
{
    Mineclone::Application app("Mineclone");
    app.run();
    return 0;
}