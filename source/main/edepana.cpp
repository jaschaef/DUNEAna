#include "Utils.h"
#include "Looper.h"

#include <iostream>

int main(int argc, char **argv)
{
  if(argc < 4) {
    std::cout << "Usage: edepana InputFolder [Endcap/Barrel] OutputFile" << std::endl;
    return -1;
  }

  std::string InputFolder = argv[1];
  std::string Type = argv[2];
  std::string OutputFile = argv[3];

  std::cout << "InputFolder " << InputFolder << std::endl;
  std::cout << "Analysis of the " << Type << std::endl;
  std::cout << "OutputFile " << OutputFile << std::endl;

  Utils U(InputFolder, OutputFile, Type);

  Looper L(U, "edep");
  L.Init();
  std::cout << "Loop over Rootfile" << std::endl;
  L.loop_edep();

  std::cout << "Write histograms" << std::endl;
  L.WriteHistograms();
  L.CloseTFile();

  return 0;
}
