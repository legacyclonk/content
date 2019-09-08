/* Schweben */

#strict

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("FloatPSpell", this(), 180)) return(iChkEff!=-1 && RemoveObject());
  
  AddEffect("FloatPSpell",oClonk,280,2,0,GetID());
  
  // An den Zauberer hängen
  //SetAction("Exist", oCaller);
  
  RemoveObject(this());
  
  return(true);
}

func FxFloatPSpellStart(pClonk, iEffectNumber, iTemp)
{
  if(iTemp) return();
  EffectVar(0,pClonk,iEffectNumber)=200;
  EffectVar(1,pClonk,iEffectNumber)=CreateObject(GVTY,0,0,GetController(pClonk));
  ObjectSetAction(EffectVar(1,pClonk,iEffectNumber), "Exist", pClonk);
}

func FxFloatPSpellTimer(pClonk, iEffectNumber, iEffectTime)
{
  if(!EffectVar(0,pClonk,iEffectNumber)) return(-1);
  if(!EffectVar(1,pClonk,iEffectNumber)) return(-1);
  EffectVar(0,pClonk,iEffectNumber)--;
  
  var iSearchRadius = GetRange();
  var pObj;
  CreateParticle("Aura",GetX(pClonk),GetY(pClonk), RandomX(-1,1),RandomX(-1,1), 12*iSearchRadius,RGB(50,255,180));
var i=2;
  while(i--)
  {
    var angle = RandomX(-180, 180);
    CreateParticle("PSpark",GetX(pClonk)+Sin(angle, iSearchRadius),GetY(pClonk)-Cos(angle, iSearchRadius),
		    -Sin(angle, iSearchRadius/3),Cos(angle, iSearchRadius/3),RandomX(50,70), 
		    RGBa(150,155,80, 75),pClonk);
  }
  CastParticles("PSpark", 3, iSearchRadius/3, GetX(pClonk), GetY(pClonk)+8, 50, 70, RGBa(150,155,80, 75), RGBa(150,155,80, 75), pClonk);
  // Schnelle Objekte in der Umgebung suchen
  while(pObj = FindObject(0, GetX(pClonk),GetY(pClonk), -1, -1, OCF_HitSpeed1, 0,0, NoContainer(), pObj))	  
    if(ObjectDistance(pClonk, pObj) > iSearchRadius) break;
    else if(!(GetOCF(pObj) & OCF_CrewMember))
      EffectVar(1,pClonk,iEffectNumber)->~DoStop(pObj, pClonk);
  return(1);
}

func FxFloatPSpellStop(pClonk, iEffectNumber, iReason, iTemp)
{
  if(iTemp) return();	
  RemoveObject(EffectVar(1,pClonk,iEffectNumber));
}

private func FxFloatPSpellEffect(string szNewEffectName, object pTarget, int iNumber) 
{ 
  if(szNewEffectName eq "FloatPSpell") { return(-2); }
}

private func FxFloatPSpellAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iChange0, int iChange1, int iChange2, int iTime) {
  // Effekt übernehmen
  EffectVar(0,pTarget,iNumber) += 300;
}

protected func FxFloatPSpellInfo() { return("{{GVTY}} $SpellInfo$"); }

private func DoStop(object pObj, object pCaller)
  {
  // Hat schon einen Effekt? beenden
  if(GetEffect("StopNSpell", pObj)) return();
  pObj->~MagicStop(pCaller);
  // In der Liste speichern
  Local()++;
  Local(Local()) = pObj;
  // StillhalteEffekt
  AddEffect("StopNSpell", pObj, 200, 1, 0, GetID());
  return(1);
  }

func FxStopNSpellTimer(pTarget, iNumber, iEffectTime)
{
  // Beenden	
  if(iEffectTime>600) return(-1);
  // Stoppen
  SetXDir(0,pTarget);	
  SetYDir(-2,pTarget);
  //SetPosition(GetX(pTarget), GetY(pTarget)-1, pTarget);
  // Ist der Zauber zu weit weg? beenden
  if( !(pTarget->FindObject(GVTY, -GetRange(), -GetRange(), GetRange()*2, GetRange()*2))) return(-1);
}

public func GetRange() { return(100); }

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("262"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)

protected func AttachTargetLost()
{
  RemoveObject();
}
