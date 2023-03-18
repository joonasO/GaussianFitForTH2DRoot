
#ifndef __GlobalFunctions__
#include "GlobalFunctions.h"
#endif
#ifndef __ReadFile__
#include "ReadFile.h"
#endif
#ifndef __Settings__
#include "Settings.h"
#endif
#ifndef __ReadNTuple__
#include "ReadNTuple.h"
#endif
#ifndef __ReadMatrix__
#include "ReadMatrix.h"
#endif
#ifndef __FitGaussians__
#include "FitGaussians.h"
#endif
#ifndef __WriteOutput__
#include "WriteOutput.h"
#endif
#ifndef __DrawFigure__
#include "DrawFigure.h"
#endif

Bool_t Debug;
TString globalRootFileName;
std::vector<TString> globalHistogramName;


//#include"ReadNTuple.h"
// TH1F *histogram,TString output=" "
void gaussianFitter(TString inputPath,TString outputFileName,TString histogramFileName,TString rootFileName){

	ReadFile file;
	file.readFile(inputPath);
	file.readRootFile(rootFileName);
	file.readHistogram(histogramFileName);
	ReadNTuple nTuple;
	nTuple.readOutNTuple(file.peakData);
  	FitGaussians fit;
	fit.mainFitter(file.histogram,nTuple.peakEnergies(), nTuple.energiesLow(), nTuple.energiesHigh(),nTuple.peakWidths(),outputFileName);
	WriteOutput output;
	output.initialiseOutput(outputFileName);
	output.readFunction(file.histogram,nTuple.energiesLow(),nTuple.energiesHigh(),fit.fGaussiansMatrix,fit.fGaussianPlusBGs,fit.fBackgrounds,fit.fFitParametersVector,outputFileName,Debug);
	DrawFigure figure;
	figure.makeToFigure(file.histogram,outputFileName,fit.fGaussiansPlusBackgroundNames,nTuple.energiesLow(), nTuple.energiesHigh(),fit.fBackgrounds,fit.fGaussiansMatrix,fit.fGaussianPlusBGs,fit.fFitParametersVector);
	//delete file;
	//delete nTuple;
	//delete output;
	//delete figure;
	//ReadNTuple *peakInformation=new ReadNTuple(file->returnPeakData());
}

void gaussianFitterMatrix(TString matrixHistogramName,TString matrixGateFileName,TString outputFileName,TString matrixPeakFileName,TString rootFileName){

	ReadFile file;
	
	file.readRootFile(rootFileName);
	std::cout << "Root File Is Read" << '\n';
	file.readFile(matrixPeakFileName);
	std::cout << "Input Matrix read" << '\n';
	file.readMatrix(matrixHistogramName);
	file.readMatrixGatesFile(matrixGateFileName);
	std::cout << "Input MatrixGate read" << '\n';
	ReadNTuple nTuple;
	WriteOutput output;
	DrawFigure figure;
	ReadMatrix matrixHistogram;
	for (Int_t i = 0; i < file.gate1Vector.size(); i++) {
		
		matrixHistogram.createHistogram(file.matrix,file.gateOnAxis[i],file.gate1Vector[i],file.gate2Vector[i],file.backgroundGate1Vector[i],file.backgroundGate2Vector[i],file.backgroundGate3Vector[i],file.backgroundGate4Vector[i]);
		std::cout << "Input Create Histograms" << '\n';
		nTuple.readOutNTuple(file.peakData);
		FitGaussians fit;
		fit.mainFitter(matrixHistogram.histogram,nTuple.peakEnergies(), nTuple.energiesLow(), nTuple.energiesHigh(),nTuple.peakWidths(),outputFileName);
		output.initialiseOutput(outputFileName);
		output.readFunction(file.histogram,nTuple.energiesLow(),nTuple.energiesHigh(),fit.fGaussiansMatrix,fit.fGaussianPlusBGs,fit.fBackgrounds,fit.fFitParametersVector,outputFileName,Debug);
		figure.makeToFigure(file.histogram,outputFileName,fit.fGaussiansPlusBackgroundNames,nTuple.energiesLow(), nTuple.energiesHigh(),fit.fBackgrounds,fit.fGaussiansMatrix,fit.fGaussianPlusBGs,fit.fFitParametersVector);

		
		
	}
	//delete nTuple;
	//delete file;
	//delete output;
	//delete figure;

	//ReadNTuple *peakInformation=new ReadNTuple(file->returnPeakData());
}
int main (){
	std::cout << "Settings file to be read" << '\n';
	Settings settings;
	std::cout << "Settings object created" << '\n';
	settings.setSettings("../Settings/Settings.txt");
	std::cout << "Settings set settings" << '\n';
	TString settingsFileName= settings.settingsFile();
	TString outputFileName= settings.outputFileName();
	TString rootFileName= settings.rootFileName();
	std::vector<TString>  matrixHistogramName= settings.matrixHistogramName();
	std::vector<TString>  matrixGateFileName= settings.matrixGateFileName();
	std::vector<TString>  matrixPeakFileName= settings.matrixPeakFileName();
	std::vector<TString>  inputHistogramFileName= settings.inputFileName();
	std::vector<TString>  histogramFileName= settings.histogramFileName();
	std::cout << "Settings file is read" << '\n';
	

	Debug=true;
	if(Debug){

		cout<<endl<<"Debug_Mode"<<endl;
		cout<<endl<<"Settings file name: "<<settingsFileName<<endl;
		cout<<endl<<"Output file name: "<<outputFileName<<endl;
		cout<<endl<<"Root file name: "<<rootFileName<<endl;
	}
	if(histogramFileName.size()==inputHistogramFileName.size()){
		for (Int_t i = 0; i < histogramFileName.size(); i++) {
			gaussianFitter(inputHistogramFileName[i],outputFileName,histogramFileName[i],rootFileName);
		}
	}
	else{
		std::cout << "Number of histograms and input files doesn't match!" << '\n';
	}
	cout<<endl<<"How many matrix file: "<<matrixGateFileName.size()<<endl;
	cout<<endl<<"How many Gatematrix file: "<<matrixPeakFileName.size()<<endl;
	if (matrixGateFileName.size()==matrixPeakFileName.size()){
		for (Int_t i = 0; i < histogramFileName.size(); i++) {
			gaussianFitterMatrix(matrixHistogramName[i],matrixGateFileName[i],outputFileName,matrixPeakFileName[i],rootFileName);
		}
	}
	return 0;
}
