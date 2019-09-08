/* Wipfrennen */
#strict

func Initialize()
{
  var x,y;
  // Stalagmiten
  // Die müssen abhänging von MapZoom gemacht werden, sonst sind sie zu ungenau
  CreateObject(_STA,554,574+19,-1);
  CreateObject(_STA,580,575+19,-1);
  CreateObject(_STA,621,575+19,-1);
  CreateObject(_STA,659,566+19,-1);
  // Schlangengrube
  for(var i=0; i<5+Random(4); i++)
    CreateObject(SNKE,117*GetScenMapZoom()+15*GetScenMapZoom(),84*GetScenMapZoom(),-1);
  // Nebel
  while(i<70+Random(50)) {
    i++; 
    x=290*GetScenMapZoom()+Random(50*GetScenMapZoom());
    y=60*GetScenMapZoom()+Random(20*GetScenMapZoom());
    CreateParticle("Fog",x,y,0,0,RandomX(1000,1500));
  }
  while(Var(0)=FindObject(0,336*GetScenMapZoom(),50*GetScenMapZoom(),64*GetScenMapZoom(),21*GetScenMapZoom(),0,0,0,0,Var(0))) 
    // (GetCategory(Var(0))&C4D_Object()) das ist bei Bäume true.... die sind ja seit neuestem objekte................
    if(!Stuck(Var(0)) || ObjectCall(Var(0),"IsTree")) SetCategory(GetCategory(Var(0))|1048576,Var(0));    

  // Steinfallen    
  CreateObject(RCTP,107*GetScenMapZoom(),57*GetScenMapZoom(),-1);
  //CreateObject(RCTP,113*GetScenMapZoom(),54*GetScenMapZoom(),-1);
  CreateObject(RCTP,117*GetScenMapZoom(),52*GetScenMapZoom(),-1);
  CreateObject(RCTP,122*GetScenMapZoom(),51*GetScenMapZoom(),-1);
  //CreateObject(RCTP,125*GetScenMapZoom(),51*GetScenMapZoom(),-1);
  CreateObject(RCTP,130*GetScenMapZoom(),49*GetScenMapZoom(),-1);
  CreateObject(RCTP,133*GetScenMapZoom(),48*GetScenMapZoom(),-1);
  CreateObject(RCTP,136*GetScenMapZoom(),46*GetScenMapZoom(),-1);
  return();
}

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(1); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return(50); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(100); }

// Gewinnbedingung
//  1: Spieler hat gewonnen
//  0: gewinnen erlaubt wenn am Rand
// -1: kein gewinnen mögliche
func CheckRACEGoal(iPlr)
{
  // Nur wenn er auf einem Wipf reitet, kann er gewinnen
  if(ObjectCall(GetCursor(iPlr),"IsRiding") && GetID(GetActionTarget(0,GetCursor(iPlr)))==RWPF) 
      return(0);
  // Sonst muss er sich erst noch einen Wipf suchen
  return(-1);
}

func InitializePlayer(iPlr)
{
  SetPosition(10,10, GetHiRank(iPlr));
  return(JoinPlayer(iPlr));
}

protected func RelaunchPlayer(int iPlr)
{
 // Verkünden
 Log("$MsgRestart$", GetPlayerName(iPlr));

 // Ein neuer Clonk für den Spieler
 var pClonk = CreateObject(CLNK, 10, 10, iPlr);
 MakeCrewMember(pClonk, iPlr);
 DoEnergy(GetPhysical("Energy", 0, pClonk)/1000 - 50, pClonk);

 // Den Spieler neu starten lassen
 return(JoinPlayer(iPlr));
}

func JoinPlayer(iPlr)
{ 
  var wipf=CreateObject(RWPF,Var(1)=10+Random(8*GetScenMapZoom()),GetHgt(Var(1))-1,iPlr);
  ObjectSetAction(wipf,"Walk");
  AddEffect("IntGAMEEvtlBackground",wipf,1,10);
  // SetObjectOrder will _exakt_ die gleiche Kategorie haben
  SetCategory(8,GetHiRank(iPlr));
  SetCategory(8,wipf);
  GetCrew(iPlr)->SetAction("Ride", wipf);
  wipf->LocalN("rider") = GetCrew(iPlr);
  SetDir(1,wipf);
  SetComDir(COMD_None(),wipf);
  wipf->RWPF::AdjustVertex();
  SetCursor(iPlr,GetHiRank(iPlr));
  SelectCrew(iPlr, GetHiRank(iPlr), 1);
  AddEffect("IntGAMEEvtlBackground",GetHiRank(iPlr),1,10);
}

func OnRestart(iPlr)
{
  // Beim Neustarten Wipf mitlöschen
  var pClonk = GetCrew(iPlr), pWipf;
   if (pClonk->CLNK::IsRiding())
    if (pWipf = GetActionTarget(0, pClonk))
      RemoveObject(pWipf);
  // Clonk soll trotzdem noch entfernt werden
  return(0);
}

global func FxIntGAMEEvtlBackgroundTimer(pTarget, iEffectNumber, iEffectTime)
{
  // innerhalb von hinter-Mat-Gebiet hinters Mat verschieben
  if(~GetCategory(pTarget)&C4D_Background())
  {
    if(GetX(pTarget)>336*GetScenMapZoom()  && GetY(pTarget)<72*GetScenMapZoom())
      SetCategory(GetCategory(pTarget)|C4D_Background(),pTarget);
  }
  // außerhalb ggf Kategorie zurücksetzen
  else
  {
    if(GetX(pTarget)<=336*GetScenMapZoom() || GetY(pTarget)>=72*GetScenMapZoom())
      SetCategory(8,pTarget);
  }
}

global func FxIntGAMEEvtlBackgroundEffect(szNewEffect, iEffectTarget,iEffectNumber,iNewEffectNumber)
{
  if(szNewEffect eq "IntGAMEEvtlBackground") return(-1); 
}

func GetHgt(int iX)
{ 
  var cnt=0;
  while(!GBackSolid(iX,cnt) && cnt<=LandscapeWidth())
    cnt+=2;
  return(cnt);
}
