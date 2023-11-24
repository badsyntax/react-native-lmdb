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

    NSURL *documentsPath = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
                                                                   inDomains:NSUserDomainMask] lastObject];

    rnlmdb::install(*(facebook::jsi::Runtime *)cxxBridge.runtime, [documentsPath.path UTF8String]);
}

- (void)invalidate {
    rnlmdb::cleanUp();
}

@end
