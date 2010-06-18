//
//  EAGLView.m
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "EAGLView.h"

#import "ES2Renderer.h"
#import "TouchManager.h"

@implementation EAGLView

@synthesize animating;
@dynamic animationFrameInterval;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:
- (id)initWithCoder:(NSCoder*)coder
{    
    if ((self = [super initWithCoder:coder]))
    {
        // Get the layer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

        renderer = [[ES2Renderer alloc] init];
        [self setMultipleTouchEnabled:YES];
        animating = FALSE;
        displayLinkSupported = FALSE;
        animationFrameInterval = 1;
        displayLink = nil;
        animationTimer = nil;
		gesture = [[DBGestureRecognizer alloc] initWithTarget:self action:@selector(recognizedSwipe:)];
		[gesture setCancelsTouchesInView:NO];
		[gesture setDelaysTouchesBegan:NO];
		[gesture setDelaysTouchesEnded:NO];
		pinch = [[DBPinchRecognizer alloc] initWithTarget:self action:@selector(recognizedPinch:)];
		[pinch setCancelsTouchesInView:NO];
		[pinch setDelaysTouchesBegan:NO];
		[pinch setDelaysTouchesEnded:NO];
		rotate = [[DBRotateRecognizer alloc] initWithTarget:self action:@selector(recognizedRotate:)];
		[rotate setCancelsTouchesInView:NO];
		[rotate setDelaysTouchesBegan:NO];
		[rotate setDelaysTouchesEnded:NO];
		
		
		[self addGestureRecognizer:gesture];
		[self addGestureRecognizer:pinch];
		[self addGestureRecognizer:rotate];

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString *reqSysVer = @"3.1";
        NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            displayLinkSupported = TRUE;
    }

    return self;
}

- (void)recognizedPinch:(DBPinchRecognizer*)recognizer;
{
	CGPoint location = [recognizer locationInView:self];
	Deadbeef::TouchManager::GetInstance().UpdatePinch([recognizer scale], [recognizer velocity], location.x, location.y);
	//NSLog(@"Pinchy! %f, %f, (%f,%f)", [recognizer velocity], [recognizer scale], location.x, location.y);
}

- (void)recognizedSwipe:(DBGestureRecognizer*)recognizer;
{
	Deadbeef::TouchManager::GetInstance().UpdateSwipe([recognizer angle]-M_PI_2, [recognizer distance]);

}

- (void)recognizedRotate:(DBRotateRecognizer*)recognizer;
{
	CGPoint location = [recognizer locationInView:self];
	Deadbeef::TouchManager::GetInstance().UpdateRotation([recognizer rotation], [recognizer velocity], location.x, location.y);
}

- (void)drawView:(id)sender
{
    [renderer render];
}

- (void)layoutSubviews
{
    [renderer resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        animationFrameInterval = frameInterval;

        if (animating)
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation
{
    if (!animating)
    {
        if (displayLinkSupported)
        {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [displayLink setFrameInterval:animationFrameInterval];
            [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        }
        else
            animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * animationFrameInterval) target:self selector:@selector(drawView:) userInfo:nil repeats:TRUE];

        animating = TRUE;
    }
}

- (void)stopAnimation
{
    if (animating)
    {
        if (displayLinkSupported)
        {
            [displayLink invalidate];
            displayLink = nil;
        }
        else
        {
            [animationTimer invalidate];
            animationTimer = nil;
        }

        animating = FALSE;
    }
}

- (void)dealloc
{
    [renderer release];

    [super dealloc];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
{
    Deadbeef::TouchManager& manager = Deadbeef::TouchManager::GetInstance();
    for (UITouch *touch in touches)
    {
        const CGPoint point = [touch locationInView:self];
        const CGPoint prevPoint = [touch previousLocationInView:self];
        
        manager.UpdateState(Deadbeef::kTouchBegan,prevPoint.x, prevPoint.y, point.x, point.y);
    }
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
{
    Deadbeef::TouchManager& manager = Deadbeef::TouchManager::GetInstance();
    for (UITouch *touch in touches)
    {
        const CGPoint point = [touch locationInView:self];
        const CGPoint prevPoint = [touch previousLocationInView:self];
        
        manager.UpdateState(Deadbeef::kTouchMoved,prevPoint.x, prevPoint.y, point.x, point.y);
    }
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
{
    Deadbeef::TouchManager& manager = Deadbeef::TouchManager::GetInstance();
    for (UITouch *touch in touches)
    {
        const CGPoint point = [touch locationInView:self];
        const CGPoint prevPoint = [touch previousLocationInView:self];
        
        manager.UpdateState(Deadbeef::kTouchEnded,prevPoint.x, prevPoint.y, point.x, point.y);
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;
{
    Deadbeef::TouchManager& manager = Deadbeef::TouchManager::GetInstance();
    for (UITouch *touch in touches)
    {
        const CGPoint point = [touch locationInView:self];
        const CGPoint prevPoint = [touch previousLocationInView:self];
        
        manager.UpdateState(Deadbeef::kTouchCancelled,prevPoint.x, prevPoint.y, point.x, point.y);
    }
}

@end
