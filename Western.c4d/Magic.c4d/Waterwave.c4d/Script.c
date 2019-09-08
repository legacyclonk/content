/*-- Wasserwelle --*/

#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  var iNum = AddEffect("WaterwaveNSpell",0,130,1,0,GetID(),0,GetX(pCaster),GetY(pCaster),pCaster);
  if (iNum <= 0) return (iNum<0, RemoveObject());
  Sound("MgWind*");
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // its magic!
  var iNum = AddEffect("WaterwaveNSpell",0,130,1,0,GetID(),Abs(iAngle)-90,GetX(pCaller),GetY(pCaller),pCaller);
  if (iNum <= 0) return (iNum<0, RemoveObject());
  Sound("MgWind*");

  // Objekt löschen
  RemoveObject();
  return(1);
  }

func FxWaterwaveNSpellStart(object pTarget, int iNumber, int iTemp, int iAngle, int iX, int iY, object pCaller) {
  if(iTemp) return();

  // Radius anpassen
  EffectVar(1,pTarget,iNumber) = 20;

  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
}

func FxWaterwaveNSpellTimer(object pTarget, int iNumber, int iTime) {

  var iRaduis = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit
  if(iRaduis >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);

  // Position
  var iX = EffectVar(2,pTarget,iNumber);
  var iY = EffectVar(3,pTarget,iNumber);
  
  // Effekt!
  CreateParticle("WaterWave",iX,iY, RandomX(-1,1),RandomX(-1,1), 400+10*iRaduis,RGB(255,255,255));

  var angle, obj;
  // Alle Gegner wegschleudern
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, OCF_Alive(), 0,0, NoContainer(), obj))
  { 
    if(obj == Contained()) continue;
    if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-5, iRaduis+5))
    {   
    angle=Angle(iX, iY, GetX(obj), GetY(obj) );     
    Fling(obj, Sin(angle, 80)/9, -Cos(angle, 80)/9);
    }
  } 
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, OCF_OnFire(), 0,0, NoContainer(), obj))
  {
    if(OnFire(obj)) Extinguish (obj);  
  } 
    
  EffectVar(1,pTarget,iNumber)+=8;
}

func FxWaterwaveNSpellMaxRange() { return(100); }

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return("344"); }
