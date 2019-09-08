/*-- Projektil --*/

// EarthBloodSuckerFlight:
//  EffectVar(0): Größe
//  EffectVar(1): Zielgröße
//  EffectVar(2): Richtungswinkel
//  EffectVar(3): Abschuss frei? Wenn ja: Abschussframe+1
//  EffectVar(4): Caster (Für Positionierung)
//  EffectVar(5): Explosionsgrößenboost (Als Kombo mit Flints)

static const MBLS_Speed          = 40,
             MBLS_AimSearchRange = 200,
             MBLS_AimDirAdjust   = 10;

#strict

local iSizeBoost;

protected func Initialize()
  {
  // Effekt zur Steuerung
  var iResult = AddEffect("EarthBloodSuckerFlight", this(), 20, 1, this(), MBLS, 1, 100, 0);
  // Effekt abgelehnt?
  if (iResult <= 0) return(RemoveObject());
  // Flugaktivität
  SetAction("Fly");
  return(1);
  }

private func FindHitTarget(int range)
  {
  var targets = FindHitTargets(range);
  if (GetLength(targets)) return targets[0];
  }
  
private func FindHitTargets(int range)
  {
  var owner_check;
  if (!ObjectCount(NTMG)) owner_check = Find_Hostile(GetController());
  var range_check;
  if (!range) range_check = Find_AtPoint(); else range_check = Find_Distance(range);
  return FindObjects(Sort_Distance(), range_check, Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_Exclude(pCaster), Find_PathFree(), owner_check);
  }
 
private func HitTarget(object pObject)
  {
  var line = CreateObject(S_10);
  DrawVertexLine(line, GetX(pCaster), GetY(pCaster), GetX(), GetY(), 10);
  ObjectSetAction(line, "Connect", pObject, pCaster);
  return true;
  }

protected func Hit()
  {
  var pObject = FindHitTarget();
  if(pObject) HitTarget(pObject);
  RemoveObject();
  return(1);
}

private func DrawVertexLine (obj, x0, y0, x1, y1, prtdist)
  {
  // Parameter gültig?
  if (!prtdist) return(0);
  // Anzahl der benötigten Vertexes berechnen
  var prtnum = Max(Distance(x0, y0, x1, y1) / prtdist, 2);
  if(prtnum>24) prtnum = 24;
  var i=prtnum;
  var count;
  // Vertexes erzeugen!
  while (i>-1)
    {
    AddVertex(x0+(x1-x0)*i/prtnum, y0+(y1-y0)*i--/prtnum, obj);
    count++;
    }
  // Erfolg; Anzahl erzeugter Vertexes zurückgeben
  return (prtnum);
  }

local pCaster;
public func SetCaster(object pToCaster)
  {
  pCaster = pToCaster;    
  SetController(GetController(pCaster));
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("EarthBloodSuckerFlight", this()), "SetCaster", pToCaster));
  }

public func SetAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("EarthBloodSuckerFlight", this()), "SetAngle", iToAngle));
  }

public func AimAngle(int iToAngle)
  {
  // Weitergeben an den Effekt
  return(EffectCall(this(), GetEffect("EarthBloodSuckerFlight", this()), "AimAngle", iToAngle));
  }

public func Abort()
  {
  // Abbruch: Objekt entfernen (Entfernt den Effekt gleich mit)
  CastParticles("NoGravSpark", 7, 25, 0, 0, 10, RGB(200,0,10), RGB(100,0,0));
  RemoveObject();
  }

public func FxEarthBloodSuckerFlightSetAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel setzen; Abschuss freigeben
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  EffectVar(3, pTarget, iEffectNumber)=GetEffect(0, pTarget, iEffectNumber, 6)+1;
  return(1);
  }

public func FxEarthBloodSuckerFlightAimAngle(object pTarget, int iEffectNumber, int iAngle)
  {
  // Winkel aktualisieren
  EffectVar(2, pTarget, iEffectNumber)=iAngle;
  return(1);
  }

