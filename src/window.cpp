#include "3D_ENGINE/window.hpp"
#include <GLFW/glfw3.h>

Window::Window() {
    initGlfwSettings();

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(GLSCR_WIDTH, GLSCR_HEIGHT, "3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    // Set the user pointer to this instance
    glfwSetWindowUserPointer(window, this);

    glPointSize(pixelScale);                         // pixel size
    glOrtho(0, GLSCR_WIDTH, 0, GLSCR_HEIGHT, -1, 1); // origin bottom left
}

void Window::initGlfwSettings() {
    glfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

void Window::pixel(int x, int y, int c) {
    int rgb[3];
    if(c==0){ rgb[0]=255; rgb[1]=255; rgb[2]=  0;} //Yellow	
    if(c==1){ rgb[0]=160; rgb[1]=160; rgb[2]=  0;} //Yellow darker	
    if(c==2){ rgb[0]=  0; rgb[1]=255; rgb[2]=  0;} //Green	
    if(c==3){ rgb[0]=  0; rgb[1]=160; rgb[2]=  0;} //Green darker	
    if(c==4){ rgb[0]=  0; rgb[1]=255; rgb[2]=255;} //Cyan	
    if(c==5){ rgb[0]=  0; rgb[1]=160; rgb[2]=160;} //Cyan darker
    if(c==6){ rgb[0]=160; rgb[1]=100; rgb[2]=  0;} //brown	
    if(c==7){ rgb[0]=110; rgb[1]= 50; rgb[2]=  0;} //brown darker
    if(c==8){ rgb[0]=  0; rgb[1]= 60; rgb[2]=130;} //background 
    glColor3ub(rgb[0],rgb[1],rgb[2]); 
    glBegin(GL_POINTS);
    glVertex2i(x*pixelScale+2,y*pixelScale+2);
    glEnd();
}

//void pixel(int x, int y, color c) {
//    glColor3ub(c.r, c.g, c.b); 
//    glBegin(GL_POINTS);
//    glVertex2i(x*pixelScale+2,y*pixelScale+2);
//    glEnd();
//}

void Window::movePlayer() {
    // move up, down, left, right
    if (K.a == 1 && K.m == 0)
        std::cout << "left" << std::endl;
    if (K.d == 1 && K.m == 0)
        std::cout << "right" << std::endl;
    if (K.w == 1 && K.m == 0)
        std::cout << "up" << std::endl;
    if (K.s == 1 && K.m == 0)
        std::cout << "down" << std::endl;

    // strafe left, right
    if (K.sr == 1)
        printf("strafe left\n");
    if (K.sl == 1)
        printf("strafe right\n");

    // move up, down, look up, look down
    if (K.a == 1 && K.m == 1)
        printf("look up\n");
    if (K.d == 1 && K.m == 1)
        printf("look down\n");
    if (K.w == 1 && K.m == 1)
        printf("move up\n");
    if (K.s == 1 && K.m == 1)
        printf("move down\n");
}

void Window::clearBackground() {
    for (int y = 0; y < SCR_HEIGHT; y++)
        for (int x = 0; x < SCR_WIDTH; x++)
            pixel(x, y, 8);
}

void Window::draw3D() {
    int x, y, c = 0;
    for (y = 0; y < SCR_HEIGHT_HALF; y++) {
        for (x = 0; x < SCR_WIDTH_HALF; x++) {
            pixel(x, y, c);
            c += 1;
            if (c > 8)
                c = 0;
        }
    }

    // frame rate
    tick += 1;
    if (tick > 20)
        tick = 0;

    pixel(SCR_WIDTH_HALF, SCR_HEIGHT_HALF + tick, 0);
}

void Window::display() {
    clearBackground();
    movePlayer();
    draw3D();
    glFlush();
}

void Window::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Get the user pointer and cast it to Window
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W)
            win->K.w = 1;
        if (key == GLFW_KEY_S)
            win->K.s = 1;
        if (key == GLFW_KEY_A)
            win->K.a = 1;
        if (key == GLFW_KEY_D)
            win->K.d = 1;
        if (key == GLFW_KEY_M)
            win->K.m = 1;
        if (key == GLFW_KEY_COMMA)
            win->K.sr = 1;
        if (key == GLFW_KEY_PERIOD)
            win->K.sl = 1;
    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W)
            win->K.w = 0;
        if (key == GLFW_KEY_S)
            win->K.s = 0;
        if (key == GLFW_KEY_A)
            win->K.a = 0;
        if (key == GLFW_KEY_D)
            win->K.d = 0;
        if (key == GLFW_KEY_M)
            win->K.m = 0;
        if (key == GLFW_KEY_COMMA)
            win->K.sr = 0;
        if (key == GLFW_KEY_PERIOD)
            win->K.sl = 0;
    }
}
