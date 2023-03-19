#ifdef _BUILD_ACGE
#define ACGE_API __declspec(dllexport)
#else
#define ACGE_API __declspec(dllimport)
#endif

//Function-like macro definitions
#define COM_SAFE_RELEASE(pInterface) if(pInterface) { pInterface->Release(); pInterface = nullptr; }