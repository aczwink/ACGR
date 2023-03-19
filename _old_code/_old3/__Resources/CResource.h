/*
namespace ACGE
{
	//Forward declarations
	class CMaterial;
	class CMesh;
	class CTexture;
	
	class CResource
	{
	private:
		//Members
		byte *pBuffer;
		uint32 size;
		void *pAggregated;

	public:
		//Constructors		
		CResource(EACFFTag type);
		
		CMaterial *GetMaterial(const ACStdLib::CString &refPath);
		CTexture *GetTexture(const ACStdLib::CString &refPath);
		ACStdLib::CBufferInputStream GetStream(const ACStdLib::CString &refPath);
		void Release();
		void ReleaseAggregate();
	};
}
*/