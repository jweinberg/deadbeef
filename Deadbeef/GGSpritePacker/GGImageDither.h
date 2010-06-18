//
//  GGImageDither.h
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 12/30/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface GGImageDither : NSObject {

}

+(void) ditherImageAtPath:(NSString*)inPath andWriteToPath:(NSString*)outPath;

@end
