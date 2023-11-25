# react-native-lmdb

<div style="overflow:hidden">
<a href="https://www.symas.com/lmdb"><img alt="LMDB" src="./img/lmdb-logo.png" width="100" align="left" /></a>

<div>

[LMDB](https://www.symas.com/lmdb) (Lightning Memory-Mapped Database) is an embedded transactional database in the form of a key-value store. It can handle vast amounts of data and is comparable in speed to [MMKV](https://github.com/Tencent/MMKV).

This package embeds & provides React Native bindings for LMDB.

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

## Benchmarks

NOTES: these numbers might not represent current state. I am still in the process of profiling and optimising.

<table width="100%"><tr><td>

### iOS (Simulator)

|                  | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000       | 42038.66ms        | 1815.24ms         |
| put 10_000 (txn) | 1512.22ms         | n/a               |
| get 10_000       | 33.74ms           | 57.98ms           |
| db size run 1    |                   |                   |
| db size run 2    |                   |                   |
| db size run 3    |                   |                   |

</td><td>

### iOS (iPhone 7)

|                  | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000       | 42038.66ms        | 1815.24ms         |
| put 10_000 (txn) | 1512.22ms         | n/a               |
| get 10_000       | 33.74ms           | 57.98ms           |
| db size run 1    |                   |                   |
| db size run 2    |                   |                   |
| db size run 3    |                   |                   |

</td></tr></table>

<table width="100%"><tr><td>

### Android (Emulator)

|                  | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000       | 1769.53ms         | 117.94ms          |
| put 10_000 (txn) | 288.43ms          | n/a               |
| get 10_000       | 26.74ms           | 16.29ms           |
| db size run 1    |                   |                   |
| db size run 2    |                   |                   |
| db size run 3    |                   |                   |

</td><td>

### Android (Pixel 6a)

|                  | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000       | 1769.53ms         | 117.94ms          |
| put 10_000 (txn) | 288.43ms          | n/a               |
| get 10_000       | 26.74ms           | 16.29ms           |
| db size run 1    |                   |                   |
| db size run 2    |                   |                   |
| db size run 3    |                   |                   |

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
