//
//  GGImageCompress.h
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 11/22/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

typedef struct
{
	uint16_t width;
	uint16_t height;
	uint8_t compressed;
	uint16_t data_start;
	uint32_t data_length;
	uint16_t alpha_start;
	uint32_t alpha_length;
}GGImageHeader;

@interface GGImageCompress : NSObject {

}
+(void) compressImageAtPath:(NSString*)inPath andWriteToPath:(NSString*)outPath;
@end
