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

  public LmdbModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  private static native void nativeInitialize(long jsiPtr, String docDir);
  private static native void nativeDestruct();

  @ReactMethod(isBlockingSynchronousMethod = true)
  public void install() {
      System.loadLibrary("react-native-lmdb"); // as defined in CMakeLists.txt
      nativeInitialize(
        this.getReactApplicationContext().getJavaScriptContextHolder().get(),
        this.getReactApplicationContext().getFilesDir().getAbsolutePath()
      );
  }

  @Override
  public void onCatalystInstanceDestroy() {
    nativeDestruct();
  }
}
