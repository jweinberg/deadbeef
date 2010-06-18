//
//  ES2Renderer.m
//  Deadbeef
//
//  Created by Joshua Weinberg on 4/15/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ES2Renderer.h"
#import "CocoaWrapper.h"
#import "Texture.h"
#import "Vect2.h"
#import "Matrix.h"
#import "GameObject.h"
#import "TouchManager.h"
#import "ParallaxSprite.h"
#import "Sprite.h"
#import <iostream>
#import "ShaderManager.h"
#import "SoundManager.h"
#import "Scene.h"

@implementation ES2Renderer

@synthesize start;
// Create an OpenGL ES 2.0 context
- (id)init
{
    if ((self = [super init]))
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

        if (!context || ![EAGLContext setCurrentContext:context])
        {
            [self release];
            return nil;
        }
		

		// Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
		
		glGenFramebuffers(1, &backingBuffer);
		glGenTextures(1, &backingTexture);
		
		glBindTexture(GL_TEXTURE_2D, backingTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		
		
		glBindFramebuffer(GL_FRAMEBUFFER, backingBuffer);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backingTexture, 0);
		//(GLuint name, float s, float t, float textureWidth, float textureHeight, float fullWidth, float fullHeight)
		screenQuad = new Deadbeef::Quad(Deadbeef::TextureRef(new Deadbeef::Texture(backingTexture, 1.0f, .75f, 1024, 768, 1024, 1024)));
																						   
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			NSLog(@"Dead!!");
		}
		
		//Create Othographic Projection Matrix
        orthoProjection = new Deadbeef::Matrix(Deadbeef::Matrix::MakeOrthoProjection(0, 32, 0, 24, 1, -1));
        //iPad Orientation Matrix
		//        orientation = new Deadbeef::Matrix(Deadbeef::Matrix::MakeRotationZ(-90));
        *orthoProjection = (*orthoProjection);
        //orthoProjection = orthoProjection * orientation;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//Initialize Scene
		//scene = new Deadbeef::Scene(); 
        
		Deadbeef::ParallaxSprite * ps = new Deadbeef::ParallaxSprite("SkySkape0:SkySkape00", 
																	 "SkySkape1:SkySkape01",
																	 "SkySkape2:SkySkape02");
		//Initialize Background
		background = Deadbeef::ParallaxSpriteRef(ps);

		//Deadbeef::Sprite* testie = new Deadbeef::Sprite("test:cow2");
		//testie->position = Deadbeef::Vect2(16, 12);
		
		//mainScene->AddChild(*testie);
		testSprite = new Deadbeef::Sprite("test:Origin_Icon");

		Deadbeef::SceneRef menuScene = Deadbeef::SceneRef(new Deadbeef::MenuScene(Deadbeef::Vect2(16.0f,-15.0f)));
		
		sceneController = new Deadbeef::SceneController(menuScene);
		
		menuScene->SetBackground(background);
		
		defaultSprite = new Deadbeef::Sprite("default:default");
		defaultSprite->position = Deadbeef::Vect2(16,12);

    }

   return self;
}

- (void)render
{
	NSDate * tempDate = [NSDate date];
	if (!start)
		self.start = [NSDate date];

	elapsedTime = [tempDate timeIntervalSinceDate:start];
    self.start = tempDate;
	// This application only creates a single context which is already set current at this point.
    // This call is redundant, but needed if dealing with multiple contexts.
    [EAGLContext setCurrentContext:context];
    
    // This application only creates a single default framebuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple framebuffers.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	
	sceneController->Update(elapsedTime);
	sceneController->Render(elapsedTime, *orthoProjection);
	if (defaultSprite)
	{
		defaultSprite->Render(elapsedTime, *orthoProjection);
		
		defaultSprite->alpha -= .3f * elapsedTime;
		if (defaultSprite->alpha < 0.0f)
		{
			delete defaultSprite;	
			defaultSprite = 0;
		}
	}
	
#ifdef Debug
	testSprite->Render(elapsedTime, *orthoProjection);
#endif
	
	//Reset the touch manager
	Deadbeef::TouchManager::GetInstance().ResetGestures();
    // This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{
    // Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }

    return YES;
}

- (void)dealloc
{
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }

    if (colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }

	if (backingBuffer)
	{
		glDeleteFramebuffers(1, &backingBuffer);
		backingBuffer = 0;
	}
	
	if (backingTexture)
	{
		glDeleteTextures(1, &backingTexture);
		backingTexture = 0;
	}
	
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];

    [context release];
    context = nil;

    [super dealloc];
}

@end
