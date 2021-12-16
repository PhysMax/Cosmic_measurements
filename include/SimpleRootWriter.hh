#ifndef SimpleRootWriter_H
#define SimpleRootWriter_H 1

#include "G4SystemOfUnits.hh"
#include "G4Types.hh"

#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TFile.h"
#include <string>
#include <iostream>

using namespace std;

class SimpleRootWriter{
public:
    void Initialize(double alpha, bool SaveTree=false);       
    void Fill();                // store an event
    void Finalize();    // write tree to a file
    static SimpleRootWriter* GetPointer(){
        if(pInstance==0) pInstance = new SimpleRootWriter();
        return pInstance;
    };

    void ResetEdep() {edepScint1 = 0.; edepScint2 = 0.;};
    void AddEdep1(G4double edep) {edepScint1 += edep;}
    void AddEdep2(G4double edep) {edepScint2 += edep;}
    void FillDeltaEnergy(G4double e) { h1[3]->Fill(e/keV); };
    void FinalizeEvent() {
        G4double thr = 100*keV;
        //cout << "edep1 [MeV] " << edepScint1/MeV << endl;
        h1[0]->Fill(edepScint1/MeV);
        //fill into histogram
        if (edepScint1 > thr && edepScint2 > thr) {
            h1[1]->Fill(2);
            h1[2]->Fill(fAngleDetectors);
        }
        else if (edepScint1 > thr) {
            h1[1]->Fill(0);
        }
        else if (edepScint2 > thr) {
            h1[1]->Fill(1);
        }
    };
    void FillTheta(double theta) {
        h1[4]->Fill(theta); 
    };

private:
    SimpleRootWriter(){fSaveTree = false;};     // empty constructor
    static SimpleRootWriter* pInstance;

    bool fSaveTree;

    G4double edepScint1;
    G4double edepScint2;

    G4double fAngleDetectors;

public:

    TH1F*  h1[500];
    TH2F*  h2[100];
     
    TTree* tree;

private:
    TFile* file;
};

#endif
