#include "dk2nuTree.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <filename.root> <fnameID>" << std::endl;
    return 1;
  }

  TString filename = argv[1];
  TString fnameID = argv[2];

  dk2nuTree analysis(filename, fnameID);

  return 0;
}
