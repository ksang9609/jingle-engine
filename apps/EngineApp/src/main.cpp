#include <Windows.h>

// D3D libraries
#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// D3D headers
#include <d3d11.h>
#include <d3dcompiler.h>

// ImGui headers
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

struct FVertexSimple
{
	float x, y, z; // Position
	float r, g, b, a; // Color
};

// Structure for a 3D vector
struct FVector
{
	float x, y, z;
	FVector(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}

	float LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	float Length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	FVector operator+(const FVector& v) const
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z
		};
	}

	FVector operator-(const FVector& v) const
	{
		return {
			x - v.x,
			y - v.y,
			z - v.z
		};
	}

	FVector operator*(const float& f) const
	{
		return { x * f, y * f, z * f };
	}

	FVector operator/(const float& f) const
	{
		return { x / f, y / f, z / f };
	}

	FVector Normalized() const
	{
		return *this / Length();
	}

	float dot(const FVector& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	static float dot(const FVector& vA, const FVector& vB)
	{
		return vA.x * vB.x + vA.y * vB.y + vA.z * vB.z;
	}
};

static FVector operator*(const float& f, const FVector& v)
{
	return { v.x * f, v.y * f, v.z * f };
}

#include "Sphere.h"

FVertexSimple triangle_vertices[] =
{
	{  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
	{  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
	{ -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
};

FVertexSimple cube_vertices[] =
{
	// Front face (Z+)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Bottom-left (red)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
	{  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-right (blue)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

	// Back face (Z-)
	{ -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-left (cyan)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
	{  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)

	// Left face (X-)
	{ -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-left (purple)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
	{ -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)
	{ -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

	// Right face (X+)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f }, // Bottom-left (orange)
	{  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
	{  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
	{  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
	{  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
	{  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.5f, 1.0f }, // Top-right (dark blue)

	// Top face (Y+)
	{ -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f, 1.0f }, // Bottom-left (light green)
	{ -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
	{  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)
	{ -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
	{  0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Top-right (brown)
	{  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)

	// Bottom face (Y-)
	{ -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Bottom-left (brown)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
	{ -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
	{  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Top-right (green)
	{  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
};



class URenderer
{
public:
	// Pointers to manage Direct3D 11 device, device context, and swap chain
	ID3D11Device* Device = nullptr; // Direct3D device to communicate with GPU
	ID3D11DeviceContext* DeviceContext = nullptr; // Context to execute the GPU instructions
	IDXGISwapChain* SwapChain = nullptr; // Swap chain to swap the frame buffer

	// Variables to manage the resources and the state for rendering
	ID3D11Texture2D* FrameBuffer = nullptr;
	ID3D11RenderTargetView* FrameBufferRTV = nullptr;
	ID3D11RasterizerState* RasterizerState = nullptr;
	ID3D11Buffer* ConstantBuffer = nullptr;

	FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };
	D3D11_VIEWPORT ViewportInfo;

	// Shader
	ID3D11VertexShader* SimpleVertexShader;
	ID3D11PixelShader* SimplePixelShader;
	ID3D11InputLayout* SimpleInputLayout;
	unsigned int Stride;

	// Constant buffer
	struct FConstants
	{
		FVector Location;
		float Pad0;
		FVector Scale;
		float Pad1;
	};

public:
	void Create(HWND hWindow)
	{
		CreateDeviceAndSwapChain(hWindow);
		CreateFrameBuffer();
		CreateRasterizerState();
	}

	void CreateDeviceAndSwapChain(HWND hWindow)
	{
		D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

		DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
		swapchaindesc.BufferDesc.Width = 0;
		swapchaindesc.BufferDesc.Height = 0;
		swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapchaindesc.SampleDesc.Count = 1;
		swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchaindesc.BufferCount = 2;
		swapchaindesc.OutputWindow = hWindow;
		swapchaindesc.Windowed = TRUE;
		swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
			featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
			&swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

		SwapChain->GetDesc(&swapchaindesc);

		ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
	}

	void ReleaseDeviceAndSwapChain()
	{
		if (DeviceContext)
		{
			DeviceContext->Flush();
		}

		if (SwapChain)
		{
			SwapChain->Release();
			SwapChain = nullptr;
		}

		if (Device)
		{
			Device->Release();
			Device = nullptr;
		}

		if (DeviceContext)
		{
			DeviceContext->Release();
			DeviceContext = nullptr;
		}
	}

	void CreateFrameBuffer()
	{
		SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

		D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
		framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
	}

	void ReleaseFrameBuffer()
	{
		if (FrameBuffer)
		{
			FrameBuffer->Release();
			FrameBuffer = nullptr;
		}

		if (FrameBufferRTV)
		{
			FrameBufferRTV->Release();
			FrameBufferRTV = nullptr;
		}
	}

	void CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC rasterizerdesc = {};
		rasterizerdesc.FillMode = D3D11_FILL_SOLID;
		rasterizerdesc.CullMode = D3D11_CULL_BACK;

		Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
	}

	void ReleaseRasterizerState()
	{
		if (RasterizerState)
		{
			RasterizerState->Release();
			RasterizerState = nullptr;
		}
	}

	void CreateShader()
	{
		ID3DBlob* vertexshaderCSO;
		ID3DBlob* pixelshaderCSO;

		D3DCompileFromFile(L"resources/Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

		Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

		D3DCompileFromFile(L"resources/Shaders/ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

		Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

		Stride = sizeof(FVertexSimple);

		vertexshaderCSO->Release();
		pixelshaderCSO->Release();
	}

	void ReleaseShader()
	{
		if (SimpleInputLayout)
		{
			SimpleInputLayout->Release();
			SimpleInputLayout = nullptr;
		}

		if (SimplePixelShader)
		{
			SimplePixelShader->Release();
			SimplePixelShader = nullptr;
		}

		if (SimpleVertexShader)
		{
			SimpleVertexShader->Release();
			SimpleVertexShader = nullptr;
		}
	}

	ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
	{
		D3D11_BUFFER_DESC vertexbufferdesc = {};
		vertexbufferdesc.ByteWidth = byteWidth;
		vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

		ID3D11Buffer* vertexBuffer;

		Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

		return vertexBuffer;
	}

	void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
	{
		vertexBuffer->Release();
	}

	void CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantbufferdesc = {};
		constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
		constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
		constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
	}

	void ReleaseConstantBuffer()
	{
		if (ConstantBuffer)
		{
			ConstantBuffer->Release();
			ConstantBuffer = nullptr;
		}
	}

	void Release()
	{
		ReleaseRasterizerState();

		DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

		ReleaseFrameBuffer();
		ReleaseDeviceAndSwapChain();
	}

	void SwapBuffer()
	{
		SwapChain->Present(1, 0);
	}

	void Prepare()
	{
		DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetViewports(1, &ViewportInfo);
		DeviceContext->RSSetState(RasterizerState);

		DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
		DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void PrepareShader()
	{
		DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
		DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
		DeviceContext->IASetInputLayout(SimpleInputLayout);

		if (ConstantBuffer)
		{
			DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
		}
	}

	void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

		DeviceContext->Draw(numVertices, 0);
	}

	void UpdateConstant(FVector location, FVector scale)
	{
		if (ConstantBuffer)
		{
			D3D11_MAPPED_SUBRESOURCE constantbufferMSR; // mapped sub resource

			DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
			FConstants* constants = (FConstants*)constantbufferMSR.pData;

			constants->Location = location;
			constants->Scale = scale;

			DeviceContext->Unmap(ConstantBuffer, 0);
		}
	}
};

class UPrimitive
{
public:
	virtual ~UPrimitive() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(URenderer& renderer) = 0;
	virtual bool Intersects(UPrimitive* other) = 0;
	//virtual void Translate(const FVector& v) = 0;
	virtual void AddGravity(float deltaTime) = 0;
};

// Border
const float LeftBorder = -1.0f;
const float RightBorder = 1.0f;
const float TopBorder = -1.0f;
const float BottomBorder = 1.0f;

// Gravity
const FVector Gravity = FVector{ 0.0f, -9.8f, 0.0f };

class UBall : public UPrimitive
{
public:
	FVector Location;
	FVector Velocity;
	float Radius;
	float Mass;
	static int TotalNumBalls;

	ID3D11Buffer* VertexBuffer;
	UINT NumVertices;

	UBall(ID3D11Buffer* pBuffer, UINT numVertices)
	{
		// Set random Radius (0.05f to 0.2f)
		Radius = static_cast<float>(rand()) / RAND_MAX * 0.15f + 0.05f;

		Mass = pow(Radius, 3);

		// Set random Location and Velocity
		Location = {
			(static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f, // -1.0f to 1.0f
			(static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f, // -1.0f to 1.0f
			0.0f
		};
		Velocity = {
			(static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f, // -1.0f to 1.0f
			(static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f, // -1.0f to 1.0f
			0.0f
		};

		TotalNumBalls += 1;

		VertexBuffer = pBuffer;
		NumVertices = numVertices;
	}

	~UBall()
	{
		TotalNumBalls -= 1;
	}

	void Update(float deltaTime)
	{
		Location.x += Velocity.x * deltaTime;
		Location.y += Velocity.y * deltaTime;
		Location.z += Velocity.z * deltaTime;

		BounceWall();
	}

	void Render(URenderer& renderer)
	{
		renderer.UpdateConstant(Location, FVector{ Radius, Radius, Radius });
		renderer.RenderPrimitive(VertexBuffer, NumVertices);
	}

	bool Intersects(UPrimitive* other)
	{
		UBall* otherBall = dynamic_cast<UBall*>(other);
		if (otherBall == nullptr)
		{
			return false;
		}

		FVector direction = otherBall->Location - Location;
		float distance = direction.LengthSquared();
		float combinedRadii = Radius + otherBall->Radius;

		return distance <= combinedRadii * combinedRadii;
	}

	static void BounceBall(UBall& ballA, UBall& ballB)
	{
		// Position correction
		FVector VecAB = ballB.Location - ballA.Location;
		FVector DirAB = VecAB.Normalized();
		float Penetration = ballA.Radius + ballB.Radius - VecAB.Length();

		ballA.Location = ballA.Location - DirAB * Penetration * 0.5;
		ballB.Location = ballB.Location + DirAB * Penetration * 0.5;

		// Elastic collision
		float m1 = ballA.Mass;
		float m2 = ballB.Mass;
		FVector v1 = ballA.Velocity;
		FVector v2 = ballB.Velocity;
		FVector x1 = ballA.Location;
		FVector x2 = ballB.Location;

		FVector newV1 = v1 - (2 * m2 / (m1 + m2)) *
			FVector::dot(v1 - v2, x1 - x2) / (x1 - x2).LengthSquared() *
			(x1 - x2);
		FVector newV2 = v2 - (2 * m1 / (m1 + m2)) *
			FVector::dot(v2 - v1, x2 - x1) / (x2 - x1).LengthSquared() *
			(x2 - x1);

		ballA.Velocity = newV1;
		ballB.Velocity = newV2;
	}

	void BounceWall()
	{
		if (Location.x < LeftBorder + Radius)
		{
			Location.x = LeftBorder + Radius;
			Velocity.x *= -1.0f;
		}
		if (Location.x > RightBorder - Radius)
		{
			Location.x = RightBorder - Radius;
			Velocity.x *= -1.0f;
		}
		if (Location.y < TopBorder + Radius)
		{
			Location.y = TopBorder + Radius;
			Velocity.y *= -1.0f;
		}
		if (Location.y > BottomBorder - Radius)
		{
			Location.y = BottomBorder - Radius;
			Velocity.y *= -1.0f;
		}
	}

	void AddGravity(float deltaTime)
	{
		Velocity = Velocity + Gravity * deltaTime;
	}
};

int UBall::TotalNumBalls = 0;

void InitPrimitiveList(
	int initialCapacity,
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	capacity = initialCapacity > 0
		? initialCapacity
		: 4;
	count = 0;
	primitiveList = new UPrimitive * [capacity];
	for (int i = 0; i < capacity; i++)
	{
		primitiveList[i] = nullptr;
	}
}

void ResizePrimitiveList(
	int newCapacity,
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	UPrimitive** newList = new UPrimitive * [newCapacity];
	for (int i = 0; i < newCapacity; i++)
	{
		newList[i] = nullptr;
	}

	for (int i = 0; i < min(count, newCapacity); i++)
	{
		newList[i] = primitiveList[i];
	}

	delete[] primitiveList;
	primitiveList = newList;
	capacity = newCapacity;
}

void AddNewBall(
	ID3D11Buffer* pBuffer,
	UINT numVertices,
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	if (count >= capacity)
	{
		ResizePrimitiveList(capacity * 2, primitiveList, count, capacity);
	}

	primitiveList[count++] = new UBall(pBuffer, numVertices);
}

void RemoveRandomBall(
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	if (count == 0)
	{
		return;
	}

	int delIndex = rand() % count;

	delete primitiveList[delIndex];

	primitiveList[delIndex] = primitiveList[count - 1];
	primitiveList[count - 1] = nullptr;
	count--;
}

void ModifyNumOfBalls(
	int newCount,
	ID3D11Buffer* pBuffer,
	UINT numVertices,
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	if (newCount == count || newCount < 1)
	{
		return;
	}

	if (newCount > count)
	{
		for (int i = 0; i < newCount - count; i++)
		{
			AddNewBall(
				pBuffer,
				numVertices,
				primitiveList,
				count,
				capacity);
		}
		return;
	}

	// if new count < count
	for (int i = 0; i < count - newCount; i++)
	{
		RemoveRandomBall(primitiveList, count, capacity);
	}
}

void ReleasePrimitiveList(
	UPrimitive**& primitiveList,
	int& count,
	int& capacity)
{
	for (int i = 0; i < count; i++)
	{
		delete primitiveList[i];
	}

	delete[] primitiveList;
	primitiveList = nullptr;
	count = 0;
	capacity = 0;
}

void RenderAllBalls(
	URenderer& renderer,
	UPrimitive** const& primitiveList,
	const int& count)
{
	for (int i = 0; i < count; i++)
	{
		primitiveList[i]->Render(renderer);
	}
}

void UpdateAllBalls(
	float deltaTime,
	UPrimitive** const& primitiveList,
	const int& count)
{
	for (int i = 0; i < count; i++)
	{
		primitiveList[i]->Update(deltaTime);
	}
}

void ResolveAllBallCollisions(
	UPrimitive** const& primitiveList,
	const int& count)
{
	for (int i = 0; i < count; i++)
	{
		for (int j = i + 1; j < count; j++)
		{
			if (primitiveList[i]->Intersects(primitiveList[j]))
			{
				UBall* a = dynamic_cast<UBall*>(primitiveList[i]);
				UBall* b = dynamic_cast<UBall*>(primitiveList[j]);

				if (a && b)
				{
					UBall::BounceBall(*a, *b);
				}
			}
		}
	}
}

void AddAllBallGravity(
	float deltaTime,
	UPrimitive** const& primitiveList,
	const int& count)
{
	for (int i = 0; i < count; i++)
	{
		primitiveList[i]->AddGravity(deltaTime);
	}
}

class UBallSnapshot
{
public:
	FVector Location;
	float Radius;

	ID3D11Buffer* VertexBuffer;
	UINT NumVertices;

	UBallSnapshot(
		FVector location,
		float radius,
		ID3D11Buffer* pBuffer,
		UINT numVertices)
	{
		Location = location;
		Radius = radius;

		VertexBuffer = pBuffer;
		NumVertices = numVertices;
	}

	UBallSnapshot(const UBall& ball)
		: Location{ ball.Location }
		, Radius{ ball.Radius }
		, VertexBuffer{ ball.VertexBuffer }
		, NumVertices{ ball.NumVertices }
	{
	}

	void Render(URenderer& renderer)
	{
		renderer.UpdateConstant(Location, FVector{ Radius, Radius, Radius });
		renderer.RenderPrimitive(VertexBuffer, NumVertices);
	}
};

struct FReplayFrame
{
	int NumBalls;
	UBallSnapshot** BallSnapshotList;

	FReplayFrame()
		: NumBalls{ 0 }
		, BallSnapshotList{ nullptr }
	{
	}

	FReplayFrame(int numBalls)
		: NumBalls{ numBalls }
	{
		BallSnapshotList = new UBallSnapshot * [numBalls];
		for (int i = 0; i < numBalls; i++)
		{
			BallSnapshotList[i] = nullptr;
		}
	}

	void Release()
	{
		for (int i = 0; i < NumBalls; i++)
		{
			delete BallSnapshotList[i];
		}
		delete[] BallSnapshotList;
		BallSnapshotList = nullptr;
		NumBalls = 0;
	}
};

class UReplayBuffer
{
public:
	const static int BufferCapacity = 150; // 30 FPS * 5 sec
	FReplayFrame ReplayFrame[BufferCapacity];
	int ReplayWriteIndex = 0;
	int ReplayCount = 0;

	UReplayBuffer()
	{
		for (int i = 0; i < BufferCapacity; i++)
		{
			ReplayFrame[i] = FReplayFrame();
		}
	}

	~UReplayBuffer()
	{
		for (int i = 0; i < ReplayCount; i++)
		{
			ReplayFrame[i].Release();
		}
	}

	void RecordFrame(
		UPrimitive** const& primitiveList,
		int count)
	{
		ReplayCount = min(ReplayCount + 1, BufferCapacity);

		if (ReplayFrame[ReplayWriteIndex].BallSnapshotList)
		{
			ReplayFrame[ReplayWriteIndex].Release();
		}

		ReplayFrame[ReplayWriteIndex].NumBalls = count;
		ReplayFrame[ReplayWriteIndex].BallSnapshotList =
			new UBallSnapshot * [count];

		for (int i = 0; i < count; i++)
		{
			ReplayFrame[ReplayWriteIndex].BallSnapshotList[i] = nullptr;

			UBall* ball = dynamic_cast<UBall*>(primitiveList[i]);
			if (ball == nullptr)
			{
				continue;
			}

			ReplayFrame[ReplayWriteIndex].BallSnapshotList[i] =
				new UBallSnapshot(*ball);
		}
		ReplayWriteIndex = (ReplayWriteIndex + 1) % BufferCapacity;
	}

	void RenderReplayFrame(
		int frameNum,
		URenderer& renderer)
	{
		if (frameNum < 0 || frameNum >= ReplayCount)
		{
			return;
		}

		int OldestIndex = (ReplayCount == BufferCapacity) ? ReplayWriteIndex : 0;
		int ActualIndex = (OldestIndex + frameNum) % BufferCapacity;

		FReplayFrame& Frame = ReplayFrame[ActualIndex];

		for (int i = 0; i < Frame.NumBalls; i++)
		{
			Frame.BallSnapshotList[i]->Render(renderer);
		}
	}
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPAram);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
	{
		return true;
	}

	switch (message)
	{
	case WM_DESTROY:
		// Sinal that the app should quit
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Window class name
	WCHAR WindowClass[] = L"JungleWindowClass";

	// Window title name
	WCHAR Title[] = L"Game Tech Lab";

	WNDCLASSW wndclass = { 0, WndProc, 0, 0, 0, 0, 0, 0, 0, WindowClass };

	RegisterClassW(&wndclass);

	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024,
		nullptr, nullptr, hInstance, nullptr);

	// Create Renderer Class
	URenderer renderer;

	renderer.Create(hWnd);
	renderer.CreateShader();
	renderer.CreateConstantBuffer();

	// Create ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init((void*)hWnd);
	ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);

	// Create vertex buffer after renderer and shader

	UINT numVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);
	UINT numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
	UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);


	ID3D11Buffer* vertexBufferTriangle = renderer.CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
	ID3D11Buffer* vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
	ID3D11Buffer* vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));

	bool bIsExit = false;

	enum ETypePrimitive
	{
		EPT_Triangle,
		EPT_Cube,
		EPT_Sphere,
		EPT_Max,
	};

	// Frame timer
	const int targetFPS = 30;
	const double targetFrameTime = 1000.0 / targetFPS;

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);

	LARGE_INTEGER startTime, endTime, PrevTime;
	double elapsedTime = 0.0f;
	float DeltaTime = 0.0f;
	QueryPerformanceCounter(&PrevTime);

	// Ball Management
	UPrimitive** PrimitiveList = nullptr;
	int PrimitiveCount;
	int PrimitiveCapacity;


	InitPrimitiveList(4, PrimitiveList, PrimitiveCount, PrimitiveCapacity);
	AddNewBall(
		vertexBufferSphere,
		numVerticesSphere,
		PrimitiveList,
		PrimitiveCount,
		PrimitiveCapacity);

	// Gravity
	bool bUseGravity = true;

	//// Extra features
	// Time scaling
	float TimeScale = 1.0f;
	bool bPause = false;

	// Replay
	UReplayBuffer Replaybuffer;
	int ReplayFrameNum = 0;

	// Main Loop
	while (bIsExit == false)
	{
		// Timer
		QueryPerformanceCounter(&startTime);

		DeltaTime = static_cast<float>(startTime.QuadPart - PrevTime.QuadPart) /
			static_cast<double>(frequency.QuadPart);
		PrevTime = startTime;

		// Message handling
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessageW(&msg);

			if (msg.message == WM_QUIT)
			{
				bIsExit = true;
				break;
			}
		}

		if (bUseGravity)
		{
			AddAllBallGravity(
				DeltaTime * TimeScale,
				PrimitiveList,
				PrimitiveCount);
		}
		UpdateAllBalls(
			DeltaTime * TimeScale,
			PrimitiveList,
			PrimitiveCount);
		ResolveAllBallCollisions(
			PrimitiveList,
			PrimitiveCount);

		// Do something

		// Preparing
		renderer.Prepare();
		renderer.PrepareShader();

		if (bPause) // replay mode
		{
			Replaybuffer.RenderReplayFrame(ReplayFrameNum, renderer);
		}
		else
		{
			RenderAllBalls(renderer, PrimitiveList, PrimitiveCount);
			// Record replay buffer
			Replaybuffer.RecordFrame(PrimitiveList, PrimitiveCount);
		}

		// ImGui
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Jungle Property Window");

		ImGui::Text("Hello Jungle World!");

		if (ImGui::Checkbox("Gravity", &bUseGravity));

		int numBalls = UBall::TotalNumBalls;
		ImGui::InputInt("Number of Balls", &numBalls);

		ModifyNumOfBalls(
			numBalls,
			vertexBufferSphere,
			numVerticesSphere,
			PrimitiveList,
			PrimitiveCount,
			PrimitiveCapacity);

		if (ImGui::Checkbox("Pause", &bPause))
		{
			TimeScale = 1.0f - bPause;

			if (bPause)
			{
				ReplayFrameNum = max(0, Replaybuffer.ReplayCount - 1);
			}
		}

		if (bPause)
		{
			int MaxFrame = max(0, Replaybuffer.ReplayCount - 1);
			if (ReplayFrameNum > MaxFrame)
			{
				ReplayFrameNum = MaxFrame;
			}
			ImGui::SliderInt("Replay Frame", &ReplayFrameNum, 0, MaxFrame);
		}

		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Swap buffer after rendering is completed
		renderer.SwapBuffer();

		do
		{
			Sleep(0);

			QueryPerformanceCounter(&endTime);

			elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;
		} while (elapsedTime < targetFrameTime);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	ReleasePrimitiveList(PrimitiveList, PrimitiveCount, PrimitiveCapacity);
	renderer.ReleaseVertexBuffer(vertexBufferTriangle);
	renderer.ReleaseVertexBuffer(vertexBufferCube);
	renderer.ReleaseVertexBuffer(vertexBufferSphere);
	renderer.ReleaseConstantBuffer();
	renderer.ReleaseShader();
	renderer.Release();

	return 0;
}