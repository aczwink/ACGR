#ifdef _BUILD_ACGE
#define ACGE_API __declspec(dllexport)
#else
#define ACGE_API __declspec(dllimport)
#endif