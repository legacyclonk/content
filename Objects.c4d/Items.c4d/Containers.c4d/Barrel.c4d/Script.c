/*-- Fass --*/

#strict

// Anzahl Materialpixel, die in ein Fass passen
static const BARL_MaxFill = 200;

// Füllstand
local iFillLevel;

// Füllmaterial+1
// Benutzt nur vom Lavafass, um zwischen Lava und DuroLava zu unterscheiden
// Kann für Fässer aus alten Spielständen 0 sein!
local iFillMaterial;

// Gegenwärtiges Material, mit dem nicht aufgefüllt werden konnte
local iBackMaterial;

// ID des leeren Fasses
global func EmptyBarrelID() { return(BARL); }

// Verkauf
protected func SellTo()
{
  // Fasskonfiguration: Gegebenenfalls Verkauf zum gefüllten Fass
  if (BarrelConfiguration() & 2) return(GetID());
  // Ansonsten beim Verkauf entleeren
  return(EmptyBarrelID());
}

/* TimerCall */

private func FillCheck()
{
  // Wenn das normale Fass per ChangeDef zurueckgeaendert wurde
  // koennte die Fuellvariable noch voll sein
  if(BarrelMaterial() == -1)
  {
  	if(iFillLevel)
  		iFillLevel=0;
  }
  
  // Material an aktueller Position überprüfen
  var iMaterial;
  if ( ((iMaterial = GetMaterial()) == -1) || (iMaterial == iBackMaterial) ) return(0);
  // Fass für dieses Material suchen
  var idBarrel;
  if (!(idBarrel = GetBarrelType(iMaterial))) return(iBackMaterial = iMaterial);
  // Fass füllen wenn entweder leer oder mit dem gleichen Material aufgefüllt werden kann
  if(BarrelMaterial() != -1 && BarrelMaterial() != iMaterial) return(0);
  // Wenn nicht bereits voll
  if(iFillLevel >= BarrelMaxFill()) return(0);
  // Fass füllen
  iFillLevel += ExtractMaterialAmount(0, 0, iMaterial, BarrelMaxFill() - iFillLevel);
  // Zum vollen Fass werden, wenn wir es nicht bereits sind
  if(BarrelMaterial() == -1)
  {
    iFillMaterial = iMaterial+1;
    ChangeDef(idBarrel);
  }
  return(1);
}

/* Füllung */

// Aktuelle Füllung
public func GetAmount() { return(iFillLevel); }

// Füllmaterial
public func BarrelMaterial() { return(-1); }
public func BarrelMaterialName() { return(); }

// Maximale Füllung
public func BarrelMaxFill() { return(BARL_MaxFill); }

// Füllung erhöhen/verringern
public func BarrelDoFill(int iChange, int iMat)
{
  var iNewFill = BoundBy(iFillLevel + iChange, 0, BarrelMaxFill());
  if (iNewFill == iFillLevel) return();
  iNewFill -= iFillLevel; iFillLevel += iNewFill;
  // Aktuelles Füllmaterial ggf. überschreiben
  if (iMat && iChange>0) iFillMaterial = iMat;
  // Tatsächliche Änderung des Füllstandes zurückgeben
  return(iNewFill);
}

// Fass voll?
public func BarrelIsFull() 
{ 
  return(iFillLevel == BarrelMaxFill());
}

// Ein Pixel Füllung entfernen
public func BarrelExtractLiquid()
{
  // Füllung verringern
  if (!BarrelDoFill(-1)) 
    // Konnte Füllung nicht verringern: Fass ist leer/kein Material
    return(-1);
  // Füllmaterial speichern    
  var iMaterial = BarrelMaterial();
  // Jetzt leer?
  if (!iFillLevel) 
    ChangeDef(EmptyBarrelID());
  // Füllmaterial zurückliefern
  return(iMaterial);
}

/* Wert nach Füllung */

// Wert = Wert(Leeres Fass) + (Wert(Volles Fass) - Wert(Leeres Fass)) * (Füllung / MaxFüllung)
protected func CalcValue()
{
  var iValueEmpty = GetValue(0, EmptyBarrelID());
  var iValueFull = GetValue(0, GetID());
  return(iValueEmpty + ((iValueFull - iValueEmpty) * iFillLevel) / BarrelMaxFill());
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("ClonkHit*");
  BarrelEject();
  return(1);
}

private func BarrelEject()
{
  var strMaterial;
  if (!(strMaterial = BarrelMaterialName())) return(1);
  Sound("Splash1");
  CastPXS(strMaterial, iFillLevel, 20);
  iFillLevel = 0;
  ChangeDef(EmptyBarrelID());
  return(1);  
}

/* Initialisierung */

protected func Initialize()
{
  // Kein Material im Hintergrund
  iBackMaterial = -1;
  // Materialfass? Füllen
  if (BarrelMaterialName()) 
    iFillLevel = BarrelMaxFill();
  return(1);
}


/* Aktivierung auf Doppel-Graben */

protected func Activate()
{
  [$TxtEject$]
  // Ausleeren
  return(BarrelEject());
}
  
/* Produkteigenschaften */

public func IsAnvilProduct() { return(1); }
