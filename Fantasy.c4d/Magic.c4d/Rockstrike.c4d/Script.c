/*-- Steinschlag --*/

#strict

func Activate(pCaller)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("BigRockNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  // Zauber
  Sound("Magic*");

  // Zielen wenn möglich
  if (pCaller->~DoSpellAim(this())) return(1);
  // Clonk kann nicht zielen:
  ActivateAngle(pCaller, GetDir(pCaller)*180-90);
  return(1);
}

local pClonk;
local iAimAngle;
local rock_id;

// Aktivierung nach Zielen (iAngle: -90=links; 0=oben; +90=rechts)
public func ActivateAngle(object pCaller, int iAngle)
  {
  // Erst nach dem Zielen: Nach Pfeilen für einen Pfeilhagel suchen
  rock_id = BIRK;
  var pArrow;
  if (pCaller) if (pArrow = pCaller->~GetComboArrow(this()))
  {
    rock_id = GetID(pArrow);
    RemoveObject(pArrow);
  }

  SetPosition(GetX(pCaller), GetY(pCaller));
  pClonk = pCaller;
  iAimAngle = iAngle;
  var i = RandomX(10, 15);
  while(i--)
  { ScheduleCall(this(), "CastRock", i, 0, pClonk, iAimAngle+RandomX(10, -10)); 
  }
  // Objekt löschen
  return(Schedule("RemoveObject()", 100, 0, this()));
  }

protected func CastRock(object pCaller, int iAngle)
{
   var iOutX, iOutY, iOutR, iOutXDir, iOutYDir, iOutRDir;
 
  // Austrittsgeschwindigkeit ermitteln 
  iOutXDir = Sin(iAngle, 100);
  iOutYDir = -Cos(iAngle, 100);
  // Austrittsposition... 
  iOutX = Sin(iAngle, 25 * GetCon() / 100);
  iOutY = -Cos(iAngle, 25 * GetCon() / 100); 
  // Drehung 
  iOutR = iAngle; 
  if(iAngle > 0) iOutRDir = 3; else iOutRDir = -3;
  
  // Stein abfeuern
  var pRock = CreateObject(rock_id, GetX(pCaller)-GetX()+iOutX, GetY(pCaller)-GetY()+iOutY, GetOwner(pCaller));
  if (pRock)
    {
    if(rock_id!=BIRK) AddEffect("IntArrowVanish",pRock,1,3,0,MGBW);
    pRock->SetXDir(iOutXDir); pRock->SetYDir(iOutYDir);
    pRock->SetR(iOutR); pRock->SetRDir(iOutRDir);
    if(rock_id->~IsArrow()) pRock->~Launch();
    }
}

public func IsAimerSpell() { return(1); }

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("432"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
