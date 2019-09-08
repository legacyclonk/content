#strict

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;

  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this(), pCaster)) return(1);
  
  if (pComboObj = FindContents(SPHR, pCaster))
    {
    fBlast = true;
    RemoveObject(pComboObj);
    }

  var iNum = AddEffect("AirblastNSpell",0,130,1,0,GetID(),90*(GetDir(pCaster)*2-1),GetX(pCaster),GetY(pCaster),pCaster);
  if (iNum <= 0) return (iNum<0, RemoveObject());
  EffectVar(4, 0, iNum) = fBlast;
  EffectVar(5, 0, iNum) = GetOwner();
  Sound("MgWind*");
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  var fBlast, pComboObj;
  if (pComboObj = FindContents(SPHR, pCaller))
    {
    fBlast = true;
    RemoveObject(pComboObj);
    }
  // its magic!
  var iNum = AddEffect("AirblastNSpell",0,130,1,0,GetID(),iAngle,GetX(pCaller),GetY(pCaller),pCaller);
  if (iNum <= 0) return (iNum<0, RemoveObject());
  Sound("MgWind*");
  
  EffectVar(4, 0, iNum) = fBlast;
  EffectVar(5, 0, iNum) = GetOwner();

  // Objekt löschen
  RemoveObject();
  return(1);
  }

// EffectVars: 0 - Winkel
//             1 - Entfernung vom Start (0...MaxRange)
//             2 - Start:X
//             3 - Start:Y

func FxAirblastNSpellStart(object pTarget, int iNumber, int iTemp, int iAngle, int iX, int iY, object pCaller) {
  if(iTemp) return();

  // Richtung speichern
//  var iAngle = EffectVar(0,pTarget,iNumber);// = (iAngle+90)*(GetDir(pCaller)*2-1);
  EffectVar(0,pTarget,iNumber) = iAngle;
  EffectVar(1,pTarget,iNumber) = 20;

  // Position
  EffectVar(2,pTarget,iNumber) = iX;
  EffectVar(3,pTarget,iNumber) = iY;
  
  // Controller merken
  EffectVar(6,pTarget,iNumber) = GetController(pCaller);
  
  // globalen Wind beeinflussen
  SetWind(GetWind()+Sin(iAngle,40));
}

func FxAirblastNSpellTimer(object pTarget, int iNumber, int iTime) {
  var iPos, pObj, iX, iY, iSpeed, iRandom, iAngle, iSize, iStep;

  var iPos = EffectVar(1,pTarget,iNumber);
  // löschen falls schon zu weit alles weggeblasen
  if(iPos >= EffectCall(pTarget,iNumber,"MaxRange")) return(-1);

  iStep = -iPos/18+EffectCall(pTarget,iNumber,"MaxRange")/17;

  iX = EffectVar(2,pTarget,iNumber);
  iY = EffectVar(3,pTarget,iNumber);

  iAngle = EffectVar(0,pTarget,iNumber);
  iSize = iStep+iPos/5;
  
  iX += Sin(iAngle, iPos);
  iY -= Cos(iAngle, iPos);

  //Log("%d,%d",iSize,iStep);


  //  Log("%d,%d,%d,%d",iX+Sin(iAngle,iPos)-iSize,iY-Cos(iAngle,iPos)-iSize,iSize*2,iSize*2);

  // Objekte wegblasen
  while(pObj=FindObject(0,iX-iSize,iY-iSize,iSize*2,iSize*2,OCF_Collectible()|OCF_Alive(),0,0,NoContainer(),pObj))
    {
    if(Stuck(pObj)) continue;
    if (GetProcedure(pObj) eq "ATTACH") continue; // Keine getragenen Schilde, Alchemiebeutel, etc.
    iSpeed=iStep*6;
    if(iSpeed<0) iSpeed=0;
   
    // Sound fürs Wegblasen
    Sound("MgWndB", false, pObj);

    //Log("%s: %d; %d,%d",GetName(pObj),iSpeed,Sin(iAngle,iSpeed+20),-iSpeed);

    if(GetAlive(pObj))
      {
      iRandom=Random(3);
      //DoEnergy(-1-iRandom,pObj);
      //pObj->~CatchBlow(-1-iRandom,this());
      pObj->SetKiller(EffectVar(6,pTarget,iNumber));
      Fling(pObj,Sin(iAngle,iSpeed+20)/9,-iSpeed/2/9);
      }
    else
      {
       SetSpeed(Sin(iAngle,iSpeed+20),-iSpeed/2,pObj);
       
       // Richtigen Controller setzen
       SetController(EffectVar(6,pTarget,iNumber),pObj);
      }
    }
    
  // Explosionen bei Kombo
  var iMat;
  if (EffectVar(4, pTarget, iNumber))
    if (GBackSolid(iX, iY))
      {
      iMat = GetMaterial(iX,iY);
      if (iMat >= 0 && iMat != Material("Vehicle"))
        {
        Explode(17, CreateObject(ROCK, iX,iY,EffectVar(5, pTarget, iNumber)));
        return(-1);
        }
      }

  var clr = iPos*255/EffectCall(pTarget,iNumber,"MaxRange");
  CreateParticle("PSpark",iX,iY,0,0,iSize*15,RGBa(200,200,255,clr*127/255));

  // alle 20 Pixel wird nach Objekten gesucht
  EffectVar(1,pTarget,iNumber)+=iStep;
}

func FxAirblastNSpellMaxRange() { return(170); }

public func IsAimerSpell() { return(1); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("333"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
