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

## Installation

```sh
npm install react-native-lmdb
```

## Usage

```js
import { open, put, get } from 'react-native-lmdb';

open('mydb.mdb');

put('key1', 'value1');
put('key2', 'value2');

console.log(get('key1'));
console.log(get('key2'));
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

---

Made with [create-react-native-library](https://github.com/callstack/react-native-builder-bob)
