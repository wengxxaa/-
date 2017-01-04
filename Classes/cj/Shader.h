#ifndef SHADER_H_
#define SHADER_H_

#include"cocos2d.h"
using namespace cocos2d;

class Effect : public cocos2d::Ref
{
public:
	cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; }
	virtual void setTarget(Sprite *sprite) {}

protected:
	bool initGLProgramState(const std::string &fragmentFilename);
	Effect();
	virtual ~Effect();
	cocos2d::GLProgramState* _glprogramstate;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	std::string _fragSource;
	cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

// Blur
class EffectBlur : public Effect
{
public:
	CREATE_FUNC(EffectBlur);
	virtual void setTarget(Sprite *sprite) override;
	void setBlurRadius(float radius);
	void setBlurSampleNum(float num);

protected:
	bool init(float blurRadius = 10.0f, float sampleNum = 5.0f);

	float _blurRadius;
	float _blurSampleNum;
};

//shader
class ShaderEffect {
	//blur
public:
	static bool SpriteBlur(Sprite *sp);
};


#endif
