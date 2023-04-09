#include "WriteOutput.h"

WriteOutput::WriteOutput(){};
WriteOutput::~WriteOutput(){
  cout << "destructor WriteOutput" << endl;
};
Double_t WriteOutput::binning(TH1F *h1){
  Double_t bin1,bin2,binDifference;
  bin1= h1->GetXaxis()->GetBinCenter(1);
  bin2= h1->GetXaxis()->GetBinCenter(2);
  binDifference=bin2-bin1;
  return binDifference;
};

void WriteOutput::initialiseOutput(TString output){

	TString dir = gSystem->UnixPathName(__FILE__);
	dir.ReplaceAll("WriteOutput.cxx","");
  dir.ReplaceAll("/./","/");
  TString toSystem="mkdir ";
  toSystem=toSystem.Append(dir);
  toSystem=toSystem.Append("../Output/");
  system(toSystem.Data());
  toSystem=toSystem.Append(output);
  toSystem=toSystem.Append("/");
  cout<<"OutputFolder:"<<toSystem.Data()<<"\n";
  system(toSystem.Data());

  TString rootFolder=toSystem.Copy();
  rootFolder=rootFolder.Append("RootFiles/");
  system(rootFolder.Data());
  wRootOutputPath=rootFolder;
  TString fitDataFolder=toSystem.Copy();
  fitDataFolder=fitDataFolder.Append("Fits/");
  system(fitDataFolder.Data());
};
void WriteOutput::initialiseExcelOutput(TString output,TString gate1, TString gate2){
	ofstream toExcel;
  TString toExcelFile;
  toExcelFile="../Output/";
  toExcelFile=toExcelFile.Append(output);
  toExcelFile=toExcelFile.Append("/Fits/");
  toExcelFile=toExcelFile.Append(output);
  wOutputPath=toExcelFile.Copy();
  toExcelFile.Append("ToExcel");
  toExcelFile.Append("_Gate_");
  toExcelFile.Append(gate1);
  toExcelFile.Append("_");
  toExcelFile.Append(gate2);
  toExcelFile.Append(".txt");
  toExcel.open(Form("%s",toExcelFile.Data()));
  toExcel<<output.Data()<<endl;
  toExcel<< "Energy"<<" "<< "Energy_Error"<<" "<< "Calculated_Area" << " "<<"Calculated_Area_Error" <<" "<<"Numerical_Area"<<  " "<<"Numerical_Area_Error"<<  " "<<"Total_Peak_Area"<<  " "<<"Total_Peak_Area_Error"<<" Sigma"<<" Sigma_error"<<endl;
  toExcel.close();
  wGate1=gate1;
  wGate2=gate2;
};

void WriteOutput::writeRootOutputFile(TFile *file, TH1F *Raw, TH1F *Background1, TH1F *Background2, TH1F *substractedSpectrum, TH1F *BackgroundSubstactedTotal){
	TString title="";
	title.Append(Raw->GetTitle());
	file->WriteObject(Raw, title.Data());
	title="";
	title.Append(Background1->GetTitle());
	file->WriteObject(Background1, title.Data());
	title="";
	title.Append(Background2->GetTitle());
	file->WriteObject(Background2, title.Data());
	title="";
	title.Append(substractedSpectrum->GetTitle());
	file->WriteObject(substractedSpectrum, title.Data());
	title="";
	title.Append(BackgroundSubstactedTotal->GetTitle());
	file->WriteObject(BackgroundSubstactedTotal, title.Data());
};


