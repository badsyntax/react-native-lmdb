# react-native-lmdb

<div style="overflow:hidden">
<a href="https://www.symas.com/lmdb"><img alt="LMDB" src="./img/lmdb-logo.png" width="130" align="left" /></a>
<div>

[LMDB](https://www.symas.com/lmdb) (Lightning Memory-Mapped Database) is an embedded transactional database in the form of a key-value store.

This package embeds & provides React Native bindings for LMDB.

_NOTE: Under development, not ready for consumption yet!_

</div>
</div>

<br clear="left"/>

## Installation

```sh
npm install react-native-lmdb
cd ios && pod install
```

## Usage

```js
import { open } from 'react-native-lmdb';

// Define the largest size of the db (100mb in this case)
const mapSize = 1024 * 1024 * 100;

const { put, get, del } = open('mydb.mdb', mapSize);

put('key1', 'value1');
put('key2', 'value2');

console.log(get('key1'));
console.log(get('key2'));

del('key1');
del('key2');
```

## Optimisation

LMDB uses transactions for read/write ops. Batch ops should use a shared transaction to improve perf.

Write lots of data in a single transaction:

```ts
const tidx = beginTransaction();
put('some', 'data', tidx);
put('other', 'data', tidx);
// ...
writeTransaction(tidx); // write the data to the db
```

For reading data, you can use a global read transaction, then reset it to sync with the db.

```ts
// Make this global, and adjust all get() calls to use this transaction
const tidx = beginTransaction();

const value1 = get('key1', tidx);
const value2 = get('key2', tidx);

// Elsewhere in your app, after making changes to your db...
put('key1', 'new data');
resetTransaction(tidx); // this allow subsequent get() calls to use the latest db snapshot

const value1New = get('key1', tidx);
```

## Motivation

MMKV is a great tool but isn't designed for vast amounts of data.

SQLite can handle vast amounts of data but is async thus increases complexity and introduces possible race conditions.

LMDB is mature, synchronous, and can handle anything you throw at it. 💪

## Goals of this Project

- Simple API
- Performance over features

## Benchmarks

I am still in the process of profiling and optimising.

<!-- <table width="100%"><tr><td>

### iOS (Simulator)

|               | react-native-mmkv | react-native-lmdb |
| ------------- | ----------------- | ----------------- |
| put 10_000    |                   |                   |
| get 10_000    |                   |                   |
| db size run 1 |                   |                   |
| db size run 2 |                   |                   |
| db size run 3 |                   |                   |

</td><td>

### iOS (iPhone 7)

|                  | react-native-mmkv | react-native-lmdb |
| ---------------- | ----------------- | ----------------- |
| put 10_000       |                   |                   |
| put 10_000 (txn) |                   |                   |
| get 10_000       |                   |                   |
| db size run 1    |                   |                   |
| db size run 2    |                   |                   |
| db size run 3    |                   |                   |

</td></tr></table>

<table width="100%"><tr><td>

### Android (Emulator)

|               | react-native-mmkv | react-native-lmdb |
| ------------- | ----------------- | ----------------- |
| put 10_000    |                   |                   |
| get 10_000    |                   |                   |
| db size run 1 |                   |                   |
| db size run 2 |                   |                   |
| db size run 3 |                   |                   |

</td><td>

### Android (Pixel 6a)

|               | react-native-mmkv | react-native-lmdb |
| ------------- | ----------------- | ----------------- |
| put 10_000    |                   |                   |
| get 10_000    |                   |                   |
| db size run 1 |                   |                   |
| db size run 2 |                   |                   |
| db size run 3 |                   |                   |

</td></tr></table>

We can conclude:

// @TODO -->

## Credits

- Thanks to [sysmas](https://www.symas.com/) for open sourcing lmdb.

## Donate

If you find LMDB useful please consider supporting the OpenLDAP foundation: https://www.openldap.org/foundation/

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)

```

```
