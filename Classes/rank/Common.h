#ifndef COMMON_H_
#define COMMON_H_

#include"iostream"
#include "cocos2d.h"
using namespace cocos2d;

#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#define  EXTRAL_CLASS_JAVA	"com/google/extra/Extra"
#endif

class Common
{
public:
	static std::string UrlEncode(const std::string& szToEncode);

	static std::string UrlDecode(const std::string& szToDecode);

	static void split(const std::string& str, std::string split, std::vector<std::string> &vec);

	//static Texture2D* getTextureByUrl(const std::string& url);
};

#define URL_ENCODE(str) Common::UrlEncode(str);
#define URL_DECODE(str) Common::UrlDecode(str);

#endif
