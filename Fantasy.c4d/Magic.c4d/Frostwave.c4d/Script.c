#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;
  
  var iNum = AddEffect("FrostwaveNSpell",0,130,1,0,GetID(),0,GetX(pCaster),GetY(pCaster),pClonk);
  if (iNum <= 0) return (iNum<0, RemoveObject());
  Sound("MgWind*");
  RemoveObject();
  return(1);
}

func FxFrostwaveNSpellStart(object pTarget, int iNumber, int iTemp, int iAngle, int iX, int iY, object pCaller) {
  if(iTemp) return();
  
  // Radius anpassen
  EffectVar(1,pTarget,iNumber) = 20;

  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
  
  // Zauberer
  EffectVar(4,pTarget,iNumber) = pCaller;
  EffectVar(6,pTarget,iNumber) = GetController(pCaller);
  
  // Kombo
  var pCombo = FindContents(ICE1, pCaller);
  if (pCombo)
    {
    EffectVar(5,pTarget,iNumber) = true;
    RemoveObject(pCombo);
    }
}

func FxFrostwaveNSpellTimer(object pTarget, int iNumber, int iTime) {

  var iRaduis = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit
  if(iRaduis >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);

  // Position
  var iX = EffectVar(2,pTarget,iNumber);
  var iY = EffectVar(3,pTarget,iNumber);
  
  // Kombo?
  var dwSearchOCF = OCF_Alive;
  if (EffectVar(5,pTarget,iNumber)) dwSearchOCF += OCF_Collectible;
  
  // Effekt!
  CreateParticle("FrostWave",iX,iY, RandomX(-1,1),RandomX(-1,1), 400+10*iRaduis,RGB(255,255,255));
var obj;

  var angle;
  // Alle Gegner wegschleudern
  while(obj = FindObject(0, -iRaduis-5+iX,-iRaduis-5+iY,iRaduis*2+10,iRaduis*2+10, dwSearchOCF, 0,0, NoContainer(), obj))
  {
    if(obj == Contained()) continue;
    if(Inside(Distance(GetX(obj), GetY(obj), iX, iY), iRaduis-5, iRaduis+5))
    {   
    angle=Angle(iX, iY, GetX(obj), GetY(obj) );     
    if(GetAlive(obj)) 
    { 
        obj->SetKiller(EffectVar(6,pTarget,iNumber));
        Fling(obj, Sin(angle, 80)/9, -Cos(angle, 80)/9);
        if(!Frozen(obj)) Freeze(obj, EffectVar(4,pTarget,iNumber)); 
    }
    else 
     { 
        SetXDir(Sin(angle, 50), obj); SetYDir(-Cos(angle, 50), obj);
        SetController(GetController(EffectVar(4,pTarget,iNumber)),obj);
     }
    }
  } 
    
  EffectVar(1,pTarget,iNumber)+=8;
}

func FxFrostwaveNSpellMaxRange() { return(150); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("335"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
