#include "stdio.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    // helper handling input from keyboard and mouse
    bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

    // handle input from keyboard and mouse
    bool keys[1024];

    GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};