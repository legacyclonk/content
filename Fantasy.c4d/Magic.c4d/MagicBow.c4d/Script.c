#strict

local idArrow, original_caster;

func Activate(pCaster,pRealcaster) {
  var pClonk, pComboObj, fBlast;
  // Zauberer rausfinden
  if(pRealcaster) pClonk=pRealcaster;
  else pClonk=pCaster;
  original_caster = pCaster;

  var iResult;
  if (iResult = CheckEffect("MagicBowNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  // Nach Pfeilen und Pfeilpacks durchsuchen
  if(Contents(0,pClonk) && !GetEffect("*ArrowVanish*",Contents(0,pClonk)))
  {
    if(Contents(0,pClonk)->~IsArrowPack())
    {
      idArrow = Contents(0,pClonk)->~UnpackTo();
      Contents(0,pClonk)->DoPackCount(-1);
    }
    else if(Contents(0,pClonk)->~IsArrow())
    {
      idArrow = GetID(Contents(0,pClonk)); RemoveObject(Contents(0,pClonk));        
    }
  }
  
  // Zielen wenn möglich
  if (pClonk->~DoSpellAim(this()))
  { 
   var angle=-70;
   if(GetDir(pCaster) == DIR_Right())angle=70;
   AddEffect("IntReload",pCaster,1,1,0,GetID(), angle);
   // Wenn das Zielen abgebrochen wird, sollte der Pfeil nicht verloren gehen
   if(idArrow)
   {
    AddEffect("KeepArrow",this(),1,1,0,GetID(),pCaster);
   }
   return(1); 
  }
  
  // Sonst einfach grade zielen
  ActivateAngle(pCaster, GetDir(pCaster)*180-90, 1);
  RemoveObject();
  return(1);
}

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle, fOneShot)
  {
  // Austrittsgeschwindigkeit ermitteln
  var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
 
  var pArrow; 
  
  // Damit der gleich erstellte Pfeil nicht versucht sich zu packen
  AddEffect("IntNoPacking", pCaller, 1);
  
  // Damit nicht trotzdem ein Pfeil zurueckerstattet wird
  if(idArrow)
  if(GetEffect("KeepArrow",this()))
   EffectCall(this(),GetEffect("KeepArrow",this()),"Remove");
  
  // Wenn keine Pfeil ID da ist, dann machen wir einen Magiepfeil
  if(!idArrow)
  {
      // Der Pfeil
      pArrow = CreateContents(ARRW, pCaller);      
      SetClrModulation(RGBa(128,128,255,0), pArrow);
      SetObjectBlitMode(1, pArrow); // Durchscheinend
      AddEffect("IntArrowVanish",pArrow,1,3,0,GetID()); // Verschwindet wieder
      SetMass(1, pArrow); // Bischen leichter
  }
  else pArrow = CreateContents(idArrow, pCaller); // Sonst einfach passenden Pfeil erstellen
  
  // Packeffekt wieder entfernen
  RemoveEffect("IntNoPacking", pCaller);
  
  // Zufallsabweichung
  var iDeviation = 5;
  iAngle += Random(iDeviation * 2 + 1) - iDeviation;
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 65);
  iOutYDir = -Cos(iAngle, 65);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Pfeildrehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
  // Besitzer des Projektils setzen
  pArrow->SetOwner(pCaller->GetOwner());
  // Pfeil abfeuern
  Exit(pArrow,AbsX(GetX(pCaller) + iOutX),AbsY(GetY(pCaller) + iOutY),iOutR, 0,0,iOutRDir );
  pArrow->SetXDir(iOutXDir); pArrow->SetYDir(iOutYDir);
  pArrow->~Launch();
  // Sound
  Sound("Bow");
  // Nachladen (Nur wenn von einem CrewMember gezielt wurde, also nicht bei Artefakt)
  if(!fOneShot)
  {
    if ((GetOCF(pCaller) & OCF_CrewMember()) && pCaller->~MaxMagicPhysical())
    {
      Schedule("Call(\"~DoMagic\",MGBW)", 16, 1, pCaller);
      AddEffect("IntReload",pCaller,1,21,0,GetID(), iAngle);
    }
    else if (original_caster && original_caster != pCaller)
    {
      // Zaubern direkt aus dem Zauberturm?
      if (GetCategory(original_caster) & C4D_Structure)
      {
        Schedule(Format("Call(\"~DoMagic\",MGBW,Object(%d),%d)", ObjectNumber(pCaller), GetDir(pCaller)), 16, 1, original_caster);
        AddEffect("IntReload",pCaller,1,21,0,GetID(), iAngle);
      }
    }
  }
  // Objekt löschen
  RemoveObject();
  return(1);
  }

func FxIntArrowVanishStart(object pTarget, int iNumber, int iTemp) {
  if(iTemp) return();

  // Richtung speichern
  EffectVar(0,pTarget,iNumber) = 0;
}

func FxIntArrowVanishTimer(object pTarget, int iNumber, int iTime) {
  // Wenn sich der Pfeil nicht bewegt ausfaden
  if(Abs(GetXDir(pTarget))+Abs(GetYDir(pTarget))<5)
    EffectVar(0,pTarget,iNumber)++;
  else EffectVar(0,pTarget,iNumber)=0;
  SetClrModulation(RGBa(255,255,255, EffectVar(0,pTarget,iNumber)*20));
  if(EffectVar(0,pTarget,iNumber)>=12) RemoveObject(pTarget);
}

func FxIntReloadStart(object pTarget, int iNumber, int iTemp, int iAngle) {
  if(iTemp) return();

  // Richtung speichern
  EffectVar(0,pTarget,iNumber) = iAngle;
}

func FxIntReloadStop(object pTarget, int iNumber, int iTime) {
  // Clonk in gleiche Richtung nochmal casten lassen    
  var iAngle = EffectVar(0, pTarget, iNumber);
  Message("", pTarget);
  if(!LocalN("pAimer", pTarget)) return();
  LocalN("iAngle", LocalN("pAimer", pTarget)) = iAngle;
  LocalN("pArrow", LocalN("pAimer", pTarget))->~UpdateAngle(iAngle);
  LocalN("pTargetObject", LocalN("pAimer", pTarget))->~AimingAngle(iAngle);
}

private func FxKeepArrowStart(pTarget,iEffectNumber,iTemp,par1)
{
 if(iTemp)return;
 // Zauberer merken
 EffectVar(0,pTarget,iEffectNumber)=par1;
 return 1;
}

private func FxKeepArrowTimer(pTarget,iEffectNumber)
{
 return 1;
}

private func FxKeepArrowRemove(pTarget,iEffectNumber)
{
 EffectVar(1,pTarget,iEffectNumber)=true;
}

private func FxKeepArrowStop(pTarget,iEffectNumber)
{
 // Wenn geschossen wurde nicht neu erzeugen
 if(EffectVar(1,pTarget,iEffectNumber))return;
 
 var pCaster=EffectVar(0,pTarget,iEffectNumber);
 if(!pCaster || !GetAlive(pCaster))return;
 
 if(LocalN("idArrow",pTarget))
 {
  // Den Pfeil neu erzeugen
  CreateContents(LocalN("idArrow",pTarget),pCaster);
 }
 return;
}

public func IsAimerSpell() { return(1); }

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("344"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
