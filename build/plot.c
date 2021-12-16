void plot() {
    TFile *file = new TFile("output.root", "READ");
    TH1F *hist = (TH1F*)file->Get("angle_nhits");

    TF1 *f = new TF1("f", "[1]*TMath::Cos(x/180*TMath::Pi())*TMath::Exp([0]/TMath::Cos(x/180*TMath::Pi()))+[2] ",0 , 90);
    f->SetParameter(0, -0.2);
    f->SetParName(0,"A");
    f->SetParameter(1, 1);
    f->SetParName(1,"B");
    f->SetParameter(2, 0);
    f->SetParName(2,"C");

    hist->Draw();
    hist->Fit("f");
}
