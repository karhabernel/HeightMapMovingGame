![스크린샷 2024-06-11 014539](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/35e92bbd-b59c-4be3-a561-1fde3ca8602e)

DX를 이용하여 제작한 지형에 따라 움직이는 큐브 게임입니다.
ImGui 조작을 통해 큐브 텍스쳐, 지형, 지면 텍스쳐(타일, 높이에 따른 지형변화 포함), 하늘 텍스쳐 및 광원 방향의 조정이 가능합니다.

![2](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/ef80554f-71fe-459e-a6d7-8c90fc3f871b)

GameTest.zip 내 GaemTest 바로가기를 통해 포트폴리오 내용 확인이 가능합니다.

<details>
    <summary>1. 기획</summary>

![그림1](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/03d6fd33-a96e-4cb1-8d8c-5bc30fbbb3b7)
DX를 통해 지형과 큐브를 그리고, 큐브 아래의 지형 각도에 맞추어 큐브의 rotation을 조정하는 게임을 만들고자 합니다.


</details>


<details>
    <summary>2. 지형 구성</summary>

지형은 머티리얼 역할을 할 텍스처 2개(지면, 언덕)과 HeightMap 역할을 할 흑백 이미지 한장을 이용하여 만들었습니다.

![Dirt2](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/46ad99ab-2311-42f6-be87-dba7df212215)
![Cliff (Layered Rock) (2)](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/4d386fda-7a08-4caa-af87-194ad362ccdf)

머티리얼 역할을 할 텍스처 2개



![Gray256](https://github.com/karhabernel/HeightMapMovingGame/assets/124255059/cd3cde4e-063c-4345-9963-6a44da0d161e)

HeightMap으로 사용한 흑백 이미지

HeightMap은 픽셀의 RGB 중 R값을 확인하여 R값이 높을수록(즉, 흰색일수록) 고도가 높아지도록 처리하였습니다.



	void Terrain::CreateVertexData()
	{
		width = heightMap->GetWidth();
		height = heightMap->GetHeight();
		
		vector<Color> pixels;
		heightMap->ReadPixel(&pixels);	

		vertexCount = width * height;
		vertices = new VertexTerrain[vertexCount];

		for (UINT y = 0; y < height; y++)
		{
			for (UINT x = 0; x < width; x++)
			{
				UINT index = width * y + x;
				UINT reverse = width * (height - y - 1) + x;	

				vertices[index].Position.x = (float)x;
				vertices[index].Position.y = pixels[reverse].r * 255.f / 10.f;
				vertices[index].Position.z = (float)y;

				vertices[index].Uv.x = x / ((float)width - 1);
				vertices[index].Uv.y = 1 - (y / ((float)height - 1));
			}
		}
  	}
 
 


</details>


<details>
    <summary>3. 큐브 구성</summary>

큐브는 한 텍스처를 여섯 개의 면에 투영하는 식으로 머티리얼 작업을 진행하였습니다.

큐브 하단 사각형의 네 꼭짓점의 Pitch값을 구하기 위하여 atan2 함수를 사용하였습니다.

Terrain에서 미리 작업해 둔 해당 지면 Vertex의 Normal Vector 값을 가져온 뒤, atan2 함수를 통해 pitch값을 구한 뒤, 큐브 하단 사각형 꼭짓점의 Pitch값을 전부 더하고 4로 나누어 평균값을 구했습니다.



		Vector3 p[4];
		for (UINT i = 0; i < 4; i++)
			p[i] = vertices[index[i]].Normal;

		float pitch = atan2(p[0].y, p[0].z);
		float pitch2 = atan2(p[1].y, p[1].z);
		float pitch3 = atan2(p[2].y, p[2].z);
		float pitch4 = atan2(p[3].y, p[3].z);
		
		pitch = (pitch + pitch2 + pitch3 + pitch4) / 4;

		return Vector3(-pitch, 0, 0);
  
  


</details>
