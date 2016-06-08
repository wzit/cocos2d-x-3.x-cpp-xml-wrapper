/******************************************************************************/
/*
* Copyright 2014-2015 Vladimir Tolmachev
*
* Author: Vladimir Tolmachev
* Project: ml
* e-mail: tolm_vl@hotmail.com
* If you received the code is not the author, please contact me
*/
/******************************************************************************/
#ifndef __ml_LayerBlur__
#define __ml_LayerBlur__

#include "cocos2d.h"
#include "IntrusivePtr.h"

NS_CC_BEGIN;

#define DEFAULT_BLUR_METHODS \
		virtual bool isBlurActive() override { return !isRunning(); } \
		virtual void visit( Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags )override { if( getIsUseBlur() )LayerBlur::visitWithBlur( renderer, parentTransform, parentFlags ); else LayerExt::visit( renderer, parentTransform, parentFlags ); } \
		virtual void visitContent( Renderer * renderer, Mat4 const & parentTransform, uint32_t parentFlags ) override { LayerExt::visit( renderer, parentTransform, parentFlags ); }

class LayerBlur
{
public:
	LayerBlur();
	bool init();

	bool getIsUseBlur()const;
	void setIsUseBlur( bool var );
protected:
	virtual bool isBlurActive() = 0;
	virtual void visitContent( Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags ) = 0;
protected:
	void visitWithBlur( Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags );
private:
	IntrusivePtr< RenderTexture > _rendererBloor;
	bool _useBlur;
	size_t _blurCounter;
	size_t _blurSamples;
};

NS_CC_END
#endif //__ml_LayerBlur__