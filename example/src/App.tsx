import React, { useState } from 'react';
import { StyleSheet, View, Text, Button } from 'react-native';
import { open, init } from 'react-native-lmdb';

import { MMKV } from 'react-native-mmkv';

const mmkvStorage = new MMKV();

type RunResults = {
  open?: number;
  get10_000?: number;
  put10_000?: number;
  putBatch10_000?: number;
  get2?: number;
  put2?: number;
};

type ComparisonResults = {
  lmdb: RunResults;
  mmkv: RunResults;
};

function getLongString() {
  let x = '1234567890';
  const iterations = 5;
  for (var i = 0; i < iterations; i++) {
    x += x.concat(x);
  }
  return x;
}

const longString = getLongString();
const results: ComparisonResults = { mmkv: {}, lmdb: {} };

init('mydb5.mdb', 1024 * 1024 * 200);

const nowOpen = performance.now();
const {
  get,
  put,
  // putBatch,
  drop,
  transaction,
  // beginTransaction,.
  // commitTransaction,
  // resetTransaction,
} = open('subdb');
results.lmdb.open = performance.now() - nowOpen;

export default function App() {
  const [memoryLeakStarted, _setMemoryLeakStarted] = useState(false);
  const [runResults, setRunResults] = useState<ComparisonResults | undefined>();

  // console.log('not found', get('not found'));

  function runBenchmarks() {
    console.log('run');
    const nowPut = performance.now();
    // put('key1', JSON.stringify({ hello: 'world' }));
    // put('key2', JSON.stringify({ hello: 'world2' }));
    results.lmdb.put2 = performance.now() - nowPut;

    // const nowGet = performance.now();
    // console.log(get('key1'));
    // console.log(get('key2'));
    // results.lmdb.get2 = performance.now() - nowGet;

    const nowBatchPut = performance.now();
    for (let i = 0; i < 10_000; i++) {
      // put(`key${i}`, longString);
    }
    results.lmdb.put10_000 = performance.now() - nowBatchPut;

    const nowBatchPutMmkv = performance.now();
    for (let i = 0; i < 10_000; i++) {
      // mmkvStorage.set(`key${i}`, getLongString());
    }
    results.mmkv.put10_000 = performance.now() - nowBatchPutMmkv;

    const nowBatchGetMmkv = performance.now();
    for (let i = 0; i < 10_000; i++) {
      mmkvStorage.getString(`key${i}`);
    }
    results.mmkv.get10_000 = performance.now() - nowBatchGetMmkv;

    const rtxn = transaction();
    // beginTransaction(rtxn);
    console.log('rtxn', rtxn);
    console.log('foo');
    const nowBatchGet = performance.now();
    for (let i = 0; i < 10_000; i++) {
      // get(`key${i}`);
    }
    results.lmdb.get10_000 = performance.now() - nowBatchGet;

    // const batchData: BatchValues = [];

    // for (let i = 0; i < 10_000; i++) {
    //   batchData.push({
    //     key: `key${i}`,
    //     value: longString,
    //   });
    // }

    // const nowBatchPut2 = performance.now();
    // putBatch(batchData);
    // results.putBatch10_000 = performance.now() - nowBatchPut2;

    setRunResults({ ...results });
  }

  // Test if we have a memory leak in our native code.
  // Write and read a very large string while freeing JS memory.
  // This should max out disk operations.
  // App memory should rise and fall as we free JS memory.
  // As you continue to run the test app memory will slowly increase but I don't
  // think this indicates a leak.
  async function runMemoryLeakTest() {
    // setMemoryLeakStarted(true);
    // for (let i = 0; i < 200_000; i++) {
    //   // force a gc every 20_000
    //   if (i % 20_000 === 0) {
    //     await new Promise((res) => setTimeout(res));
    //   }
    //   put(`key1`, longString);
    //   get(`key1`);
    // }
    // setMemoryLeakStarted(false);
  }

  function dropDb() {
    drop();
  }

  return (
    <View style={styles.container}>
      <Text>Hello LMDB</Text>
      <Button title="Run Benchmarks" onPress={runBenchmarks} />
      <Button title="Run Memory Leak Test" onPress={runMemoryLeakTest} />
      <Button title="Drop db" onPress={dropDb} />
      {memoryLeakStarted && <Text>Memory leak test running...</Text>}
      {!!runResults && (
        <>
          <Text>open: {runResults.lmdb.open}ms</Text>
          <Text>put (2): {runResults.lmdb.put2}ms</Text>
          <Text>get (2): {runResults.lmdb.get2}ms</Text>
          <Text>lmdb put (10_000): {runResults.lmdb.put10_000}ms</Text>
          <Text>
            lmdb put batch (10_000): {runResults.lmdb.putBatch10_000}ms
          </Text>
          <Text>lmdb get (10_000): {runResults.lmdb.get10_000}ms</Text>
          <Text>mmkv put (10_000): {runResults.mmkv.put10_000}ms</Text>
          <Text>mmkv get (10_000): {runResults.mmkv.get10_000}ms</Text>
        </>
      )}
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
