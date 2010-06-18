//
//  DBGestureRecognizer.m
//  Deadbeef
//
//  Created by Joshua Weinberg on 5/5/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "DBGestureRecognizer.h"
#import <UIKit/UIGestureRecognizerSubclass.h>
#import "Vect2.h"

@implementation DBGestureRecognizer

- (id)initWithTarget:(id)target action:(SEL)action;
{
	if ((self = [super initWithTarget:target action:action]))
	{
		prevAngle = CGFLOAT_MAX;
	}
	return self;
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    if ([touches count] != 1) {
        self.state = UIGestureRecognizerStateFailed;
        return;
    }
	startPoint = [[touches anyObject] locationInView:self.view];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesMoved:touches withEvent:event];
    if (self.state == UIGestureRecognizerStateFailed) return;
    CGPoint nowPoint = [[touches anyObject] locationInView:self.view];
    CGPoint prevPoint = [[touches anyObject] previousLocationInView:self.view];

	float angle = atan2(nowPoint.y - prevPoint.y, nowPoint.x - prevPoint.x);
	


	//Direction
	if (prevAngle == CGFLOAT_MAX)
		prevAngle = angle;

	float diffAngle = atan2(sin(prevAngle - angle), cos(prevAngle - angle)); 
	if (fabs(diffAngle) < M_PI_4)
	{
		prevAngle = angle;
	}
	else
	{
		self.state = UIGestureRecognizerStateFailed;
	}
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesEnded:touches withEvent:event];
    if ((self.state == UIGestureRecognizerStatePossible) && prevAngle != CGFLOAT_MAX) {
        self.state = UIGestureRecognizerStateRecognized;
		endPoint = [[touches anyObject] locationInView:self.view];
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesCancelled:touches withEvent:event];
	startPoint = CGPointZero;
    self.state = UIGestureRecognizerStateFailed;
}

- (BOOL)canBePreventedByGestureRecognizer:(UIGestureRecognizer *)preventingGestureRecognizer;
{
	return NO;
}

- (BOOL)canPreventGestureRecognizer:(UIGestureRecognizer *)preventedGestureRecognizer;
{
	return NO;
}
- (void)reset;
{
	[super reset];
	prevAngle = CGFLOAT_MAX;
}

- (float)angle;
{
	return fmod(prevAngle + M_PI * 2.0f,  M_PI * 2.0f);
}

- (float)distance;
{
	return fabs(Deadbeef::Vect2(endPoint.x, endPoint.y).distance(Deadbeef::Vect2(startPoint.x, startPoint.y)));
}

@end
