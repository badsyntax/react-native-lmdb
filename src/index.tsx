import { NativeModules } from 'react-native';

export type BatchValues = Array<{ key: string; value: string }>;

// This is the maximum size for the database
export const DEFAULT_MAP_SIZE = 1024 * 1024 * 100; // 100mb

declare function init(dbName: string, mapSize: number): void;
declare function open(subDbName: string): number;
declare function get(dbi: number, key: string, tidx: number): string | null;
declare function del(dbi: number, key: string): void;
declare function put(dbi: number, key: string, value: string): void;
declare function putBatch(dbi: number, values: BatchValues): void;
declare function drop(): void;
declare function beginTransaction(): number;
declare function resetTransaction(tidx: number): void;
declare function commitTransaction(tidx: number): void;

// Create the native JSI functions
NativeModules.Lmdb.install();

function _init(dbName: string, mapSize = DEFAULT_MAP_SIZE) {
  return init(dbName, mapSize);
}

function _open(subDbName: string) {
  const dbi = open(subDbName);
  return {
    get: (key: string, tidx: number) => get(dbi, key, tidx),
    put: (key: string, value: string) => put(dbi, key, value),
    putBatch: (values: BatchValues) => putBatch(dbi, values),
    del: (key: string) => del(dbi, key),
    drop,
    resetTransaction,
    beginTransaction,
    commitTransaction,
  };
}

export { _open as open, _init as init };
