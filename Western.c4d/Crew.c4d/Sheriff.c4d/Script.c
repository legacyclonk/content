/*-- Sheriff --*/

#strict
#include COWB

local fGrabbedInfo;

// Der Sheriff benutzt immer die InfoSektion eines Cowboys
protected func Recruitment()
{
  if(!fGrabbedInfo) // Ein Hack für Peter
    {
    // Cowboy erstellen
    var cb = CreateObject(COWB, 0, 10, GetOwner());
    MakeCrewMember(cb, GetOwner());
    // InfoSektion klauen
    fGrabbedInfo = true;
    GrabObjectInfo(cb);
    // Den anderen Clonk wieder löschen
    RemoveObject(cb);
    // Volles Leben
    DoEnergy(100);
    // Neues Portrait
    AdjustPortrait();
    // Workaround für HEAD-Bug
    SetPlrViewRange(500);
    }
  // Effekt anhängen
  AddEffect("CheckImprison", this(), 1, 10, this());
  // Weiter im Text
  return(_inherited());
}

// Symboleffekt
func FxCheckImprisonTimer(pTarget, iNumber)
{
  // Die Symbole werden immer wieder erzeugt und erschaffen, und blinken somit
  var i, obj;
  // Wenn Symbole da sind, alle entfernen
  if(EffectVar(0, pTarget, iNumber))
  {
    while(EffectVar(i, pTarget, iNumber)) RemoveObject(EffectVar(i++, pTarget, iNumber));
    return();	  
  }
  // Nur wenn der Sheriff Cursor ist
  if(GetCursor(GetOwner())!=pTarget) return();
  // Ist ein Büro da?
  if(FindOffice(2000))
  {
    // Suche alle passenden Banditen
    while(obj = FindBandit(400, obj))
    {
      if(obj)
      {
        EffectVar(i, pTarget, iNumber) = CreateObject(SIMP, 0, 0, GetOwner());
        EffectVar(i, pTarget, iNumber)->SetVisibility(VIS_Owner());
        ObjectSetAction(EffectVar(i, pTarget, iNumber), "Imprison",obj);
	      i++;
      }
    }
  }
}

func FxCheckImprisonStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  // Symbole entfernen
  var i;
  while(EffectVar(i, pTarget, iNumber)) RemoveObject(EffectVar(i++, pTarget, iNumber));
}

// Anzahl verfügbarer Portraits
private func GetPortraitCount() { return(1); }

/* Banditen einsperren */

public func ContextImprison()
{
  [$CtxImprison$|Image=HDCF|Condition=ImprisonCond]
  var bandit, office;
  // Banditen / Büro suchen
  bandit = FindBandit(50);
  office = FindOffice(2000);
  // Eines ist nicht da?
  if(!bandit) return(PlayerMessage(GetOwner(), "$MsgNoBandit$", this()));
  if(!office) return(PlayerMessage(GetOwner(), "$MsgNoOffice$", this()));
  if(!FindContents(HDCF)) return(PlayerMessage(GetOwner(), "$MsgNoHandcuffs$", this()));
  RemoveObject(FindContents(HDCF));
  // Einsperren, den Wicht
//  office->Imprison(bandit, this());
  AddEffect("Imprison", bandit, 300, 20, 0, GetID(), office, this());
  // Cooler Spruch
  Message("$MsgImprisoned$", this());
  return(1);
}

public func ImprisonCond()
{
  // Überpüft, ob man in der Lage ist, Banditen einzusperren
  return(FindBandit(50) && FindOffice(2000));
}

public func FindBandit(int iRadius, bandit)
{
  // Sucht einen Banditen in der Nähe, den man einsperren könnte
  while(bandit = FindObject(0, -iRadius/2, -iRadius/2, iRadius, iRadius, OCF_Alive(), 0,0, NoContainer(), bandit))
   // Ist ein Bandit
   if(bandit->~IsBandit())
    // Spieler sind verfeindet
    if(Hostile(GetOwner(), GetOwner(bandit)))
      // Clonk ist geschwächt
      if(GetEnergy(bandit)<=GetPhysical("Energy", 0, bandit)/2000)
        // juchei
        return(bandit);
}

public func FindOffice(int iRadius)
{
  // Sucht ein Büro in der Nähe, in das man Banditen sperren kann
  var office;
  while(office = FindObject(OfficeID(), -iRadius/2, -iRadius/2, iRadius, iRadius, OCF_Fullcon(), 0,0,0, office))
    // Büro ist verbündet
    if(!Hostile(GetOwner(), GetOwner(office)))
      // juchei
      return(office);
}

private func OfficeID() { return(SHOF); }

// Ist ein Sheriff
public func IsSheriff() { return(1); }

