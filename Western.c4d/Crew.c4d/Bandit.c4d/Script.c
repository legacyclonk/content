/*-- Bandit --*/

#strict
#include COWB

local fGrabbedInfo;
local fBoss;

// Der Bandit benutzt immer die InfoSektion eines Cowboys
protected func Recruitment()
{
  if(!fGrabbedInfo) // Ein Hack für Peter
    {
    // Cowboy erstellen
    var cb = CreateObject(COWB, 0, 10, GetOwner());
    MakeCrewMember(cb, GetOwner());
    // InfoSektion klauen
    fGrabbedInfo = true;
    GrabObjectInfo(cb);
    // Den anderen Clonk wieder löschen
    RemoveObject(cb);
    // Volles Leben
    DoEnergy(100);
    // Neues Portrait
    AdjustPortrait();
    // Workaround für HEAD-Bug
    SetPlrViewRange(500);
    }
  // Weiter im Text
  return(_inherited());
}

/* Boss */
public func MakeBoss()
{
  if(fBoss) return();
  fBoss = 1;
  // Grafik ändern
  SetGraphics("Boss");
  SetPortrait("BanditBoss", this(), GetID());
  // Besser kämpfen
  SetPhysical("Fight", Max(GetPhysical("Fight", 0)+25000, 75000), 2);
  return(1);
}

/* Begrenzung der Steuerung wärend er verhaftet ist */
local fIsCaught;

// Weder Graben noch Werfen
func ControlThrow()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

func ControlDig()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

func ControlDigSingle()
{
  if(fIsCaught) return(1);
  return(_inherited());
}

// Auf kein Inventarwechsel
func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=CXIV]
  if(fIsCaught) return(1);
  return(_inherited());
}

// Gefesselte schlagen nicht zu
private func Fighting()
{
  if (!Random(2) && !fIsCaught) SetAction("Punch");
  return(1);
}

// Clonk darf sein gefesseltes Menü nicht beenden
func MenuQueryCancel(iSelection, pMenuObj)
{
  if(GetMenu() == SHSI) return(1);
  return(_inherited(iSelection, pMenuObj));
}

// Nichts aufnehmen
public func RejectCollect(a,b)
{
  if(fIsCaught) return(1);	
  return(_inherited(a,b));
}

// Captain? (hat die gleiche Funktion wie der Kavalleriehauptmann)
public func IsCaptain() { return(fBoss); }

// Ist ein Bandit
public func IsBandit() { return(1); }
