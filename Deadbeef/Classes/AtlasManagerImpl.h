//
//  GGAtlasManager.h
//  OpenGLEngine
//
//  Created by Joshua Weinberg on 9/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Texture.h"
#import <map>

class Texture;

typedef std::map<std::string, Deadbeef::TextureWeakRef> TextureMap;


@interface AtlasManagerImpl : NSObject {
	
	NSMutableDictionary* atlasInformation;
	NSMutableDictionary* textures;
	NSMutableDictionary* animations;
	NSMutableDictionary* fonts;
	NSMutableArray* atlasFiles;
	
	NSString* fontName;
	NSMutableDictionary* fontChars;
	NSString* animationName;
	NSMutableArray* animationFrames;
	TextureMap *map;
}

- (void) parseAtlasXML:(NSString*)xmlPath;
- (BOOL) texture:(Deadbeef::TextureRef&)aTexture andSourceRect:(CGRect&)aRect forSpriteNamed:(NSString*) name;
- (NSArray*) framesForAnimationNamed:(NSString*)aAnimation;
@end
