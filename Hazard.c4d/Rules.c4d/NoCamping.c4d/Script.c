/*-- Arena --*/

#strict

local aAFK, aCursors, aPlrAFKDeaths;

protected func Activate(iPlr) {
  // AFK-Menü
  CreateMenu(GetID(), GetCursor(iPlr), this(),0, GetName());
  if(aAFK[iPlr])
    AddMenuItem("$Back$", Format("AFK(%d,%d)",iPlr,0), CLNK, GetCursor(iPlr), 0,0, "$Back$");
  else
    AddMenuItem("$AFK$", Format("AFK(%d,%d)",iPlr,1), CLNK, GetCursor(iPlr), 0,0, "$AFK$");
}

public func AFK(int iPlr, bool afk) {
  aAFK[iPlr] = afk;

  if(afk)
  {
    aCursors[iPlr] = GetCursor(iPlr);
    var obj = CreateObject(AFKH, AbsX(aCursors[iPlr]->GetX()), AbsY(aCursors[iPlr]->GetY()), iPlr);
    SetCursor(iPlr, obj, 1,1);
    SetPlrView(iPlr, aCursors[iPlr]);
    Log("$isAFK$", GetPlayerName(iPlr));
  }
  else
  {
    aPlrAFKDeaths[iPlr] = 0;
    var obj = FindObjectOwner(AFKH, iPlr);
    if(obj) obj->RemoveObject();
    SetCursor(iPlr, aCursors[iPlr], 1,1);
    SetPlrView(iPlr, aCursors[iPlr]);
    aCursors[iPlr] = false;
    Log("$isBack$", GetPlayerName(iPlr));
  }
}

protected func Initialize() {
  aAFK = CreateArray(GetPlayerCount());
  aCursors = CreateArray(GetPlayerCount());
  aPlrAFKDeaths = CreateArray(GetPlayerCount());
  Schedule("Log(\"$Intro$\");", 1);
}

/* Alle Clonks bekommen einen Effekt, der ihr Verhalten überprüft */

public func OnClonkRecruitment(object pClonk) {
  // Effekt erzeugen
  Schedule(Format("AddEffect(\"CampingCheck\", Object(%d), 20, 20, this());", ObjectNumber(pClonk)), 1);
}

public func FxCampingCheckStart(object pClonk, int iNo, int iTemp)
{
  if(iTemp) return();

  EffectVar(0, pClonk, iNo) = pClonk->GetX();
  EffectVar(1, pClonk, iNo) = pClonk->GetY();
  EffectVar(5, pClonk, iNo) = pClonk->GetX();
  EffectVar(6, pClonk, iNo) = pClonk->GetY();
}

// Effektvariablen
// EffectVar 0 und 1 speichern die Position des Clonks
// EffectVar 2 ist ein Check, für minimale Bewegungsänderung und führt sehr schnell zur Bestrafung
// EffectVar 3 checkt auf weitere Entfernungen. Auch Clonks, die ständig an der selben Stelle rumturnen, bekommen eine Bestrafung
// EffectVar 4,5,6 helfen Nr. 3

public func FxCampingCheckTimer(object pClonk, int iNo, int iTime)
{
  // Clonk ist kein Cursor -> kein Campen
  // Spieler ist AFK -> kein Campen
  if(GetCursor(pClonk->GetOwner()) != pClonk || aAFK[pClonk->GetOwner()])
  {
    if(EffectVar(2, pClonk, iNo)) EffectVar(2, pClonk, iNo)--;
    if(EffectVar(3, pClonk, iNo)) EffectVar(3, pClonk, iNo)--;
    return();
  }
  // Clonk befindet sich in TIM1 oder TIM2 -> kein Campen
  if(pClonk->Contained())
    if(pClonk->Contained()->GetID() == TIM1 || pClonk->Contained()->GetID() == TIM1)
    {
      if(EffectVar(2, pClonk, iNo)) EffectVar(2, pClonk, iNo)--;
      if(EffectVar(3, pClonk, iNo)) EffectVar(3, pClonk, iNo)--;
      return();
    }
  // Spieler starb 2x durch "Campen" -> auto-AFK
  if(aPlrAFKDeaths[pClonk->GetOwner()] >= 2)
    return(AFK(pClonk->GetOwner(), 1));
  // Check, damit nur einmal die Warnnachricht erscheint
  var warn = true;
  if(EffectVar(2, pClonk, iNo) >= 25 ||
     EffectVar(3, pClonk, iNo) >= 50)
       warn = false;
  // Clonk hat sich nur minimal bewegt?
  var inc;
  if(Inside(EffectVar(0,pClonk,iNo), pClonk->GetX()-5, pClonk->GetX()+5))
    if(Inside(EffectVar(1,pClonk,iNo), pClonk->GetY()-5, pClonk->GetY()+5))
    {
      if(EffectVar(2, pClonk, iNo) < 25)
        EffectVar(2, pClonk, iNo)++;
      inc = true;
    }
  // Clonk hat sich doller bewegt? -> Abbauen
  if(!inc && EffectVar(2, pClonk, iNo))
    EffectVar(2, pClonk, iNo)--;
  // Größerer Check
  inc = false;
  if(Inside(EffectVar(5,pClonk,iNo), pClonk->GetX()-75, pClonk->GetX()+75))
    if(Inside(EffectVar(6,pClonk,iNo), pClonk->GetY()-75, pClonk->GetY()+75))
    {
      if(EffectVar(3, pClonk, iNo) < 50)
        EffectVar(3, pClonk, iNo)++;
      inc = true;
    }
  EffectVar(4, pClonk, iNo)++;
  // Clonk hat sich bewegt -> Camp-Tode wegmachen, da kein auto-AFK (ist nicht 100%ig sicher, aber besser als nichts)
  if(!inc && aPlrAFKDeaths[pClonk->GetOwner()])
    aPlrAFKDeaths[pClonk->GetOwner()] = 0;
  // Die Großflächenposition wird nur alle 10 Checks erneuert -> Clonk muss sich in diesem Zeitraum mindestens 100 Pixel weit bewegen
  if(EffectVar(4, pClonk, iNo) >= 10)
  {
    EffectVar(4, pClonk, iNo) = 0;
    EffectVar(5, pClonk, iNo) = pClonk->GetX();
    EffectVar(6, pClonk, iNo) = pClonk->GetY();
  }
  // Clonk hat sich doller bewegt? -> Abbauen
  if(!inc && EffectVar(3, pClonk, iNo))
    EffectVar(3, pClonk, iNo)--;

  // Neue Position speichern
  EffectVar(0, pClonk, iNo) = pClonk->GetX();
  EffectVar(1, pClonk, iNo) = pClonk->GetY();

//  Log("%d", EffectVar(2, pClonk, iNo));
//  Log("%d", EffectVar(3, pClonk, iNo));
  // Bestrafung
  if(EffectVar(2, pClonk, iNo) >= 25 ||
     EffectVar(3, pClonk, iNo) >= 50)
     {
       // Warnmeldung, erst danach kommen Strafen
       if(warn)
       {
         PlayerMessage(pClonk->GetOwner(), "$CampingAlert$", pClonk);
         Sound("Ding", 1, 0,0, pClonk->GetOwner()+1);
       }
       else
       {
         // Tod durch Campen -> wird gezählt
         if(pClonk->GetEnergy() <= 5) aPlrAFKDeaths[pClonk->GetOwner()]++;
         pClonk->DoEnergy(-5);
         Sound("Punch*", 1, 0,0, pClonk->GetOwner()+1);
       }
     }
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }
