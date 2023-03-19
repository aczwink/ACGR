//Libs
#include <ACGameEngine.h>
//Namespaces
using namespace ACGE;

class CApplication : public AApplication
{
	class CVertex
	{
	public:
		//Members
		CVector3 position;
		CColor color;

		//Constructor
		inline CVertex(float32 x, float32 y, float32 z, float32 r, float32 g, float32 b, float32 a)
		{
			this->position.x = x;
			this->position.y = y;
			this->position.z = z;

			this->color = CColor(r, g, b, a);
		}
	};

	struct SShaderConstants
	{
		CMatrix4x4 WVP;
	};
private:
	//Members
	CColor backgroundColor;
	int8 backgroundColorDirection[3];
	CInputLayout *pInputLayout;
	CIndexBuffer *pIndexBuffer;
	CVertexBuffer *pVertexBuffer;
	CConstantBuffer *pWVPBuffer;
	SShaderConstants shaderConstants;
	ID3D11RasterizerState *pRasterizerState;
	
	CMatrix4x4 camView;
	CMatrix4x4 camProjection;
	
	CMatrix4x4 cube1World;
	CMatrix4x4 cube2World;
	CMatrix4x4 rotation;
	CMatrix4x4 scale;
	CMatrix4x4 translation;
	float32 rot;
	
