# react-native-lmdb

<div style="overflow:hidden">
<a href="https://www.symas.com/lmdb"><img alt="LMDB" src="./img/lmdb-logo.png" width="130" align="left" /></a>
<div>

[LMDB](https://www.symas.com/lmdb) (Lightning Memory-Mapped Database) is an embedded transactional database in the form of a key-value store. It can handle vast amounts of data and is slightly faster at reading than [MMKV](https://github.com/Tencent/MMKV).

This package embeds & provides React Native bindings for LMDB.

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

## Motivation

MMKV is a great tool but isn't designed for vast amounts of data.

// @TODO: expand

<details>
<summary>Toggle MMKV related issues</summary>

- https://github.com/Tencent/MMKV/issues/610
- https://github.com/mrousavy/react-native-mmkv/issues/440
- https://github.com/mrousavy/react-native-mmkv/issues/397
- https://github.com/apollographql/apollo-cache-persist/issues/500
- https://github.com/ammarahm-ed/react-native-mmkv-storage/issues/286

</details>

<br/>

SQLite can handle vast amounts of data but is async thus increases complexity and introduces possible race conditions.

LMDB is mature, synchronous, and can handle anything you throw at it.

## Goals of this Project

- Simple API
- Performance over features

## Benchmarks

### iOS Simulator:

| Action           | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000 items |                   |                   |
| get 10_000 items |                   |                   |
| put 50_000 items |                   |                   |
| get 50_000 items |                   |                   |

### Android Emulator:

| Action           | react-native-lmdb | react-native-mmkv |
| ---------------- | ----------------- | ----------------- |
| put 10_000 items |                   |                   |
| get 10_000 items |                   |                   |
| put 50_000 items |                   |                   |
| get 50_000 items |                   |                   |

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
