import React, { useState } from 'react';
import { StyleSheet, View, Text, Button } from 'react-native';
import RNFS from 'react-native-fs';
import prettyBytes from 'pretty-bytes';

import { open, init } from 'react-native-lmdb';

import { MMKV } from 'react-native-mmkv';

console.log('documents', RNFS.DocumentDirectoryPath);

const LMDB_NAME = 'mydb19.mdb';

const mmkvStorage = new MMKV();

type RunResults = {
  open?: number;
  get10_000?: number;
  put10_000?: number;
  dbSize?: number;
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

init(LMDB_NAME, 1024 * 1024 * 200);

const nowOpen = performance.now();
const { get, put, beginTransaction, commitTransaction } = open('subdb');
results.lmdb.open = performance.now() - nowOpen;

function handleError(err: Error) {
  console.error(err);
}

export default function App() {
  const [runResults, setRunResults] = useState<ComparisonResults | undefined>();

  async function runBenchmarks() {
    const wtxn = beginTransaction(true);
    const nowBatchPut = performance.now();
    for (let i = 0; i < 10_000; i++) {
      put(`key${i}`, longString, wtxn);
    }
    results.lmdb.put10_000 = performance.now() - nowBatchPut;
    commitTransaction(wtxn);

    const nowBatchPutMmkv = performance.now();
    for (let i = 0; i < 10_000; i++) {
      mmkvStorage.set(`key${i}`, getLongString());
    }
    results.mmkv.put10_000 = performance.now() - nowBatchPutMmkv;

    const nowBatchGetMmkv = performance.now();
    for (let i = 0; i < 10_000; i++) {
      mmkvStorage.getString(`key${i}`);
    }
    results.mmkv.get10_000 = performance.now() - nowBatchGetMmkv;

    const rtxn = beginTransaction();
    const nowBatchGet = performance.now();
    for (let i = 0; i < 10_000; i++) {
      get(`key${i}`, rtxn);
    }
    results.lmdb.get10_000 = performance.now() - nowBatchGet;
    commitTransaction(rtxn);

    const mmkvDbPath = RNFS.DocumentDirectoryPath + '/mmkv/mmkv.default';
    const lmdbPath = RNFS.DocumentDirectoryPath + `/${LMDB_NAME}/data.mdb`;

    const { size: mmkvSize } = await RNFS.stat(mmkvDbPath);

    results.mmkv.dbSize = mmkvSize;

    const { size: lmdbSize } = await RNFS.stat(lmdbPath);

    results.lmdb.dbSize = lmdbSize;

    setRunResults({ ...results });
  }

  return (
    <View style={styles.container}>
      <Text>Hello LMDB</Text>
      <Button
        title="Run Benchmarks"
        onPress={() => runBenchmarks().catch(handleError)}
      />
      {!!runResults && (
        <>
          <Text>open: {runResults.lmdb.open}ms</Text>
          <Text>lmdb put (10_000): {runResults.lmdb.put10_000}ms</Text>
          <Text>lmdb get (10_000): {runResults.lmdb.get10_000}ms</Text>
          <Text>lmdb size: {prettyBytes(runResults.lmdb.dbSize || 0)}</Text>
          <Text> </Text>
          <Text>mmkv put (10_000): {runResults.mmkv.put10_000}ms</Text>
          <Text>mmkv get (10_000): {runResults.mmkv.get10_000}ms</Text>
          <Text>mmkv size: {prettyBytes(runResults.mmkv.dbSize || 0)}</Text>
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
