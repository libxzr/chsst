# chsst

**ch**ange **s**hared **s**ub**t**rees

## Introduction

[Shared subtree](https://www.kernel.org/doc/html/latest/filesystems/sharedsubtree.html) is a kernel feature which has a wide usage in conjunction with [mount namespace](https://man7.org/linux/man-pages/man7/mount_namespaces.7.html). Android kernel does support this feature and it is already in use to propagate mount events between isolated namespaces. But unfortunately, Android has their own implementation of `mount` command in [toybox](https://android.googlesource.com/platform/external/toybox/), which [doesn't support this feature yet](https://android.googlesource.com/platform/external/toybox/+/refs/tags/android-13.0.0_r6/toys/lsb/mount.c#74). So this makes it hard to debug and play with shared subtrees. This is why `chsst` was born.

## Usage

Get prebuilts from [releases](https://github.com/libxzr/chsst/releases).

Push the binary matching your architecture via adb. For example arm64-v8a.
```
$ adb root
$ adb push <path-to-prebuilts>/arm64-v8a/chsst /data
```

Run it and see help.
```
$ adb shell chmod u+x /data/chsst
$ adb shell /data/chsst --help
chsst (CHange Shared SubTrees) by LibXZR <i@xzr.moe>
Usage:
 chsst <mountpoint> <operation>
 chsst <operation> <mountpoint>
Operations:
 --make-shared           mark a subtree as shared
 --make-slave            mark a subtree as slave
 --make-private          mark a subtree as private
 --make-unbindable       mark a subtree as unbindable
 --make-rshared          recursively mark a whole subtree as shared
 --make-rslave           recursively mark a whole subtree as slave
 --make-rprivate         recursively mark a whole subtree as private
 --make-runbindable      recursively mark a whole subtree as unbindable
```

The usage is basically same as that of `mount` in util-linux.

## Building

It's easy to build it with [Android NDK](https://developer.android.com/ndk).

```
$ export PATH="/path/to/your/ndk:${PATH}"
$ git clone https://github.com/libxzr/chsst
......
$ cd chsst
$ ndk-build
......
$ tree libs          
libs
├── arm64-v8a
│   └── chsst
├── armeabi-v7a
│   └── chsst
├── x86
│   └── chsst
└── x86_64
    └── chsst
```

## License

[MIT](LICENSE)
