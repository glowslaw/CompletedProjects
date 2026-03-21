#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "lodepng.h"
#include <fstream>


#include "textRenderer.h"
#include "shaderprogram.h"
#include "GameObject.h"

GLuint readTexture(const char* filename);

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	GLuint id;
	std::string type;
};


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	unsigned int VAO, VBO, EBO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void setupMesh();
};

class Model
{
public:
	std::vector<Mesh> meshes;
	std::string directory = "";

	void draw(ShaderProgram *shader);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
};

struct TextureExternal
{
public:
	GLuint texture;
	GLuint specular;
	int textureUnit;
};

struct Text
{
	std::string value;
	float x, y;
	float scale;
};

class ResourceManager
{
public:
	ShaderProgram* complexShader;
	ShaderProgram* textShader;
	ShaderProgram* simpleShader;
	GameObject root;
	TextRenderer ui;

	std::vector<GameObject*> renderQueue;
	
	std::vector<Model> modelPalette;
	std::vector<TextureExternal> texturePalette;
	std::vector<Text> textQueue;

	ResourceManager();
	void initShaders(const char* vertexSource, const char* fragmentSource,
					const char* textVertexSource, const char* textFragmentSource,
					const char* simpleVertexSource, const char* simpleFragmentSource);
	void drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, glm::mat4 uiProjection, glm::vec3 lighPos);
	int loadModel(const char* fileName); //loading model from .obj file
	int addTexture(const char* textureSource, const char* specSource ="");
	int addText(std::string value, float x, float y, float scale);
	
};

