/*
 *  GameObject.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <tr1/functional>
#include <tr1/memory>
#include <vector>  

using namespace std::tr1::placeholders;
#include "Matrix.h"

namespace Deadbeef
{	
	class GameObject;
	typedef std::tr1::shared_ptr<GameObject> GameObjectRef;
	typedef std::vector<GameObjectRef> ChildList;
	
    class GameObject
	{
        
        
        
    public: 
		GameObject();
		virtual ~GameObject();
	
		virtual void Update(float elapsedTime);
		virtual void Render(float elapsedTime, const Matrix& projection) const;
		virtual GameObjectRef FindChild(GameObject& child);
		virtual void AddChild(GameObjectRef ref);
		virtual void AddChild(GameObject& child);
        virtual void RemoveChild(GameObject& child);
		virtual GameObject& GetParent() const;
		virtual GameObject& GetRoot();		
		Matrix relativeTransform;
		int depth;
	protected:
        unsigned int m_ChildrenCount;
        ChildList m_Children;
        GameObject* parent;
    
	};

}
