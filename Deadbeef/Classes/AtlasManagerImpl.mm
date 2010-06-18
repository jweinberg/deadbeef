//
//  GGAtlasManager.m
//  OpenGLEngine
//
//  Created by Joshua Weinberg on 9/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "AtlasManagerImpl.h"
#import "Texture.h"
#import <map>
#import <iostream>
#import "CocoaWrapper.h"

@interface GGAtlasEntry : NSObject
{
	CGRect sourceRect;
	NSString *texturePath;
}

- (id) initWithRect:(CGRect) aRect path:(NSString*) aPath;

@property (nonatomic, readonly) NSString* texturePath;
@property (nonatomic, readonly) CGRect sourceRect;
@end

@implementation GGAtlasEntry

@synthesize texturePath;
@synthesize sourceRect;

- (id) initWithRect:(CGRect) aRect path:(NSString*) aPath
{
	if(self = [super init])
	{
		sourceRect = aRect;
		texturePath = [aPath retain];
	}
	return self;
}

- (void) dealloc
{
	[texturePath release];
	[super dealloc];
}

@end

@implementation AtlasManagerImpl

-(id) init
{
	if( self = [super init] )
	{
		atlasInformation = [[NSMutableDictionary alloc] init];
		textures = [[NSMutableDictionary alloc] init];
		animations = [[NSMutableDictionary alloc] init];
		fonts = [[NSMutableDictionary alloc] init];
		atlasFiles = [[NSMutableArray alloc] init];
		map = new TextureMap();
		NSString* xmlPath = [[NSBundle mainBundle] pathForResource:@"atlas" ofType:@"xml"];
		[self parseAtlasXML:xmlPath];
	}
	return self;
}

-(void) parseAtlasXML:(NSString*)xmlPath
{
	NSXMLParser* parser = [[NSXMLParser alloc] initWithData:[NSData dataWithContentsOfFile:xmlPath]];
	[parser setDelegate:self];
	[parser parse];
	[parser release];
	
}

-(void) dealloc
{
	[atlasInformation release];
	[textures release];
	[animations release];
	[super dealloc];
}

-(Deadbeef::TextureRef)textureNamed:(NSString*)name;
{
	TextureMap::iterator it = map->find([name UTF8String]);

    if (it != map->end() && (it->second).expired())
    {
        map->erase(it);
        it = map->end();
    }
    
	if ( it == map->end() )
	{
		//texture not loaded
		std::cerr << "Loading new texture" << std::endl;
		Deadbeef::TextureRef ref = Deadbeef::TextureRef(new Deadbeef::Texture(CocoaWrapper::PathForResource([name UTF8String], "png")));
		map->insert(std::pair<std::string, Deadbeef::TextureWeakRef>([name UTF8String],ref));
		return ref;
	}
	return Deadbeef::TextureRef(it->second);
}

-(BOOL) texture:(Deadbeef::TextureRef&)aTexture andSourceRect:(CGRect&)aRect forSpriteNamed:(NSString*) name
{
	GGAtlasEntry* entry = [atlasInformation objectForKey:name];

	if( !entry )
	{
		NSString* spriteName = [[[name componentsSeparatedByString:@":"] objectAtIndex:0] uppercaseString];
		if ([atlasFiles containsObject:[[NSString stringWithFormat:@"%@_atlas",spriteName] stringByAppendingPathExtension:@"xml"]])
		{
			NSString* xmlPath = [[NSBundle mainBundle] pathForResource:[NSString stringWithFormat:@"%@_atlas",spriteName] ofType:@"xml"];
			[self parseAtlasXML:xmlPath];
			entry = [atlasInformation objectForKey:name];
		}
	}
	if( entry )
	{
		NSString* texturePath = [[[entry texturePath] lastPathComponent] stringByDeletingPathExtension];
		aTexture = [self textureNamed:texturePath];
		aRect = [entry sourceRect];
		return YES;
	}
	assert(!"Sprite name not found.");
	return NO;
}

- (NSDictionary*) charactersForFont:(NSString*)aFont
{
	return [fonts objectForKey:aFont];
}

- (NSArray*) framesForAnimationNamed:(NSString*)aAnimation
{
	NSArray * frames = [animations objectForKey:aAnimation];
	if (!frames)
		frames = [NSArray arrayWithObject:aAnimation];
	
	return frames;
}

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName attributes:(NSDictionary *)attributeDict {
	
    if ( [elementName isEqualToString:@"sprite"]) {
		float x = [[attributeDict objectForKey:@"x"] floatValue];
		float y = [[attributeDict objectForKey:@"y"] floatValue];		
		float width = [[attributeDict objectForKey:@"width"] floatValue];		
		float height = [[attributeDict objectForKey:@"height"] floatValue];		
		CGRect sourceRect =  CGRectMake(x, y, width, height);
		NSString *atlasName = [attributeDict objectForKey:@"texture"];
		NSString *textureName = [attributeDict objectForKey:@"name"];
		
		GGAtlasEntry* entry = [[GGAtlasEntry alloc] initWithRect:sourceRect path:atlasName];
		[atlasInformation setObject:entry forKey:textureName];
		[entry release];
        return;
    }
	else if ( [elementName isEqualToString:@"atlas_file"] )
	{
		[atlasFiles addObject:[attributeDict objectForKey:@"name"]];
	}
	else if ( [elementName isEqualToString:@"animation"] ){
		animationName = [[attributeDict objectForKey:@"name"] retain];
		animationFrames = [[NSMutableArray alloc] init];
		return;
	}
	else if ( [elementName isEqualToString:@"frame"] ){
		//assert(animationName==nil);
		NSAssert(animationName, @"Animation name was nil");
		[animationFrames addObject:[attributeDict objectForKey:@"name"]];
		return;
	}
	else if ( [elementName isEqualToString:@"font"] ){
		fontName = [[attributeDict objectForKey:@"name"] retain];
		fontChars = [[NSMutableDictionary alloc] init];
		return;
	}
	else if ( [elementName isEqualToString:@"char"] ){
		//assert(fontName==nil);
		NSAssert(fontName, @"Font name was nil");
		[fontChars setObject:[attributeDict objectForKey:@"name"] forKey:[attributeDict objectForKey:@"id"]];
		return;
	}
}

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	if( [elementName isEqualToString:@"animation"] )
	{
		[animations setObject:animationFrames forKey:animationName];		
		[animationName release];
		animationName = nil;
		[animationFrames release];
		return;
	}
	else if( [elementName isEqualToString:@"font"] )
	{
		[fonts setObject:fontChars forKey:fontName];
		[fontName release];
		fontName = nil;
		[fontChars release];
		return;
	}
}


@end
