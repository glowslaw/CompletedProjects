#include "gameManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameManager* myGm;


GameManager::GameManager(GLFWwindow* window)
{
    myGm = this;
    aspectRatio = 1600.f/1200.f;
    this->window = window;
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);

	status = 1;
    timeDelta = 0;
    screenSizeX = 1600;
    screenSizeY = 1200;
    lightPos = glm::vec3(22070, 28294, -39823);
    cameraOffset = glm::vec3(0,2+3,-10-5);
    cameraOrientation = glm::quat(glm::vec3(0, 0, 0));
    updatePerspective();
    uiProjection = glm::ortho(0.0f, 1600.f, 0.0f, 1200.f);
    updateCamera(vec3(0,0,0));
}

void GameManager::updateTimeDelta()
{
    timeDelta = glfwGetTime();
    glfwSetTime(0);
}

void GameManager::updateCamera(glm::vec3 pos)
{
    camera = glm::lookAt(
        pos + cameraOffset,
        pos,
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameManager::cameraFollow(glm::vec3 pos,glm::quat orientation)
{
    cameraOrientation = glm::mix(cameraOrientation, orientation, timeDelta*2.0f);
    camera = glm::lookAt(
        pos + cameraOrientation * cameraOffset,
        pos + orientation * glm::vec3(0.0f, 3.0f, 0.0f),
        orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameManager::updatePerspective()
{
    perspective = glm::perspective(50.0f * 3.14f / 180.0f, aspectRatio, 0.5f, 8000000.0f);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {

    if (height == 0) return;
    myGm->aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    myGm-> updatePerspective();
    myGm->uiProjection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
    
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) myGm->keyStatus[0] = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) myGm->keyStatus[0] = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) myGm->keyStatus[1] = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) myGm->keyStatus[1] = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) myGm->keyStatus[2] = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) myGm->keyStatus[2] = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) myGm->keyStatus[3] = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) myGm->keyStatus[3] = false;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) myGm->keyStatus[4] = true;
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) myGm->keyStatus[4] = false;
    if (key == GLFW_KEY_E && action == GLFW_PRESS) myGm->keyStatus[5] = true;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) myGm->keyStatus[5] = false;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) myGm->keyStatus[6] = true;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) myGm->keyStatus[6] = false;
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) myGm->keyStatus[7] = true;
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) myGm->keyStatus[7] = false;
    if (key == GLFW_KEY_R && action == GLFW_PRESS) myGm->keyStatus[8] = true;
    if (key == GLFW_KEY_R && action == GLFW_RELEASE) myGm->keyStatus[8] = false;

}

float sgn(float val) {
    return (float(0) < val) - (val  <float(0));
}

void GameManager::updateControls(PhysObject* plane)
{
    if (keyStatus[0])
    {
        plane->pitch += timeDelta * 0.5f;
        if (plane->pitch > 1.0f) plane->pitch = 1.0f;
    }
    else if (keyStatus[1])
    {
        plane->pitch -= timeDelta * 0.5f;
        if (plane->pitch < -1.0f) plane->pitch = -1.0f;
    }
    else if(plane->pitch != 0)
    {
        float adj = sgn(plane->pitch) * timeDelta * 0.5f;
        plane->pitch -= adj;
        if (sgn(plane->pitch) != sgn(adj)) plane->pitch = 0;
    }

    if (keyStatus[2])
    {
        plane->roll += timeDelta * 0.5f;
        if (plane->roll > 1.0f) plane->roll = 1.0f;
    }
    else if (keyStatus[3])
    {
        plane->roll -= timeDelta * 0.5f;
        if (plane->roll < -1.0f) plane->roll = -1.0f;
    }
    else if(plane->roll != 0)
    {
        float adj = sgn(plane->roll) * timeDelta * 0.5f;
        plane->roll -= adj;
        if (sgn(plane->roll) != sgn(adj)) plane->roll = 0;
    }

    if (keyStatus[4])
    {
        plane->yaw += timeDelta * 0.5f;
        if (plane->yaw > 1.0f) plane->yaw = 1.0f;
    }
    else if (keyStatus[5])
    {
        plane->yaw -= timeDelta * 0.5f;
        if (plane->yaw < -1.0f) plane->yaw = -1.0f;
    }
    else if (plane->yaw != 0)
    {
        float adj = sgn(plane->yaw) * timeDelta * 0.5f;
        plane->yaw -= adj;
        if (sgn(plane->yaw) != sgn(adj)) plane->yaw = 0;
    }

    if (keyStatus[6])
    {
        plane->throttle += timeDelta*0.8f;
        if (plane->throttle > 1.0f) plane->throttle = 1.0f;
    }
    if (keyStatus[7])
    {
        plane->throttle -= timeDelta * 0.8f;
        if (plane->throttle < 0) plane->throttle = 0;
    }

}
