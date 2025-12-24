#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int main()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    std::cout << "GLFW OK\n";
    glfwTerminate();
    return 0;
    return 0;
}
