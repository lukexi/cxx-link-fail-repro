As a last attempt, if I define the dependencies like this in cxxylib:
```
  extra-libraries:     stdc++
  extra-lib-dirs:      C:\msys64\usr\local\ghc-7.10.1\mingw\lib
```

I get this:
```
$ cabal clean && cabal run
Preprocessing executable 'lasagna' for lasagna-0.1.0.0...
[2 of 2] Compiling Main             ( src\Main.hs, dist\build\lasagna\lasagna-tmp\Main.o )
GHC runtime linker: fatal error: I found a duplicate definition for symbol
   _ZNK9__gnu_cxx24__concurrence_lock_error4whatEv
whilst processing object file
   C:\msys64\usr\local\ghc-7.10.1\mingw\lib\libstdc++.a
This could be caused by:
   * Loading two different object files which export the same symbol
   * Specifying the same object file twice on the GHCi command line
   * An incorrect `package.conf' entry, causing some object to be
     loaded twice.

<no location info>:
    ghc.exe: panic! (the 'impossible' happened)
  (GHC version 7.10.1 for x86_64-unknown-mingw32):
        loadArchive "C:\\msys64\\usr\\local\\ghc-7.10.1\\mingw\\lib\\libstdc++.a": failed

Please report this as a GHC bug:  http://www.haskell.org/ghc/reportabug
```