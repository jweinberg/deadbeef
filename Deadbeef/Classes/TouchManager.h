/*
 *  TouchManager.h
 *  Deadbeef
 *
 *  Created by Josh Weinberg on 4/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "Singleton.h"
#import <vector>
#import "Touch.h"
#import "Vect2.h"

namespace Deadbeef
{
    typedef std::vector<TouchRef> TouchList;
    
	class Swipe
	{
	public:
		Swipe();
		Swipe(const float direction, const float distance);
		virtual ~Swipe();
		const bool isValid() const { return valid; }
		const float getDirection() const { return direction; }
		const float getDistance() const { return distance; }
		
	private:
		float direction;
		float distance;
		bool valid;
	};
	
	class Pinch
	{
	public:
		Pinch();
		Pinch(const float scale, const float velocity, const float x, const float y);
		virtual ~Pinch();
		const bool isValid() const { return valid; }
		const float getScale() const { return scale; }
	private:
		float scale;
		float velocity;
		float x;
		float y;
		bool valid;
	};
	
	class Rotation
	{
	public:
		Rotation();
		Rotation(const float rotation, const float velocity, const float x, const float y);
		virtual ~Rotation();
		const bool isValid() const { return valid; }
		const float getRotation() const { return rotation; }
	private:
		float rotation;
		float velocity;
		float x;
		float y;
		bool valid;
	};
	
    class TouchManager : public Singleton<TouchManager>
    {
    public:
        TouchManager();
        virtual ~TouchManager();
        
        void UpdateState(const kTouchState state, const float prevX, const float prevY, const float x, const float y);
        void UpdateRotation(const float rotation, const float velocity, const float x, const float y);
		void UpdatePinch(const float scale, const float velocity, const float x, const float y);
        void UpdateSwipe(const float distance, const float direction);
		const TouchList& Touches() const { return touches; }
		const Rotation& CurrentRotation() const { return rotation; }
		const Pinch& CurrentPinch() const { return pinch; }
		const Swipe& CurrentSwipe() const { return swipe; }
		void ResetGestures();
    private:
        TouchList touches;
		Rotation rotation;
		Pinch pinch;
		Swipe swipe;
		TouchList::iterator FindTouch(const Vect2 &point);
    };
}