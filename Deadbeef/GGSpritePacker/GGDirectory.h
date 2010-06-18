//
//  GGDirectory.h
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 9/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

extern float TEXTURE_SIZE;

extern NSString* const GGDirectoryCoordinates;
extern NSString* const GGDirectoryImage;

@interface GGImageNode : NSObject {
	GGImageNode* children[2];
    CGRect nodeRect;
	NSString* imageKey;
}

-(GGImageNode*) insertImage:(NSImage*) aImage withKey:(NSString*) key;

-(GGImageNode*) childAtIndex:(NSUInteger) idx;
-(CGRect)rect;

@property (nonatomic, retain) NSString* imageKey;
@end

@interface GGDirectory : NSObject {
	
}


+(NSDictionary*) coordinatesAndImageForPackedDirectory:(NSString*) folderPath fromRootDirectory:(NSString*) rootPath forceSquare:(BOOL)square;
@end
