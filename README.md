TLDR:

GHC crashes when it tries to link a library that uses C++ while compiling a file that uses Template Haskell with
```
ghc.exe: internal error: checkProddableBlock: invalid fixup in runtime linker: 0000000000380564
```

LAYOUT:

A cabal package with:

A library, `cxxylib`, which consists of a single C++ function in `cxxy.cpp` and an FFI import of that fuction in `CxxyLib.hs`.
An executable which depends on `cxxylib` and simply calls the FFI'd function in `main`.

WHAT WORKS:

The problems only occurs if I both add an exception handler to the C++ file, and use Template Haskell in the client library.

If I don't use Template Haskell, and I just add `libstdc++` to cxxylib.cabal:
```
  extra-libraries:     libstdc++
```
Everything works perfectly, and I can call into my FFI'd C++ function from the client executable.

Everything also works even with exceptions and Template Haskell if I just call `cabal exec -- ghc app/Main.hs`. This is presumably because GHC doesn't try to link cxxylib when it is just compiling the Template Haskell splices, whereas Cabal asks it to.

WHAT DOESN'T:

If I then use Template Haskell, GHC can't find libstdc++ anymore while compiling Main.hs:
```
$ cabal clean && cabal run
cleaning...
Package has never been configured. Configuring with default flags. If this
fails, please run configure manually.
Resolving dependencies...
Configuring lasagna-0.1.0.0...
Preprocessing executable 'lasagna' for lasagna-0.1.0.0...
[1 of 2] Compiling Printf           ( src\Printf.hs, dist\build\lasagna\lasagna-tmp\Printf.o )
[2 of 2] Compiling Main             ( src\Main.hs, dist\build\lasagna\lasagna-tmp\Main.o )
ghc.exe: libstdc++: The specified module could not be found.

<no location info>:
    <command line>: can't load .so/.DLL for: libstdc++.dll (addDLL: could not load DLL)
```


If I then add the path to where libstdc++ resides, and give it the exact name, in cxxylib:
```
  extra-libraries:     libstdc++-6
  extra-lib-dirs:      C:\msys64\usr\local\ghc-7.10.1\mingw\bin
```

GHC can't find `_Unwind_SjLj_Register`:
```
$ cabal clean && cabal run
cleaning...
Package has never been configured. Configuring with default flags. If this
fails, please run configure manually.
Resolving dependencies...
Configuring lasagna-0.1.0.0...
Preprocessing executable 'lasagna' for lasagna-0.1.0.0...
[1 of 2] Compiling Printf           ( src\Printf.hs, dist\build\lasagna\lasagna-tmp\Printf.o )
[2 of 2] Compiling Main             ( src\Main.hs, dist\build\lasagna\lasagna-tmp\Main.o )
ghc.exe: C:\msys64\home\lukex_000\cxx-link-fail-repro\.cabal-sandbox\x86_64-windows-ghc-7.10.1\cxxylib-0.1.0.0-GXd3hzZmGM0AbZL9shGGsy\HScxxylib-0.1.0.0-GXd3hzZmGM0AbZL9shGGsy.o: unknown symbol `_Unwind_SjLj_Register'

<no location info>:
    ghc.exe: unable to load package `cxxylib-0.1.0.0'
```


If I then add libgcc_s_sjlj-1 to the dependencies:
```
  extra-libraries:     libgcc_s_sjlj-1 libstdc++-6
  extra-lib-dirs:      C:\msys64\usr\local\ghc-7.10.1\mingw\bin
```

I get this:
```
$ cabal clean && cabal run
Package has never been configured. Configuring with default flags. If this
fails, please run configure manually.
Resolving dependencies...
Configuring lasagna-0.1.0.0...
Preprocessing executable 'lasagna' for lasagna-0.1.0.0...
[1 of 2] Compiling Printf           ( src\Printf.hs, dist\build\lasagna\lasagna-tmp\Printf.o )
[2 of 2] Compiling Main             ( src\Main.hs, dist\build\lasagna\lasagna-tmp\Main.o )
ghc.exe: internal error: checkProddableBlock: invalid fixup in runtime linker: 0000000000380564
    (GHC version 7.10.1 for x86_64_unknown_mingw32)
    Please report this as a GHC bug:  http://www.haskell.org/ghc/reportabug

This application has requested the Runtime to terminate it in an unusual way.
Please contact the application's support team for more information.
```


In my own library, even if I pass `cc-options: -fno-exceptions` (which allows me to remove `libgcc_s_sjlj-1.dll`) I can trigger the `checkProddableBlock` error.