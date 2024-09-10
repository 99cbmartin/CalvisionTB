#include <TFile.h>
#include <TGraphErrors.h>
#include <iostream>

void findMinimumPoints(const char* filename) {
    TFile file(filename);

    if (!file.IsOpen()) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return;
    }

    for (Int_t X = 0; X <= 6; ++X) {
        for (Int_t Y = 0; Y <= 6; ++Y) {
            TString graphName = Form("PWOpF_Corr_Ch%d_Amp%d", X, Y);

            TGraphErrors *graph = (TGraphErrors*)file.Get(graphName.Data());

            if (!graph) {
                std::cerr << "Warning: TGraphErrors " << graphName << " not found in file" << std::endl;
                continue;
            }

            Int_t numPoints = graph->GetN();

            Double_t minY = graph->GetY()[0]; // Initialize minY with the first y-value
            Double_t minX = graph->GetX()[0]; // Initialize minX with the first x-value

            for (Int_t i = 1; i < numPoints; ++i) {
                Double_t y = graph->GetY()[i];
                if (y < minY) {
                    minY = y;
                    minX = graph->GetX()[i];
                }
            }

            std::cout << "For Amplitude " << Y << " and Channel " << X << " the lowest y value was at (" << minX << ", " << minY << ")" << std::endl;
        }
    }

}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.root>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];

    findMinimumPoints(filename);

    return 0;
}
