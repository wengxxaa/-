#include"shader.h"
#include "cjconfig.h"
//
// Effect
//

bool Effect::initGLProgramState(const std::string &fragmentFilename)
{
	auto fileUtiles = FileUtils::getInstance();
	auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
	auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
	auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_fragSource = fragSource;
#endif

	_glprogramstate = (glprogram == nullptr ? nullptr : GLProgramState::getOrCreateWithGLProgram(glprogram));
	CC_SAFE_RETAIN(_glprogramstate);

	return _glprogramstate != nullptr;
}

Effect::Effect()
	: _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	_backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
		[this](EventCustom*)
	{
		auto glProgram = _glprogramstate->getGLProgram();
		glProgram->reset();
		glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
		glProgram->link();
		glProgram->updateUniforms();
	}
	);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect()
{
	CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

//
//EffectBlur
//

void EffectBlur::setTarget(Sprite *sprite)
{
	if (_glprogramstate == nullptr)
		return;

	Size size = sprite->getTexture()->getContentSizeInPixels();
	_glprogramstate->setUniformVec2("resolution", size);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
	_glprogramstate->setUniformFloat("blurRadius", _blurRadius);
	_glprogramstate->setUniformFloat("sampleNum", _blurSampleNum);
#endif
}

bool EffectBlur::init(float blurRadius, float sampleNum)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT)
	initGLProgramState("Shaders/example_Blur.fsh");
#else
	initGLProgramState("Shaders/example_Blur_winrt.fsh");
#endif
	_blurRadius = blurRadius;
	_blurSampleNum = sampleNum;

	return true;
}

void EffectBlur::setBlurRadius(float radius)
{
	_blurRadius = radius;
}

void EffectBlur::setBlurSampleNum(float num)
{
	_blurSampleNum = num;
}


//
//shader
//
bool ShaderEffect::SpriteBlur(Sprite *sp)
{
	if (!sp)
	{
		cj::LogText("SpriteBlur: sprite is null");
		return false;
	}
	auto blur = EffectBlur::create();
	blur->setBlurRadius(3.0f);
	blur->setTarget(sp);
	sp->setGLProgramState(blur->getGLProgramState());
	return true;
}