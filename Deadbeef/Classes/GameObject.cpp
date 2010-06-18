/*
 *  GameObject.cpp
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "GameObject.h"
#include "Sprite.h"
#include <iostream>
#include <algorithm>

namespace Deadbeef
{
	bool CompareDepth (GameObjectRef a,GameObjectRef b)
	{
		return (a->depth < b->depth);
	}
    
	GameObject::GameObject() : m_ChildrenCount(0), parent(NULL), depth(5)
	{
    
	}

	GameObject::~GameObject()
	{
	}

	void GameObject::Update(float elapsedTime)
	{
        //Only update children
		for (int i = 0; i < m_ChildrenCount; ++i) 
        {
            m_Children[i]->Update(elapsedTime);
        }
	}
	
	void GameObject::Render(float elapsedTime, const Matrix& projection) const
	{
        //Only render children
        for (int i = 0; i < m_ChildrenCount; ++i) 
        {
            m_Children[i]->Render(elapsedTime, projection);
        }
	}
	
	void GameObject::AddChild(GameObject& child)
	{
        bool found = false;
        
        for (int i = 0; i < m_ChildrenCount; ++i) 
        {
            if((m_Children[i].get()) == &child)
            {
                found = true;
            }
        }
        
        if(!found)
        {
            ++m_ChildrenCount;
            child.parent = this;    //If this becomes shares point a "new" might be necisary
            m_Children.push_back(GameObjectRef(&child));
			stable_sort(m_Children.begin(), m_Children.end(), CompareDepth);
        }
	}
	
	void GameObject::AddChild(GameObjectRef ref)
	{
		++m_ChildrenCount;
		ref->parent = this;
		m_Children.push_back(ref);
		stable_sort(m_Children.begin(), m_Children.end(), CompareDepth);
	}
	
	GameObjectRef GameObject::FindChild(GameObject& child)
	{
		for (int i = 0; i < m_ChildrenCount; ++i) 
        {
            if ((m_Children[i].get()) == &child) 
            {
                return m_Children[i];
            }
        }
		return GameObjectRef();
	}
	
	void GameObject::RemoveChild(GameObject& child)
	{

        
        //Find child in vector
        for (int i = 0; i < m_ChildrenCount; ++i) 
        {
            if ((m_Children[i].get()) == &child) 
            {
                --m_ChildrenCount;
                m_Children.erase(m_Children.begin() + i);
            }
        }
		stable_sort(m_Children.begin(), m_Children.end(), CompareDepth);
	}
	
	GameObject& GameObject::GetParent()const
	{
		return *this->parent;
	}
	
	GameObject& GameObject::GetRoot() 
	{
		GameObject* node;
		for (node = this; node->parent; node = node->parent);
		return *node;
	}
	
}