	//Methods
	void RenderScene()
	{
		IDXGISwapChain *pSwapChain;
		ID3D11DeviceContext *pImmediateContext;
		CMatrix4x4 WVP;
		
		pSwapChain = this->d3d.GetSwapChain();
		pImmediateContext = this->d3d.GetImmediateContext();
		
		pImmediateContext->ClearRenderTargetView(this->d3d.GetRenderTargetView(), this->backgroundColor);
		pImmediateContext->ClearDepthStencilView(this->d3d.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//Set the WVP matrix and send it to the constant buffer in effect file
		WVP = this->cube1World * camView * camProjection;
		this->shaderConstants.WVP = WVP.Transpose();
		this->pWVPBuffer->Update(&this->shaderConstants);
		
		//Draw the first cube
		pImmediateContext->DrawIndexed(36, 0, 0);
		
		WVP = this->cube2World * camView * camProjection;
		this->shaderConstants.WVP = WVP.Transpose();
		this->pWVPBuffer->Update(&this->shaderConstants);

		//Draw the second cube
		pImmediateContext->DrawIndexed(36, 0, 0);
		
		pSwapChain->Present(0, 0);
	}
	
	void UpdateScene()
	{
		this->backgroundColor.r += this->backgroundColorDirection[0] * 0.00005f;
		this->backgroundColor.g += this->backgroundColorDirection[1] * 0.00002f;
		this->backgroundColor.b += this->backgroundColorDirection[2] * 0.00001f;

		for(uint8 i = 0; i < 3; i++)
		{
			if(this->backgroundColor.c[i] >= 1 || this->backgroundColor.c[i] <= 0)
				this->backgroundColorDirection[i] *= -1;
		}
		
		//Keep the cubes rotating
		this->rot += .0005f;
		if(this->rot > 2*PI)
			this->rot = 0.0f;
		
		this->cube1World = CMatrix4x4::Identity();
		
		//Define cube1's world space matrix
		this->rotation = CMatrix4x4::RotationPitchYawRoll(0, this->rot, 0);
		this->translation = CMatrix4x4::Translation(0, 0, 4);
		
		//Set cube1's world space using the transformations
		this->cube1World = this->translation * this->rotation;
		
		//Reset cube2World
		this->cube2World = CMatrix4x4::Identity();
		
		//Define cube2's world space matrix
		this->rotation = CMatrix4x4::RotationPitchYawRoll(0, -this->rot, 0);
		this->scale = CMatrix4x4::Scale(1.3f, 1.3f, 1.3f);
		
		//Set cube2's world space matrix
		this->cube2World = this->rotation * this->scale;
	}
public:
	//Constructor
	CApplication()
	{
		this->pIndexBuffer = nullptr;
		this->pInputLayout = nullptr;
		this->pVertexBuffer = nullptr;
		this->pWVPBuffer = nullptr;

		this->rot = 0.01f;

		this->pRasterizerState = NULL;
	}
	
	//Destructor
	~CApplication()
	{
		if(this->pIndexBuffer)
			delete this->pIndexBuffer;
		if(this->pInputLayout)
			delete this->pInputLayout;
		if(this->pVertexBuffer)
			delete this->pVertexBuffer;
		if(this->pWVPBuffer)
			delete this->pWVPBuffer;
		COM_SAFE_RELEASE(this->pRasterizerState);
	}
	
	//Methods
	void InitScene()
	{
		ID3D11Device *pDevice;
		ID3D11DeviceContext *pImmediateContext;
		ID3D11VertexShader *pVertexShader;
		ID3DBlob *pVertexShaderBuffer;

		CConfig &refConfig = CConfig::Get();
		
		pDevice = this->d3d.GetDevice();
		pImmediateContext = this->d3d.GetImmediateContext();
		
		this->backgroundColorDirection[0] = 1;
		this->backgroundColorDirection[1] = 1;
		this->backgroundColorDirection[2] = 1;

		pVertexShader = this->shaderManager.GetVertexShader("default.fx");
		pVertexShaderBuffer = this->shaderManager.GetShaderData(pVertexShader);

		//Set vertex and pixel shaders
		pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
		pImmediateContext->PSSetShader(this->shaderManager.GetPixelShader("default.fx"), NULL, 0);
		
		//Create some vertices
		CVertex vertices[] =
		{
			CVertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			CVertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			CVertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
			CVertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
			CVertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			CVertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
			CVertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
			CVertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		};
		
		this->pVertexBuffer = new CVertexBuffer(sizeof(vertices) / sizeof(vertices[0]), sizeof(vertices[0]), vertices, this->d3d);
		
		//indices
		uint32 indices[] =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		this->pIndexBuffer = new CIndexBuffer(sizeof(indices) / sizeof(indices[0]), indices, this->d3d);

		//input layout
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		
		this->pInputLayout = new CInputLayout(sizeof(layout) / sizeof(layout[0]), layout, pVertexShaderBuffer, this->d3d);

		//WVP in shaders
		this->pWVPBuffer = new CConstantBuffer(sizeof(SShaderConstants), this->d3d);

		//camera stuff
		CVector3 camPosition, camTarget, camUp;
		CMatrix4x4 world, WVP;

		camPosition = CVector3(0, 5.0f, -12.0f);
		camTarget = CVector3(0, 0, 0);
		camUp = CVector3(0, 1, 0);

		camView = CMatrix4x4::LookAtLH(camPosition, camTarget, camUp);
		camProjection = CMatrix4x4::PerspectiveFovLH(DEG2RAD(45), (float32)refConfig.GetResolutionWidth() / (float32)refConfig.GetResolutionHeight(), 1.0f, 1000.0f);
		world = CMatrix4x4::Identity();
		WVP = world * camView * camProjection;
		this->shaderConstants.WVP = WVP.Transpose();

		this->pWVPBuffer->Update(&this->shaderConstants);

		//rasterizer state
		HRESULT hr;
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		
		hr = pDevice->CreateRasterizerState(&rasterizerDesc, &this->pRasterizerState);
		pImmediateContext->RSSetState(this->pRasterizerState);
	}
};

#include <DirectXMath.h>
using namespace DirectX;
void Test()
{
	XMVECTOR vec;
	
	vec = XMVectorSet(20, 10, 40, 50);
	
	XMMatrixRotationAxis(vec, 30);
	XMMatrixRotationRollPitchYaw(30, 40, 50);
	XMMatrixTranslation(0.0f, 0.0f, 4.0f);
	XMMatrixScaling(1.3f, 1.3f, 1.3f);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *lpCmdLine, int nCmdShow)
{
	CApplication app;

	Test();
	
	if(!app.Init(hInstance, "Develop", nCmdShow))
	{
		MessageBox(NULL, "Can't init engine", "ERROR", MB_ICONERROR);
		return EXIT_FAILURE;
	}
	
	app.InitScene();
	
	return app.Run();
}