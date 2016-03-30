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

#ifndef __Panel_h__
#define __Panel_h__
#include "cocos2d.h"
#include "ml/macroses.h"
NS_CC_BEGIN

/*
name frames:
1 2 3
4 5 6
7 8 9
*/

class Panel : public Node
{
	DECLARE_BUILDER( Panel );
	CREATE_0(Panel);
	CREATE_1(Panel);
	CREATE_2(Panel);
	virtual bool init()override;
	bool init( const std::string& folder );
	bool init( const std::string& folder, const Size& size );
public:
	void setFolder( const std::string& folder );
	virtual void setContentSize( const Size& size ) override;
protected:
	void destroyFrames();
	void createFrames();
private:
	std::vector<SpritePointer> _frames;
	std::string _folder;
};

NS_CC_END
#endif // #ifndef Panel