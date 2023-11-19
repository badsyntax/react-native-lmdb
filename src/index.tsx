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

export function open(dbName: string) {
  let err: any;
  Lmdb.open(dbName, (e: any) => {
    err = e;
  });
  if (err) {
    throw err;
  }
}

export function put(key: string, value: string) {
  Lmdb.put(key, value);
}

export function get(key: string): string {
  return Lmdb.get(key);
}
