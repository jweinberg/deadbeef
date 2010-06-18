#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import "GGDirectory.h"
#import "GGPacker.h"

#include <getopt.h>

float TEXTURE_SIZE = 1024.f;
BOOL pvrCompress = NO;
BOOL ggCompress = NO;
BOOL dither = NO;

static struct option long_options[] =
{
	/* These options set a flag. */
	{"size",   required_argument, 0, 's'},
	/* These options don't set a flag.
	 We distinguish them by their indices. */
	{"pvr", no_argument, (int*)&pvrCompress, YES},
	{"gg", no_argument, (int*)&ggCompress, YES},
	{"dither",  no_argument, (int*)&dither, YES},
	{0, 0, 0, 0}
};


int main (int argc, char * const * argv) 
{

	int c;
	
	if( argc < 3 )
		return 0;
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	NSString* outputDirectory = [NSString stringWithUTF8String:argv[2]];
	NSString* rootArtDirectory = [NSString stringWithUTF8String:argv[1]];
	
	
	while (1)
	{
		/* getopt_long stores the option index here. */
		int option_index = 3;
		c = getopt_long (argc, argv, "s:", long_options, &option_index);
		
		/* Detect the end of the options. */
		if (c == -1)
			break;
		
		switch (c)
		{
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;
			case 's':
				TEXTURE_SIZE = atoi(optarg);
				break;
		}
	}
	
	
	GGPacker *packer = [[GGPacker alloc] initWithOutputDirectory:outputDirectory rootDirectory:rootArtDirectory];
	[packer setGgCompress:ggCompress];
	[packer setDither:dither];
	[packer processDirectory];
	[packer writeAtlasXML];
		
    [pool drain];
    return 0;
}


