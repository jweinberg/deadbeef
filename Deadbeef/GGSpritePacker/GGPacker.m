//
//  GGPacker.m
//  GGSpritePacker
//
//  Created by Joshua Weinberg on 10/3/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GGDirectory.h"
#import "GGPacker.h"
#import "GGImageCompress.h"
#import "GGImageDither.h"

@implementation GGPacker

@synthesize ggCompress, dither;

-(id) initWithOutputDirectory:(NSString*)aOutputDirectory rootDirectory:(NSString*)aRootArtDirectory
{
	if( self = [super init] )
	{
		outputDirectory = [aOutputDirectory retain];
		rootArtDirectory = [aRootArtDirectory retain];
		
		
		
		animations = [[NSMutableDictionary alloc] init];
		coordinateDictionary = [[NSMutableDictionary alloc] init];
		fonts = [[NSMutableDictionary alloc] init];
		
		
		
		fontIdentifiers = [[NSArray alloc] initWithObjects:								
									@"EXCLAMATION",
									@"DOUBLEQUOTE",
									@"POUND",
									@"DOLLAR",
									@"PERCENT",
									@"AND",
									@"SINGLEQUOTE",
									@"LEFTPAREN",
									@"RIGHTPAREN",
									@"ASTRISK",
									@"PLUS",
									@"COMMA",
									@"MINUS",
									@"PERIOD",
									@"FORWARDSLASH",
									@"0",
									@"1",
									@"2",
									@"3",
									@"4",
									@"5",
									@"6",
									@"7",
									@"8",
									@"9",
									@"COLON",
									@"SEMICOLON",
									@"LESSTHAN",
									@"EQUALS",
									@"GREATERTHAN",
									@"QUESTION",
									@"AT",
									@"A",
									@"B",
									@"C",
									@"D",
									@"E",
									@"F",
									@"G",
									@"H",
									@"I",
									@"J",
									@"K",
									@"L",
									@"M",
									@"N",
									@"O",
									@"P",
									@"Q",
									@"R",
									@"S",
									@"T",
									@"U",
									@"V",
									@"W",
									@"X",
									@"Y",
									@"Z",
									@"LEFTBRACE",
									@"BACKSLASH",
									@"RIGHTBRACE",
									@"CARET",
									@"UNDERSCORE",
									@"GRAVE",
									@"AA",
									@"BB",
									@"CC",
									@"DD",
									@"EE",
									@"FF",
									@"GG",
									@"HH",
									@"II",
									@"JJ",
									@"KK",
									@"LL",
									@"MM",
									@"NN",
									@"OO",
									@"PP",
									@"QQ",
									@"RR",
									@"SS",
									@"TT",
									@"UU",
									@"VV",
									@"WW",
									@"XX",
									@"YY",
									@"ZZ",
									@"LEFTCURLY",
									@"BAR",
									@"RIGHTCURLY",
									@"TILDE",
									nil];
		
		pvrCompress = NO;
		extension = @"png";
	}
	return self;
}

-(void) dealloc
{
	[outputDirectory release];
	[rootArtDirectory release];
	
	[animations release];
	[coordinateDictionary release];
	[fonts release];
	
	[fontIdentifiers release];
	
	[super dealloc];
}


