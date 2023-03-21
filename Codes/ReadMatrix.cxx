#include"ReadMatrix.h"

ReadMatrix::ReadMatrix(){
}

ReadMatrix::~ReadMatrix(){
  cout << "destructor" << endl;
}

TH1F* projectionToMatrixX(TH2F *matrix,Float_t gate1, Float_t gate2){
  TString number;
  TString title=(TString) matrix->GetTitle();
  title.Append("_");
  number.Form("%d",gate1);
  title.Append(number);
  title.Append("_");
  number.Form("%d",gate2); 
  title.Append(number);
  Double_t gate1_D=(Double_t) gate1;
  Double_t gate2_D=(Double_t) gate2;
  Int_t bin1=matrix->FindBin(0,gate1_D);
  Int_t bin2=matrix->FindBin(0,gate2_D);
  TH1D *histogramD= matrix->ProjectionX(title.Data(),bin1,bin2);
  TH1F *histogramF;
  histogramD->Copy(* histogramF);
  return histogramF;

}



TH1F* projectionToMatrixY(TH2F *matrix,Float_t gate1, Float_t gate2){
  cout<<"Projection Inside y"<<"\n";
  TString number;
  TString title;
  const char* titleMatrix=matrix->GetTitle();
  cout<< titleMatrix<<"\n";
  title.Append(titleMatrix);
  cout<<"Title"<<"\n";
  title.Append("_");
  number.Form("%0.f",gate1); 
  title.Append(number);
  title.Append("_");
  number.Form("%0.f",gate2);
  title.Append(number);
  
  Double_t gate1_D=(Double_t) gate1;
  Double_t gate2_D=(Double_t) gate2;
  Int_t bin1=matrix->GetXaxis()->FindBin(gate1_D);
  Int_t bin2=matrix->GetXaxis()->FindBin(gate2_D);
  cout<<"Bin1:"<< bin1<<"\n";
  cout<<"Bin2:"<< bin2<<"\n";
  cout<<"Gate1:"<<gate1_D<<"\n";
  cout<<"Gate2:"<<gate2_D<<"\n";
  cout<<"names"<<"\n";
  TH1D *histogramD= matrix->ProjectionY(title.Data(),bin1,bin2);
  cout<<"Copy"<<"\n";
  TH1F *histogramF;
  histogramF=(TH1F *)histogramD->Clone();
  cout<<"Copy"<<histogramF->GetTitle()<<"\n";
  return histogramF;

}

void ReadMatrix::createHistogram(TH2F *matrix, TString axis, TString gate1Text,TString gate2Text,
  TString background1Text,TString background2Text,TString background3Text,TString background4Text){
  TString title;
  TString number;
  Double_t factor1;
  Double_t factor2;
  Float_t gate1=atof(gate1Text.Data());
  Float_t gate2=atof(gate2Text.Data());
  Float_t background1=atof(background1Text.Data());
  Float_t background2=atof(background2Text.Data());
  Float_t background3=atof(background3Text.Data());
  Float_t background4=atof(background4Text.Data());
  if (axis.CompareTo("x")==0){
    histogramTotal=projectionToMatrixX(matrix,gate1,gate2);
    histogramTotal->Copy(* histogram);
    title=histogramTotal->GetTitle();
    title.Append("_Projection_X");
    histogramTotal->SetNameTitle(title.Data(),title.Data());
    if (background1==-1){

    }
    if (background1!=-1&&background3==-1){
      backgroundHistogram1=projectionToMatrixX(matrix,background1,background2 );
      factor1=(Double_t)(-1*(gate1-gate2)/(background1-background2));
      histogram->Add(backgroundHistogram1,factor1);
    }
    if (background1!=-1&&background3!=-1){
      backgroundHistogram1=projectionToMatrixX(matrix,background1,background2 );
      backgroundHistogram2=projectionToMatrixX(matrix,background3,background4 );
      factor1=(Double_t)(-((background2-background1)/((background2-background1)+(background4-background3)))*(gate2-gate1)/(background2-background1));
      factor2=(Double_t)(-((background4-background3)/((background2-background1)+(background4-background3)))*(gate2-gate1)/(background2-background1));

      histogram->Add(backgroundHistogram1,factor1);
      histogram->Add(backgroundHistogram2,factor2);
    }
  }

  if (axis.CompareTo("y")==0){
    cout<<"Projection y"<<"\n";
    histogramTotal=projectionToMatrixY(matrix,gate1,gate2);
    histogram=(TH1F *)histogramTotal->Clone();
    title=histogramTotal->GetTitle();
    title.Append("_Projection_Y_");
    number.Form("%0.f",gate1);
    title.Append(number);
    title.Append("_");
    number.Form("%0.f",gate2);
    title.Append(number);
    histogramTotal->SetNameTitle(title.Data(),title.Data());
    if (background1==-1){

    }
    if (background1!=-1&&background3==-1){
      backgroundHistogram1=projectionToMatrixY(matrix,background1,background2 );
      factor1=(Double_t)(-1*(gate1-gate2)/(background1-background2));
      histogram->Add(backgroundHistogram1,factor1);
    }
    if (background1!=-1&&background3!=-1){
      backgroundHistogram1=projectionToMatrixY(matrix,background1,background2 );
      title=backgroundHistogram1->GetTitle();
      title.Append("_Background_");
      number.Form("%0.f",background1);
      cout<<"Background1:"<<number.Data()<<"\n";
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background2);
      title.Append(number);
      backgroundHistogram1->SetNameTitle(title.Data(),title.Data());
      backgroundHistogram2=projectionToMatrixY(matrix,background3,background4 );
      title=backgroundHistogram2->GetTitle();
      title.Append("_Background_");
      number.Form("%.0f",background3);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background4);
      title.Append(number);
      backgroundHistogram2->SetNameTitle(title.Data(),title.Data());
      factor1=(Double_t)(-((background2-background1)/((background2-background1)+(background4-background3)))*(gate2-gate1)/((background2-background1)+(background4-background3)));
      factor2=(Double_t)(-((background4-background3)/((background2-background1)+(background4-background3)))*(gate2-gate1)/((background2-background1)+(background4-background3)));
      histogram->Add(backgroundHistogram1,factor1);
      histogram->Add(backgroundHistogram2,factor2);
      substractedHistogram=(TH1F *)histogram->Clone();
      substractedHistogram->Add(histogramTotal,-1);
      substractedHistogram->Scale(-1);
      title="Substracted_Spectrum_";
      number.Form("%0.f",background1);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background2);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background3);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background4);
      title.Append(number);
      substractedHistogram->SetNameTitle(title.Data(),title.Data());

      title=histogram->GetTitle();
      title.Append("_Total_Gated_");
      number.Form("%0.f",gate1);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",gate2);
      title.Append(number);
      title.Append("_");
      title.Append("_BgSubs_");
      number.Form("%0.f",background1);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background2);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background3);
      title.Append(number);
      title.Append("_");
      number.Form("%0.f",background4);
      title.Append(number);
      histogram->SetNameTitle(title.Data(),title.Data());
    }
  }

}