// Dieser Effekt kümmert sich um den Banditen auf dem Weg zum Gefängnis
func FxImprisonStart(pTarget, iNumber, fTmp, pOffice, pSheriff)
{
  if(fTmp) return();
  // Sachen speichern
  var pBandit;
  EffectVar(0, pTarget, iNumber) = pOffice;
  EffectVar(1, pTarget, iNumber) = pBandit = CreateObject(BNDT, GetX(pTarget), GetY(pTarget)+10, GetOwner(pSheriff));
  EffectVar(2, pTarget, iNumber) = pSheriff;

  // Löschen
  Extinguish(pTarget);

  // Handlungsunfähig machen
  LocalN("fIsCaught", pBandit) = 1;

  // Zur Crew hinzufügen
  MakeCrewMember(pBandit, GetOwner(pSheriff));

  // Pysicals kopieren
  SetPhysical("Energy", GetPhysical("Energy", 0,pTarget), 2, pBandit);
  SetPhysical("Breath", GetPhysical("Breath", 0,pTarget), 2, pBandit);
  SetPhysical("Walk", GetPhysical("Walk", 0,pTarget), 2, pBandit);
  SetPhysical("Swim", GetPhysical("Swim", 0,pTarget), 2, pBandit);
  SetPhysical("Scale", GetPhysical("Scale", 0,pTarget), 2, pBandit);
  SetPhysical("Hangle", GetPhysical("Hangle", 0,pTarget), 2, pBandit);

  // Energie setzen
  DoEnergy(GetEnergy(pTarget)-GetEnergy(pBandit), pBandit);

  // Farbe Aktion und Phase setzen
  SetColorDw(GetColorDw(pTarget), pBandit);
  ObjectSetAction(pBandit, GetAction(pTarget));
  SetPhase(GetPhase(pTarget), pBandit);
  SetClrModulation(RGBa(255,255,255,255), pTarget);
  if(!ActIdle(pBandit)) ObjectSetAction(pBandit, "Walk");
  SetDir(GetDir(pTarget), pBandit);
  if(pTarget->~IsCaptain()) pBandit->MakeBoss();

  // Namen anpassen
  SetName(Format("$NamePrisoner$", GetName(pTarget)), pBandit);

  // Alles fallen lassen  
  while(Contents(0, pTarget)) Exit(Contents(0, pTarget));

  // Symbole erzeugen
  CreateContents(HDCF, pTarget);
  CreateContents(HDCF, pBandit);
  ObjectSetAction(CreateObject(SIMP, 0, 0, GetOwner(pBandit)), "Imprison", pBandit);

  // Bandit wird in den "Transport"-Bandit versetzt
  Enter(pBandit, pTarget);
  
  // Zum Gefängnis laufen
  SetCommand(pBandit, "Enter", pOffice);

  // Der gefangene bekommt ein Menü zu Gesicht
  CreateMenu (HDCF, pTarget, pTarget, 0, GetName(pTarget), 0, 2);
  AddMenuItem(Format("$MsgCaught$", GetName(pTarget)), "", NONE, pTarget, 0, 0, Format("$MsgCaught$", GetName(pTarget)));

  // Beide sollen die gleiche Energie haben
  AddEffect("ImprisonShiftDamage", pBandit, 1, 0, 0, GetID(), pTarget);
}

func FxImprisonTimer(pTarget, iNumber, iEffectTime)
{
  // Variablen hohlen
  var pOffice = EffectVar(0, pTarget, iNumber),
      pBandit = EffectVar(1, pTarget, iNumber),
      pSheriff = EffectVar(2, pTarget, iNumber);
  // Sind wir schon da?
  if(Contained(pBandit)==pOffice) return(-1);
  // Von einem anderen Banditen befreit durch kämpfen?
  if(GetAction(pBandit)S="Fight") return(-1);
  // Bandit hat sich endlich selber befreit!
  if(iEffectTime>35*60*2) return(-1, Message("$MsgFree$", pTarget, GetName(pTarget)));
  // Wenn der Bandit angewählt ist nichts machen
  if(GetCursor(GetOwner(pBandit))==pBandit) return();
  // ... sonst selber versuchen zum Gefängnis zu kommen
  if(!GetCommand(pBandit)) SetCommand(pBandit, "Enter", pOffice);
}

func FxImprisonStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  var pOffice = EffectVar(0, pTarget, iNumber),
      pBandit = EffectVar(1, pTarget, iNumber),
      pSheriff = EffectVar(2, pTarget, iNumber);
  // Symbole entfernen    
  RemoveObject(FindContents(HDCF, pTarget));
  RemoveObject(FindContents(HDCF, pBandit));
  RemoveObject(FindObject(SIMP, 0, 0, 0, 0, 0, "Imprison", pBandit));
  // Menü schließen
  CloseMenu(pTarget);
  // Bandit wurde eingebuchtet
  if(Contained(pBandit)==pOffice)
  {
    Enter(pOffice, pTarget);
    DoEnergy(100, pTarget);
    pOffice->Imprison(pTarget, pBandit);
    CreateContents(HDCF, pOffice);
    GameCall("BanditCaught", GetOwner(pTarget));
  }
  else // Bandit hat sich befreit
  {
    pBandit->Exit(pTarget, 0, +10);
    pTarget->CreateObject(HDCF);   
  }
  RemoveObject(pBandit);
  SetObjectLayer(0, pTarget);
  SetClrModulation(0, pTarget);
}

// Sorgt dafür, dass die beiden Clonks immer gleiche Energie haben
func FxImprisonShiftDamageStart(pBandit, iNumber, fTmp, pTarget)
{
  if(fTmp) return();
  EffectVar(0, pBandit, iNumber) = pTarget;
}

func FxImprisonShiftDamageDamage(pBandit, iNumber, iDmgEngy, iCause)
{
  DoEnergy(iDmgEngy/1000, EffectVar(0, pBandit, iNumber));
  return(iDmgEngy);
}

func FxImprisonShiftDamageStop(pBandit, iNumber, iReason, fTemp)
{
  if(fTemp) return();
  if(iReason==4)
  {
    if(GetAlive(EffectVar(0, pBandit, iNumber))) Kill(EffectVar(0, pBandit, iNumber));    
  }
}
