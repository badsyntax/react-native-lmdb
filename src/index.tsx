import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-lmdb' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

const Lmdb = NativeModules.Lmdb
  ? NativeModules.Lmdb
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

const defaultMapSize = 1024 * 1024 * 100; // 100mb

export function open(dbName: string, mapSize = defaultMapSize) {
  Lmdb.open(dbName, mapSize);
  return {
    put: (key: string, value: string): void => Lmdb.put(key, value),
    get: (key: string): string | null => Lmdb.get(key),
    del: (key: string): void => Lmdb.del(key),
  };
}
