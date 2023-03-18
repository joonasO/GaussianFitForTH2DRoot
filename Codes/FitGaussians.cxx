#ifndef __FitgGaussian_h_
#include "FitGaussians.h"
#endif


FitGaussians::FitGaussians(){}
FitGaussians::~FitGaussians(){
  std::cout << "destructorGaussianFile" << '\n';
}
void FitGaussians::mainFitter(TH1F* histogram,std::vector<Float_t> energyPeaks, std::vector<Float_t> energyLows,std::vector<Float_t> energyHighs,std::vector<Float_t> peakWidth,TString output){
  Int_t numberOfPeaks=0;
  std::vector<Float_t> peaksToFitted;
  std::vector<Float_t> peaksToFittedWidth;

  for(Int_t i = 0; i < energyPeaks.size(); i++){
    peaksToFitted.push_back(energyPeaks[i]);
    peaksToFittedWidth.push_back(peakWidth[i]);

    numberOfPeaks=numberOfPeaks+1;

    if(Debug){
      cout<<"Peak:"<<peaksToFitted.back()<<endl;
      cout<<"Width:"<<peaksToFittedWidth.back()<<endl;
      cout<<"energyLow: "<<energyLows[i]<<endl;
      cout<<"energyHigh: "<<energyHighs[i]<<endl;
    }

    if (i+1==energyPeaks.size()||energyLows[i]!=energyLows[i+1]){
      if(Debug){
      cout<<"Peaks To Fitted: "<<peaksToFitted[numberOfPeaks-1]<<endl;
      cout<<"Number of Peaks: "<<numberOfPeaks<<endl;
    }
      fittingProcedure(histogram, peaksToFitted,energyLows[i],energyHighs[i], numberOfPeaks,output,peaksToFittedWidth);
      numberOfPeaks=0;
      peaksToFitted.clear();
      peaksToFittedWidth.clear();
      if(Debug){
      cout<<"Number of Peaks2: "<<numberOfPeaks<<endl;
    }
    }
  }
}

Double_t FitGaussians::gaussiansPlusBackground(Double_t *x, Double_t *par){
  Double_t gaussiansPlusBackground=par[1]+par[2]*x[0];

  for(Int_t i=0;i<par[0];i++){
    gaussiansPlusBackground=gaussiansPlusBackground+par[i*3+3]*TMath::Exp(-(x[0]-par[i*3+4])*(x[0]-par[i*3+4])/(2*(par[i*3+5]*par[i*3+5])));
  }
  return gaussiansPlusBackground;
}

Double_t FitGaussians::gaussFunction(Double_t *x,Double_t *par){
  Double_t gaussFunctionValue=par[0]+par[1]*x[0]+par[2]*TMath::Exp(-(x[0]-par[3])*(x[0]-par[3])/(2*(par[4]*par[4])));
  return gaussFunctionValue;
}

Double_t FitGaussians::backgroundFunction(Double_t *x,Double_t *par){
  Double_t linearFunction=par[0]+x[0]*par[1];
  return linearFunction;
}

Double_t FitGaussians::binning(TH1F *h1){
  Double_t bin1,bin2,binDifference;
  bin1= h1->GetXaxis()->GetBinCenter(1);
  bin2= h1->GetXaxis()->GetBinCenter(2);
  binDifference=bin2-bin1;
  return binDifference;
}

void FitGaussians::estimateBackgroundParameters(TH1F *h1,Float_t low,Float_t high){
  Int_t lowBin,highBin;

  Double_t binDifference=binning(h1);
  lowBin=Int_t(low/binDifference);
  highBin=Int_t(high/binDifference);
  Double_t numberBins,lowerEnergyBackground,higherEnergyBackground;
  lowerEnergyBackground=h1->GetBinContent(lowBin);
  higherEnergyBackground=h1->GetBinContent(highBin);
  fPolynomeLinearCoefficient=(higherEnergyBackground-lowerEnergyBackground)/(high-low);
  fPolynomeConstantCoefficient=lowerEnergyBackground-fPolynomeLinearCoefficient*low;
  numberBins = h1->GetNbinsX();
  if(Debug){
    cout<<"Binning"<<endl;
    cout<<numberBins<<endl;
    cout<<binDifference<<endl;
    cout<<low<<endl;
    cout<<high<<endl;
    cout<<"Parameter initialisation"<<endl;
    cout<<lowerEnergyBackground<<endl;
    cout<<higherEnergyBackground<<endl;
    cout<<fPolynomeLinearCoefficient<< endl;
    cout<<fPolynomeConstantCoefficient<< endl;
  }
}