-(void)processDirectory
{
	//Only check toplevel dirs, maybe check subdirs at some point in the future
	NSDirectoryEnumerator *directories = [[NSFileManager defaultManager] enumeratorAtURL:[NSURL URLWithString:rootArtDirectory]
															  includingPropertiesForKeys:nil 
																				 options:NSDirectoryEnumerationSkipsHiddenFiles | 
										  NSDirectoryEnumerationSkipsPackageDescendants | 
										  NSDirectoryEnumerationSkipsSubdirectoryDescendants 
																			errorHandler:nil];
	
	
	//Matches animation frames, regex expands to find things like ANIM_001
	NSPredicate* animationPredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES '.+[0-9]{3}'"];
	NSPredicate* startFramePredicate = [NSPredicate predicateWithFormat:@"SELF MATCHES '.+001'"];	 
	
	//Find images that belong to fonts, these are images that end with the endings described above
	NSPredicate* fontCharPredicate = [NSPredicate predicateWithBlock:^(id evaluatedObject, NSDictionary *bindings) 
									  {
										  NSString *charID = [[[[evaluatedObject stringByDeletingPathExtension] componentsSeparatedByString:@"_"] lastObject] uppercaseString];
										  return [fontIdentifiers containsObject:charID];										
									  }];
	
	
	for( NSURL* path in directories )
	{
		BOOL isDirectory;
		[[NSFileManager defaultManager] fileExistsAtPath:[path path] isDirectory:&isDirectory];
		
		if( isDirectory )
		{			
			NSString *relativePath = [[path path] substringFromIndex:[[rootArtDirectory stringByStandardizingPath] length] + 1];
			
			NSDictionary *packedFolder = [GGDirectory coordinatesAndImageForPackedDirectory:relativePath 
																		  fromRootDirectory:rootArtDirectory
																				forceSquare:pvrCompress];
			
			NSString *folderFixedName = [[[relativePath uppercaseString] stringByReplacingOccurrencesOfString:@"/" withString:@"_"] stringByReplacingOccurrencesOfString:@" " withString:@"_"];
			NSImage *finalImage = [packedFolder objectForKey:GGDirectoryImage];
			NSDictionary *currentDirCoordinates = [packedFolder objectForKey:GGDirectoryCoordinates];
			[coordinateDictionary setObject:currentDirCoordinates forKey:folderFixedName];							  
			
			
			NSArray *animationFrames = [[currentDirCoordinates allKeys] filteredArrayUsingPredicate:animationPredicate];
			NSArray *startFrames = [animationFrames filteredArrayUsingPredicate:startFramePredicate];
			NSArray *fontChars = [[currentDirCoordinates allKeys] filteredArrayUsingPredicate:fontCharPredicate];
			
			for( NSString *fontChar in fontChars )
			{
				NSArray* split = [fontChar componentsSeparatedByString:@"_"];
				NSArray* allButLast = [NSArray arrayWithArray:[split subarrayWithRange:NSMakeRange(0, [split count]-1)]];
				NSString* commonString = [allButLast componentsJoinedByString:@"_"];
				
				
				NSMutableArray *font;
				if( [[fonts allKeys] containsObject:commonString] )
				{
					font = [fonts objectForKey:commonString];
				}
				else 
				{
					font = [NSMutableArray array];
					[fonts setObject:font forKey:commonString];
				}
				[font addObject:fontChar];
			}
			
			for( NSString *startFrame in startFrames )
			{
				NSString* commonString = [[startFrame stringByDeletingPathExtension] substringToIndex:[[startFrame stringByDeletingPathExtension] length] - 3];
				NSPredicate* myFramesPredicate = [NSPredicate predicateWithFormat:@"SELF BEGINSWITH %@", commonString];
				NSArray* myAnimationFrames = [animationFrames filteredArrayUsingPredicate:myFramesPredicate];
				NSArray* sortedFrames = [myAnimationFrames sortedArrayUsingComparator:^(id obj1, id obj2)
										 {
											 int frameVal1 = [[obj1 substringWithRange:NSMakeRange([commonString length], 3)] intValue];
											 int frameVal2 = [[obj2 substringWithRange:NSMakeRange([commonString length], 3)] intValue];
											 return (NSComparisonResult)(frameVal1 - frameVal2);
										 }];
				
				[animations setObject:sortedFrames forKey:commonString];
			}
			
			//NSLog(@"%@", coordinateDictionary);
			NSData *data = [[[finalImage representations] objectAtIndex:0] representationUsingType: NSPNGFileType
																						properties: nil];
			
			NSString *pngPath = [[outputDirectory stringByAppendingPathComponent:folderFixedName] stringByAppendingPathExtension:@"png"];
			[data writeToFile: pngPath
				   atomically: NO];
			
			if( pvrCompress )
			{
				NSTask* pvrTool = [[NSTask alloc] init];
				[pvrTool setCurrentDirectoryPath:@"/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/"];
				[pvrTool setLaunchPath:@"/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool"];
				[pvrTool setArguments:[NSArray arrayWithObjects:@"-e",
									   @"PVRTC",
									   @"--channel-weighting-linear",
									   @"--bits-per-pixel-4",
									   @"-o",
									   [NSString stringWithFormat:@"%@",[[outputDirectory stringByAppendingPathComponent:folderFixedName] stringByAppendingPathExtension:@"pvr"]],
									   @"-f",
									   @"PVR",
									   pngPath,nil]];
				
				[pvrTool launch];
				[pvrTool waitUntilExit];
				[pvrTool release];
				[[NSFileManager defaultManager] removeItemAtURL:[NSURL URLWithString:pngPath] error:nil];
				extension = @"pvr";
			}
			else if(ggCompress)
			{
				[GGImageCompress compressImageAtPath:pngPath andWriteToPath:[[outputDirectory stringByAppendingPathComponent:folderFixedName] stringByAppendingPathExtension:@"ggimage"]];
				[[NSFileManager defaultManager] removeItemAtURL:[NSURL fileURLWithPath:pngPath] error:nil];
				extension = @"ggimage";
				
			}
			else if(dither)
			{
				[GGImageDither ditherImageAtPath:pngPath andWriteToPath:[[outputDirectory stringByAppendingPathComponent:folderFixedName] stringByAppendingPathExtension:@"ggimage"]];
				//				[[NSFileManager defaultManager] removeItemAtURL:[NSURL fileURLWithPath:pngPath] error:nil];

			}
			
			
		}
	}
}

