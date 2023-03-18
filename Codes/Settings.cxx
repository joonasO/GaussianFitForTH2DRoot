#include "Settings.h"

Settings::Settings(){
};

Settings::~Settings(){
  cout << "destructor" << endl;
};
void Settings::setSettings(TString filename){
  std::cout << "Setting" << '\n';
  setFile( filename );
  std::cout << "Filename Set" << '\n';
  readSettings();

  if(fDebugOpt)
    std::cout << "Debug" << '\n';
    printSettings();
};


TString Settings::readSettingsFile(TString line){
  ifstream in;
  TString text, returnValue;
  in.open(fSettingsFileName.Data());
  while(1){
      in>>text>>returnValue;
    if(strcmp(text.Data(),line.Data())==0){
      return returnValue;
    }
}
std::cout << "Error in settings file: "<<line << '\n';
}

std::vector<TString> Settings::readSettingsFileVector(TString line){
  ifstream in;
  TString text,returnValue;
  std::vector<TString> returnValueVector;
  in.open(fSettingsFileName.Data());
  while(!in.eof()){
      in>>text>>returnValue;

    if(strcmp(text.Data(),line.Data())==0){
      returnValueVector.push_back(returnValue);
    }
}
return returnValueVector;
}


void Settings::readSettings(){
  fDebugOpt = true;
  
  fOutputFileName = readSettingsFile("Output_File_Name:");
  //fOutputRootFileName = readSettingsFile("Output_Root_File_Name:");
  fRootFileName= readSettingsFile("Root_File_Name:");
  fMatrixHistogramFileName=readSettingsFileVector("Matrix_Histogram_Name:");
  fMatrixGateFileName= readSettingsFileVector("Matrix_Gate_File:");
  fMatrixPeakFileName= readSettingsFileVector("Matrix_Peak_File:");
  fInputFileName= readSettingsFileVector("Input_File_Name:");
  fHistogramFileName= readSettingsFileVector("Histogram_Name:");
  globalRootFileName=fRootFileName;
  globalHistogramName=fHistogramFileName;
  if(fDebugOpt)
    cout << " ... done!" <<endl<<"settingsFileName"<<fSettingsFileName.Data()<< endl;
    cout << " ... done!" <<endl<<"roofFileName"<<fRootFileName.Data()<< endl;
    cout << " ... done!" <<endl<<"globalroofFileName"<<globalRootFileName.Data()<< endl;


};
void Settings::printSettings(){

  cout << endl << "List of Input files:" << endl;
  for (Int_t i = 0; i < fInputFileName.size(); i++) {
  cout<< endl <<"Input File: "<< fInputFileName[i]<< endl;

  }


};

Bool_t Settings::checkIntpuFile(TString PathAndName){

  ifstream CheckFile;
  CheckFile.open(PathAndName.Data());
  if (!CheckFile.good()) {
    cerr << "   Error invalid file: " << PathAndName.Data() << " !!!" << endl;
    return false;
  }
  CheckFile.close();

  return true;
};
