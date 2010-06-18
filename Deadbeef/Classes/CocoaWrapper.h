/*
 *  CocoaWrapper.h
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <string>

class CocoaWrapper
{
public:
	static const std::string PathForResource(const std::string &name, const std::string &type);

private:
	CocoaWrapper();
	~CocoaWrapper();
};