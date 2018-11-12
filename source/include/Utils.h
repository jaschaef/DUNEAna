#ifndef __Utils_H__
#define __Utils_H__ 1

#include <string>
#include <vector>

#include "TFile.h"
#include "TH1.h"

class Utils
{
public:

    Utils(std::string InputFolder, std::string OutputFile, std::string Type);

    ~Utils();

    std::vector<std::string> GetVectorFiles() { return _vecFile; }

    std::string GetTypeStr() { return _type; }

    std::string GetInputFolder() { return _inputFolder; }

    std::string GetOutputFile() { return _outputFile; }

    bool GetListOfFiles();

    bool BookTFile();

    bool WriteHistogram(TH1 *h);

    bool CloseTFile();
    
protected:

private:
    std::vector<std::string> _vecFile;
    TFile* _file;
    std::string _type;
    std::string _inputFolder;
    std::string _outputFile;
};

#endif
