/*
 *  CocoaWrapper.mm
 *  Deadbeef
 *
 *  Created by Joshua Weinberg on 4/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "CocoaWrapper.h"
#import <Foundation/Foundation.h>

CocoaWrapper::CocoaWrapper()
{
}

CocoaWrapper::~CocoaWrapper()
{
}

const std::string CocoaWrapper::PathForResource(const std::string &name, const std::string &type)
{
	return std::string([[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:name.c_str()]
														ofType:[NSString stringWithUTF8String:type.c_str()]] UTF8String]);
}