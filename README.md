# react-native-lmdb

React Native bindings for LMDB (proof of concept).

This package is in early development and not ready for consumption. Currently it only works on iOS.

Initial benchmarks show good performance:

| Action           | Time     |
| ---------------- | -------- |
| open/create db   | 1.62ms   |
| put 2 items      | 0.91ms   |
| put 10_000 items | 927.39ms |
| get 2 items      | 0.035ms  |
| get 1000 items   | 4.42ms   |
| get 10_000 items | 44.25ms  |

## LMDB

> Lightning Memory-Mapped Embedded Database

LMDB is an embedded transactional database in the form of a key-value store. It's fast, small, and can handle large amounts of data. 

https://www.symas.com/lmdb

## Installation

```sh
npm install react-native-lmdb
```

## Usage

```js
import { open } from 'react-native-lmdb';

const { put, get } = open('mydb.mdb');

put('key1', 'value1');
put('key2', 'value2');

console.log(get('key1'));
console.log(get('key2'));
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## Credits

Thanks to [lmdb++](https://github.com/drycpp/lmdbxx) for the useful c++ wrapper.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
