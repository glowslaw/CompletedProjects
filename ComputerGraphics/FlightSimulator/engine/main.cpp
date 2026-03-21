#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "openglFunctions.h"
#include "gameManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "PhysObject.h"
#include "worldGeneration.h"
#include "collisionFunctions.h"

#define PI 3.1413

int main()
{
    GameManager gm(initOpenGL(1600, 1200));
    ResourceManager res;

    res.initShaders("shaders/vertex2.glsl", "shaders/fragment.glsl", 
                    "shaders/textVertex.glsl", "shaders/textFragment.glsl",
                    "shaders/simpleVertex.glsl", "shaders/simpleFragment.glsl");

    //Model definitions

    int planeModel = res.loadModel("piper.obj");
    int boxModel = res.loadModel("skyBox.obj");
    int planeModel2 = res.loadModel("plane2.obj");

   
    //Texture definitions
    int piper_diffuse = res.addTexture("piper_diffuse.png");


    PhysObject plane = PhysObject(&res.root, planeModel2, -1, glm::vec3(-200, 0, -8200), glm::vec3(0.01), glm::vec3(0,0,0));
    res.renderQueue.push_back(&plane);


    Generate_Sky_Ground(&res, glm::vec3(0, -10, 0), 80, glm::vec3(0, 0, 0), 8); ///wczesniej bylo 100 ale rogi mapy robily sie czarne
    Generate_Airport(&res, glm::vec3(0, 0, -8450), 2, glm::vec3(0, 0, 0));
    Generate_City(&res);

    GameObject floor = GameObject(&res.root);

    res.addText("Speed: ", 10, 130, 1.f);
    res.addText("Attitude: ", 10, 70, 1.f);
    res.addText("Throttle: ", 10, 10, 1.f);

    glfwSetTime(0);
    while(!glfwWindowShouldClose(gm.window))
    {
        while(!glfwWindowShouldClose(gm.window))
        {
            gm.updateTimeDelta();

                //Game logic 
            if (Collision(&plane))
            {
                res.addText("Crashed!", gm.screenSizeX/2 - 170.f, gm.screenSizeY - 200, 1.6f);
                res.addText("Press R to try again", gm.screenSizeX/2 - 270.f, gm.screenSizeY - 300.f, 1.2f);
                res.drawScene(gm.camera, gm.perspective, gm.uiProjection,gm.lightPos);
                glfwSwapBuffers(gm.window);
                break;
            }
            plane.updatePhysics(gm.timeDelta);
            plane.checkCollision(&floor);
            plane.updateUI(&res);
            plane.updateModelMatrix();

            gm.cameraFollow(plane.pos, plane.rotation);

            res.drawScene(gm.camera, gm.perspective, gm.uiProjection, gm.lightPos);
            glfwSwapBuffers(gm.window);

            glfwPollEvents();

            gm.updateControls(&plane);

        }

        while (!gm.keyStatus[8] && !glfwWindowShouldClose(gm.window)) glfwPollEvents();
        
        res.textQueue.pop_back();
        res.textQueue.pop_back();
        plane.reset(glm::vec3(-200, 0, -8200));
    }
    freeResources(gm.window);
}