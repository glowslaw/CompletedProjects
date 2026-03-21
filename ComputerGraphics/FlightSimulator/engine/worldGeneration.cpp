#include "worldGeneration.h"



void Generate_Airport(ResourceManager* res, glm::vec3 position, float size, glm::vec3 rotation)
{
    int hangarModel = res->loadModel("Hangar_OBJ.obj");
    int hangarTex = res->addTexture("Hangar_BaseColor.png");///hangar
    int streetModel = res->loadModel("street.obj");
    int streetTex = res->addTexture("street.png");///street
    int city_baseModel = res->loadModel("city_base.obj");
    int city_baseTex = res->addTexture("sand.png");///city_base
    int towerModel = res->loadModel("tower2.obj");
    int towerTex = res->addTexture("bricks.png");///city_base

    GameObject* tower = new GameObject(&res->root, towerModel, -1, glm::vec3(-150, 30, 200) * size + position, glm::vec3(3000) * size, glm::vec3(0, 0, 0) + rotation);
    res->renderQueue.push_back(tower);

    GameObject* hangar = new GameObject(&res->root, hangarModel, hangarTex, glm::vec3(0, 0, 50) * size + position, glm::vec3(0.025) * size, glm::vec3(0, 0, 0) + rotation);
    res->renderQueue.push_back(hangar);
    GameObject* hangar2 = new GameObject(&res->root, hangarModel, hangarTex, glm::vec3(-100, 0, 50) * size + position, glm::vec3(0.025) * size, glm::vec3(0, 0, 0) + rotation);
    res->renderQueue.push_back(hangar2);
    GameObject* hangar3 = new GameObject(&res->root, hangarModel, hangarTex, glm::vec3(100, 0, 50) * size + position, glm::vec3(0.025) * size, glm::vec3(0, 0, 0) + rotation);
    res->renderQueue.push_back(hangar3);

    for (int i = -1; i < 2; i++)
    {
        GameObject* street = new GameObject(&res->root, streetModel, streetTex, glm::vec3(3.45 + 100 * i, -2, 100) * size + position, glm::vec3(2, 2, 6) * size, glm::vec3(0, PI / 2, 0) + rotation, false);
        res->renderQueue.push_back(street);
        GameObject* street2 = new GameObject(&res->root, streetModel, streetTex, glm::vec3(3.45 + 100 * i, -2, 100 + 63.5) * size + position, glm::vec3(2, 2, 6) * size, glm::vec3(0, PI / 2, 0) + rotation, false);
        res->renderQueue.push_back(street2);
        GameObject* street3 = new GameObject(&res->root, streetModel, streetTex, glm::vec3(3.45 + 100 * i, -2, 100 + 2 * 63.5) * size + position, glm::vec3(2, 2, 6) * size, glm::vec3(0, PI / 2, 0) + rotation, false);
        res->renderQueue.push_back(street3);
        GameObject* street4 = new GameObject(&res->root, streetModel, streetTex, glm::vec3(3.45 + 100 * i, -2, 100 + 3 * 63.5) * size + position, glm::vec3(2, 2, 6) * size, glm::vec3(0, PI / 2, 0) + rotation, false);
        res->renderQueue.push_back(street4);
        GameObject* street5 = new GameObject(&res->root, streetModel, streetTex, glm::vec3(3.45 + 100 * i, -2, 100 + 4 * 63.5) * size + position, glm::vec3(2, 2, 6) * size, glm::vec3(0, PI / 2, 0) + rotation,false);
        res->renderQueue.push_back(street5);
    }

    GameObject* airport_base = new GameObject(&res->root, city_baseModel, city_baseTex, glm::vec3(0, -12, 200) * size + position, glm::vec3(20, 1, 20) * glm::vec3(12) * glm::vec3(size), glm::vec3(0, 0, 0) + rotation, false);
    res->renderQueue.push_back(airport_base);
}

void Generate_Sky_Ground(ResourceManager* res, glm::vec3 position, float size, glm::vec3 rotation, float partition)
{
    int boxModel = res->loadModel("skyBox.obj");
    int skyboxTex = res->addTexture("skybox3.png");
    int grassModel = res->loadModel("grass.obj");
    int grassTex = res->addTexture("grass.png");///grass


    GameObject* skyBox = new GameObject(&res->root, boxModel, skyboxTex, glm::vec3(0, -9, 0) * size + position, glm::vec3(50) * size, glm::vec3(0, 0, 0) + rotation, false);
    res->renderQueue.push_back(skyBox);



    for (int i = 0; i < partition; i++)
    {
        for (int j = 0; j < partition; j++)
        {
            ///+(500-500/partition)
            GameObject* grass = new GameObject(&res->root, grassModel, grassTex, glm::vec3(-350 / partition + (500 - 500 / partition) - (1000 * j) / partition, -20 / partition, -250 / partition + (500 - 500 / partition) - (1000 * i) / partition) * size + position, glm::vec3(2) * size / partition, glm::vec3(0, 0, 0) + rotation, false);
            res->renderQueue.push_back(grass);
        }
    }
}

void Generate_City(ResourceManager* res)
{
    int cityModel = res->loadModel("city.obj");

    int streetTex = res->addTexture("street.png");///street
    GameObject* bricks = new GameObject(&res->root, -1, streetTex);
    bricks->scale *= 0.05;
    bricks->updateModelMatrix();
    res->renderQueue.push_back(bricks);

    GameObject* city = new GameObject(&res->root, cityModel, -1, glm::vec3(0, 5, 0) + glm::vec3(0, 45, 0), glm::vec3(0.125), glm::vec3(0, 0, 0));
    res->renderQueue.push_back(city);

    int city_baseModel = res->loadModel("city_base.obj");
    int city_baseTex = res->addTexture("grass.png");
    GameObject* city_base = new GameObject(&res->root, city_baseModel, city_baseTex, glm::vec3(-50, -200, -75) + glm::vec3(0, 45, 0), glm::vec3(215, 40, 205) * glm::vec3(5), glm::vec3(0, 0, 0));
    res->renderQueue.push_back(city_base);
}

void Generate_Plane_Hitboxes(ResourceManager* res)
{
    int hitboxModel = res->loadModel("skyBox.obj");
    int hitboxTex = res->addTexture("bricks.png");

    GameObject* plane_hitbox = new GameObject(&res->root, hitboxModel, hitboxTex, glm::vec3(-200, 0 + 2, -8200 + 7), glm::vec3(0.05), glm::vec3(0, 0, 0));
    res->renderQueue.push_back(plane_hitbox);
    GameObject* plane_hitbox2 = new GameObject(&res->root, hitboxModel, hitboxTex, glm::vec3(-200 + 8, 0 + 2, -8200 - 2), glm::vec3(0.05), glm::vec3(0, 0, 0));
    res->renderQueue.push_back(plane_hitbox2);
    GameObject* plane_hitbox3 = new GameObject(&res->root, hitboxModel, hitboxTex, glm::vec3(-200 - 8, 0 + 2, -8200 - 2), glm::vec3(0.05), glm::vec3(0, 0, 0));
    res->renderQueue.push_back(plane_hitbox3);


    ///glm::vec3(-200, 10, -8200)
}