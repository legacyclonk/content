/* Verseuchen */

#strict

local iRand;
local fSnake;

public func Activate(pCaster,pRealcaster) {
  Sound("PlagueWave");
  if(pRealcaster) pCaster=pRealcaster;

  var iResult;
  if (iResult = CheckEffect("CurseNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  var pCombo;  
  if (pCombo = FindContents(SNKE, pCaster))
    {
    fSnake = true;
    RemoveObject(pCombo);
    }
  
  SetAction("Active",pCaster);
  return(1);
}

protected func Effect() {
  iRand=RandomX(GetActTime()*70,GetActTime()*90);
  CreateParticle("PxSpark",0,0,0,0,iRand,RGB(40,255,0));
}

protected func Slay() {
  var pClonk;
  while(pClonk=FindObject(0,-iRand/6,-iRand/6,iRand/3,iRand/3,OCF_Alive(),0,0,0, pClonk))
    if(GetActionTarget() != pClonk)
      AddEffect("Poison",pClonk,182,10,0,GetID(), fSnake);
  RemoveObject();
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { if (GetName(0, ROK1)) return(ROK1); else return(EART); }
public func GetSpellCombo(pMage) { if (GetName(0, ROK1)) return ("455"); else return("244"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

func FxPoisonStart(object pClonk,int iEffectNumber, int iTemp, fSnake)
{
  // temporäre Effekte ignorieren
  if(iTemp) return();
  var idType = GetID(pClonk);
  if(GetDefHeight(idType)>GetDefWidth(idType)) EffectVar(0, pClonk, iEffectNumber) = GetDefHeight(idType);
  else EffectVar(0, pClonk, iEffectNumber) = GetDefWidth(idType);
  EffectVar(1, pClonk, iEffectNumber) = fSnake;
}

func FxPoisonTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(iEffectTime >= 1200) return(-1);
  var fSnake = EffectVar(1, pClonk, iEffectNumber);
  var idType = GetID(pClonk);
  CreateParticle("PoisonWave", GetX(pClonk), GetY(pClonk), 0, -1, EffectVar(0, pClonk, iEffectNumber)*5+50, RGBa(fSnake*200, 255, 0), pClonk, 1);
  CreateParticle("PSpark", GetX(pClonk), GetY(pClonk), 0, -1, EffectVar(0, pClonk, iEffectNumber)*5+50, RGBa(fSnake*55, 55, 0), pClonk, 1);
  if(GBackLiquid(GetX(pClonk), GetY(pClonk))) return(-1);
         
  if(!Random(5-2*fSnake)) DoEnergy(-1, pClonk);
  var obj;
  var iRadius = EffectCall(pClonk,iEffectNumber,"MaxRange");
  while(obj = FindObject(0, -iRadius+GetX(pClonk),-iRadius+GetY(pClonk),iRadius*2,iRadius*2, OCF_Alive(), 0,0, NoContainer(), obj))
  {
    if(!GetEffect("Poison", obj)) if(!GetAction(obj)S="Field") AddEffect("Poison",obj,182,10,0,MGPL,fSnake);
  }
  return(1); 
}

func FxPoisonMaxRange() { return(25); }

func FxPoisonStop(object pTarget,int iEffectNumber,int iReason, bool fTemp)
{
  if(fTemp) return();
  return(1);
}

func FxPoisonEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
  // keine weiteren Vergiftungen zulassen während diese noch ausgestanden wird
  if(szNewEffectName eq "Poison") return(-1);
  // wenn man vergiftet ist, ist das Amulett des Athleten wirkungslos
//  if(szNewEffectName eq "PhysicalBless") return(-1);
}
