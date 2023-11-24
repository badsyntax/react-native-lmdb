# react-native-lmdb

React Native bindings for LMDB (proof of concept).

> Lightning Memory-Mapped Database

LMDB is an embedded transactional database in the form of a key-value store.

https://www.symas.com/lmdb

This package is in early development and not ready for consumption.

## Goals of this Project

- Simple API
- Performance over features

## Benchmarks

Initial benchmarks show good performance:

iOS Simulator:

| Action           | Time     |
| ---------------- | -------- |
| open/create db   | 1.62ms   |
| put 2 items      | 0.91ms   |
| put 10_000 items | 927.39ms |
| get 2 items      | 0.035ms  |
| get 1000 items   | 4.42ms   |
| get 10_000 items | 44.25ms  |

Android Emulator:

| Action           | Time      |
| ---------------- | --------- |
| open/create db   | 0.67ms    |
| put 2 items      | 26.76ms   |
| put 10_000 items | 4343.95ms |
| get 2 items      | 0.24ms    |
| get 1000 items   | 4.42ms    |
| get 10_000 items | 28.03ms   |

## Installation

```sh
npm install react-native-lmdb
```

## Usage

```js
import { open } from 'react-native-lmdb';

const { put, get, del } = open('mydb.mdb');

put('key1', 'value1');
put('key2', 'value2');

console.log(get('key1'));
console.log(get('key2'));

del('key1');
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## Credits

- Thanks to [sysmas](https://www.symas.com/) for open sourcing lmdb.
- Thanks to [drycpp/lmdb++](https://github.com/drycpp/lmdbxx) & [hoytech/lmdb++](https://github.com/hoytech/lmdbxx) for the useful c++ wrapper.

## License

MIT Richard Willis

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
