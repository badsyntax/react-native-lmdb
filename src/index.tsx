import { NativeModules } from 'react-native';

export type BatchValues = Array<{ key: string; value: string }>;

// This is the maximum size for the database
export const DEFAULT_MAP_SIZE = 1024 * 1024 * 100; // 100mb

declare function init(dbName: string, mapSize: number): void;
declare function open(subDbName: string): number;
declare function get(key: string): string | null;
declare function del(pointer: number, key: string): void;
declare function put(pointer: number, key: string, value: string): void;
declare function putBatch(pointer: number, values: BatchValues): void;
declare function drop(): void;
declare function transaction(): number;
declare function beginTransaction(tidx: number): void;
declare function resetTransaction(tidx: number): void;
declare function commitTransaction(tidx: number): void;

// Create the native JSI functions
NativeModules.Lmdb.install();

function _init(dbName: string, mapSize = DEFAULT_MAP_SIZE) {
  return init(dbName, mapSize);
}

function _open(subDbName: string) {
  const pointer = open(subDbName);
  return {
    get,
    put: (key: string, value: string) => put(pointer, key, value),
    putBatch: (values: BatchValues) => putBatch(pointer, values),
    del: (key: string) => del(pointer, key),
    drop,
    transaction,
    resetTransaction,
    beginTransaction,
    commitTransaction,
  };
}

export { _open as open, _init as init };
