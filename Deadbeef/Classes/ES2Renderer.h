//
//  ES2Renderer.h
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ESRenderer.h"

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "Sprite.h"
#import "ParallaxSprite.h"
#import "Matrix.h"
#import "Scene.h"
#import "GameScene.h"
#import "Quad.h"
#import "ParticleEmitter.h"
#import "MenuScene.h"
#import "SceneController.h"


@interface ES2Renderer : NSObject <ESRenderer>
{
@private
    EAGLContext *context;

    // The pixel dimensions of the CAEAGLLayer
    GLint backingWidth;
    GLint backingHeight;

    // The OpenGL ES names for the framebuffer and renderbuffer used to render to this view
    GLuint defaultFramebuffer, colorRenderbuffer, backingBuffer, backingTexture;

	Deadbeef::Quad * screenQuad;
	Deadbeef::Matrix *orthoProjection;
	Deadbeef::Matrix *orientation;
    Deadbeef::ParallaxSpriteRef background;
	Deadbeef::Sprite * s;
	Deadbeef::ParticleEmitter * tempEmitter;
	//Deadbeef::Scene *scene;
	
	Deadbeef::Sprite* defaultSprite;
	Deadbeef::Sprite* testSprite; 
	Deadbeef::Sprite* testSprite2; 

	Deadbeef::SceneController * sceneController;
	NSDate * start;
	NSTimeInterval elapsedTime;
}
@property (nonatomic, retain) NSDate* start;
- (void)render;
- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer;

@end

