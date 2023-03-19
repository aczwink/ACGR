#ifdef BUILD_CURSEGINE
#define CURSEGINE_API __declspec(dllexport)
#else
#define CURSEGINE_API __declspec(dllimport)
#endif

//Function-like macro definitions
#define SAFE_RELEASE(pInterface) if(pInterface) { pInterface->Release(); pInterface = NULL; }

//Math
#define PI 3.141592654f
#define DEG2RAD(x) (x * 0.0174532925f)

//Objects
#define D3D CEngine::Get().graphics.d3d