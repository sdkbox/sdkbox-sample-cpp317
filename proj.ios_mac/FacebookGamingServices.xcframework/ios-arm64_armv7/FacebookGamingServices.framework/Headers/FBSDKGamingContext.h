// Copyright (c) 2014-present, Facebook, Inc. All rights reserved.
//
// You are hereby granted a non-exclusive, worldwide, royalty-free license to use,
// copy, modify, and distribute this software in source code or binary form for use
// in connection with the web services and APIs provided by Facebook.
//
// As with any software that integrates with the Facebook platform, your use of
// this software is subject to the Facebook Developer Principles and Policies
// [http://developers.facebook.com/policy/]. This copyright notice shall be
// included in all copies or substantial portions of the software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, FBSDKGamingContextType) {
  FBSDKGamingContextGeneric = 0,
  FBSDKGamingContextLink,
};

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(GamingContext)
@interface FBSDKGamingContext : NSObject

- (instancetype)init NS_UNAVAILABLE;
+ (instancetype)new NS_UNAVAILABLE;

/**
A shared object that holds data about the current user's  game instance which could be solo game or multiplayer game with other users.
*/
@property (nullable, class, nonatomic) FBSDKGamingContext *currentContext;

/**
 A unique identifier for the current game context. This represents a specific game instance that the user is playing in.
 */
@property (readonly, nonatomic) NSString* identifier;

/**
  The number of players in the current user's  game instance
 */
@property (readonly) NSInteger size;

/**
 Internal Type exposed to facilitate transition to Swift.
 API Subject to change or removal without warning. Do not use.

 Creates a context with an identifier. If the identifier is nil or empty, a context will not be created.

 @warning UNSAFE - DO NOT USE
 */
+ (nullable instancetype)createContextWithIdentifier:(NSString *)identifier size:(NSInteger)size;

@end

NS_ASSUME_NONNULL_END
