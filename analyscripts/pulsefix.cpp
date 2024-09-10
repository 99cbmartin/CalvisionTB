#include <iostream>
#include <TFile.h>
#include <TProfile.h>
#include <TCanvas.h>

void NormalizeAndPrintProfiles(const char* filename) {
    TFile file("../outputroots/PWOparFilterAvgwfJan26.root", "UPDATE");
    if (!file.IsOpen()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    TProfile* h126 = dynamic_cast<TProfile*>(file.Get("h126"));
    TProfile* h127 = dynamic_cast<TProfile*>(file.Get("h127"));
    if (!h126 || !h127) {
        std::cerr << "Error: Could not retrieve TProfile objects from file " << filename << std::endl;
        file.Close();
        return;
    }

    Double_t integral_h126 =  h126->Integral(h126->FindBin(20), h126->FindBin(30));
    Double_t integral_h127 = h127->Integral(h127->FindBin(20), h127->FindBin(30));

    if (integral_h126 != 0.0) {
        h126->Scale(1.0 / integral_h126);
    }
    if (integral_h127 != 0.0) {
        h127->Scale(1.0 / integral_h127);
    }

    TCanvas canvas("canvas", "Normalized Profiles", 800, 600);

    h126->SetLineColor(kRed);
    h126->Draw();

    h127->SetLineColor(kGreen);
    h127->Draw("SAME");

    canvas.SaveAs("Normalized_Profiles2.png");

	 TProfile* hDiff = static_cast<TProfile*>(h126->Clone("hDiff"));
    hDiff->Add(h127, -1);

    TCanvas* canvasDiff = new TCanvas("canvasDiff", "Difference between Scaled TProfiles");
    hDiff->Draw();
    canvasDiff->Print("Difference2.png");
    file.Close();
}

int main() {
    const char* filename = "your_root_file.root";
    NormalizeAndPrintProfiles(filename);
    return 0;
}

