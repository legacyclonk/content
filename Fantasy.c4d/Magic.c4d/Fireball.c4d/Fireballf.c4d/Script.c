/*-- Feuerball --*/

// FireballFlight:
//  EffectVar(0): Größe
//  EffectVar(1): Zielgröße
//  EffectVar(2): Richtungswinkel
//  EffectVar(3): Abschuss frei? Wenn ja: Abschussframe+1
//  EffectVar(4): Caster (Für Positionierung)
//  EffectVar(5): Explosionsgrößenboost (Als Kombo mit Flints)

static const FIRB_Speed          = 50,
             FIRB_AimSearchRange = 200,
             FIRB_AimDirAdjust   = 20;

#strict

local iSizeBoost;

protected func Initialize()
  {
  // Feuerballeffekt zur Steuerung
  var iResult = AddEffect("FireballFlight", this(), 20, 1, this(), FIRB, 1, 100, 0);
  // Effekt abgelehnt?
  if (iResult <= 0) return(RemoveObject());
  // Flugaktivität
  SetAction("Fly");
  return(1);
  }

protected func Hit()
  {
  var iSize=EffectVar(0,this(),GetEffect("FireballFlight", this()));
  if(!iSize) return();
  iSize;
  for( var i=5; i>0; --i)
    {
    CreateParticle("Fire", RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), iSize+Random(55), RGBa(Random(255),0,0,90));
    CreateParticle("Fire2", RandomX(-2,2), RandomX(-2,2), Sin(Random(360), RandomX(50,65))+RandomX(-2,2), Cos(Random(360), RandomX(50,65))+RandomX(-2,2), iSize+Random(55));
    }
  CastObjects(DFLM, RandomX(13, 24)+iSizeBoost/20, RandomX(20,30)+iSizeBoost/20);
  BlastObjects(GetX(), GetY(), iSize/6);
  CreateParticle("Blast", 0,0,0,0, iSize*5);
  var iBs;
  if(iSize/2 > 30) iBs=3;
  if(iSize/2 > 15) iBs=2;
  else iBs=1;
  Sound(Format("Blast%d", iBs), this());
  if (iSizeBoost)
    Explode(iSizeBoost/6);
  else
    RemoveObject();
  }

public func SetCaster(object pToCaster)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("FireballFlight", this()), "SetCaster", pToCaster));
  }
  
public func SetSizeBoost(int iTo)
  {
  // Weitergeben an den Effekt
  iSizeBoost = iTo;
  return(EffectCall(this(), GetEffect("FireballFlight", this()), "SetSizeBoost", iTo));
  }

public func SetAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("FireballFlight", this()), "SetAngle", iToAngle));
  }

public func AimAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("FireballFlight", this()), "AimAngle", iToAngle));
  }

public func Abort()
  {
  // Abbruch: Objekt entfernen (Entfernt den Effekt gleich mit)
  CastParticles("Fire", 7, 25, 0, 0, 10, RGBa(0,0,0,50), RGBa(255,0,0,90));
  RemoveObject();
  }

public func FxFireballFlightSetAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel setzen; Abschuss freigeben
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  EffectVar(3, pTarget, iEffectNumber)=GetEffect(0, pTarget, iEffectNumber, 6)+1;
  return(1);
  }

public func FxFireballFlightAimAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel aktualisieren
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  return(1);
  }

public func FxFireballFlightSetCaster(object pTarget, int iEffectNumber, object pCaster)
  {
  // Caster setzen
  EffectVar(4, pTarget, iEffectNumber)=pCaster;
  return(1);
  }
  
public func FxFireballFlightSetSizeBoost(object pTarget, int iEffectNumber, int iSizeBoost)
  {
  // Vergrößerte Exposion setzen
  EffectVar(5, pTarget, iEffectNumber)=iSizeBoost;
  return(1);
  }

func FxFireballFlightStart(object pTarget, int iEffectNumber, int iTemp, int var1, int var2, int iAngle, bool fLaunchFree)
{
  if(iTemp) return();
  EffectVar(0, pTarget, iEffectNumber)=var1;
  if(var2)
    EffectVar(1, pTarget, iEffectNumber)=var2;
  else
    EffectVar(1, pTarget, iEffectNumber)=var1;
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  EffectVar(3, pTarget, iEffectNumber)=fLaunchFree;
}

