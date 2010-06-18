//
//  DeadbeefAppDelegate.h
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLViewController;

@interface DeadbeefAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLViewController *glViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLViewController *glViewController;

@end

