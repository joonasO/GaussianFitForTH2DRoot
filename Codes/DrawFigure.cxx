#ifndef __DrawFigure_h_
#include "DrawFigure.h"
#endif

DrawFigure::DrawFigure(){};
DrawFigure::~DrawFigure(){};

void DrawFigure::makeToFigure(TH1F *h1,TString output,std::vector<TString>functionNames,std::vector<Float_t> lows, std::vector<Float_t> highs,std::vector<TF1 *> Backgrounds,std::vector<std::vector<TF1 *>> gaussians,std::vector<TF1 *> gaussianPlusBGs,std::vector<TFitResultPtr> fitParameters){
	Int_t j=0;
	for(Int_t i = 0; i < gaussianPlusBGs.size(); i++){
		figureMain(h1,output,functionNames[i],lows[j],highs[j],gaussians[i].size(),Backgrounds[i],gaussians[i],gaussianPlusBGs[i], fitParameters[i]);
		j=j+gaussians[i].size();
	}


}
void DrawFigure::figureMain(TH1F *h1,TString output,TString functionName,Float_t low, Float_t high, Int_t numberOfPeaks,TF1 *Background,std::vector<TF1 *> gaussian,TF1 *gaussianPlusBG,TFitResultPtr fitParameters){
  Double_t w = 2400;
  Double_t h = 1200;
	TString titleResult=output.Copy();
	titleResult.Append("_");
	titleResult.Append(functionName);
	TH1F *h1_Close=(TH1F*)h1->Clone();
  TString CanvasWhole=titleResult.Data();
  CanvasWhole.Append("Whole");
  TString CanvasClose=titleResult.Data();
  CanvasClose.Append("Close");
  TCanvas * c = new TCanvas(CanvasWhole.Data(), CanvasWhole.Data(), w, h);
  TCanvas * c1 = new TCanvas(CanvasClose.Data(), CanvasClose.Data(), w, h);
  c->cd();
  h1->Draw();
  gaussianPlusBG->SetLineColor(kBlack);
  gaussianPlusBG->SetLineStyle(2);
  gaussianPlusBG->Draw("Same");
  for(Int_t i=0;i<gaussian.size();i++){
    gaussian[i]->SetLineColor(kBlue);
    gaussian[i]->Draw("same");
}
  Background->SetLineColor(kViolet);
  Background->Draw("same");

	TString title;
  c->Update();
  title=h1->GetTitle();
  TString namepicture="../Output/";
  namepicture= namepicture.Append(output);
  namepicture= namepicture.Append("/Fits/");
  namepicture= namepicture.Append(output);
  namepicture= namepicture.Append("_");
  namepicture= namepicture.Append(functionName);
  namepicture= namepicture.Append("_");
  namepicture= namepicture.Append(title);
  namepicture=namepicture.Append(".pdf");
  c->SaveAs(Form("%s",namepicture.Data()));
  c->Print(namepicture,"Title:Close");


  delete c;
  c1->cd();
	h1_Close->SetAxisRange(low,high);
	h1_Close->Draw();
  gaussianPlusBG->SetLineColor(kBlack);
  gaussianPlusBG->SetLineStyle(2);
  gaussianPlusBG->Draw("same");
  for(Int_t i=0;i<numberOfPeaks;i++){
    gaussian[i]->SetLineColor(kBlue);
    gaussian[i]->Draw("same");
}
  Background->SetLineColor(kViolet);
  Background->Draw("same");
  c1->SetWindowSize(2*(w + (w - c1->GetWw())), 2*(h + (h - c1->GetWh())));

  c1->Update();
  title=h1->GetTitle();
  namepicture="../Output/";
  namepicture= namepicture.Append(output);
  namepicture= namepicture.Append("/Fits/");
  namepicture= namepicture.Append(output);
  namepicture= namepicture.Append("_");
  namepicture= namepicture.Append(functionName);
  namepicture= namepicture.Append("_");
  namepicture= namepicture.Append(title);
  namepicture=namepicture.Append("Close.pdf");
  c1->SaveAs(Form("%s",namepicture.Data()));
  c1->Print(namepicture,"Title:Close");
  delete c1;

}
