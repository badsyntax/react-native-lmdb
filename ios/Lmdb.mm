#import <React/RCTBridgeModule.h>
#import "Lmdb.h"
#import "lmdb++.h"

@implementation Lmdb
RCT_EXPORT_MODULE()

NSURL *get_db_path(NSString *dbName)
{
    NSURL *documentsPath = [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory
                                                                   inDomains:NSUserDomainMask] lastObject];

    NSURL *dbPath = [documentsPath URLByAppendingPathComponent:dbName];

    return dbPath;
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(open:(NSString *)dbName withMapSize:(nonnull NSNumber *)mapSize)
{
    NSURL *dbPath = get_db_path(dbName);

    NSError *error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:dbPath.path
                              withIntermediateDirectories:YES
                                               attributes:nil
                                                    error:&error];
    if (error != nil) {
      // @TODO
    }

    try {
        rnlmdb::open([dbPath.path UTF8String], [mapSize longValue]);
    } catch (lmdb::error& error) {
        // @TODO
    }

    return nil;
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(put:(NSString *)key withValue:(NSString *)value)
{
    rnlmdb::put([key UTF8String], [value UTF8String]);
    return nil;
}


RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(get:(NSString *)key)
{
    return @(rnlmdb::get([key UTF8String]).c_str());
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(del:(NSString *)key)
{
    rnlmdb::del([key UTF8String]);
    return nil;
}

@end