-(void)writeAtlasXML
{
	FILE *outFile = fopen([[outputDirectory stringByAppendingPathComponent:@"atlas.xml"] UTF8String], "w");
	fprintf(outFile, "<atlas>\n");
	
	for( NSString* key in coordinateDictionary )
	{
		NSString *outputImageName = key;
		NSDictionary *coordinates = [coordinateDictionary objectForKey:key];
		fprintf(outFile, "\t<atlas_file name=\"%s_atlas.xml\"/>\n", [outputImageName UTF8String]);
		[self writeXMLForCoordinateDictionary:coordinates withName:outputImageName];
	}
	
	
	
	for( NSString* font in fonts )
	{
		NSString *fontName = [font stringByReplacingOccurrencesOfString:@"/" withString:@":"];
		NSArray *fontChars = [fonts objectForKey:font];
		fprintf(outFile, "\t<font name=\"%s\">\n", [fontName UTF8String]);
		for( NSString* fontChar in fontChars )
		{
			fprintf(outFile, "\t\t<char id=\"%s\" name=\"%s\"/>\n", [[[fontChar componentsSeparatedByString:@"_"] lastObject] UTF8String], [[fontChar stringByReplacingOccurrencesOfString:@"/" withString:@":"] UTF8String]);				
		}
		fprintf(outFile, "\t</font>\n");
	}
	
	for( NSString* key in animations )
	{
		NSString *animationName = [[key stringByReplacingOccurrencesOfString:@"/" withString:@":"] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"_"]];
		NSArray *animationFrames = [animations objectForKey:key];
		fprintf(outFile, "\t<animation name=\"%s\">\n", [animationName UTF8String]);
		for( NSString* animationFrame in animationFrames )
		{
			fprintf(outFile, "\t\t<frame name=\"%s\"/>\n", [[animationFrame stringByReplacingOccurrencesOfString:@"/" withString:@":"] UTF8String]);
		}
		fprintf(outFile, "\t</animation>\n");
	}
	
	
	fprintf(outFile, "</atlas>\n");
	fclose(outFile);
}

-(void)writeXMLForCoordinateDictionary:(NSDictionary*) coordinates withName:(NSString*) outputImageName
{
	FILE *outFile = fopen([[[outputDirectory stringByAppendingPathComponent:[outputImageName stringByAppendingString:@"_atlas"]] stringByAppendingPathExtension:@"xml"] UTF8String], "w");
	fprintf(outFile, "<atlas>\n");

	for( NSString* origionalImageName in [coordinates allKeys] )
	{			
		NSRect rect = [[coordinates objectForKey:origionalImageName] rectValue];
		fprintf(outFile, "\t<sprite name=\"%s\" texture=\"%s\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\"/>\n", 
				[[origionalImageName stringByReplacingOccurrencesOfString:@"/" withString:@":"] UTF8String], 
				[[outputImageName stringByAppendingPathExtension:extension] UTF8String], 
				rect.origin.x,
				rect.origin.y,
				rect.size.width,
				rect.size.height);
	}
	fprintf(outFile, "</atlas>\n");
	fclose(outFile);
}

@end
