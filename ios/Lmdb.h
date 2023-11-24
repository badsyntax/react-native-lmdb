#ifdef __cplusplus
#import "react-native-lmdb.h"
#endif

#ifdef RCT_NEW_ARCH_ENABLED

@interface Lmdb : NSObject <NativeLmdbSpec>
#else
#import <React/RCTBridgeModule.h>

@interface Lmdb : NSObject <RCTBridgeModule>
#endif

@property(nonatomic, assign) BOOL setBridgeOnMainQueue;

@end
