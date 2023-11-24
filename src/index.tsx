const DEFAULT_MAP_SIZE = 1024 * 1024 * 100; // 100mb

declare function open(dbName: string, mapSize: number): void;
declare function get(key: string): string | null;
declare function del(key: string): void;
declare function put(key: string, value: string): void;

function jsGet(key: string) {
  return get(key);
}

function jsPut(key: string, value: string) {
  put(key, value);
}

function jsDel(key: string) {
  del(key);
}

function jsOpen(dbName: string, mapSize = DEFAULT_MAP_SIZE) {
  open(dbName, mapSize);
  return {
    get: jsGet,
    put: jsPut,
    del: jsDel,
  };
}

export { jsOpen as open };

// export function openA(dbName: string) {
//   open(dbName, 1024 * 1024 * 100);
// }

// export function getA(key: string) {
//   return get(key);
// }

// export function putA(key: string, value: string) {
//   put(key, value);
// }