void WriteOutput::readFunction(TH1F* h1, std::vector<Float_t> lows,std::vector<Float_t> highs,std::vector<std::vector<TF1 *>> gaussianFunctions , std::vector<TF1 *> gaussianPlusBackground,std::vector<TF1 *> backgrounds,  std::vector<TFitResultPtr> fitParameters,TString output,Bool_t Debug){
	Int_t j=0;
	for(Int_t i = 0; i < gaussianPlusBackground.size(); i++){
    cout<<"j"<<j<<endl;
    cout<<"Low"<<lows[j]<<endl;
    cout<<"High"<<highs[j]<<endl;
    cout<<"Gaussian"<<gaussianFunctions[i].size()<<endl;
		calculateArea(h1,fitParameters[i],lows[j],highs[j], gaussianPlusBackground[i],backgrounds[i],gaussianFunctions[i],output,Debug);
		j=j+gaussianPlusBackground[i]->GetParameter(0);
	}


};
void WriteOutput::calculateArea(TH1F* h1 ,TFitResultPtr fitParameters, Float_t low,Float_t	high, TF1* gaussianPlusBG,TF1* background,std::vector<TF1 *> gaussian,TString output, Bool_t Debug){
	Double_t binDifference=binning(h1);
	//Double_t binDifference=1;
	Int_t numberOfPeaks=gaussianPlusBG->GetParameter(0);
	//fitParameters->Print("V");
	TMatrixDSym covarianceMatrix=fitParameters->GetCovarianceMatrix();
	TMatrixDSym covarianceMatrixSingle(5);
	Double_t *parametersBackgroud=new Double_t[2];
	parametersBackgroud[0]=gaussianPlusBG->GetParameter(1);
	parametersBackgroud[1]=gaussianPlusBG->GetParameter(2);
  cout<<"Background Constant value :"<<gaussianPlusBG->GetParameter(1)<<endl;
  cout<<"Background Linear value :"<<gaussianPlusBG->GetParameter(2)<<endl;
  cout<<"Background Constant value2 :"<<background->GetParameter(0)<<endl;
  cout<<"Background Constant value2 :"<<background->GetParameter(1)<<endl;
	TMatrixDSym covarianceMatrixBackground(2);
	for (Int_t i=1;i<3;i++){
		TMatrixDRow(covarianceMatrixBackground,i-1)[i-1]= covarianceMatrix(i,i);
		cout<<covarianceMatrix(i,i)<<endl;
		if(i==1){
			TMatrixDRow(covarianceMatrixBackground,i)[i-1]=covarianceMatrix(i+1,i);
			TMatrixDRow(covarianceMatrixBackground,i-1)[i]=covarianceMatrix(i,i+1);
			cout<<covarianceMatrix(i+1,i)<<endl;
		}
	}
	Double_t peakPlusBackgroundIntegral = (gaussianPlusBG->Integral(low,high))/binDifference;
	Double_t peakPlusBackgroundIntegralError = (gaussianPlusBG->IntegralError(low,high,fitParameters->GetParams(),covarianceMatrix.GetMatrixArray()))/binDifference;
	Double_t backgroundIntegral = (background->Integral(low,high))/binDifference;
	Double_t backgroundIntegralError = (background->IntegralError(low,high,parametersBackgroud,covarianceMatrixBackground.GetMatrixArray()))/binDifference;
  Double_t totalPeakArea=peakPlusBackgroundIntegral-backgroundIntegral;
  Double_t totalPeakAreaError=TMath::Power(TMath::Power(peakPlusBackgroundIntegralError,2)+TMath::Power(backgroundIntegralError,2),0.5);
  Double_t histogramIntegral = h1->Integral(low/binDifference,high/binDifference);
	Double_t polynom_constant,polynom_linear,polynom_quadratic;
  std::vector<Double_t> constant,constant_error,mean,mean_error,sigma,sigma_error,fwhm,fwhm_error;
	for (Int_t i=0; i<numberOfPeaks;i++){
		constant.push_back(gaussianPlusBG->GetParameter(i*3+3));
		constant_error.push_back(gaussianPlusBG->GetParError(i*3+3));
		mean.push_back(gaussianPlusBG->GetParameter(i*3+4));
		mean_error.push_back(gaussianPlusBG->GetParError(i*3+4));
		sigma.push_back(gaussianPlusBG->GetParameter(i*3+5));
		sigma_error.push_back(gaussianPlusBG->GetParError(i*3+5));
		fwhm.push_back(2*TMath::Power(2*TMath::Log(2),0.5)*sigma[i]);
		fwhm_error.push_back(2*TMath::Power(2*TMath::Log(2),0.5)*sigma_error[i]);
	}

	Double_t peakNumericalIntegral[numberOfPeaks],peakNumericalIntegralError[numberOfPeaks],peakCalculatedIntegral[numberOfPeaks],peakCalculatedIntegralError[numberOfPeaks];
	Double_t errorSquared,error1,error2;

	TString titleResult=output.Copy();
	titleResult.Append("_");
	TString functionName=gaussianPlusBG->GetTitle();
	titleResult.Append(functionName);
	TString number;

		for (Int_t i=1;i<3;i++){
			TMatrixDRow(covarianceMatrixSingle,i-1)[i-1]= covarianceMatrix(i,i);
			if(i==1){
				TMatrixDRow(covarianceMatrixSingle,i)[i-1]=covarianceMatrix(i+1,i);
				TMatrixDRow(covarianceMatrixSingle,i-1)[i]=covarianceMatrix(i,i+1);
			}
		}
		for(Int_t i=0;i<numberOfPeaks;i++){
			for (Int_t j=2; j<5;j++){
			for (Int_t k=2; k<5;k++){
			covarianceMatrixSingle(j,k)=covarianceMatrix(i*3+j+1,i*3+k+1);


			}

			covarianceMatrixSingle(0,j)=covarianceMatrix(1,i*3+j+1);
			covarianceMatrixSingle(1,j)=covarianceMatrix(2,i*3+j+1);
			covarianceMatrixSingle(j,0)=covarianceMatrix(1,i*3+j+1);
			covarianceMatrixSingle(j,1)=covarianceMatrix(2,i*3+j+1);

		}
	if(false){
		cout<< "Fit Parameters:"<<endl;
		fitParameters->Print("V");
    cout<< "Covariance Matrix For Background:"<<endl;
		covarianceMatrixBackground.Print();
    cout<< "Covariance Matrix For Single:"<<endl;
		covarianceMatrixSingle.Print();
		cout<< "Covariance Matrix Total:"<<endl;
		covarianceMatrix.Print();
		cout<< "Matrix:"<<endl;
	}
		for (Int_t j=0; j<5;j++){
		for (Int_t k=0; k<5;k++){
			if(false){


				cout<<covarianceMatrixSingle(k,j)<<", ";
			}
		}
		if(false){
			cout<<endl;
		}
	}
			if(false){
				std::cout << "Parameters To Check:" << '\n';
				std::cout << "Background Constant: " << gaussianPlusBG->GetParameter(1)<<'\n';
				std::cout << "Background Linear: " << gaussianPlusBG->GetParameter(2)<<'\n';
				std::cout << "Gaussian Constant: " << constant[i]<<'\n';
				std::cout << "Gaussian mean: " << mean[i]<<'\n';
				std::cout << "Gaussian sigma: " << sigma[i]<<'\n';
        std::cout << "Gaussian low: " << low <<'\n';
        std::cout << "Gaussian high: " << high <<'\n';
        std::cout << "Gaussian Constant2: " << gaussian[i]->GetParameter(2) <<'\n';
        std::cout << "Gaussian mean2: " << gaussian[i]->GetParameter(3) <<'\n';
        std::cout << "Gaussian sigma: " << gaussian[i]->GetParameter(4) <<'\n';
        std::cout << "Bin Difference: " << binDifference <<'\n';
        std::cout << "integral: " << (gaussian[i]->Integral(low*binDifference,high*binDifference)) <<'\n';
        std::cout << "gaussianPlusBG integral: " << (gaussianPlusBG->Integral(low*binDifference,high*binDifference)) <<'\n';
        std::cout << "background integral: " << (background->Integral(low*binDifference,high*binDifference)) <<'\n';
        std::cout << "Gaussian Constant3: " << gaussian[1]->GetParameter(2) <<'\n';
        std::cout << "Gaussian mean3: " << gaussian[1]->GetParameter(3) <<'\n';
        std::cout << "Gaussian sigma3: " << gaussian[1]->GetParameter(4) <<'\n';
        std::cout << "integral3: " << (gaussian[1]->Integral(low*binDifference,high*binDifference)) <<'\n';
        gaussian[i]->Print();
			}
      Double_t *parametersPeak=new Double_t[5];

      parametersPeak[0]=gaussianPlusBG->GetParameter(1);
      parametersPeak[1]=gaussianPlusBG->GetParameter(2);
      parametersPeak[2]= constant[i];
      parametersPeak[3]= mean[i];
      parametersPeak[4]= sigma[i];
			peakNumericalIntegral[i] = (gaussian[i]->Integral(low,high))/binDifference;
			peakNumericalIntegralError[i] =(gaussian[i]->IntegralError(low,high,parametersPeak,covarianceMatrixSingle.GetMatrixArray()))/binDifference;
			peakCalculatedIntegral[i]=(sigma[i]*constant[i]*TMath::Power(2*TMath::Pi(),0.5))/binDifference;
			error1=sigma_error[i]*constant[i]*TMath::Power(2*TMath::Pi(),0.5);
			error2=sigma[i]*constant_error[i]*TMath::Power(2*TMath::Pi(),0.5);
			errorSquared=TMath::Power(error1,2)+TMath::Power(error2,2);
			peakCalculatedIntegralError[i]=(TMath::Power(errorSquared,0.5))/binDifference;
			number.Form("%d",i);
			titleResult.Append("_");
			titleResult.Append(number);
			doOutput(titleResult, peakCalculatedIntegral[i], peakCalculatedIntegralError[i], peakNumericalIntegral[i], peakNumericalIntegralError[i],totalPeakArea,totalPeakAreaError,peakPlusBackgroundIntegral, peakPlusBackgroundIntegralError, histogramIntegral, backgroundIntegral, backgroundIntegralError, constant[i], constant_error[i], mean[i],mean_error[i], sigma[i], sigma_error[i], fwhm[i], fwhm_error[i], polynom_constant, polynom_linear, output, fitParameters, Debug);
      delete[] parametersPeak;
  }
};

