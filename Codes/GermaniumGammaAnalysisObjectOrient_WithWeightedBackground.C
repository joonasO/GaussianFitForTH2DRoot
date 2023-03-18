#include "Riostream.h"
#include "TMath.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include<vector>
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<cstring>

class DrawHistogram{
public:

		TH1F *DrawH2(TNtuple *GammaGammaTuple,TH1F *initialHistogram, Float_t low_Energy, Float_t high_Energy, const char* GammaGamma){
			Float_t e_gamma1,e_gamma2,counts;
			GammaGammaTuple->SetBranchAddress("x",&e_gamma1);
			GammaGammaTuple->SetBranchAddress("y",&e_gamma2);
			GammaGammaTuple->SetBranchAddress("z",&counts);
			Int_t nentries = (Int_t)GammaGammaTuple->GetEntries();
			for (Int_t i = 0; i < nentries; i++) {
				GammaGammaTuple->GetEntry(i);

				if (strcmp(GammaGamma,"g1")==0){
					for (Int_t j=0; j<counts; j++){
						if(e_gamma2>low_Energy&&e_gamma2<high_Energy){
		    			initialHistogram->Fill(e_gamma1);
						}
					}
				}
				if (strcmp(GammaGamma,"g2")==0){
					for (Int_t j=0; j<counts; j++){
						if(e_gamma1>low_Energy&&e_gamma1<high_Energy){
		    			initialHistogram->Fill(e_gamma2);
						}
					}
				}

			}
			TAxis *axisX=initialHistogram->GetXaxis();
			TAxis *axisY= initialHistogram->GetYaxis();
			Int_t binningWidth=(Int_t)axisX->GetBinWidth(1);
			axisX->SetLabelFont(132);
			axisY->SetLabelFont(132);
			axisX->CenterTitle(true);
			axisY->CenterTitle(true);
			axisX->SetTitleFont(132);
			axisY->SetTitleFont(132);
			TString binningWidthString=Form("%d",binningWidth);
			TString xaxisName="Energy [keV]";
			TString yaxisName="Counts/ ";
			yaxisName.Append(binningWidthString);
			yaxisName.Append(" keV");
			initialHistogram->SetOption("HIST");
			initialHistogram->SetLineColor(kBlack);
			initialHistogram->GetXaxis()->SetTitle(xaxisName.Data());
			initialHistogram->GetYaxis()->SetTitle(yaxisName.Data());
		return initialHistogram;
	}
	TH1F *DrawH1(TNtuple *GammaElectronTuple,TH1F *initialHistogram,const char* GammaElectron){
		Float_t e_gamma1,e_gamma2,counts;
		GammaElectronTuple->SetBranchAddress("x",&e_gamma1);
		GammaElectronTuple->SetBranchAddress("y",&e_gamma2);
		GammaElectronTuple->SetBranchAddress("z",&counts);
		Int_t nentries = (Int_t)GammaElectronTuple->GetEntries();

		for (Int_t i = 0; i < nentries; i++) {
			GammaElectronTuple->GetEntry(i);

			for (Int_t j=0; j<counts; j++){
				if(strcmp(GammaElectron,"g1")==0){
					initialHistogram->Fill(e_gamma1);
				}
				if(strcmp(GammaElectron,"g2")==0){
				initialHistogram->Fill(e_gamma2);
			}
		}

		}
		return initialHistogram;
	}
};
DrawHistogram Drawhist;






