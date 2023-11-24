package com.lmdb;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

import java.io.File;
import java.nio.file.Paths;

@ReactModule(name = LmdbModule.NAME)
public class LmdbModule extends ReactContextBaseJavaModule {
  public static final String NAME = "Lmdb";
  private final String documentsDirectoryPath;

  public LmdbModule(ReactApplicationContext reactContext) {
    super(reactContext);
    documentsDirectoryPath = reactContext.getFilesDir().getAbsolutePath();
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  static {
    System.loadLibrary("cpp");
  }

  private static native void nativeOpen(String dbPath, double mapSize);
  private static native void nativePut(String key, String value);
  private static native String nativeGet(String key);
  private static native void nativeDel(String key);

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void open(String dbName, Double mapSize) {
    File dbPath = new File(documentsDirectoryPath, dbName);
    if (!dbPath.exists()) {
      dbPath.mkdirs(); // with intermediate directories
    }
    nativeOpen(dbPath.getPath(), mapSize);
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void put(String key, String value) {
    nativePut(key, value);
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public String get(String key) {
    return nativeGet(key);
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void del(String key) {
    nativeDel(key);
  }
}
