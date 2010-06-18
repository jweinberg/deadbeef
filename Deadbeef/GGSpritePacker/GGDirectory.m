//
//  GGDirectory.m
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 9/13/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GGDirectory.h"

NSString* const GGDirectoryCoordinates = @"GGDirectoryCoordinates";
NSString* const GGDirectoryImage = @"GGDirectoryImage";


@implementation GGImageNode
@synthesize imageKey;


-(void) setRect: (CGRect) rect
{
	nodeRect = rect;
}

-(CGRect)rect
{
	return nodeRect;
}

-(NSString*)imageKey
{
	return imageKey;
}

-(GGImageNode*) childAtIndex:(NSUInteger) idx
{
	if( idx < 0 || idx > 1 )
		return nil;
	
	return children[idx];
}

-(GGImageNode*) insertImage:(NSImage*) aImage withKey:(NSString*) key
{
	if ( children[0] != nil )
	{
		GGImageNode* newNode = nil;
		//	(try inserting into first child)
		newNode = [children[0] insertImage:aImage withKey:key];
		if(newNode != NULL)
			return newNode;
		//(no room, insert into second)
		return [children[1] insertImage:aImage withKey:key];
	}
	
	//(if there's already a lightmap here, return)
	if(imageKey)
		return nil;
			
	NSImageRep* rep = [[aImage representations] objectAtIndex:0];
	CGSize imageSize = CGSizeMake([rep pixelsWide] , [rep pixelsHigh]);
	//(if we're too small, return)
	if (!CGRectContainsRect(CGRectMake(0, 0, nodeRect.size.width, nodeRect.size.height), 
							 CGRectMake(0, 0, imageSize.width, imageSize.height)))
		return nil;
				
	
	if( nodeRect.size.width - imageSize.width < 1 &&
	   nodeRect.size.height - imageSize.height < 1)
	{
		self.imageKey = key;
		return self;
	}
			
	
	
	// (otherwise, gotta split this node and create some kids)
	children[0] = [[GGImageNode alloc] init];
	children[1] = [[GGImageNode alloc] init];
	
	//  (decide which way to split)
	CGFloat dw = nodeRect.size.width - imageSize.width;
	CGFloat dh = nodeRect.size.height - imageSize.height;
	
	if (dw > dh)
	{
		[children[0] setRect:CGRectMake(nodeRect.origin.x, 
										nodeRect.origin.y, 
										imageSize.width, 
										nodeRect.size.height)];
		
		[children[1] setRect:CGRectMake(nodeRect.origin.x + imageSize.width + 1, 
										nodeRect.origin.y, 
										nodeRect.size.width - (imageSize.width + 1), 
										nodeRect.size.height)];
	}
	else
	{
		[children[0] setRect:CGRectMake(nodeRect.origin.x, 
										nodeRect.origin.y, 
										nodeRect.size.width,
										imageSize.height)];
		
		[children[1] setRect:CGRectMake(nodeRect.origin.x, 
										nodeRect.origin.y + imageSize.height + 1, 
										nodeRect.size.width,
										nodeRect.size.height - (imageSize.height + 1))];
	}
	//(insert into first child we created)
	return [children[0] insertImage:aImage withKey:key];
}

-(void) dealloc
{
	[children[0] release];
	[children[1] release];
	[super dealloc];
}

@end

@implementation GGDirectory

+(NSDictionary*) coordinatesAndImageForPackedDirectory:(NSString*) folderPath fromRootDirectory:(NSString*) rootPath forceSquare:(BOOL)square
{	
	
	//Dictionary of all the coordinate locations of each image on this sheet
	NSMutableDictionary* coordinateDictionary = [NSMutableDictionary dictionary];
	
	//Dictionary will hold all the NSImages that we create
	NSMutableDictionary* images = [NSMutableDictionary dictionary];
	
	//Get an enumerator for this directory, we want to skip everything other then what is just in this folder, no subs or hidden
	NSDirectoryEnumerator* spriteEnumerator = [[NSFileManager defaultManager] enumeratorAtURL:[NSURL URLWithString:[rootPath stringByAppendingPathComponent:folderPath]]
																			includingPropertiesForKeys:nil 
																							   options:NSDirectoryEnumerationSkipsHiddenFiles | 
																									   NSDirectoryEnumerationSkipsPackageDescendants | 
																									   NSDirectoryEnumerationSkipsSubdirectoryDescendants 
																							errorHandler:nil];

	
	for(NSURL* aPath in spriteEnumerator)
	{
		if ([[aPath pathExtension] isEqualToString:@"png"])
		{
			//Create an NSImage with the PNG
			NSImage* image = [[NSImage alloc] initWithContentsOfFile:[aPath path]];
			
			//Figure out the relative path based on the root
			NSMutableString* relativePath = [NSMutableString stringWithString:[aPath path]];
			[relativePath deleteCharactersInRange:NSMakeRange(0, [rootPath length]+1)];
			
			//Store this image into the dictionary
			[images setObject:image forKey:relativePath];
		}
	}
	
	
	GGImageNode* rootNode = nil;
	
	
	//GLfloat texture_size = 256.0f;
	
	CGSize textureSize = CGSizeMake(64.0f, 64.0f);
	
	BOOL packedAll = YES;
	BOOL expandedWidth = NO;
	BOOL expandedHeight = NO;
	
	NSArray* sortedImages = [[images allKeys] sortedArrayUsingComparator:
							 ^(id obj1, id obj2) { 
								 NSImage* img1 = [images objectForKey:obj1];
 								 NSImage* img2 = [images objectForKey:obj2];
								 return (NSComparisonResult)-(img1.size.width * img1.size.height - img2.size.width * img2.size.height);
							 }];
	do {
		packedAll = YES;
		[rootNode release];
		rootNode = [[GGImageNode alloc] init];
		
		[rootNode setRect:CGRectMake(0, 0, textureSize.width, textureSize.height)];
		for(NSString* imageKey in sortedImages)
		{
			if( ![rootNode insertImage:[images objectForKey:imageKey] withKey:imageKey] )
			{
				if( square )
				{
					textureSize.width *= 2.0f;
					textureSize.height *= 2.0f;
				}
				else {
					if( expandedWidth == NO && expandedHeight == NO )
					{
						textureSize.width *= 2.0f;
						expandedWidth = YES;
					}
					else if ( expandedWidth == YES && expandedHeight == NO )
					{
						textureSize.width /= 2.0f;
						textureSize.height *= 2.0f;
						expandedHeight = YES;
					}
					else {
						textureSize.width *= 2.0f;
						expandedWidth = NO;
						expandedHeight = NO;
					}
				}

				if( textureSize.height > TEXTURE_SIZE || textureSize.width > TEXTURE_SIZE  )
				{
					fprintf(stderr, "error: The texture size for %s was too large\n", [folderPath UTF8String]);
					//exit(1);
				}
				packedAll = NO;
				break;
			}
		}
	} while (!packedAll);

	NSBitmapImageRep* imageRep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																		 pixelsWide:textureSize.width
																		 pixelsHigh:textureSize.height
																	  bitsPerSample:8
																	samplesPerPixel:4
																		   hasAlpha:YES
																		   isPlanar:NO
																	 colorSpaceName:NSDeviceRGBColorSpace
																		bytesPerRow:textureSize.width * 4
																	   bitsPerPixel:32];
	
	NSGraphicsContext* context = [NSGraphicsContext graphicsContextWithBitmapImageRep:imageRep];

	[NSGraphicsContext setCurrentContext:context];
	[context setShouldAntialias:NO];
	[context setImageInterpolation:NSImageInterpolationNone];
	NSMutableArray *imageQueue = [NSMutableArray array];
	[imageQueue addObject:rootNode];
	do {
		GGImageNode* aNode = [imageQueue objectAtIndex:0];
		[imageQueue removeObjectAtIndex:0];
		
		if( [aNode childAtIndex:0] )
			[imageQueue addObject:[aNode childAtIndex:0]];
		if( [aNode childAtIndex:1] )
			[imageQueue addObject:[aNode childAtIndex:1]];
		
		NSImage* currentImage = [images objectForKey:[aNode imageKey]];
		
		if( !currentImage )
			continue;
	
		CGRect rect2 = [aNode rect];
		CGRect rect = [aNode rect];
		rect2.origin.y = textureSize.height - rect2.size.height - rect2.origin.y;

		//Draw our image as a CGImage into the large buffer
		[currentImage drawInRect:NSRectFromCGRect(rect2)
						fromRect:NSMakeRect(0, 0, currentImage.size.width, currentImage.size.height) 
					   operation:NSCompositeSourceOver 
						fraction:1.0f];
		
		//This is used to make an entry into the XML file

		rect.origin.x /= textureSize.width;
		rect.origin.y /= textureSize.height;
		rect.size.height /= textureSize.height;
		rect.size.width /= textureSize.width;
		
		[coordinateDictionary setObject:[NSValue valueWithRect:NSRectFromCGRect(rect)] forKey:[[aNode imageKey] stringByDeletingPathExtension]];
		
	} while ([imageQueue count]);
	
	
	NSImage* finalImage = [[[NSImage alloc] init] autorelease];
	[finalImage addRepresentation:imageRep];
	[imageRep release];
	
	
	return [NSDictionary dictionaryWithObjectsAndKeys:coordinateDictionary, GGDirectoryCoordinates, finalImage, GGDirectoryImage, nil];
}

@end
