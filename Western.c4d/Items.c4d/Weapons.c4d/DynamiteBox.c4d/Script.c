/*-- Dynamitkiste --*/

#strict 2
#include ARWP

/* Paketeigenschaften */

// Dies ist eine Dynamitkiste
public func UnpackTo() { return(DYNA); }
public func MaxPackCount() { return(5); }
// Ist weder ein Pfeilpacket noch auf dem Amboss herstellbar
public func IsAnvilProduct() { return(0); }
public func IsArrowPack() { return(0); }
// Kann dafür aber in der Waffenschmiede hergestellt werden
public func IsArmoryProduct() { return(1); }

// Nur eine Kiste ist tragbar
public func CarryLimit() { return(1); }

// Lässt sich nur im Waffenlager zusammenlegen
public func JoinPack(object pContainer)
{
  if(pContainer->~ProductCondition() != "IsArmoryProduct") return(0);
  return(_inherited(pContainer));
}

/* Per Doppelklick auf Graben in Komponenten zerlegen */

public func Activate(object pCaller)
{
  [$TxtTakeDynamite$]
  // Eine Stange kommt raus
  DoPackCount(-1);
  // In den Clonk rein
  CreateContents(UnpackTo(),pCaller);
  Sound("Connect");
  // Fertig
  return(1);
}

/* Aktivierung (durch Inhaltssteuerung) */

public func ControlDigDouble(pCaller) { return(Activate(pCaller)); }

// Aktivierung durch Explosion von außen
protected func Incineration() { return(Ignition()); }

/* Zündung durch Handkasten */

public func Ignition()
{
  var iCount;
  // Ist die Dynamitkiste überhaupt schon fertiggestellt?
  if(!(GetOCF(this()) & OCF_Fullcon)) {
    // Von ihr können noch keine Stangen Dynamit entnommen worden sein
    // Sprengkraft hängt von zugeführten Objektkomponenten ab
    if(!iCount = GetComponent(SPHR,0,this()) + GetComponent(GUNP,0,this())) return(RemoveObject(this()));
    return(Explode(60*iCount/(GetComponent(SPHR,0,0,GetID())+GetComponent(GUNP,0,0,GetID()))));
  }
  iCount = PackCount();
  // Explosionsgröße nach Inhalt anpassen
  if(iCount == 1) return(Explode(26));
  if(iCount == 2) return(Explode(38)); // +12
  if(iCount == 3) return(Explode(46)); // + 8
  if(iCount == 4) return(Explode(54)); // + 8
  if(iCount == 5) return(Explode(60)); // + 6
  return(1);
}

protected func SellTo(int iPlr)
{
  if(PackCount() == MaxPackCount()) return(GetID());
  return(UnpackTo());
}
  
protected func Sale(int iPlr)
{
  // Paket war voll? Dann stimmt der normale Verkauf
  if(PackCount() == MaxPackCount()) return(1);
  // Andernfalls in Dynamitstangen wechseln
  if(PackCount() > 1) DoHomebaseMaterial(iPlr,UnpackTo(),PackCount()-1);
  // Fertig
  return(1);
}