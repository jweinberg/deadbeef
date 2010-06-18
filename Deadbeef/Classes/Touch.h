/*
 *  Touch.h
 *  Deadbeef
 *
 *  Created by Josh Weinberg on 4/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <tr1/memory>

namespace Deadbeef
{
	typedef enum
    {
        kTouchBegan,
		kTouchHold,
        kTouchMoved,
        kTouchEnded,
        kTouchCancelled,
    }kTouchState;
    
	
    class Touch
    {
    public:
        Touch(const float x, const float y);
        virtual ~Touch();
        void UpdateTouch(const float newX, const float newY, kTouchState newState);
    
    //private:
        float prevX;
        float prevY;
        float x;
        float y;
		kTouchState state;
        
    };

    typedef std::tr1::shared_ptr<Touch> TouchRef;
}