#include "stdafx.h"
#include "GetHeightDemo.h"

void GetHeightDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(30, 0, 0);
	Context::Get()->GetCamera()->Position(110, 45, 20);
	((Freedom*)Context::Get()->GetCamera())->Speed(50, 10);

	shader = new Shader(L"10_Terrain.fxo");
	cubeShader = new Shader(L"13_StaticMesh.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Dirt2.png");
	terrain->LayerMap(L"Terrain/Cliff (Layered Rock).jpg");
	terrain->AlphaMap(L"Terrain/Gray256.png");



	/*triShader = new Shader(L"12_Tri.fxo");
	Vertex vertices[3];
	vertices[0].Position = Vector3(0, 1, 0);
	vertices[1].Position = Vector3(-1, 0, 0);
	vertices[2].Position = Vector3(+1, 0, 0);*/

	CreateStaticMesh();

	/*D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 3;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subRessource = { 0 };
	subRessource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subRessource, &vertexBuffer));*/

}

void GetHeightDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(cubeShader);
	SafeDelete(terrain);

	SafeDelete(cube);

	//SafeDelete(triShader);
	//SafeRelease(vertexBuffer);
}

void GetHeightDemo::Update()
{
	//Terrain Paramters
	{
		static int pass = shader->PassCount() - 1;
		ImGui::SliderInt("Pass", &pass, 0, shader->PassCount() - 1);
		terrain->Pass() = pass;

		static float tile = 4.f;
		ImGui::SliderFloat("Tile", &tile, 1, 10);
		terrain->Tile(tile);

		static float intensity = 2.5f;
		ImGui::SliderFloat("Intensity", &intensity, 1, 10);
		terrain->Intensity(intensity);

		ImGui::SliderFloat3("Light Direction", lightDirection, -1, 1);
		terrain->SetLightDirection(lightDirection);
		shader->AsVector("LightDirection")->SetFloatVector(lightDirection);
		cubeShader->AsVector("LightDirection")->SetFloatVector(lightDirection);

		static int base;
		static int layer;
		static int cube;
		ImGui::SliderInt("BaseMap", &base, 0, 4);
		ImGui::SliderInt("LayerMap", &layer, 0, 4);
		ImGui::SliderInt("Cube", &cube, 0, 7);
		
		if (ImGui::Button("Apply"))
		{
			ChangeTexture((UINT)base, (UINT)layer);
			ChangeCube((UINT)cube);
		}

	}

	//Tri Key Input
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			position.x += 20.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			position.x -= 20.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			position.z += 20.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			position.z -= 20.f * Time::Delta();

		//position.y = terrain->GetHeightByInterp(position) + 1;
		position.y = terrain->GetHeightByRaycast(position) + 1;
		cube->GetTransform()->Position(position);
		
		// Get Rotation
		Vector3 normal = terrain->GetNormalData(position);
		cube->GetTransform()->Rotation(normal);

		Matrix R, T;
		D3DXMatrixRotationX(&R, D3DX_PI);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);
		Matrix world = R * T;

		//triShader->AsMatrix("World")->SetMatrix(world);
		//triShader->AsMatrix("View")->SetMatrix(Context::Get()->View());
		//triShader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
	}

	
	terrain->Update();
	cube->Update();
}

void GetHeightDemo::Render()
{
	terrain->Render();
	cube->Render();

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//triShader->Draw(0, 0, 3);


	string str = to_string(position.x) + ", " + to_string(position.y) + ", " + to_string(position.z);
	Gui::Get()->RenderText(5, 60, str);
}

void GetHeightDemo::CreateStaticMesh()
{
	cube = new StaticMesh_Cube(cubeShader);
	cube->GetTransform()->Scale(2, 2, 2);
	cube->DiffuseMap(L"Floor.png");
}

void GetHeightDemo::ChangeCube(UINT inTexture)
{
	switch (inTexture)
	{
	case 0: cube->DiffuseMap(L"Floor.png"); break;
	case 1: cube->DiffuseMap(L"Stones.png"); break;
	case 2: cube->DiffuseMap(L"Bricks.png"); break;
	case 3: cube->DiffuseMap(L"Box.png"); break;
	case 4: cube->DiffuseMap(L"Red.png"); break;
	case 5: cube->DiffuseMap(L"Yellow.png"); break;
	case 6: cube->DiffuseMap(L"Magenta.png"); break;
	case 7: cube->DiffuseMap(L"Blue.png"); break;
	}

}

void GetHeightDemo::ChangeTexture(UINT inBase, UINT inLayer)
{
	switch (inBase)
	{
	case 0: terrain->BaseMap(L"Terrain/Dirt2.png"); break;
	case 1: terrain->BaseMap(L"Terrain/Dirt (Grass).jpg"); break;
	case 2: terrain->BaseMap(L"Terrain/Grass (Meadows).jpg"); break;
	case 3: terrain->BaseMap(L"Terrain/Grass (Rocky).jpg"); break;
	case 4: terrain->BaseMap(L"Terrain/Grass (Muddy).jpg"); break;
	}

	switch (inLayer)
	{
	case 0: terrain->LayerMap(L"Terrain/Cliff (Layered Rock).jpg"); break;
	case 1: terrain->LayerMap(L"Terrain/Cliff (Grassy).jpg"); break;
	case 2: terrain->LayerMap(L"Terrain/Cliff (Sandstone).jpg"); break;
	case 3: terrain->LayerMap(L"Terrain/Dirt (Grass).jpg"); break;
	case 4: terrain->LayerMap(L"Terrain/Grass (Hill).jpg"); break;
	}

}
