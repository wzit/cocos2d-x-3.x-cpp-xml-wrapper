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

#ifndef __SplashScene_h__
#define __SplashScene_h__
#include "cocos2d.h"
#include "ml/macroses.h"
#include "ml/SmartScene.h"
NS_CC_BEGIN





class SplashScene : public SmartScene
{
	DECLARE_BUILDER( SplashScene );
	CREATE_2( SplashScene );
	bool init( const std::string& folder, const std::function<void()>& onFinish );
public:
protected:
	virtual void onLayerPushed( LayerPointer layer )override;
	virtual void onLayerPoped( LayerPointer layer )override;
private:
	std::list< LayerPointer > _splashes;
	std::function<void()> _onFinish;
};




NS_CC_END
#endif // #ifndef SplashScene