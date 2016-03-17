/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: Defense of Greece
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/

#ifndef __SmartScene_h__
#define __SmartScene_h__
#include "cocos2d.h"
#include "ml/macroses.h"
NS_CC_BEGIN

class SmartScene : public Scene
{
	DECLARE_BUILDER( SmartScene );
	CREATE_1(SmartScene);
	bool init( LayerPointer mainlayer );
public:
	LayerPointer getMainLayer() { return _mainlayer; }
	void resetMainLayer( Layer* mainlayer );
	void pushLayer( Layer* layer, bool exitPrevios = true, bool waitShadow = false );
	void on_layerClosed( Layer* layer );

	virtual void onEnter()override;
	virtual void onExit()override;

	virtual void onLayerPushed( LayerPointer layer ) {}
	virtual void onLayerPoped( LayerPointer layer ) {}
	void useShadow( bool var ) { _useShadow = var; }
protected:
	void pushShadow();
	void popShadow();
	void onShadowAppearanceEnded();
private:
	LayerPointer _mainlayer;
	bool _nowBlockedTopLayer;
	bool _autoPushShadow;
	bool _useShadow;
	bool _dispachStack;
	struct
	{
		LayerPointer layer;
		LayerPointer dummy;
		bool exitPrevios;
	}_tempLayerData;

	std::deque< std::deque<LayerPointer> > _stack;
	std::deque<SpritePointer> _shadows;
	CC_SYNTHESIZE( std::string, _shadowResource, ShadowResource );
	CC_SYNTHESIZE( float, _durationShadowFade, DurationShadowFade );
	CC_SYNTHESIZE( float, _opacityShadowFade, OpacityShadowFade );
	CC_SYNTHESIZE( Color3B, _colorShadow, ColorShadow );
};

NS_CC_END
#endif // #ifndef SmartScene