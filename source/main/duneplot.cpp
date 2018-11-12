#include "Utils.h"

#include <string>
#include <vector>
#include <iostream>

#include "TF1.h"
#include "TApplication.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "/afs/desy.de/user/e/ebrianne/.root/CaliceStyle.h"

int main(int argc, char **argv)
{
    CaliceStyle();

    if(argc < 3) {
        std::cout << "Usage: duneana InputFolder [Endcap/Barrel]" << std::endl;
        return -1;
    }

    std::string InputFolder = argv[1];
    std::string Type = argv[2];

    std::cout << "InputFolder " << InputFolder << std::endl;
    std::cout << "Analysis of the " << Type << std::endl;

    Utils U(InputFolder, "", Type);
    U.GetListOfFiles();
    std::vector<std::string> vecfile = U.GetVectorFiles();

    TApplication app("app", &argc, argv);

    TGraphErrors *gr = new TGraphErrors();
    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(kBlack);

    TGraphErrors *gr2 = new TGraphErrors();
    gr2->SetMarkerStyle(20);
    gr2->SetMarkerColor(kRed);

    TGraphErrors *gr3 = new TGraphErrors();
    gr3->SetMarkerStyle(20);
    gr3->SetMarkerColor(kBlue);

    int i = 0;
    for (auto &file : vecfile)
    {
        float energy = std::atof( file.substr( file.find("GeV") - 3, 3 ).c_str() )*1000;

        TFile *f = new TFile(file.c_str(), "OPEN");
        TH1* h = dynamic_cast<TH1*>(f->Get("_hEnergySumSim"));
        TH1* h2 = dynamic_cast<TH1*>(f->Get("_hEnergySumDigi"));
        TH1* h3 = dynamic_cast<TH1*>(f->Get("_hEnergySumReco"));

        float mean = h->GetFunction("gaus")->GetParameter(1);
        float sigma = h->GetFunction("gaus")->GetParameter(2);
        float mean_err = h->GetFunction("gaus")->GetParError(1);
        float sigma_err = h->GetFunction("gaus")->GetParError(2);

        float error = std::sqrt( std::pow(sigma_err/mean, 2) + std::pow(sigma*mean_err/(mean*mean), 2) );

        gr->SetPoint(i, energy, sigma/mean);
        gr->SetPointError(i, 0, error);

        // //-------
        //
        // h2->Fit("gaus", "");
        //
        // mean = h2->GetFunction("gaus")->GetParameter(1);
        // sigma = h2->GetFunction("gaus")->GetParameter(2);
        // mean_err = h2->GetFunction("gaus")->GetParError(1);
        // sigma_err = h2->GetFunction("gaus")->GetParError(2);
        //
        // error = std::sqrt( std::pow(sigma_err/mean, 2) + std::pow(sigma*mean_err/(mean*mean), 2) );
        //
        // gr2->SetPoint(i, energy, sigma/mean);
        // gr2->SetPointError(i, 0, error);
        //
        // //-------
        //
        // h3->Fit("gaus", "");
        //
        // mean = h3->GetFunction("gaus")->GetParameter(1);
        // sigma = h3->GetFunction("gaus")->GetParameter(2);
        // mean_err = h3->GetFunction("gaus")->GetParError(1);
        // sigma_err = h3->GetFunction("gaus")->GetParError(2);
        //
        // error = std::sqrt( std::pow(sigma_err/mean, 2) + std::pow(sigma*mean_err/(mean*mean), 2) );
        //
        // gr3->SetPoint(i, energy, sigma/mean);
        // gr3->SetPointError(i, 0, error);

        delete h;
        delete h2;
        delete h3;
        f->Close();
        i++;
    }

    TF1 *f1 = new TF1("resfcn", "std::sqrt( std::pow([0]/std::sqrt(x/1000), 2) + [1]*[1] )");
    f1->SetLineColor(kBlack);
    f1->SetParNames("Stock", "Cst");
    f1->SetParameter(0, 0.05);
    f1->SetParameter(1, 0.02);
    f1->SetParLimits(0, 0., 0.2);
    f1->SetParLimits(1, 0., 0.1);

    gr->Fit("resfcn");

    float stock = f1->GetParameter(0);
    float cst = f1->GetParameter(1);

    TMultiGraph *m = new TMultiGraph();
    m->Add(gr);
    // m->Add(gr2);
    // m->Add(gr3);

    TLegend *leg = new TLegend(0.3, 0.7, 0.7, 0.8);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->AddEntry(gr, TString::Format("Simulation : %.2f % / #sqrt{E [GeV]} #oplus %.2f %", stock*100, cst*100), "p");
    // leg->AddEntry(gr2, "Digitisation", "p");
    // leg->AddEntry(gr3, "Reconstruction", "p");

    TCanvas *c1 = new TCanvas("c1", "Energy Resolution", 800, 600);
    c1->cd();
    m->Draw("AP");
    m->GetXaxis()->SetTitle("Photon Energy [MeV]");
    m->GetYaxis()->SetTitle("#sigma_{E}/E_{mean}");
    m->GetYaxis()->SetRangeUser(0.02, 0.25);
    m->GetXaxis()->SetRangeUser(0, 1800);
    leg->Draw("same");

    c1->SaveAs("Baseline_Barrel_EnergyRes.pdf");

    TFile *fOut = new TFile("ResultsERes.root", "RECREATE");
    fOut->cd();
    gr->Write("Baseline_Barrel_EnergyRes");
    fOut->Close();

    app.Run();

    return 0;
}
