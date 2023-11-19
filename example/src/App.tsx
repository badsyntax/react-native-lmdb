import * as React from 'react';

import { StyleSheet, View, Text } from 'react-native';
import { get, open, put } from 'react-native-lmdb';

const nowOpen = performance.now();
open('mydb.mdb');
const endOpen = performance.now() - nowOpen;

const nowPut = performance.now();
put('key1', 'value1');
put('key2', 'value2');
const endPut = performance.now() - nowPut;

const nowGet = performance.now();
const value1 = get('key1');
const value2 = get('key2');
const endGet = performance.now() - nowGet;

const nowBatchPut = performance.now();
for (let i = 0; i < 10_000; i++) {
  put(`key${i}`, `value${i}`);
}
const endBatchPut = performance.now() - nowBatchPut;

const nowBatchGet = performance.now();
for (let i = 0; i < 10_000; i++) {
  get(`key${i}`);
}
const endBatchGet = performance.now() - nowBatchGet;

const nowBatchGet1 = performance.now();
for (let i = 0; i < 1_000; i++) {
  get(`key${i}`);
}
const endBatchGet1 = performance.now() - nowBatchGet1;

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Hello LMDB</Text>
      <Text>key1 value: {value1}</Text>
      <Text>key2 value: {value2}</Text>
      <Text>open: {endOpen}ms</Text>
      <Text>put (2): {endPut}ms</Text>
      <Text>get (2): {endGet}ms</Text>
      <Text>put (10_000): {endBatchPut}ms</Text>
      <Text>get (10_000): {endBatchGet}ms</Text>
      <Text>get (1000): {endBatchGet1}ms</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
