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

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(open:(NSString *)dbName withMapSize:(nonnull NSNumber *)mapSize withErrorCallback:(RCTResponseSenderBlock)callback)
{
    NSURL *dbPath = get_db_path(dbName);

    NSError * error = nil;
    [[NSFileManager defaultManager] createDirectoryAtPath:dbPath.path
                              withIntermediateDirectories:YES
                                               attributes:nil
                                                    error:&error];
    if (error != nil) {
        callback(@[[NSString stringWithFormat:@"error creating directory: %@",error],
                   [NSNumber numberWithInt:1]]);
        return nil;
    }

    try {
        rnlmdb::open([dbPath.path UTF8String], [mapSize longValue]);
        callback(@[[NSNull null],
                   [NSNumber numberWithInt:0]]);
    } catch (lmdb::error& error) {
        callback(@[[NSString stringWithUTF8String: error.what()],
                   [NSNumber numberWithInt:1]]);
    }

    return nil;
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(put:(NSString *)key withValue:(NSString *)value)
{
    rnlmdb::put([key UTF8String], [value UTF8String]);
    return nil;
}

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(putBatch:(NSDictionary *)valueDict)
{
    std::unordered_map<std::string, std::string> valueMap;
    for (id keyId in [valueDict allKeys])
    {
        NSString *value = [[valueDict objectForKey: keyId] stringValue];
        NSString *key = [keyId stringValue];
        valueMap[[key UTF8String]] = [value UTF8String];
    }
    rnlmdb::putBatch(valueMap);
    return nil;
}


RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(get:(NSString *)key)
{
    return @(rnlmdb::get([key UTF8String]).c_str());
}

@end
