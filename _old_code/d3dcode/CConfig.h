//Libs
#include <ACStdLib.h>
//Local
#include "Definitions.h"
//Definitions
#define CONFIG_SECTION_VIDEO "Video"
#define CONFIG_KEY_VIDEO_RESOLUTIONHEIGHT "ResolutionHeight"
#define CONFIG_KEY_VIDEO_RESOLUTIONWIDTH "ResolutionWidth"
#define CONFIG_KEY_VIDEO_WINDOWED "Windowed"

namespace ACGE
{
	class ACGE_API CConfig
	{
	private:
		//Members
		ACStdLib::CIniFile cfgFile;
		//Constructor
		CConfig();
		//Methods
		void AutoConfigVideo();
	public:
		//Functions
		static inline CConfig &Get()
		{
			static CConfig cfg;

			return cfg;
		}
		//Inline
		inline uint16 GetResolutionHeight() const
		{
			return (uint16)this->cfgFile.GetIntValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_RESOLUTIONHEIGHT);
		}

		inline uint16 GetResolutionWidth() const
		{
			return (uint16)this->cfgFile.GetIntValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_RESOLUTIONWIDTH);
		}

		inline bool IsWindowed() const
		{
			return this->cfgFile.GetBoolValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_WINDOWED);
		}

		inline void SetResolution(uint16 width, uint16 height)
		{
			this->cfgFile.SetValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_RESOLUTIONHEIGHT, (int64)height);
			this->cfgFile.SetValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_RESOLUTIONWIDTH, (int64)width);
		}

		inline void SetWindowed(bool windowed)
		{
			this->cfgFile.SetValue(CONFIG_SECTION_VIDEO, CONFIG_KEY_VIDEO_WINDOWED, windowed);
		}
	};
}