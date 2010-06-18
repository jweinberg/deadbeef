//
//  DeadbeefAppDelegate.m
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "DeadbeefAppDelegate.h"
#import "EAGLView.h"
#import "EAGLViewController.h"

@implementation DeadbeefAppDelegate

@synthesize window;
@synthesize glViewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions   
{
	[self.window addSubview:glViewController.view];
	[self.window makeKeyAndVisible];
    [(EAGLView*)glViewController.view startAnimation];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [(EAGLView*)glViewController.view stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [(EAGLView*)glViewController.view startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [(EAGLView*)glViewController.view stopAnimation];
}

- (void)dealloc
{
    [window release];
    [glViewController release];

    [super dealloc];
}

@end
