#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	/*  函数   */
	Model(char *path, OpenglState *glSate);
	void Draw();
private:
	
	/*  函数   */
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
		string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
public:
	OpenglState *_glSate;

	/*  模型数据  */
	vector<Texture> textures_loaded;// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
};
