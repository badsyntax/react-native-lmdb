import React, { useState } from 'react';

import { StyleSheet, View, Text, Button } from 'react-native';
import { open } from 'react-native-lmdb';

type RunResults = {
  open?: number;
  get10_1000?: number;
  put10_1000?: number;
  get2?: number;
  put2?: number;
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

const { put, get } = open('mydb123.mdb');

export default function App() {
  const [memoryLeakStarted, setMemoryLeakStarted] = useState(false);
  const [runResults, setRunResults] = useState<RunResults | undefined>();

  console.log('not found', get('not found'));

  function runBenchmarks() {
    const results: RunResults = {};
    const nowOpen = performance.now();
    results.open = performance.now() - nowOpen;

    const nowPut = performance.now();
    put('key1', JSON.stringify({ hello: 'world' }));
    put('key2', JSON.stringify({ hello: 'world2' }));
    results.put10_1000 = performance.now() - nowPut;

    const nowGet = performance.now();
    // @ts-ignore
    console.log(JSON.stringify(JSON.parse(get('key1'))));
    // @ts-ignore
    console.log(JSON.stringify(JSON.parse(get('key2'))));
    results.get2 = performance.now() - nowGet;

    const nowBatchPut = performance.now();
    for (let i = 0; i < 10_000; i++) {
      put(`key${i}`, `value${i}`);
    }
    results.put10_1000 = performance.now() - nowBatchPut;

    const nowBatchGet = performance.now();
    for (let i = 0; i < 10_000; i++) {
      get(`key${i}`);
    }
    results.get10_1000 = performance.now() - nowBatchGet;

    setRunResults(results);
  }

  // Test if we have a memory leak in our native code.
  // Write and read a very large string while freeing JS memory.
  // This should max out disk operations.
  // App memory should rise and fall as we free JS memory.
  // As you continue to run the test app memory will slowly increase but I don't
  // think this indicates a leak.
  async function runMemoryLeakTest() {
    setMemoryLeakStarted(true);
    for (let i = 0; i < 200_000; i++) {
      // force a gc every 20_000
      if (i % 20_000 === 0) {
        await new Promise((res) => setTimeout(res));
      }
      put(`key1`, longString);
      get(`key1`);
    }
    setMemoryLeakStarted(false);
  }
  return (
    <View style={styles.container}>
      <Text>Hello LMDB</Text>
      <Button title="Run Benchmarks" onPress={runBenchmarks} />
      <Button title="Run Memory Leak Test" onPress={runMemoryLeakTest} />
      {memoryLeakStarted && <Text>Memory leak test running...</Text>}
      {!!runResults && (
        <>
          <Text>open: {runResults.open}ms</Text>
          <Text>put (2): {runResults.put2}ms</Text>
          <Text>get (2): {runResults.get2}ms</Text>
          <Text>put (10_000): {runResults.put10_1000}ms</Text>
          <Text>get (10_000): {runResults.get10_1000}ms</Text>
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
