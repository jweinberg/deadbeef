/*
 *  Singleton.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

template <class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}
};