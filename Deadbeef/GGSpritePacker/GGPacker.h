//
//  GGPacker.h
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 10/3/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface GGPacker : NSObject {
	NSString* outputDirectory;
	NSString* rootArtDirectory;
	
	
	NSMutableDictionary *animations;
	NSMutableDictionary *coordinateDictionary;
	NSMutableDictionary *fonts;
	
	NSArray* fontIdentifiers;
	BOOL pvrCompress;
	BOOL ggCompress;
	BOOL dither;
	NSString* extension;
	
}

-(id) initWithOutputDirectory:(NSString*)aOutputDirectory rootDirectory:(NSString*)aRootArtDirectory;
-(void)processDirectory;
-(void)writeAtlasXML;
-(void)writeXMLForCoordinateDictionary:(NSDictionary*) coordinates withName:(NSString*) dirName;

@property (readwrite) BOOL ggCompress;
@property (readwrite) BOOL dither;
@end