public func FxEarthBloodSuckerFlightSetCaster(object pTarget, int iEffectNumber, object pCaster)
  {
  // Caster setzen
  EffectVar(4, pTarget, iEffectNumber)=pCaster;
  return(1);
  }

func FxEarthBloodSuckerFlightStart(object pTarget, int iEffectNumber, int iTemp, int var1, int var2, int iAngle, bool fLaunchFree)
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

func FxEarthBloodSuckerFlightTimer(pTarget, iEffectNumber, iEffectTime)
  {
  var iSize=EffectVar(0, pTarget, iEffectNumber);
  var iMaxSize=EffectVar(1, pTarget, iEffectNumber);
  var iAngle=EffectVar(2, pTarget, iEffectNumber);
  var iLaunchFree=EffectVar(3, pTarget, iEffectNumber);
  var pCaster = EffectVar(4, pTarget, iEffectNumber);
  var iSizeBoost=EffectVar(5, pTarget, iEffectNumber);
  if(iSize<=0) return(RemoveObject());
  // Effekt
  for( var i=1; i>0; --i)
    {
    CreateParticle("NoGravSpark",RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), -Sin(iAngle+30,10), Cos(iAngle+30,10), iSize*60/100+Random(15), RGB(RandomX(100,200),0,RandomX(10,0),0));    
    CreateParticle("NoGravSpark",RandomX(-iSize/50,iSize/50), RandomX(-iSize/50,iSize/50), -Sin(iAngle-30,10), Cos(iAngle-30,10), iSize*60/100+Random(15), RGB(RandomX(100,200),0,RandomX(10,0),0));    
    }
  // Ausrichtung nach Caster
  if (!iLaunchFree && pCaster)
    {
    var x=GetX(pCaster)+Sin(iAngle, 15), y=GetY(pCaster)-Cos(iAngle, 15);
    if (GetX()!=x || GetY()!=y)
      {
      DrawParticleLine("NoGravSpark", x-GetX(), y-GetY(), 0, 0, 5, 40, RGB(255), RGB(255), 0);
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
      CreateParticle("NoGravSpark", iX, iY, -iX/3, -iY/3, Random(25), RGBa(Random(255),0,0,90));
      CreateParticle("NoGravSpark", iX, iY, -iX/3, -iY/3, Random(35));
      }
    // Vergrößern
    EffectVar(0, pTarget, iEffectNumber) = iSize+=10;
    // Nichts weiter während des Wachsens
    return(1);
    }
  // Abschuss noch nicht freigegeben?
  if (!iLaunchFree) return(1);
  // Maximale Laufzeit
  if(iEffectTime-iLaunchFree>100) return(RemoveObject());
  // Geschwindigkeit anpassen
  SetXDir(Sin(iAngle, MBLS_Speed));
  SetYDir(-Cos(iAngle, MBLS_Speed));
  // Suche nach Crewmember zum Auftreffen
  var pObj = FindHitTarget();
  if (pObj) { HitTarget(pObj); RemoveObject(); return FX_OK; }
  // Suche nach Crewmember zum Ändern der Richtung
  var iMinAngleDiff = 360, iTargetAngle = iAngle;
  for (pObj in FindHitTargets(MBLS_AimSearchRange))
    {
    var angle = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj));
    var iR = iAngle;
    // Winkeldifferenz Flugbahn zu Zielbahn
    var dR = angle - iR;
    // In den Bereich -180...+180 bringen
    dR -= ((dR+1980)/360-5)*360;
    // Keine zu große Anpassung
    if(Abs(dR)>90) continue;
    // Ansonsten in Richtung des Zieles anpassen
    EffectVar(2, pTarget, iEffectNumber) += BoundBy(dR, -MBLS_AimDirAdjust, +MBLS_AimDirAdjust);
    break;
    }
  // OK; weiter existieren
  return(1);
  }

func FxEarthBloodSuckerFlightStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
  {
  if (!fTemp) RemoveObject(pTarget);
  return(1);
  }
