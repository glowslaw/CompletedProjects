#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
	root = GameObject();
	root.modelMatrix = glm::mat4(1.0f);
}

void ResourceManager::initShaders(const char* vertexSource, const char* fragmentSource, 
								  const char* textVertexSource, const char* textFragmentSource, 
								  const char* simpleVertexSource, const char* simpleFragmentSource)
{
	
	complexShader = new ShaderProgram(vertexSource, NULL, fragmentSource);
	textShader = new ShaderProgram(textVertexSource, NULL, textFragmentSource);
	simpleShader = new ShaderProgram(simpleVertexSource, NULL, simpleFragmentSource);

	ui = TextRenderer();
}

void ResourceManager::drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix, glm::mat4 uiProjection, glm::vec3 lightPos)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	complexShader->use();
	glUniformMatrix4fv(complexShader->u("P"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(complexShader->u("V"), 1, false, glm::value_ptr(viewMatrix));
	glUniform3f(complexShader->u("lightPos"), lightPos.x, lightPos.y, lightPos.z);

	simpleShader->use();
	glUniformMatrix4fv(simpleShader->u("P"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(simpleShader->u("V"), 1, false, glm::value_ptr(viewMatrix));
	glUniform3f(simpleShader->u("lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//complex draw
	for (GameObject* object : renderQueue)
	{
		if (!object->renderFlag) continue;

		ShaderProgram* shader;
		if (object->shading)  shader = complexShader;
		else  shader = simpleShader;
		shader->use();
		glUniform1i(complexShader->u("isSpecular"), 0);

		glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(object->modelMatrix));

		if(modelPalette[object->modelIndex].directory == "")
		{
			glUniform1i(shader->u("textureMap0"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texturePalette[object->textureIndex].texture);
		}
		modelPalette[object->modelIndex].draw(shader);
	}

	for (Text t : textQueue)
	{
		ui.RenderText(textShader, uiProjection, t.value, t.x, t.y, t.scale);
	}
	
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}


int ResourceManager::loadModel(const char* modelSource)
{
	Model model;
	
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelSource, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error during assimp model loading: " << import.GetErrorString() << std::endl;
		return 0;
	}
	//directory = path.substr(0, path.find_last_of('/'));

	model.processNode(scene->mRootNode, scene);

	modelPalette.push_back(model);
	return modelPalette.size() - 1;
}

void Model::draw(ShaderProgram *shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		for (unsigned int j = 0; j < meshes[i].textures.size(); j++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string name = meshes[i].textures[j].type;

			if (name == "texture_diffuse") {
				glUniform1i(shader->u("textureMap0"), i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);
			}
			if (name == "texture_specular") {
				glUniform1i(shader->u("textureMap1"), i);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, meshes[i].textures[j].id);

				glUniform1i(shader->u("isSpecular"), 1);
			}

			
		}

		glBindVertexArray(meshes[i].VAO);
		glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 temp;
		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;
		vertex.position = temp;

		temp.x = mesh->mNormals[i].x;
		temp.y = mesh->mNormals[i].y;
		temp.z = mesh->mNormals[i].z;
		vertex.normal = temp;

		

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices,textures);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	if(lodepng::decode(image, width, height, filename))
		std::cout << "Error during texture loading: "<<filename<<std::endl;
	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

std:: vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = readTexture(str.C_Str());
		directory = str.C_Str();
		texture.type = typeName;
		//texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

int ResourceManager::addTexture(const char* textureSource, const char* specSource)
{
	texturePalette.push_back(TextureExternal());
	int size = texturePalette.size() - 1;
	texturePalette[size].texture = readTexture(textureSource);
	if(specSource != "") texturePalette[size].specular = readTexture(specSource);
	texturePalette[size].textureUnit = size;
	return size;
}


int ResourceManager::addText(std::string value, float x,float y, float scale)
{
	Text text = {value, x, y, scale};
	textQueue.push_back(text);
	return textQueue.size() - 1;
}