void WriteOutput::doRootFile(TString title,TString gate1,TString gate2,TString backgroundGate1, TString backgroundGate2,TString backgroundGate3, TString backgroundGate4, TH1F* histogram,TH1F* histogramTotal,TH1F* histogramBackground1,TH1F* histogramBackground2 ){
  TString rootFileName=wOutputPath.Copy();
  TString nameTotal;
  TString nameHistogram;
  rootFileName.Append(title);
  rootFileName.Append(".root");
  TFile * rootFile=new TFile(rootFileName);

  nameTotal=histogramTotal->GetTitle();
  nameTotal.Append("_Total");
  nameTotal.Append("_gate_");
  nameTotal.Append(gate1);
  nameTotal.Append("_");
  nameTotal.Append(gate2);

  histogramTotal->SetName(nameTotal.Data());
  histogramTotal->SetTitle(nameTotal.Data());
  if (backgroundGate1.CompareTo("-1")==0){
    nameHistogram=histogram->GetTitle();
    nameHistogram.Append("_Projection");
    nameHistogram.Append("_gate_");
    nameHistogram.Append(gate1);
    nameHistogram.Append("_");
    nameHistogram.Append(gate2);
    histogram->SetName(nameHistogram.Data());
    histogram->SetTitle(nameHistogram.Data());
  }
  if (backgroundGate1.CompareTo("-1")!=0&&backgroundGate2.CompareTo("-1")==0){
    nameHistogram.Append("_Projection");
    nameHistogram=histogram->GetTitle();
    nameHistogram.Append("_gate_");
    nameHistogram.Append(gate1);
    nameHistogram.Append("_");
    nameHistogram.Append(gate2);
    histogram->SetName(nameHistogram.Data());
    histogram->SetTitle(nameHistogram.Data());
  }

}
void WriteOutput::doOutput(TString title,Double_t peakCalculatedIntegral,Double_t peakCalculatedIntegralError,Double_t peakNumericalIntegral,Double_t peakNumericalIntegralError,Double_t totalPeakArea,Double_t totalPeakAreaError,Double_t peakPlusBackgroundIntegral,Double_t peakPlusBackgroundIntegralError,Double_t histogramIntegral,Double_t backgroundIntegral,Double_t backgroundIntegralError,Double_t constant,Double_t constant_error,Double_t mean,Double_t mean_error,Double_t sigma,Double_t sigma_error,Double_t fwhm,Double_t fwhm_error,Double_t polynom_constant,Double_t polynom_linear,TString output,TFitResultPtr fitParameters,Bool_t Debug){
	Double_t peakArea=peakNumericalIntegral-backgroundIntegral;
  Double_t peakAreaError=TMath::Power(TMath::Power(peakNumericalIntegralError,2)+TMath::Power(backgroundIntegralError,2),0.5);
	if(false){
		cout << "Peak calculated integral+-Error " << peakCalculatedIntegral<<"+-"<<peakCalculatedIntegralError<< endl;
  	cout << "Peak plus background numerical integral+-Error " << peakNumericalIntegral<<"+-"<<peakNumericalIntegralError<< endl;
  	cout << "Background integral +-Error" << backgroundIntegral <<"+-"<<backgroundIntegralError<< endl;
  	cout << "Peak integral +-Error" << peakArea <<"+-"<<peakAreaError<< endl;
  	cout << "Total function integral +-Error " << peakPlusBackgroundIntegral<<"+-"<<peakPlusBackgroundIntegralError<< endl;
  	cout << "Histogram integral " <<histogramIntegral << endl;

	}
		TMatrixDSym covarianceMatrix=fitParameters->GetCovarianceMatrix();
		TMatrixDSym correlationMatrix=fitParameters->GetCorrelationMatrix();
		TString covarianceMatrixFile=wOutputPath.Copy();
		covarianceMatrixFile.Append(title);
		covarianceMatrixFile.Append("_");
		TString correlationMatrixFile=covarianceMatrixFile.Copy();
		correlationMatrixFile.Append("_Correlation.xml");
		covarianceMatrixFile.Append("_Covariance.xml");
		covarianceMatrix.SaveAs(covarianceMatrixFile.Data());
		correlationMatrix.SaveAs(correlationMatrixFile.Data());
		TString outputFile=wOutputPath.Copy();
		outputFile.Append(title);
		outputFile.Append(".txt");
		ofstream myfile;

		myfile.open(Form("%s",outputFile.Data()));
		myfile << title<<"\n";
		myfile<<outputFile.Data()<<endl;
		myfile<<"Chi2:"<<fitParameters->Chi2()<<"\n";
		myfile << "Constant:"<< " "<< constant<<" "<< " Error: "<< constant_error<<"\n";
		myfile << "Mean:"<< " "<< mean<<" "<< " Error: "<< mean_error<<"\n";
		myfile << "Sigma:"<< " "<< sigma<<" "<< " Error: "<< sigma_error<<"\n";
		myfile << "FWHM:"<< " "<< fwhm<<" "<< " Error: "<< fwhm_error<<"\n";
		myfile << "Peak calculated integral:" << peakCalculatedIntegral <<  " Error: "<<peakCalculatedIntegralError <<endl;
		myfile << "Peak plus background numerical integral: " << peakNumericalIntegral <<  " Error: "<<peakNumericalIntegralError <<endl;
		myfile << "Background integral: " <<backgroundIntegral<<  " Error: "<<backgroundIntegralError <<endl;
		myfile << "Peak integral: " <<peakArea<<  " Error: "<<peakAreaError <<endl;
    myfile << "Total peak integral: " <<totalPeakArea<<  " Error: "<<totalPeakAreaError <<endl;
		myfile << "Total function integral: " <<peakPlusBackgroundIntegral<<  " Error: "<<peakPlusBackgroundIntegralError <<endl;

		myfile << "Histogram integral " << histogramIntegral<< endl;
		myfile.close();
		TString toExcelFile=wOutputPath.Copy();
		
		toExcelFile.Append("ToExcel");
		toExcelFile.Append("_Gate_");
		TString number;
		toExcelFile.Append(wGate1);
		toExcelFile.Append("_");
		toExcelFile.Append(wGate2);
		toExcelFile.Append(".txt");
		ofstream toExcel;
		toExcel.open(Form("%s",toExcelFile.Data()),std::ios_base::app);
	  toExcel<< mean<<" "<< mean_error<<" "<< peakCalculatedIntegral << " "<<peakCalculatedIntegralError <<" "<<peakArea<<  " "<<peakAreaError<< " "<<totalPeakArea<<" "<<totalPeakAreaError<<" "<<sigma<<" "<<sigma_error<<endl;
	  toExcel.close();
};
