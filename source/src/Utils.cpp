#include "Utils.h"

#include "TSystemDirectory.h"
#include "TList.h"
#include "TSystemFile.h"

Utils::Utils(std::string InputFolder, std::string OutputFile, std::string Type)
: _vecFile(), _file(nullptr), _type(Type), _inputFolder(InputFolder), _outputFile(OutputFile)
{

}

Utils::~Utils()
{
    delete _file;
}

bool Utils::GetListOfFiles()
{
    //Get the list of files
    TSystemDirectory dir("inputFolder", GetInputFolder().c_str());
    TList *files = dir.GetListOfFiles(); //Displays a list of all files

    bool status = false;

    if (files) {
        files->Sort();
        TObject *obj;
        TIter next(files);

        while ( (obj = (TObject*)next()) ) {
            if (std::string(obj->GetName()).find(".root") != std::string::npos && std::string(obj->GetName()).find(GetTypeStr().c_str()) != std::string::npos) {
                std::string FullFileName = std::string(obj->GetName());
                _vecFile.push_back( std::string(GetInputFolder() + "/" + FullFileName) );
            }
        }

        status = true;
    }

    return status;
}

bool Utils::BookTFile()
{
    _file = new TFile(GetOutputFile().c_str(), "RECREATE");
    if(_file != nullptr) return true;
    else { return false; }
}

bool Utils::CloseTFile()
{
    if(_file != nullptr) {
        _file->Close();
        return true;
    }
    else{ return false; }
}

bool Utils::WriteHistogram(TH1 *h)
{
    if(_file != nullptr) {
        _file->cd();
        h->Write();
        return true;
    }
    else { return false; }
}
