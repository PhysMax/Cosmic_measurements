#ifndef SimpleGeometry_h
#define SimpleGeometry_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class SimpleGeometry : public G4VUserDetectorConstruction
{
  public:
    SimpleGeometry();
    SimpleGeometry(double alpha);
    virtual ~SimpleGeometry();

    virtual G4VPhysicalVolume* Construct();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    G4Material* DefineMaterial(G4String material, G4double density=0);

  protected:
    G4LogicalVolume*  fScoringVolume;
    G4double angle;
};

#endif

