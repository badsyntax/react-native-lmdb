# react-native-lmdb

<div style="overflow:hidden">
<a href="https://www.symas.com/lmdb"><img alt="LMDB" src="./img/lmdb-logo.png" width="100" align="left" /></a>

<div>

[LMDB](https://www.symas.com/lmdb) (Lightning Memory-Mapped Database) is an embedded transactional database in the form of a key-value store.

This package embeds & provides React Native bindings for LMDB.

NOTE: Under development, not ready for consumption yet!

</div>
</div>

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

LMDB uses transactions for read/write ops. By default a new transaction is created for each op and this introduces overhead. Batch ops must always used a shared transaction to improve perf.

Write lots of data in a single transaction:

```ts
const wtxn = transaction(TRANSACTION_WRITE);
wtxn.begin();
put('some', 'data', wtxn);
put('other', 'data', wtxn);
// ...
wtxn.commit(); // write the data to the db
```

For reading data, you can use a global read transaction, then reset it to sync with the db.

```ts
// Make this global, and adjust all get() calls to use this transaction
const rtxn = transaction();

const value1 = get('key1', rtxn);
const value2 = get('key2', rtxn);

// Elsewhere in your app, after making changes to your db...
put('key1', 'new data');
rtx.reset(); // this allow subsequent get() calls to use the latest db snapshot

const value1New = get('key1', rtxn);
```

## Motivation

MMKV is a great tool but isn't designed for vast amounts of data.

When setting and getting 10_000 large strings:

| DB   | Empty | Run 1  | Run 2  | Run 3  |
| ---- | ----- | ------ | ------ | ------ |
| mmkv | 16kb  | 33.6mb | 67.1mb | 67.1mb |
| lmdb | 33kb  | 38.3mb | 38.3mb | 38.3mb |

// @TODO - check db sizes on devices

SQLite can handle vast amounts of data but is async thus increases complexity and introduces possible race conditions.

LMDB is mature, synchronous, and can handle anything you throw at it. 💪

## Goals of this Project

- Simple API
- Performance over features

## FAQ

Q: Does LMDB store everything in memory?
A:

## Benchmarks

NOTES: these numbers might not represent current state. I am still in the process of profiling and optimising.

<table width="100%"><tr><td>

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

- mmkv is fast to read
- mmkv is fast to write
- lmdb to fast to read
- lmdb is (potentially very) slow to write depending on device, but can write fast when using a transaction (txn).

## Credits

- Thanks to [sysmas](https://www.symas.com/) for open sourcing lmdb.
- Thanks to [drycpp/lmdb++](https://github.com/drycpp/lmdbxx) & [hoytech/lmdb++](https://github.com/hoytech/lmdbxx) for the useful c++ wrapper.

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
