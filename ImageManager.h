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

#ifndef __TowerDefence__ImageManager__
#define __TowerDefence__ImageManager__
#include "cocos2d.h"
#include "Singlton.h"
#include "IntrusivePtr.h"
#include <deque>

NS_CC_BEGIN;

/*
load resources 
textures and *.plist atlases
*/

class ImageManager : public Singlton<ImageManager>
{
	friend class Singlton<ImageManager>;
private:
	ImageManager();
	~ImageManager();
	ImageManager( const ImageManager& );
	const ImageManager& operator = ( const ImageManager& );
	virtual void onCreate()override;
public:
	void push();
	void pop();
	void load_plist( const std::string & path, const std::string & name );
	void unload_plist( const std::string & name );
	Texture2D * textureForPLIST( const std::string & path );
	SpriteFrame* spriteFrame( const std::string & frame );
	static Texture2D * texture( const std::string & texture );
	static Sprite * sprite( const std::string & frameOrTexture );
private:
	struct Resources
	{
		std::map< std::string, IntrusivePtr<SpriteFrame> > frames;
		std::map< std::string, IntrusivePtr<Texture2D> > textures;
		std::map< std::string, std::string > atlases;
	};

	std::deque<Resources> _resources;
};

NS_CC_END;
#endif /* defined(__TowerDefence__ImageManager__) */
