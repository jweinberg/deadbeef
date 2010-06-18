//
//  DBRotateRecognizer.m
//  Deadbeef
//
//  Created by Joshua Weinberg on 5/6/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "DBRotateRecognizer.h"


@implementation DBRotateRecognizer

- (BOOL)canBePreventedByGestureRecognizer:(UIGestureRecognizer *)preventingGestureRecognizer;
{
	return NO;
}

- (BOOL)canPreventGestureRecognizer:(UIGestureRecognizer *)preventedGestureRecognizer;
{
	return NO;
}

@end
