#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	shader = new Shader(L"05_World.fxo");

	D3DXMatrixIdentity(&world);

	//Plane
	vertices[0].Position = Vector3(-0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[1].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0.f);
	vertices[2].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[3].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0.f);
	vertices[4].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0.f);
	vertices[5].Position = Vector3(+0.5f + 0.5f, +0.5f + 0.5f, 0.f);

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void WorldDemo::Update()
{
#ifdef MatrixMember
	//Position
	if (Keyboard::Get()->Press('D'))
		world._41 += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('A'))
		world._41 -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press('W'))
		world._42 += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		world._42 -= 2.f * Time::Delta();

	//Scale
	if (Keyboard::Get()->Press(VK_RIGHT))
		world._11 += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		world._11 -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		world._22 += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		world._22 -= 2.f * Time::Delta();
#endif

	//Position
	static Vector3 position;

	if (Keyboard::Get()->Press('D'))
		position.x += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('A'))
		position.x -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press('W'))
		position.y += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press('S'))
		position.y -= 2.f * Time::Delta();

	//Scale
	static Vector3 scale = Vector3(1, 1, 1);

	if (Keyboard::Get()->Press(VK_RIGHT))
		scale.x += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		scale.x -= 2.f * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		scale.y += 2.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		scale.y -= 2.f * Time::Delta();

	static float roll;
	roll += Time::Delta();

	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationZ(&R, roll);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	world = S * R * T;
}

void WorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

	static bool bWireFrame;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	shader->Draw(0, bWireFrame ? 1 : 0, 6);
}