void ProjectionX(const char* rootFile,Float_t firstGammaEnergy, Float_t lastGammaEnergy,Int_t bins=2048){
	TFile *f1= new TFile(rootFile,"UPDATE");
	TNtuple *t1 = (TNtuple*)f1->Get("Gamma-Gamma");
	char name[80];
	sprintf(name,"GammaEnergyGatedElectron_%g_%g",firstGammaEnergy,lastGammaEnergy);
	TH1F *E_el_Gate = new TH1F(name,name,bins,0,2048);
	E_el_Gate=Drawhist.DrawH2(t1,E_el_Gate,firstGammaEnergy,lastGammaEnergy,"g1");
	TCanvas *projectionX =new TCanvas(name,name,800,1000);
	E_el_Gate->Draw("Colz");
	f1->Write();

}
/*
else{
	char additionalName[80];
	sprintf(additionalName,"_%g_%g",firstGammaEnergy,lastGammaEnergy);
	TH1F *additionalHistogram = new TH1F(name,name,400,0,800);
	TCanvas *originalProjectionX =new TCanvas(name,name,800,1000);
	E_el_Gate->Draw("Colz");
	strcpy(name,E_el_Gate->GetName());
	strcat(name,additionalName);
	additionalHistogram=Drawhist.DrawH2(t1,E_el_Gate,firstGammaEnergy,lastGammaEnergy,"e");
	TCanvas *additionalCanvas =new TCanvas(additionalName,additionalName,800,1000);
	additionalHistogram->Draw("Colz");
	*E_el_Gate=(*E_el_Gate)+(*additionalHistogram);

}*/
void BackgroundY(const char* rootFile,Float_t firstElectronEnergy, Float_t lastElectronEnergy, Float_t firstBackground, Float_t secondBacktround, Float_t thirdBackground, Float_t fourthBackground,Int_t bins=2048){
	TFile *f1= new TFile(rootFile,"UPDATE");
	TNtuple *t1 = (TNtuple*)f1->Get("Gamma-Gamma");
	char name[80];
	char nameT[80];
	sprintf(name,"GammaEnergyGatedGamma_%g_%g_Background_%g_%g:%g_%g",firstElectronEnergy,lastElectronEnergy,firstBackground,secondBacktround,thirdBackground,fourthBackground);
	sprintf(nameT,"GammaEnergyGatedGamma_%g_%g",firstElectronEnergy,lastElectronEnergy);
	TH1F *E_gamma_x = new TH1F(name,name,bins,0,2048);
	TH1F *E_gamma_x_Tot = new TH1F(nameT,nameT,bins,0,2048);
	E_gamma_x=Drawhist.DrawH2(t1,E_gamma_x,firstElectronEnergy,lastElectronEnergy,"g2");
	E_gamma_x_Tot=Drawhist.DrawH2(t1,E_gamma_x_Tot,firstElectronEnergy,lastElectronEnergy,"g2");
	char nameb1[80];
	sprintf(nameb1,"Background_%g_%g",firstBackground,secondBacktround);
	TH1F *E_Background_Gate1 = new TH1F(nameb1,nameb1,bins,0,2048);
	E_Background_Gate1=Drawhist.DrawH2(t1,E_Background_Gate1,firstBackground,secondBacktround,"g2");
	char nameb2[80];
	sprintf(nameb2,"Background_%g_%g",thirdBackground,fourthBackground);
	TH1F *E_Background_Gate2= new TH1F(nameb2,nameb2,bins,0,2048);
	E_Background_Gate2=Drawhist.DrawH2(t1,E_Background_Gate2,thirdBackground,fourthBackground,"g2");
	Double_t factor1=(Double_t)(-0.5*(lastElectronEnergy-firstElectronEnergy)/(secondBacktround-firstBackground));
	Double_t factor2=(Double_t)(-0.5*(lastElectronEnergy-firstElectronEnergy)/(fourthBackground-thirdBackground));
	E_gamma_x->Sumw2();
	E_gamma_x->Add(E_Background_Gate1,factor1);
	E_gamma_x->Sumw2();
	E_gamma_x->Add(E_Background_Gate2,factor2);
	TCanvas *projectionX =new TCanvas(name,name,800,1000);
	E_gamma_x->Draw("HIST");
	f1->Write();


}
void BackgroundX(const char* rootFile,Float_t firstGammaEnergy, Float_t lastGammaEnergy, Float_t firstBackground, Float_t secondBacktround, Float_t thirdBackground, Float_t fourthBackground,Int_t bins=2048 ){
	TFile *f1= new TFile(rootFile,"UPDATE");
	TNtuple *t1 = (TNtuple*)f1->Get("Gamma-Gamma");
	char name[80];
	char nameT[80];
	sprintf(name,"GammaEnergyGatedGamma_%g_%g_Background_%g_%g:%g_%g",firstGammaEnergy,lastGammaEnergy,firstBackground,secondBacktround,thirdBackground,fourthBackground);
	sprintf(nameT,"GammaEnergyGatedGamma_%g_%g",firstGammaEnergy,lastGammaEnergy);
	TH1F *E_gamma_y = new TH1F(name,name,bins,0,2048);
	E_gamma_y=Drawhist.DrawH2(t1,E_gamma_y,firstGammaEnergy,lastGammaEnergy,"g1");
	TH1F *E_gamma_y_Tot = new TH1F(nameT,nameT,bins,0,2048);
	E_gamma_y_Tot=Drawhist.DrawH2(t1,E_gamma_y_Tot,firstGammaEnergy,lastGammaEnergy,"g1");
	char nameb1[80];
	sprintf(nameb1,"Background_%g_%g",firstBackground,secondBacktround);
	TH1F *E_Background_Gate1 = new TH1F(nameb1,nameb1,bins,0,2048);
	E_Background_Gate1=Drawhist.DrawH2(t1,E_Background_Gate1,firstBackground,secondBacktround,"g1");
	char nameb2[80];
	sprintf(nameb2,"Background_%g_%g",thirdBackground,fourthBackground);
	TH1F *E_Background_Gate2= new TH1F(nameb2,nameb2,bins,0,2048);
	E_Background_Gate2=Drawhist.DrawH2(t1,E_Background_Gate2,thirdBackground,fourthBackground,"g1");
	Double_t factor1=(Double_t)(-0.5*(lastGammaEnergy-firstGammaEnergy)/(secondBacktround-firstBackground));
	Double_t factor2=(Double_t)(-0.5*(lastGammaEnergy-firstGammaEnergy)/(fourthBackground-thirdBackground));
	E_gamma_y->Sumw2();
	E_gamma_y->Add(E_Background_Gate1,factor1);
	E_gamma_y->Sumw2();
	E_gamma_y->Add(E_Background_Gate2,factor2);
	TCanvas *projectionX =new TCanvas(name,name,800,1000);
	E_gamma_y->Draw("HIST");
	f1->Write();


}
//This code reads the root output of GermaniumElectron.C and GermaniumElectronForThreeDataSet.

void GermaniumGammaAnalysisObjectOrient(const char* rootFile){
//Initialise the parameters for electron energies from TNTuple.

TFile *f= new TFile(rootFile,"UPDATE");
TNtuple *t1 = (TNtuple*)f->Get("Gamma-Gamma");
TH1F *E_gamma_x = new TH1F("E_gamma_x","Gamma2 energy projection",2048,0,2048);
TH1F *E_gamma_y = new TH1F("E_gamma_y","Gamma1 energy projection",2048,0,2048);
E_gamma_x=Drawhist.DrawH1(t1,E_gamma_x,"g1");
E_gamma_y=Drawhist.DrawH1(t1,E_gamma_y,"g2");
TCanvas *c1 =new TCanvas("Projection Gamma1","Projection Gamma1",800,1000);
E_gamma_x->Draw("Colz");
TCanvas *c2 =new TCanvas("Projection Gamma2","Projection Gamma2",800,1000);
E_gamma_y->Draw("Colz");
f->Write("E_gamma_x");
f->Write("E_gamma_y");

}
