#import <React/RCTBridge+Private.h>
#import <React/RCTUtils.h>
#import "Lmdb.h"
#import "lmdb++.h"

@implementation Lmdb

@synthesize bridge=_bridge;
@synthesize methodQueue = _methodQueue;

RCT_EXPORT_MODULE(Lmdb)

+ (BOOL)requiresMainQueueSetup {
  return YES;
}

- (void)setBridge:(RCTBridge *)bridge {
    _bridge = bridge;
    _setBridgeOnMainQueue = RCTIsMainQueue();

    RCTCxxBridge *cxxBridge = (RCTCxxBridge *)self.bridge;
    if (!cxxBridge.runtime) {
        return;
    }
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
    NSURL *documentsPath = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
                                                                   inDomains:NSUserDomainMask] lastObject];

    RCTCxxBridge *cxxBridge = (RCTCxxBridge *)self.bridge;
    rnlmdb::install(*(facebook::jsi::Runtime *)cxxBridge.runtime, [documentsPath.path UTF8String]);
    return nil;
}

- (void)invalidate {
    rnlmdb::cleanUp();
}

@end