TString FitGaussians::nameFunction(TString output, TString functionType, Int_t numberOfPeaks, Float_t low, Float_t high){
  TString functionName=output;
  number.Form("%d",numberOfPeaks);
  functionName.Append(number);
  functionName.Append("_");
  number.Form("%f",low);
  functionName.Append(number);
  functionName.Append("_");
  number.Form("%f",high);
  functionName.Append(number);
  functionName.Append("_");
  functionName.Append(functionType);
  return functionName;
}

void FitGaussians::initialisationParameters(TF1 *function,TH1F *h1,std::vector<Float_t> peaksToFitted,Int_t numberOfPeaks,std::vector<Float_t>  peaksToFittedWidth){
  TString parameternames[numberOfPeaks*3+2];
  Double_t width;

	Double_t gaussianConstant;
  parameternames[0]="numberOfPeaks";
  function->SetParName(0,parameternames[0].Data());
  function->FixParameter(0,numberOfPeaks);
  parameternames[1]="c";
  cout<<"Constant:"<<fPolynomeConstantCoefficient;
  function->SetParName(1,parameternames[1].Data());
  function->SetParameter(1,fPolynomeConstantCoefficient);
  parameternames[2]="x";
  function->SetParName(2,parameternames[2].Data());
  function->SetParameter(2,fPolynomeLinearCoefficient);
  cout<<"Linear:"<<fPolynomeLinearCoefficient;


  for (Int_t i=0; i<numberOfPeaks; i++){
    number.Form("%d",3*i+2);
    parameternames[3*i+3]="constant_";
    parameternames[3*i+3].Append(number);
    parameternames[3*i+4]="mean_";
    parameternames[3*i+4].Append(number);
    parameternames[3*i+5]="sigma_";
    parameternames[3*i+5].Append(number);
    number.Form("%d",i*3+2);
    function->SetParName(i*3+3,parameternames[i*3+3].Data());
    gaussianConstant=h1->GetBinContent(peaksToFitted[i]);
    function->SetParameter(i*3+3,gaussianConstant);
    function->SetParLimits(i*3+3,0,gaussianConstant*1000);
    cout<<parameternames[i*3+4].Data();
    function->SetParName(i*3+4,parameternames[i*3+4].Data());
    function->SetParameter(i*3+4,peaksToFitted[i]);
    function->SetParLimits(i*3+4,peaksToFitted[i]-2,peaksToFitted[i]+2);
    number.Form("%d",i*3+4);
    function->SetParName(i*3+5,parameternames[i*3+5].Data());
    width=peaksToFittedWidth[i];
    function->SetParLimits(i*3+5,width-0.75,width+0.75);

  }
}
void FitGaussians::setParametersForGaussians(Int_t numberOfPeaks,TF1 **fGaussian,TF1 *fGaussianPlusBG){
  Double_t polynom_constant,polynom_linear,polynom_quadratic;
  std::vector<Double_t> constant;
  std::vector<Double_t> mean;
  std::vector<Double_t> sigma;
	polynom_constant=fGaussianPlusBG->GetParameter(1);
	polynom_linear=fGaussianPlusBG->GetParameter(2);
  for (Int_t i=0; i<numberOfPeaks;i++){

    constant.push_back(fGaussianPlusBG->GetParameter(i*3+3));
    mean.push_back(fGaussianPlusBG->GetParameter(i*3+4));
    cout<<"Parameter"<<fGaussianPlusBG->GetParameter(i*3+4)<<endl;
    cout<<"Mean Value"<<mean[i]<<endl;
    cout<<"i"<<i<<endl;
    cout<<"numberOfPeaks"<<numberOfPeaks<<endl;
    sigma.push_back(fGaussianPlusBG->GetParameter(i*3+5));
    fGaussian[i]->SetParameter(0,polynom_constant);
    fGaussian[i]->SetParameter(1,polynom_linear);
    fGaussian[i]->SetParameter(2,constant[i]);
    fGaussian[i]->SetParameter(3,mean[i]);
    fGaussian[i]->SetParameter(4,sigma[i]);
    fGaussians.push_back(fGaussian[i]);

  }
}
void FitGaussians::setParametersForBackground(TF1 *fBackground,TF1 *fGaussianPlusBG){
  Double_t polynom_constant,polynom_linear,polynom_quadratic;
	polynom_constant=fGaussianPlusBG->GetParameter(1);
	polynom_linear=fGaussianPlusBG->GetParameter(2);
	fBackground->SetParameter(0,polynom_constant);
	fBackground->SetParameter(1,polynom_linear);
}
void FitGaussians::fittingProcedure(TH1F *h1, std::vector<Float_t> peaksToFitted,Float_t low, Float_t high,Int_t numberOfPeaks,TString output,std::vector<Float_t>  peaksToFittedWidth){

    Int_t gaussianLow, gaussianHigh,gaussianPlusBGLow,gaussianPlusBGHigh;
    Int_t binning;
    TString functionName,number,singleGaussian,function,gaussianFunctionBackgroundName;
    TString gaussianFunctionName[numberOfPeaks];
    TString parameternames[numberOfPeaks*3+3];
    gaussianFunctionBackgroundName=nameFunction(output,"_Background",numberOfPeaks,low,high);
    if(Debug){
    cout<<"Function name:"<<gaussianFunctionBackgroundName.Data()<<endl;
  }
    estimateBackgroundParameters(h1,low,high);
    cout<<"Constant:"<<fPolynomeConstantCoefficient;
    gaussianLow=low;
    gaussianHigh=high;
    number.Form("%d",numberOfPeaks);
    //const char* functions=function.Data();
    //cout<<functions<<endl;
    TF1 **fGaussian=new TF1*[numberOfPeaks];
    for (Int_t i=0; i<numberOfPeaks; i++){
      singleGaussian="_SingleGaussian_";
      number.Form("%d",3*i+3);
      singleGaussian.Append(number);
      gaussianFunctionName[i]=nameFunction(output,singleGaussian,numberOfPeaks,low,high);
      fGaussian[i]=new TF1(gaussianFunctionName[i].Data(),gaussFunction,gaussianLow,gaussianHigh,5);
    }

    TF1 *fBackground = new TF1(gaussianFunctionBackgroundName.Data(),backgroundFunction,gaussianLow,gaussianHigh,2);

    TString gaussiansPlusBackgroundName=nameFunction(output,"Total",numberOfPeaks,low,high);
    Int_t numberOfParameters=numberOfPeaks*3+3;
    TF1 *fGaussianPlusBG = new TF1(gaussiansPlusBackgroundName.Data(),gaussiansPlusBackground,gaussianLow,gaussianHigh,numberOfParameters);

    initialisationParameters(fGaussianPlusBG,h1,peaksToFitted,numberOfPeaks,peaksToFittedWidth);
    fFitParameters=h1->Fit(gaussiansPlusBackgroundName.Data(),"MRSE");
		setParametersForGaussians(numberOfPeaks,fGaussian,fGaussianPlusBG);
		setParametersForBackground(fBackground,fGaussianPlusBG);
    fGaussianPlusBGs.push_back(fGaussianPlusBG);
    fBackgrounds.push_back(fBackground);
    fGaussiansMatrix.push_back(fGaussians);
    fGaussians.clear();
    fFitParametersVector.push_back(fFitParameters);
    fGaussiansPlusBackgroundNames.push_back(gaussiansPlusBackgroundName);
    //delete[] fGaussian;
    //delete fGaussianPlusBG;
    //delete fBackground;

    /*gausBGlow = mean-2.355*sigma;
    gausBGhigh = mean+2.355*sigma;
    cout << gausBGlow << " low " << gausBGhigh << " high" << endl;
*/

    /*
    *Write down the output results
    */
  }
