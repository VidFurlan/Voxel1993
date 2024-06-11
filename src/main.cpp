#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

const unsigned int res = 1;
const unsigned int pixelScale = 8 / res;
const unsigned int SCR_WIDTH = 200 * res;
const unsigned int SCR_HEIGHT = 150 * res;
const unsigned int SCR_WIDTH_HALF = SCR_WIDTH / 2;
const unsigned int SCR_HEIGHT_HALF = SCR_HEIGHT / 2;
const unsigned int GLSCR_WIDTH = SCR_WIDTH * pixelScale;
const unsigned int GLSCR_HEIGHT = SCR_HEIGHT * pixelScale;

typedef struct {
    int fr1, fr2; // frame 1 frame 2, to create constant frame rate
} frametime;
frametime T;

typedef struct {
    int w, s, a, d; // move up, down, left, right
    int sl, sr;     // strafe left, right
    int m;          // move up, down, look up, down
} keys;
keys K;

typedef struct {
    short r, g, b;
} color;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void get_resolution(int *window_width, int *window_height);
void initGlfwSettings();
GLFWwindow *initGlfwWindow();

void pixel(int x, int y, int c) {
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

void pixel(int x, int y, color c) {
    glColor3ub(c.r, c.g, c.b); 
    glBegin(GL_POINTS);
    glVertex2i(x*pixelScale+2,y*pixelScale+2);
    glEnd();
}

void movePlayer() {
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

void clearBackground() {
    for (int y = 0; y < SCR_HEIGHT; y++)
        for (int x = 0; x < SCR_WIDTH; x++)
            pixel(x, y, 8);
}

int tick;
void draw3D() {
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

void display() {
    clearBackground();
    movePlayer();
    draw3D();
    glFlush();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_W)
            K.w = 1;
        if (key == GLFW_KEY_S)
            K.s = 1;
        if (key == GLFW_KEY_A)
            K.a = 1;
        if (key == GLFW_KEY_D)
            K.d = 1;
        if (key == GLFW_KEY_M)
            K.m = 1;
        if (key == GLFW_KEY_COMMA)
            K.sr = 1;
        if (key == GLFW_KEY_PERIOD)
            K.sl = 1;

    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W)
            K.w = 0;
        if (key == GLFW_KEY_S)
            K.s = 0;
        if (key == GLFW_KEY_A)
            K.a = 0;
        if (key == GLFW_KEY_D)
            K.d = 0;
        if (key == GLFW_KEY_M)
            K.m = 0;
        if (key == GLFW_KEY_COMMA)
            K.sr = 0;
        if (key == GLFW_KEY_PERIOD)
            K.sl = 0;
    }
}

int main() {
    initGlfwSettings();

    GLFWwindow *window = initGlfwWindow();

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glPointSize(pixelScale);                         // pixel size
    glOrtho(0, GLSCR_WIDTH, 0, GLSCR_HEIGHT, -1, 1); // origin bottom left

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void get_resolution(int *window_width, int *window_height) {
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    *window_width = mode->width;
    *window_height = mode->height;
}

void initGlfwSettings() {
    glfwInit();

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow *initGlfwWindow() {
    GLFWwindow *window = glfwCreateWindow(GLSCR_WIDTH, GLSCR_HEIGHT, "3D Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    return window;
}
