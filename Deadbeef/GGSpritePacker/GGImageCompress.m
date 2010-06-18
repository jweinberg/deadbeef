//
//  GGImageCompress.m
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 11/22/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//


//GGImage is a 565 image with an 8 bit alpha channel right after

#import "GGImageCompress.h"
#import "NSData+Compression.h"
@implementation GGImageCompress

+(void) compressImageAtPath:(NSString*)inPath andWriteToPath:(NSString*)outPath
{
	GGImageHeader header;
	NSImage* inImage = [[NSImage alloc] initWithContentsOfFile:inPath];
	
	NSBitmapImageRep* imageRep = [[inImage representations] objectAtIndex:0];
	
	uint16_t *rawData = malloc(sizeof(uint16_t) * [imageRep pixelsHigh] * [imageRep pixelsWide]);
	uint8_t *alphaData = malloc(sizeof(uint8_t) * [imageRep pixelsHigh] * [imageRep pixelsWide]);
	
	// DITHERIZE!@!
	// Use the Floyd-Steinberg error diffusion matrix:
	// | 0  0  0 |            | 0.0000 0.0000 0.0000 |
	// | 0  0  7 | * (1/16) = | 0.0000 0.0000 0.4375 | = M
	// | 3  5  1 |            | 0.1875 0.3125 0.0625 |
	//
	// The error between the 5/6-bit value and the original 8-bit value is calculated for
	// each color channel in the input image.  This error is mulitplied by M in order to
	// find the diffusion values to pass on to neighboring pixels.  This process repeats
	// left to right, top to bottom.  Note that the pixel in question is m11.
	//
	// Also note that the divide by 16 is equivalent to >> 4. Kickin'!
	
	unsigned int *lineAdd = calloc([imageRep pixelsWide] * 2, sizeof(unsigned int) * 3);
	unsigned int nextPixelAdd[3] = {0, 0, 0};
	unsigned int *thisLineAdd = lineAdd;
	unsigned int *nextLineAdd = thisLineAdd + 3 * [imageRep pixelsWide];
	
	for (int y = 0; y < [imageRep pixelsHigh]; ++y)
	{
		for (int x = 0; x < [imageRep pixelsWide]; ++x)
		{
			NSColor* color = [imageRep colorAtX:x y:y];
			CGFloat floatRed = [color redComponent];
			CGFloat floatGreen = [color greenComponent];
			CGFloat floatBlue = [color blueComponent];
			CGFloat floatAlpha = [color alphaComponent];
			
			uint16_t rgba[4];
			
			rgba[0] = floatRed * (float)0xFF;
			rgba[1] = floatGreen * (float)0xFF;
			rgba[2] = floatBlue * (float)0xFF;
			rgba[3] = floatAlpha * (float)0xFF;
			
			rgba[0] *= floatAlpha;
			rgba[1] *= floatAlpha;
			rgba[2] *= floatAlpha;
			
			uint32_t in_pixel = rgba[0] | (rgba[1] << 8) | (rgba[2] << 16) | (rgba[3] << 24);
			// Add error contributions from previous pixel/row				
			rgba[0] += thisLineAdd[(x * 3) + 0] + nextPixelAdd[0];
			rgba[1] += thisLineAdd[(x * 3) + 1] + nextPixelAdd[1];
			rgba[2] += thisLineAdd[(x * 3) + 2] + nextPixelAdd[2];
			
			
			if (rgba[0] > 0xFF) { rgba[0] = 0xF8; }
			if (rgba[1] > 0xFF) { rgba[1] = 0xFC; }
			if (rgba[2] > 0xFF) { rgba[2] = 0xF8; }
			
			uint16_t out_pixel = ((rgba[2] & 0xF8) >> 3) | (((rgba[1] & 0xFC) >> 2) << 5) | (((rgba[0] & 0xF8) >> 3) << 11);
		
			rawData[y * [imageRep pixelsWide] + x] = out_pixel;
			alphaData[y * [imageRep pixelsWide] + x] = rgba[3];
		
			unsigned int error[3] = { rgba[0] - (rgba[0] & 0xF8), rgba[1] - (rgba[1] & 0xFC), rgba[2] - (rgba[2] & 0xF8) };
			
			nextPixelAdd[0] = (7 * error[0]) >> 4;
			nextPixelAdd[1] = (7 * error[1]) >> 4;
			nextPixelAdd[2] = (7 * error[2]) >> 4;
			
			if (x > 0)
			{
				nextLineAdd[(x - 1) * 3 + 0] = (3 * error[0]) >> 4;
				nextLineAdd[(x - 1) * 3 + 1] = (3 * error[1]) >> 4;
				nextLineAdd[(x - 1) * 3 + 2] = (3 * error[2]) >> 4;
			}
			
			nextLineAdd[x * 3 + 0] += (5 * error[0]) >> 4;
			nextLineAdd[x * 3 + 1] += (5 * error[1]) >> 4;
			nextLineAdd[x * 3 + 2] += (5 * error[2]) >> 4;
			
			if (x < [imageRep pixelsWide] - 1)
			{
				nextLineAdd[(x + 1) * 3 + 0] += (1 * error[0]) >> 4;
				nextLineAdd[(x + 1) * 3 + 1] += (1 * error[1]) >> 4;
				nextLineAdd[(x + 1) * 3 + 2] += (1 * error[2]) >> 4;
			}
		}
		
		memset(thisLineAdd, 0, sizeof(unsigned int) * 3 * [imageRep pixelsWide]);
		
		unsigned int *tmp = thisLineAdd;
		thisLineAdd = nextLineAdd;
		nextLineAdd = tmp;
	}

	header.width = [imageRep pixelsWide];
	header.height = [imageRep pixelsHigh];
	header.data_start = sizeof(GGImageHeader);
	header.data_length = sizeof(uint16_t) * [imageRep pixelsWide] * [imageRep pixelsHigh];
	header.alpha_start = header.data_start + header.data_length;
	header.alpha_length = sizeof(uint8_t) * [imageRep pixelsWide] * [imageRep pixelsHigh];
	header.compressed = YES;
	NSMutableData *finalData = [NSMutableData data];
	[finalData appendBytes:&header length:sizeof(GGImageHeader)];
	
	NSData* imageData = [NSData dataWithBytesNoCopy:rawData 
											 length:sizeof(uint16_t) * [imageRep pixelsWide] * [imageRep pixelsHigh] 
									   freeWhenDone:NO];
	
	NSMutableData* data = [NSMutableData dataWithBytesNoCopy:rawData 
											   length:sizeof(uint16_t) * [imageRep pixelsWide] * [imageRep pixelsHigh] 
										 freeWhenDone:NO];	
	[data appendBytes:alphaData length:sizeof(uint8_t) * [imageRep pixelsWide] * [imageRep pixelsHigh]];
	
	NSData* compressedData = [data gzipDeflate];
	[finalData appendData:compressedData];
	free(rawData);
	free(alphaData);
	
	[finalData writeToFile:outPath atomically:YES];
}

@end
