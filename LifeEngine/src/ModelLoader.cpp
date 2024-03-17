#include "ModelLoader.h"
#include "OBJ_Loader.h"

void ModelLoader::init()
{
}

void ModelLoader::update()
{
}

void ModelLoader::render()
{
}

void ModelLoader::destroy()
{
}

Mesh
ModelLoader::Load(std::string objFileName)
{
	//Temporal Load Data
	Mesh mesh;
	objl::Loader Loader;
	Loader.LoadFile(objFileName);

	mesh.name = objFileName;

	WARNING("ModelLoader::Load() [Load of file: " << objFileName.c_str() << "] \n");

	//Load and set data to custom structure (SimpleVertex)
	mesh.vertex.resize(Loader.LoadedVertices.size());

	for (int i = 0; i < mesh.vertex.size(); i++)
	{
		// Load Positions
		mesh.vertex[i].Pos.x = Loader.LoadedVertices[i].Position.X;
		mesh.vertex[i].Pos.y = Loader.LoadedVertices[i].Position.Y;
		mesh.vertex[i].Pos.z = Loader.LoadedVertices[i].Position.Z;

		// Load Textures
		mesh.vertex[i].Tex.x = Loader.LoadedVertices[i].TextureCoordinate.X;
		mesh.vertex[i].Tex.y = Loader.LoadedVertices[i].TextureCoordinate.Y;
	}

	// Load and set data to custom structure
	mesh.index.resize(Loader.LoadedIndices.size());
	for (int i = 0; i < mesh.index.size(); i++)
	{
		mesh.index[i] = Loader.LoadedIndices[i];
	}

	mesh.numVertex = mesh.vertex.size();
	mesh.numIndex = mesh.index.size();

	return mesh;
}