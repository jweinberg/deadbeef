//
//  NSData+Compression.h
//  iphone_pngconv
//
//  Created by Joshua Weinberg on 11/20/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSData (Compression)

- (NSData *)gzipDeflate;
- (NSData *)gzipInflate;

@end