func FxFireballFlightTimer(pTarget, iEffectNumber, iEffectTime)
  {
  var iSize=EffectVar(0, pTarget, iEffectNumber);
  var iMaxSize=EffectVar(1, pTarget, iEffectNumber);
  var iAngle=EffectVar(2, pTarget, iEffectNumber);
  var iLaunchFree=EffectVar(3, pTarget, iEffectNumber);
  var pCaster = EffectVar(4, pTarget, iEffectNumber);
  var iSizeBoost=EffectVar(5, pTarget, iEffectNumber);
  if(iSize<=0) return(RemoveObject());
  if(GBackLiquid()) return(RemoveObject());
  // Effekt
  for( var i=4; i>0; --i)
    {
    CreateParticle("Fire",  RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), RandomX(-2,2)+GetXDir()/2, RandomX(-2,2)+GetYDir()/2, iSize*60/100+Random(15), RGBa(Random(255),0,0,90));
    CreateParticle("Fire2", RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), RandomX(-2,2)+GetXDir()/2, RandomX(-2,2)+GetYDir()/2, iSize+Random(15));
    }
  // Ausrichtung nach Caster
  if (!iLaunchFree && pCaster)
    {
    var x=GetX(pCaster)+Sin(iAngle, 15), y=GetY(pCaster)-Cos(iAngle, 15);
    if (GetX()!=x || GetY()!=y)
      {
      DrawParticleLine("Fire", x-GetX(), y-GetY(), 0, 0, 5, 40, 16777215, 16777215, 0);
      SetPosition(x, y);
      }
    }
  // Noch nicht voll aufgeladen?
  if (iSize < iMaxSize)
    {
    for( var i=5; i>0; --i)
      {
      var iR, iD;
      var iX=Sin(iR=Random(360), iD=RandomX(iSize/10,iSize/5));
      var iY=Cos(iR, iD);
      CreateParticle("Fire", iX, iY, -iX/3, -iY/3, Random(25), RGBa(Random(255),0,0,90));
      CreateParticle("Fire2", iX, iY, -iX/3, -iY/3, Random(35));
      }
    // Vergrößern
    EffectVar(0, pTarget, iEffectNumber) = ++iSize;
    // Nichts weiter während des Wachsens
    return(1);
    }
  // Abschuss noch nicht freigegeben?
  if (!iLaunchFree) return(1);
  // Maximale Laufzeit
  if(iEffectTime-iLaunchFree>550) return(RemoveObject());
  // Geschwindigkeit anpassen
  SetXDir(Sin(iAngle, FIRB_Speed));
  SetYDir(-Cos(iAngle, FIRB_Speed));
  // Suche nach brennbaren Zielen
  var pObj;
  while(pObj=FindObject(0, 0, 1, 0, 0, OCF_Inflammable(), 0, 0, NoContainer(), pObj))
    {
    if(GBackLiquid(GetX(pObj), GetY(pObj))) continue;
    if(GetOCF(pObj) & OCF_CrewMember) if (GetAlive(pObj)) return(Hit());
    if(iSizeBoost || !Random(3)) Incinerate(pObj);
    }
  // Zielen auf CrewMember - klappt wegen Winkelüberlauf schlecht gerade nach unten (was solls...)
  var iMinAngleDiff = 360, iTargetAngle = iAngle;
  while (pObj = FindObject(0, Sin(iAngle, FIRB_AimSearchRange)-FIRB_AimSearchRange/2, -Cos(iAngle, FIRB_AimSearchRange)-FIRB_AimSearchRange/2, FIRB_AimSearchRange, FIRB_AimSearchRange, OCF_CrewMember, 0,0, NoContainer(), pObj))
    {
    if (!GetAlive(pObj)) continue;
    var iThisTargetAngle = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj));
    if (iThisTargetAngle > 180) iThisTargetAngle -= 360;
    if (Abs(iThisTargetAngle - iAngle) < iMinAngleDiff)
      {
      iMinAngleDiff = Abs(iThisTargetAngle - iAngle);
      iTargetAngle = iThisTargetAngle;
      }
    }
  EffectVar(2, pTarget, iEffectNumber) = BoundBy(iTargetAngle, iAngle-FIRB_AimDirAdjust, iAngle+FIRB_AimDirAdjust);
  // OK; weiter existieren
  return(1);
  }

func FxFireballFlightStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
  {
  if (!fTemp) RemoveObject(pTarget);
  return(1);
  }
