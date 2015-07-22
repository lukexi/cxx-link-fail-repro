#include <iostream>
#include <exception>
#include <string.h>

#define USE_EXCEPTION 1

// Make sure can call unmangled names from Haskell's FFI
extern "C" {

int talkToCxx() {

#if USE_EXCEPTION
  try {
    throw 20;
  }
  catch (int e) {
    std::cout << "An exception occurred. Exception Nr. " << e << '\n';
  }
#endif

  std::cout << "Hello From C++!";
}


}