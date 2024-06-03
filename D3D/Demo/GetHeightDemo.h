#pragma once

#include "Systems/IExecute.h"

class GetHeightDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void CreateStaticMesh();
	void ChangeCube(UINT inTexture);
	void ChangeTexture(UINT inBase, UINT inLayer);

private:
	struct Vertex
	{
		Vector3 Position;
	};


private:
	Shader* shader;
	Shader* cubeShader;
	Terrain* terrain;
	
	StaticMesh_Cube* cube;
	//Shader* triShader;
	//ID3D11Buffer* vertexBuffer;

	Vector3 position = Vector3(105, 0, 100);
	Vector3 lightDirection = Vector3(-1, -1, 1);

	UINT baseMap = 0;
	UINT layerMap = 0;
	UINT alphaMAp = 0;
};