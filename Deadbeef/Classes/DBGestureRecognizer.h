//
//  DBGestureRecognizer.h
//  Deadbeef
//
//  Created by Joshua Weinberg on 5/5/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface DBGestureRecognizer : UIGestureRecognizer {
	CGPoint startPoint;
	CGPoint endPoint;
	
	float prevAngle;
}

- (float)angle;
- (float)distance;

@end
