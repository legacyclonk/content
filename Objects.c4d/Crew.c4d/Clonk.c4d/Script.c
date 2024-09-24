/*-- Der Clonk --*/

#strict

// Zauberei - ben�tigt, wenn der Clonk Zielzauber z.B. aus dem Zauberturm zaubert
// Auch ben�tigt f�r den K�nig
local pAimer;			// Aktive Zielsteuerung; wird abgrbrochen, wenn der Zauberer gest�rt wird (Nur Fantasypack)
local pAimedSpell;		// Zauber, der gezielt wird  (Nur Fantasypack)
local pAimedSpellOrigin;        // Objekt, das einen Zielzauber initiiert hat. An dieses werden SpellFailed/SpellSucceeded-Nachrichten weitergeleitet


/* Initialisierung */

protected func Initialize()
{
  // Clonks mit Magiephysikal aus fehlerhaften Szenarien korrigieren
  if (GetID () == CLNK)
    if (GetPhysical ("Magic", 1))
      SetPhysical ("Magic", 0, 1);
  SetAction("Walk");
  SetDir(Random(2));
  // Broadcast f�r Spielregeln
  GameCallEx("OnClonkCreation", this());
  return(1);
}

/* Bei Hinzuf�gen zu der Crew eines Spielers */

protected func Recruitment(int iPlr) {
  // Alchemieregel: Jeder Clonk kriegt einen angelegten Beutel spendiert
  if(ObjectCount(ALCO)) CreateObject(ALC_,0,0,-1)->~BelongTo(this());
  // Broadcast f�r Crew
  GameCallEx("OnClonkRecruitment", this(), iPlr);
}

/* Kontext */

public func HasConstructMenu() { return HasKnowledge() && GetPhysical("CanConstruct"); }
public func HasKnowledge() { return(GetPlrKnowledge(GetOwner(),0,0,C4D_Structure())); }
public func HasBase()      { return(FindBase(GetOwner()) && GetBase(Contained()) != GetOwner()); }
public func ReleaseAllowed() { return(ObjectCount(REAC)); }
public func AtConstructionSite() { return(!Contained() && FindConstructionSite() && ObjectCount(CNMT)); }
public func AtEnergySite() { return(!Contained() && FindEnergySite()); }
public func AtTreeToChop() { return(!Contained() && FindTree() && GetPhysical("CanChop")); }

public func FindConstructionSite()
{
  return FindObject2(Find_AtRect(-1,-16,2,32), Find_OCF(OCF_Construct), Find_Layer(GetObjectLayer()));
}

public func FindEnergySite()
{
  return FindObject2(Find_AtPoint(), Find_OCF(OCF_PowerConsumer), Find_NoContainer(), Find_Layer(GetObjectLayer()), Find_Func("NeedsEnergy"));
}

public func FindTree()
{
  return FindObject2(Find_AtPoint(), Find_OCF(OCF_Chop), Find_Layer(GetObjectLayer()));
}

/* Steuerung */

protected func ControlLeft()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlLeft")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeft(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlLeftDouble()
  {
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlLeftDouble")) return(true);
  }

protected func ControlRightDouble()
  {
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRightDouble")) return(true);
  }

protected func ControlLeftReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlLeftReleased(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlRight()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlRight")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlRight(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlRightReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlRightReleased(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlUp()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlUp")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlUp(this()));
  // Bei JnR Delfinsprung
  if(GetPlrJumpAndRunControl(GetController()))
    DolphinJump();
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlUpReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlUpReleased(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlUpDouble() 
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlUpDouble")) return(1);
  DolphinJump();
}

private func DolphinJump()
{
  // nur wenn an Meeresoberfl�che
  if(!InLiquid()) return(0);
  if(GBackSemiSolid(0,-1)) return(0);
  // Nicht wenn deaktiviert (z.B. Ohnmacht)
  if (GetActMapVal("ObjectDisabled", GetAction(), GetID())) return false;
  // herausspringen
  SetPosition(GetX(),GetY()-1);
  SetAction("Jump");
  SetSpeed(GetXDir(),-BoundBy(GetPhysical("Swim")/2500,24,38));
  var iX=GetX(),iY=GetY(),iXDir=GetXDir(),iYDir=GetYDir();
  // Wenn Sprung im Wasser endet und das Wasser tief genug ist, Kopfsprung machen
  if(SimFlight(iX,iY,iXDir,iYDir,25,50))
    if(GBackLiquid(iX-GetX(),iY-GetY()) && GBackLiquid(iX-GetX(),iY+9-GetY()))
      SetAction("Dive");
}

