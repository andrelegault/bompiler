#include <iostream>
#include "bompilerConfig.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    // report version
    std::cout << argv[0] << "Version " << bompiler_VERSION_MAJOR << "."
      << bompiler_VERSION_MINOR << "." << bompiler_VERSION_PATCH << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }
}