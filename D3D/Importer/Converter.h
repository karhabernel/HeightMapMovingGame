#pragma once

class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(wstring file);


//----------------------------------------------------------------------------
//Export Bone, Mesh Data
//----------------------------------------------------------------------------
public:
	void ExportMesh(wstring savePath);

private:
	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int index);
	void WriteMeshData(wstring savePath);


//----------------------------------------------------------------------------
//Export Material
//----------------------------------------------------------------------------
public:
	void ExportMaterial(wstring savePath, bool bOverWrite = true);

private:
	void ReadMaterialData();
	bool FoundMaterialData(aiMaterial* material);
	void WriteMaterialData(wstring savePath);
	string WriteTexture(string savePath, string file);

private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
	vector<struct asMaterial*> materials;
};