protected func ControlDown()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDown")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDown(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDownReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDownReleased(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDownSingle()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownSingle")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownSingle(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDownDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDownDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDownDouble(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDig()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDig")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDig(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDigReleased()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding()) return(GetActionTarget()->~ControlDigReleased(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDigSingle()
{
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigSingle(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlDigDouble()
{
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlDigDouble")) return(1);
  // Steuerung an Pferd weiterleiten
  if (IsRiding())  return(GetActionTarget()->~ControlDigDouble(this()));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlThrow()
{
  // Bei vorherigem Doppel-Stop nur Ablegen
  if (GetPlrDownDouble(GetOwner())) return(0);
  // Steuerung an Effekt weitergeben 
  if (Control2Effect("ControlThrow")) return(1);
  // Reiten und Werfen
  if (IsRiding())
    if (Contents(0))
    {
      SetAction("RideThrow");
      return(1);
    }
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  // Steuerung an Pferd weiterleiten
  if(IsRiding()) return(GetActionTarget()->~ControlUpdate(self, comdir, dig, throw));
  // Keine �berladene Steuerung
  return(0);
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data)
{
  // Kommando MoveTo an Pferd weiterleiten
  if (szCommand eq "MoveTo")
    if (IsRiding())
      return(GetActionTarget()->~ControlCommand(szCommand, pTarget, iTx, iTy));
  // Anderes Kommando beim Reiten: absteigen (Ausnahme: Context)
  if (IsRiding() && szCommand ne "Context")
  {
    SetComDir(COMD_Stop(),GetActionTarget());
    GetActionTarget()->~ControlDownDouble(this());
  }
  // RejectConstruction Callback beim Bauen durch Drag'n'Drop aus einem Gebaeude-Menu
  if(szCommand S= "Construct")
  {
    // Data ist eigentlich keine ID, sondern ein C4Value* - Damit ein DirectCall
    // m�glich ist, muss sie aber zu einer C4ID gecastet werden.
    if(CastC4ID(Data)->~RejectConstruction(iTx - GetX(), iTy - GetY(), this()) )
    {
      return(1);
    }
  }
  // Kein �berladenes Kommando
  return(0);
}

/* Verwandlung */

private func RedefinePhysical(szPhys, idTo)
{
  // Physical-Werte ermitteln
  var physDefFrom = GetPhysical(szPhys, 0, 0, GetID()),
      physDefTo   = GetPhysical(szPhys, 0, 0, idTo),
      physCurr    = GetPhysical(szPhys);
  // Neuen Wert berechnen
  var physNew; if (physDefTo) physNew=BoundBy(physDefTo-physDefFrom+physCurr, 0, 100000);
  // Neuen Wert f�r den Reset immer tempor�r setzen, selbst wenn keine �nderung besteht, damit der Reset richtig funktioniert
  SetPhysical(szPhys, physNew, PHYS_StackTemporary);
  // Fertig
  return(1);
}

protected func FxIntRedefineStart(object trg, int num, int tmp, id idTo)
  {
  // Ziel-ID in Effektvariable
  if (tmp)
    idTo = EffectVar(0, trg, num);
  else
    {
    EffectVar(0, trg, num) = idTo;
    EffectVar(1, trg, num) = GetID();
    }
  // Physicals anpassen
  RedefinePhysical("Energy", idTo);
  RedefinePhysical("Breath", idTo);
  RedefinePhysical("Walk", idTo);
  RedefinePhysical("Jump", idTo);
  RedefinePhysical("Scale", idTo);
  RedefinePhysical("Hangle", idTo);
  RedefinePhysical("Dig", idTo);
  RedefinePhysical("Swim", idTo);
  RedefinePhysical("Throw", idTo);
  RedefinePhysical("Push", idTo);
  RedefinePhysical("Fight", idTo);
  RedefinePhysical("Magic", idTo);
  RedefinePhysical("Float", idTo);
  /*if (GetRank()<4) RedefinePhysical("CanScale", idTo);
  if (GetRank()<6) RedefinePhysical("CanHangle", idTo);*/ // z.Z. k�nnen es alle
  RedefinePhysical("CanDig", idTo);
  RedefinePhysical("CanConstruct", idTo);
  RedefinePhysical("CanChop", idTo);
  RedefinePhysical("CanSwimDig", idTo);
  RedefinePhysical("CorrosionResist", idTo);
  RedefinePhysical("BreatheWater", idTo);
  // Damit Aufwertungen zu nicht-Magiern keine Zauberenergie �brig lassen
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // Echtes Redefine nur bei echten Aufrufen (hat zu viele Nebenwirkungen)
  if (tmp) return(FX_OK);
  Redefine(idTo);
  // Fertig
  return(FX_OK);
  }
  
protected func FxIntRedefineStop(object trg, int num, int iReason, bool tmp)
  {
  // Physicals wiederherstellen
  ResetPhysical(0, "BreatheWater");
  ResetPhysical(0, "CorrosionResist");
  ResetPhysical(0, "CanSwimDig");
  ResetPhysical(0, "CanChop");
  ResetPhysical(0, "CanConstruct");
  ResetPhysical(0, "CanDig");
  ResetPhysical(0, "Float");
  ResetPhysical(0, "Magic");
  ResetPhysical(0, "Fight");
  ResetPhysical(0, "Push");
  ResetPhysical(0, "Throw");
  ResetPhysical(0, "Swim");
  ResetPhysical(0, "Dig");
  ResetPhysical(0, "Hangle");
  ResetPhysical(0, "Scale");
  ResetPhysical(0, "Jump");
  ResetPhysical(0, "Walk");
  ResetPhysical(0, "Breath");
  ResetPhysical(0, "Energy");
  // Keine R�ck�nderung bei tempor�ren Aufrufen oder beim Tod/L�schen
  if (tmp || iReason) return();
  // Damit Aufwertungen von nicht-Magiern keine Zauberenergie �brig lassen
  if (GetPhysical("Magic")/1000 < GetMagicEnergy()) DoMagicEnergy(GetPhysical("Magic")/1000-GetMagicEnergy());
  // OK; alte Definition wiederherstellen
  Redefine(EffectVar(1, trg, num));
  }

public func Redefine2(idTo)
{
  if (GetID() == idTo) return(true);
  RemoveEffect("IntRedefine", this());
  if (GetID() == idTo) return(true);
  return(!!AddEffect("IntRedefine", this(), 10, 0, this(), 0, idTo));
}

public func Redefine(idTo)
{
  // Aktivit�tsdaten sichern
  var phs=GetPhase(),act=GetAction();
  // Umwandeln
  ChangeDef(idTo);
  // Aktivit�t wiederherstellen
  var chg=SetAction(act);
  if (!chg) SetAction("Walk");
  if (chg) SetPhase(phs);
  // Fertig
  return(1);
}

/* Essen */  

public func Feed(iLevel)
{
  DoEnergy(iLevel);
  Sound("ClonkMunch");
    return(1);
}

/* Aktionen */

private func Riding()
{
  // Richtung an die des Pferdes anpassen
  SetDir(GetDir(GetActionTarget()));
  // Pferd steht still: Clonk soll auch still sitzen
  if (GetActionTarget()->~IsStill())
  {
    if (GetAction() ne "RideStill")
      SetAction("RideStill");
  }
  // Pferd steht nicht still: Clonk soll auch nicht still sitzen
  else
    if (GetAction() ne "Ride")
      SetAction("Ride");
  return(1);
}

private func Throwing()
{
  // Erstes Inhaltsobjekt werfen
  var pObj = Contents(0);
  // Wurfparameter berechnen
  var iX, iY, iR, iXDir, iYDir, iRDir;
  iX = 0; if (!GetDir()) iX = -iX;
  iY = -10;
  iR = Random(360);
  iXDir = GetPhysical("Throw") / 25000; if(!GetDir()) iXDir = -iXDir;
  iYDir = -GetPhysical("Throw") / 25000;
  iRDir = Random(40) - 20;
  // Reitet? Eigengeschwindigkeit addieren
  if (GetActionTarget())
  {
    iXDir += GetXDir(GetActionTarget()) / 10;
    iYDir += GetYDir(GetActionTarget()) / 10;
  }
  // Werfen!
  Exit(pObj, iX, iY, iR, iXDir, iYDir, iRDir);  
  // Fertig
  return(1);  
}

private func Fighting()
{
  if (!Random(2)) SetAction("Punch");
  return(1);
}

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  if (!Random(5)) Sound("Punch*");
  if (!Random(2)) return(1);
  Punch(GetActionTarget());
  return(1);
}
  
private func Chopping()
{
  if (!GetActTime()) return(); // Erster Schlag kein Sound. Clonk holt noch aus.
  Sound("Chop*");
  CastParticles("Dust",Random(3)+1,6,-8+16*GetDir(),1,10,12);
  return(1);
}
  
private func Building()
{
  if (!Random(2)) Sound("Build*");
  return(1);
}

private func Processing()
{
  Sound("Build1");
  return(1);
}

private func Digging()
{
  Sound("Dig*");
  return(1);
}

protected func Scaling()
{
  var szDesiredAction;
  if (GetYDir()>0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() ne szDesiredAction) SetAction(szDesiredAction);
  return(1);   
}

/* Ereignisse */

protected func QueryCatchBlow(object projectile)
{
  // Objekt will den Clonk nicht treffen?
  if (projectile && projectile->~QueryClonkHit(this))
    return true;;
  // Treffer OK
}
  
protected func CatchBlow()
{
  if (GetAction() eq "Dead") return(0);
  if (!Random(5)) Hurt();
  return(1);
}
  
protected func Hurt()
{
  Sound("Hurt*");
  return(1);
}
  
protected func Grab(object pTarget, bool fGrab)
{
  Sound("Grab");
  return(1);
}

protected func Get()
{
  Sound("Grab");
  return(1);
}

protected func Put()
{
  Sound("Grab");
  return(1);
}

protected func Death(int iKilledBy)
{
  // Info-Broadcasts f�r sterbende Clonks
  GameCallEx("OnClonkDeath", this(), iKilledBy);
  
  // Der Broadcast k�nnte seltsame Dinge gemacht haben: Clonk ist noch tot?
  if (GetAlive()) return();
  
  // den Beutel fallenlassen
  if(GetAlchemBag()) GetAlchemBag()->~Loose();

  Sound("Die");
  DeathAnnounce();
  // Letztes Mannschaftsmitglied tot: Script benachrichtigen
  if (!GetCrew(GetOwner()))
    GameCallEx("RelaunchPlayer",GetOwner(),iKilledBy);
  return(1);
}

protected func Destruction()
{
  // Clonk war noch nicht tot: Jetzt ist er es
  if (GetAlive())
    GameCallEx("OnClonkDeath", this(), GetKiller());
  // Dies ist das letztes Mannschaftsmitglied: Script benachrichtigen
  if (GetCrew(GetOwner()) == this())
    if (GetCrewCount(GetOwner()) == 1)
      //Nur wenn der Spieler noch lebt und nicht gerade eleminiert wird
      if (GetPlayerName(GetOwner()))
        {
        GameCallEx("RelaunchPlayer",GetOwner(),GetKiller());
        }
  return(1);
}

protected func DeepBreath()
{
  Sound("Breath");
  return(1); 
}
  
protected func CheckStuck()
{                   
  // Verhindert Festh�ngen am Mittelvertex
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(GBackSolid(0, 3))
      SetPosition(GetX(), GetY() + 1);
}

/* Status */

public func IsRiding()
{
  // Reitet der Clonk?
  return (WildcardMatch(GetAction(), "Ride*"));
}

public func IsClonk() { return(1); }

/* Kontext */

public func ContextRelease(pCaller) 
{
  [$CtxRelease$|Image=CXRL|Condition=ReleaseAllowed]
  FindObject(REAC)->Activate(GetOwner());
  return(1);
}

public func ContextEnergy(pCaller)
{
  [$TxtEnergysupply$|Image=CXEC|Condition=AtEnergySite]
  var pSite; 
  if (pSite = FindEnergySite())
    SetCommand(this(), "Energy", pSite);
  return(1);
}

public func ContextConstructionSite(pCaller)
{
  [$CtxConstructionMaterial$|Image=CXCM|Condition=AtConstructionSite]
  var pSite; 
  if (pSite = FindConstructionSite())
    PlayerMessage(GetOwner(), pSite->GetNeededMatStr(), pSite);
  return(1);
}

public func ContextChop(pCaller)
{
  [$CtxChop$|Image=CXCP|Condition=AtTreeToChop]
  var pTree; 
  if (pTree = FindTree())
    SetCommand(this(), "Chop", pTree);
  return(1);
}

public func ContextConstruction(pCaller)
{
  [$CtxConstructionDesc$|Image=CXCN|Condition=HasConstructMenu]
  SetCommand(this(), "Construct");
  ExecuteCommand();
  return(1);
}

public func ContextHome(pCaller)
{
  [$CtxHomeDesc$|Image=CXHM|Condition=HasBase]
  SetCommand(this(), "Home");
  return(1);
}

/* Hilfsfunktion */

public func ContainedCall(string strFunction, object pTarget)
{
  // Erst das betreffende Geb�ude betreten, dann die Zielfunktion aufrufen 
  SetCommand(this(), "Call", pTarget, this(), 0, 0, strFunction);
  AddCommand(this(), "Enter", pTarget);
}

/* Steuerung */

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
  // In einem Geb�ude oder Fahrzeug: das Kontextmen� des Geb�udes �ffnen
  if (Contained())
    if ((Contained()->GetCategory() & C4D_Structure) || (Contained()->GetCategory() & C4D_Vehicle))
      return(SetCommand(this(),"Context",0,0,0,Contained()), ExecuteCommand());
  // Fasst ein Objekt an: Kontextmen� des angefassten Objekts �ffnen
  if (GetAction() eq "Push")
    return(SetCommand(this(),"Context",0,0,0,GetActionTarget()), ExecuteCommand());
  // Tr�gt ein Objekt: Kontextmen� des ersten getragenen Objekts �ffnen
  if (Contents(0))
    return(SetCommand(this(),"Context",0,0,0,Contents(0)), ExecuteCommand());
  // Ansonsten das Kontextmen� des Clonks �ffnen
  return(SetCommand(this(),"Context",0,0,0,this()), ExecuteCommand());
}

protected func ControlSpecialDouble()
{
  return this->~ControlSpecial(...);
}

/* Callback beim Auswahl aus dem Construct-Kontextmenu */

public func ControlCommandConstruction(target, x, y, target2, def)
{
  // Keine Konstruktion erlaubt?
  if(def->~RejectConstruction(x - GetX(), y - GetY(), this()) )
    // Construct-Kommando beenden
    return(FinishCommand(this(), false, 0) );
}

/* Automatische Produktion */

public func ControlCommandAcquire(target, x, y, target2, def)
{
  //Falls das Teil rumliegt nur aufsammeln
  var obj = GetAvailableObject (def, target2);
  if (obj) return ();
  //Geb�ude suchen worin man's herstellen kann  
  if (obj = GetProducerOf (def)) {
    AddCommand (this (), "Call", this (), 0, 0, 0, 0, "AutoProduction", 0, 1);
    obj -> HowToProduce (this(), def);
    return (1);
  }
}

public func AutoProduction() { return (1); }

public func AutoProductionFailed() 
{
  var def = GetCommand (this (), 5, 1);
  if (!FindContents(def)) {
    var obj = GetAvailableObject (def, GetCommand (this (), 4, 1));
    if (obj) return (AddCommand (this(), "Get", obj,0,0,0,40));
    AddCommand (this(), "Buy", 0, 0, 0, 0, 100, GetCommand(this(), 5, 1), 0, 3);
  }
  return (1);
}

public func GetProducerOf(def)
{
  // Nur Objekte mit typischen OCFs befragen damit's schneller geht.
  var obj, prod, ocf = OCF_Container () | OCF_Exclusive () | OCF_Entrance () | OCF_LineConstruct () | OCF_PowerConsumer ();
  while (obj = FindObject (0, -500, -250, 1000, 500, ocf,0,0,0, obj))
    if (obj -> ~IsProducerOf (this (), def))
      if (!prod || ObjectDistance (prod) > ObjectDistance (obj))
        prod = obj;
  return (prod);
}

/* Trinken */

public func Drink(object pDrink)
{
  // Trinkaktion setzen, wenn vorhanden
  if (GetActMapVal("Name", "Drink"))
    SetAction("Drink");
  // Vorsicht: erstmal nichts mit pDrink machen,
  // die Potions l�schen sich meist selber...
}

/* Einsammeln */

public func RejectCollect(id idObject, object pObject)
{
  // Objekt kann gepackt werden
  // automatisches Packen aber nur wenn die Paktteile nicht extra gez�hlt werden
  if(!IsSpecialItem(pObject)) if(pObject->~JoinPack(this())) return(1);
    
  // Objektaufnahme mit Limit verhindern, wenn bereits genug getragen
  if(pObject->~CarryLimit() && ContentsCount(idObject) >= pObject->~CarryLimit() ) return(1);
    
  // Spezialitem?
  var i, iCount;
  if(i = IsSpecialItem(pObject))
  {
    // Noch genug Platz f�r das ganze Packet?
    if(GetSpecialCount(GetMaxSpecialCount(i-1))+Max(pObject->~PackCount(),1)<=GetMaxSpecialCount(i-1, 1)) return(0);
    iCount = GetMaxSpecialCount(i-1, 1)-GetSpecialCount(GetMaxSpecialCount(i-1));
    // Ansonten so viel wie geht rein
    if(pObject->~SplitPack(pObject->~PackCount()-iCount)) return(0);
    else return(1);
  }
  
  return(GetNonSpecialCount()>=MaxContentsCount());
}

/* Itemlimit */
public func MaxContentsCount() { return(1); }

public func GetMaxSpecialCount(iIndex, fAmount)
{
  // Hier k�nnten Spezialbehandlungen von Itemgruppen definiert werden
  // wie z.B. zu dem Inventar noch 30 Pfeile aufnehmen (siehe auch Ritter)
  //  if(iIndex == 0) { if(fAmount) return(30); return("IsArrow"); }
}

/* Liefert die Gesamtzahl eines Objekt(paket)typs */ 
private func GetObjectCount(idObj) 
  { 
  var idUnpackedObj; 
  if (idUnpackedObj = idObj->~UnpackTo()) 
    // Auch verschachtelte Pakete mitz�hlen 
    return(GetObjectCount(idUnpackedObj) * idObj->PackCount()); 
  // Ansonsten ist es nur ein Objekt 
  return(1); 
  }

/* Spezialgegenst�nde im Inventar z�hlen */ 
private func GetSpecialCount(szTest) 
  { 
  var iCnt, pObj; 
  // Einzelne Pfeile... 
  for(var i = 0; pObj = Contents(i); i++) 
    if(ObjectCall(pObj, szTest)) 
      iCnt++; 
  // Pakete... 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~UnpackTo())
      if(DefinitionCall(pObj->~UnpackTo(), szTest))
        iCnt += GetObjectCount(pObj); 
  // Wert zur�ckgeben 
  return(iCnt); 
  }
  
/* Testen eines Objektes */
private func IsSpecialItem(pObj)
{
  // Spezialitem?
  var j=-1;
  while(GetMaxSpecialCount(++j, 1))
    if(ObjectCall(pObj, GetMaxSpecialCount(j)))
      return(j+1);
  // Spezialitempacket?
  if(pObj->~UnpackTo())
  {
    j=-1;
    while(GetMaxSpecialCount(++j, 1))
      if(DefinitionCall(pObj->~UnpackTo(), GetMaxSpecialCount(j)))
        return(j+1);
  }
}

/* Anzahl an normalen Objekten */
private func GetNonSpecialCount() 
  { 
  var iCnt, pObj; 
  // Inventar einzeln auf nicht-Spezial �berpr�fen 
  for(var i = 0; pObj = Contents(i); i++)
    // Spezialitems nicht z�hlen
    if(!IsSpecialItem(pObj))
        iCnt++;
   
  // Wert zur�ckgeben 
  return(iCnt); 
  } 

/* Reiten */

public func ContextDescend(pCaller) 
{
  [$TxtDescend$|Image=DSCN|Condition=IsRiding]
  DescendVehicle();
}

public func DescendVehicle()
{
  var pOldVehicle = GetActionTarget();
  SetAction("Walk");
  // Feststecken nach Absteigen? Dann besser direkt beim Gef�hrt absteigen.
  if (Stuck()) if (pOldVehicle)
  {
    var x=GetX(), y=GetY();
    SetPosition(GetX(pOldVehicle), GetY(pOldVehicle));
    if (Stuck())
    {
      // Das Gef�hrt steckt auch? Dann hilft es alles nichts. Zur�ck zum Ursprungsort.
      SetPosition(x,y);
    }
  }  
}


/* Effektsteuerung */

private func Control2Effect(string szControl)
  {
  // Von Effektzahl abw�rts z�hlen
  var i = GetEffectCount(0, this()), iEffect;
  var res;
  while (i--)
    {
    // Effekte mit Control im Namen benachrichtigen	  
    iEffect = GetEffect("*Control*", this(), i);
    //  Message("%s", this(), GetEffect(0, this(), iEffect, 1));
    if ( GetEffect(0, this(), iEffect, 1) )
      res += EffectCall(this(), iEffect, szControl);
    }
  return(res);
  }


/* Pfeile */

// Pfeilpaket aufteilen 
public func SplitPack2Components(pPack) 
  { 
  // Aufteilen 
  if(!pPack->~Unpack(this()) ) Split2Components(pPack); 
  // Fertig, Erfolg 
  return(1); 
  }
 
/* Pfeil aus dem Inventar nehmen */ 
public func GetArrow() 
  { 
  // Einzelne Pfeile suchen 
  var pObj, pArrow; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Bei Bedarf Pakete aufteilen 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrowPack())
    {
      // Pfeil aus Paket verwenden
      if(pArrow = pObj->~GetItem()) return(pArrow);
      // oder bei alten Pfeilen Paket aufteilen
      if (SplitPack2Components(pObj))
        return(FindSingleArrow());
    }
  // Keine Pfeile gefunden 
  return(0); 
  } 

public func FindSingleArrow() 
  { 
  // Einzelne Pfeile suchen 
  var pObj; 
  for(var i = 0; pObj = Contents(i); i++) 
    if(pObj->~IsArrow()) 
      return(pObj); 
  // Keiner gefunden 
  return(0); 
  } 
  
public func GetComboArrow() 
  { 
  // Pfeile als Komboobjekt: Nur wenn das erste Inventarobjekt ein Pfeil ist
  var pObj = Contents(0), pArrow;
  if (!pObj) return();
  if(pObj->~IsArrow()) return(pObj); 
  // Bei Bedarf Pakete aufteilen 
  if(pObj->~IsArrowPack())
    {
    // Pfeil aus Paket verwenden
    if(pArrow = pObj->~GetItem()) return(pArrow);
    // oder bei alten Pfeilen Paket aufteilen
    if (SplitPack2Components(pObj))
      return(FindSingleArrow());
    }
  // Keine Pfeile gefunden 
  return(0);
  } 

/* Pfeile im Inventar z�hlen */ 
private func GetArrowCount() 
  {
   return(GetSpecialCount("IsArrow"));
  }


/* Zauberei - ben�tigt, wenn der Clonk Zielzauber z.B. aus dem Zauberturm zaubert */

public func SpellFailed(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  var pSpellOrigin = pAimedSpellOrigin;
  pAimedSpellOrigin = 0;
  if (pSpellOrigin && pSpellOrigin != this())
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall die Zutaten/Zauberenergie f�r den
    // Zauber bereit gestellt, und diese sollten nicht an den Clonk zur�ck gegeben werden
    return (pSpellOrigin->~SpellFailed(idSpell, this()));
  // Magieenergie zur�ckgeben
  DoMagicEnergy(Value(idSpell), 0, true);
  // Alchemische Zutaten zur�ckgeben
  if(ObjectCount(ALCO)) IncreaseAlchem(idSpell);
}

public func SpellSucceeded(id idSpell, object pByCaller)
{
  // Umleiten an eigentliche Zauberquelle? (Buch, Zauberturm, etc.)
  // LocalN als Workaround um ChangeDefs im Zauber
  var pSpellOrigin = LocalN("pAimedSpellOrigin");
  LocalN("pAimedSpellOrigin") = 0;
  if (pSpellOrigin && pSpellOrigin != this())
    // Auch bei nicht erfolgreicher Umleitung abbrechen: Das zaubernde Objekt hat im Normalfall das Magietraining schon erledigt
    return (pSpellOrigin->~SpellSucceeded(idSpell, this()));
  // Globaler Aufruf f�r Zauber
  OnClonkSucceededSpell(idSpell);
}

// Der Clonk kann von sich aus nicht zaubern und hat keine Aktivit�ten daf�r
private func SetMagicAction(id idForSpell) {}
private func SetCastAction() {}
private func EndMagicAction() {}


/* Zielsteuerung - nur aktiv, wenn das Fantasypack die globalen Funktionen CreateAimer und CreateSelector �berladen hat */

public func DoSpellAim(object pSpell, object pSpellOrigin)
  {
  pAimedSpell = pSpell;
  pAimedSpellOrigin = pSpellOrigin;
  pAimer = CreateAimer(this(), this(), GetDir()*180-90);

  if (!pAimer) return(0);
  
  // Callback an die Zauberquelle, dass noch gezielt wird
  if (pSpellOrigin) pSpellOrigin->~SpellAiming(pSpell, this());

  // Zielvorgang f�r Zauber
  SetComDir(COMD_Stop());
  SetCastAction();
  return(pAimer);
  }

public func OnAimerEnter(int iAngle)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  var idSpell = GetID(pAimedSpell);
  // Aktivit�t
  SetMagicAction();
  // Zauber benachrichtigen
  if (!pAimedSpell->~ActivateAngle(this(), iAngle))
    return(0, SpellFailed(idSpell));
  // OK; Zauber erfolgreich
  return(SpellSucceeded(idSpell));
  }

public func AimingAngle(int iAngle)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort(iAngle));
  // Zielaktion setzen, wenn im Laufen. Das ist etwas ungeschickt, weil damit
  // die Magic-Aktion abgebrochen wird, die der Magier bereits bis zur Haelfte
  // durchgefuehrt hat. Dummerweise laesst sich frueher nicht feststellen, ob
  // der auszufuehrende Zauber einen Aimer brauchen wird...
  if(GetActMapVal("Name", "AimMagic") )
    if(GetAction() S= "Walk" || GetAction() S= "Magic")
      SetAction("AimMagic");

  // Phase anpassen
  if(GetAction() S= "AimMagic")
    {
    // Auch richtigen Winkel verwenden wenn nach links gedreht
    var iHalfAngle = iAngle;
    if(iHalfAngle < 0) iHalfAngle = -iHalfAngle;
    SetPhase(BoundBy((iHalfAngle + 9) / 18, 0, 9) );
    }
  // Weitergabe an den Zauber
  return(pAimedSpell->~AimingAngle(this(), iAngle));
  }

