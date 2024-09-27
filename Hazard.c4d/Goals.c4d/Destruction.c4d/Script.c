/* Destruction */

#strict 2
#include TEAM

local aTargets;
// aTarget-Struktur
// [pTarget,  iTeam,  fOpt            ]
// [Ziel,     Team,   Optionales Ziel?]

public func Initialize()
{
  aTargets=[];
  iWinScore=10;
  return _inherited(...);
}

public func Activate(int iPlr)
{
  var Targets="";
  for(var tgt in aTargets)
  {
    if(tgt[1]==GetTeam(iPlr))
      if(tgt[2]==false)
        Targets=Format("%s%s,",Targets,GetName(tgt));
  }
  return MessageWindow(Format("%s $TargetsRemain$ %s",GetDesc(),Targets),iPlr);
}

// Ein Ziel hinzufügen...
public func AddTarget(object pTrgt, bool fOpt, int iMaxDmg, bool fNoBar, int iTeam)
{
  // Kein Ziel?
  if(!pTrgt)
    return false;
  // sonst hinzufügen...
  aTargets[GetLength(aTargets)] = [pTrgt,iTeam,fOpt];
  AddEffect("DEST_TargetCheck",pTrgt,50,0,0,GetID(),[iMaxDmg,iTeam,fNoBar,fOpt]);
	pTrgt->SetTeam(iTeam);
  return true;
}

// ... und wieder entfernen.
public func RemoveTarget(object pTrgt)
{
  // Kein Ziel?
  if(!pTrgt)
    return false;
  
  var newTargets=[];
  for(var aTarget in aTargets)
  {
    // Keine Arraystruktur? Überspringen...
    if(!GetLength(aTarget))
      continue;
    // Das aus der Liste zu entfernende Objekt? Überspringen...
    if(aTarget[0]==pTrgt)
      continue;
    newTargets[GetLength(newTargets)]=aTarget;
  }
  aTargets=newTargets;
  return true;
}

// Ein Ziel suchen...
public func FindTarget(object pTrgt)
{
  var aTarget;
  // Abwärts suchen...
	var i;
  for(i=GetLength(aTargets)-1;i>0;i--)
  {
    aTarget=aTargets[i];
    // Keine Arraystruktur? Überspringen...
    if(!GetLength(aTarget))
      continue;
    // Gefunden! (Rückgabe: Index + Werte)
    if(aTarget[0]==pTrgt)
      return [i,aTarget];
  }
  return -1;
}

// Ein Ziel wurde zerstört...
public func TargetDestructed(pTrgt,iTeam,iKiller)
{
  Log("$TargetDestroyed$",GetName(pTrgt),iTeam);
  Sound("TargetDestroyed",1);
  // Entfernen...
  if(pTrgt)
    RemoveTarget(pTrgt);
  // Schauen ob das Team noch ein Hauptziel hat...
  var fHaveTarget;
  for(var aTarget in aTargets)
  {
    // Keine Arraystruktur? Überspringen...
    if(!GetLength(aTarget))
      continue;
    // Gehört dem Team?
    if(aTarget[1]==iTeam)
    {
      if(aTarget[2]) continue;
      fHaveTarget=true;
      break;
    }
  }
  if(!fHaveTarget)
    EndOfTeam(iTeam);
}

// Alle Ziele eines Teams wurden zerstört
public func EndOfTeam(iTeam)
{
  // Alle Ziele zerstört... allen anderen Teams nen Punkt geben und dieses Team eliminieren
  var plr,team;
  for(var i = 0 ; i < GetPlayerCount() ; i++)
  {
    plr = GetPlayerByIndex(i);
    team = GetPlayerTeam(plr);
    if(team != iTeam)
    {
      if(!Var(team))
      {
        DoTeamScore(team, 1);
        Var(team)=1;
      }
    }
    else
    {
      DoEvaluation(plr);
      EliminatePlayer(plr);
    }
  }
}

//**----_/ FxDEST_TargetCheck \_----**//
// EffectVars:
// 0 - MaxDamage
// 1 - Die Lebensleiste
// 2 - Team
// --------------------------------------------


func FxDEST_TargetCheckStart(pTrgt,iNum,iTmp,arr)
{
  if(iTmp)
    return 0;
  EffectVar(0,pTrgt,iNum)=arr[0];
  EffectVar(2,pTrgt,iNum)=arr[1];
	
  var dwClr = RGBa(230,10,10,50);
	if(arr[3])
		dwClr=RGBa(180,230,10,50);
  // Energieleiste erstellen
  if(!arr[2])
  {
    var pBar=CreateObject(EBAR,0,0,-1);
    pBar->Set(pTrgt,dwClr);
    EffectVar(1,pTrgt,iNum)=pBar;
  }
  return 1;
}

func FxDEST_TargetCheckDamage(pTrgt,iNum,iNewDmg,iCause)
{
  var pBar=EffectVar(1,pTrgt,iNum);
  var iMaxDmg=EffectVar(0,pTrgt,iNum);
  var iDmg=GetDamage(pTrgt)+iNewDmg/*/1000*/;
  if(GetAlive(pTrgt))
    iDmg=pTrgt->GetPhysical("Energy")/1000-pTrgt->GetEnergy()+iNewDmg/1000;
  // Maximalschaden wurde erreicht: Effekt beenden
  if(iDmg>iMaxDmg)
    RemoveEffect(0,pTrgt,iNum);
  // Keine Energieleiste... nichts zum Updaten
  if(!pBar)
    return iNewDmg;
  // Sonst updaten
  pBar->Update(100-BoundBy(iDmg*100/iMaxDmg,0,100));
  return iNewDmg;
}


func FxDEST_TargetCheckStop(pTrgt,iNum,iReason,fTmp)
{
  if(fTmp)
    return 0;
    
  var iTeam=EffectVar(2,pTrgt,iNum);
  var pBar=EffectVar(1,pTrgt,iNum);
  GameCallEx("TargetDestructed",pTrgt,iTeam,GetKiller(pTrgt));
  
  if(pBar)
    RemoveObject(pBar);
  return 1;
}

//public func IsConfigurable() { return true; }