public func OnAimerAbort(int iAngle)
  {
  // Aktivit�t zur�cksetzen
  EndMagicAction();
  // Benachrichtigung
  if (!pAimedSpell) return(1);
  var idSpell = GetID(pAimedSpell);
  if (!pAimedSpell->~AbortAiming(this()))
    // Standardaktion: Zauber l�schen
    RemoveObject(pAimedSpell);
  pAimedSpell = 0;
  // OK; Zauber nicht erfolgreich
  return(SpellFailed(idSpell));
  }

public func DoSpellSelect(object pSpell, int iRadius, object pSpellOrigin)
  {
  // Zauber sichern
  pAimedSpell = pSpell;
  pAimedSpellOrigin = pSpellOrigin;
  pAimer = CreateSelector(pSpell, this(), iRadius);
  if (!pAimer) return();
  // Callback an die Zauberquelle, dass noch gezielt wird
  if (pSpellOrigin) pSpellOrigin->~SpellAiming(pSpell, this());
  // Zielvorgang f�r Zauber
  SetComDir(COMD_Stop);
  SetCastAction();
  return(pAimer);
  }

public func OnSelectorEnter(object pTarget)
  {
  // Zauber weg?
  if (!pAimedSpell) return(OnAimerAbort());
  var idSpell = GetID(pAimedSpell);
  // Aktivit�t
  SetMagicAction();
  // Zauber benachrichtigen
  if (!pAimedSpell->~ActivateTarget(this(), pTarget))
    return(0, SpellFailed(idSpell));
  // OK; Zauber erfolgreich
  return(SpellSucceeded(idSpell));
  }

public func OnSelectorAbort()
  {
  // Aktivit�t zur�cksetzen
  EndMagicAction();
  // Benachrichtigung
  if (!pAimedSpell) return(1);
  var idSpell = GetID(pAimedSpell);
  if (!pAimedSpell->~AbortSelecting(this()))
    // Standardaktion: Zauber l�schen
    RemoveObject(pAimedSpell);
  pAimedSpell = 0;
  // OK; Zauber nicht erfolgreich
  return(SpellFailed(idSpell));
  }

public func SelectorTarget(object pTarget) { if(pAimedSpell) return(pAimedSpell->~SelectorTarget(this(),pTarget)); }

// Momentanen Zauber abbrechen
protected func AbortCasting()
  {
  if (pAimer) pAimer->Abort();
  return(1);
  }
  
public func Abort() {} // dummy call to instantiate function name
