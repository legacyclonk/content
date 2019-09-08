/*-- Konto --*/

#strict

local value; // Geld auf dem Konto
local tick;  // Zähler
local lastA; // Größte Spielernummer der Verbündeten
local cnt;   // Anzahl Verbündeter

// Local(n): Konto von Spieler n, wenn befreundet

/* Zerstörung */

protected func Destruction()
{
  // Befreundete Accounts benachrichtigen
  var i,acc;
  while (i<=lastA)
    if (acc=Local(i++))
      acc->AllianceDie(this() );
}

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  value=GetWealth(GetOwner());

  // Zähler initialisieren
  tick=35;
  lastA=-1;
}

/* Überprüfung */

private func Execute()
{
  // Gehört niemandem (Spieler eliminiert?) - löschen
  if (GetOwner() < 0) return(RemoveObject() );

  // Geldwertänderung?
  if (GetWealth(GetOwner()) != value)
  {
    // Änderung berechnen und anrechenen
    var d=GetWealth(GetOwner())-value;
    value+=d;
    // Änderung auf verbündete Konten anrechnen
    var i,acc;
    while (i<=lastA) if (acc=Local(i++)) acc->UpdateValue(d);
  }
  // Alle 35 Aufrufe CheckAlliances durchführen
  if (!--tick) AllChk();
}

/* Kontobestand ändern */

public func UpdateValue(iByAmount)
{
  // Änderung auf value und Wealth getrennt anrechnen, da beide verschieden sein können,
  // wenn zwei Konten im gleichen Frame geändert wurden
  value+=iByAmount;
  SetWealth(GetOwner(), GetWealth(GetOwner()) + iByAmount);
}

/* Allianzänderungen prüfen und Konten entsprechend umkonfigurieren */

private func AllChk()
{
  // Zähler zurücksetzen
  tick=35;
  // Prüfen, ob Allianzen gebrochen wurden
  var i,o = GetOwner();
  // Alle Konten der Teammitglieder durchgehen
  for (;i<=lastA;i++)
    // Konto vorhanden und verfeindet?
    if(Local(i) && Hostile(o,i))
      // Konto löschen
      BreakAlliance(i);

  // Prüfen, ob neue Allianzen geschlossen wurden
  for(i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spieler ermitteln
    var plr = GetPlayerByIndex(i);
    // Nicht verfeindet und noch kein Konto reserviert?
    if(!Hostile(o,plr) && !Local(plr))
      // Neues Konto erstellen
      CreateAlliance(plr);
  }
}

/* Konto aus den Teamkonten löschen */

private func BreakAlliance(iPlr)
{
  // Alle Allianzen brechen, dabei Geld aufteilen
  var v1=value/(cnt+1);
  UpdateValue(v1-value);
  var o = GetOwner();

  // Alle Konten durchgehen
  for (var i = 0;i<=lastA;i++) if (Local(i))
  {
    // Auch hier Geld anpassen
    Local(i)->UpdateValue(-v1);
    // Und diesen Spieler aus der Allianz löschen
    Local(i)->RemoveAlliance(o);
    // Uns selbst natürlich auch löschen :)
    RemoveAlliance(i);
  }

  cnt=0;
  lastA=-1;
}

/* Neuen Spieler in die Allianz aufnehmen */

private func CreateAlliance(iWithPlr)
{
  // Verbündete Spieler überprüfen: Alle müssen verbündet sein
  var acc=FindObjectOwner(ACNT,iWithPlr);
  if(!acc) return(0);
  if (!CheckAlliances(acc)) return(0);
  // Neuen letzten Verbündeten berechnen
  lastA=Max(lastA, iWithPlr);
  // Allianz hinzufügen; Locals setzen
  var v2 = ObjectCall(acc,"AddAlliances",this(),value); //acc->AddAlliances(this(), value);
  AddAlliances(acc, v2-value);

  return(1);
}

/* Prüfen, ob dieser Spieler mit der Allianzgruppe pWith verbündet ist */

private func CheckAlliances(pWith)
{
  // Allianzen für alle verbündeten Konten prüfen...
  for (var i = 0;i<=lastA;i++) if (Local(i))
    if (!CheckAlliance(Local(i), pWith)) return(0);
  // ...und für sich selbst
  return(CheckAlliance(this(), pWith));
}

/* Prüfen, ob der Account pAcc mit pWith verbündet ist */

public func CheckAlliance(pAcc, pWith)
{
  // Besitzer von pAcc ermitteln 
  var o = GetOwner(pAcc);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Konto für diesen Spieler in pWith vorhanden?
    if(Local(plr, pWith) )
      // Verfeindet?
      if(Hostile(o, plr) )
        // Dann war's wohl nix.
        return(0);
  }
  // Zuletzt noch prüfen, ob der Spieler mit pWith selbst verbündet ist.
  return(!Hostile(o, GetOwner(pWith)) );
}

public func AddAlliances(pGroup, iAddValue)
{
  // Bei Verbündeten hinzufügen...
  for (var i = 0;i<=lastA;i++)
    // Verbündet?
    if (Local(i))
      // Jo, also hinzufügen
      Local(i)->AddAlliance(pGroup, iAddValue);
  // ...und bei sich selbst
  AddAlliance(pGroup, iAddValue);
  return(value);
}

public func AddAlliance(pGroup, iAddValue)
{
  // Locals setzen
  var o = GetOwner(pGroup);
  // Jeden SPieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var plr = GetPlayerByIndex(i);
    // Verbündet?
    if(Local(plr, pGroup) )
    {
      // Sind wie das selbst?
      if(Local(plr) || plr == GetOwner() ) continue;
      // Konto hinzufügen
      Local(plr) = Local(plr, pGroup);
      // Neue höchste Spielernummer in diesem Team ermitteln
      lastA = Max(lastA, plr);
      // Diese Allianz hat ein Mitglied mehr
      ++cnt;
    }
  }
  
  // Auch hier neue Allianz hinzufügen
  Local(o)=pGroup;
  // Und neue höchste Spielernummer setzen
  lastA=Max(lastA, o);
  ++cnt;
  // Geldwert anpassen
  UpdateValue(iAddValue);
}

/*  Allianz stirbt: Geldwert belassen und Zähler verringern */

public func AllianceDie()
{
  return(--cnt);
}

public func RemoveAlliance(iAlliance)
{
  // Allianz entfernen; Zähler verringern
  if (!Local(iAlliance)) return(0);
  Local(iAlliance) = 0;
  --cnt;
  while (!Local(lastA) && lastA) --lastA;
  return(1);
}


/* Info */

public func GetTeam()
{
  // Kein Team? :'(
  if (!cnt) return();
  // Alle Allianzmitglieder speichern
  var j,k;
  for (var i = 0; i <= lastA; i++)
    if (Local(i))
      Var(j++) = GetPlayerName(i);

  // Erstes Mitglied holen
  var rs = Var(0);

  // Und alle weiteren mit Komma getrennt dranhängen
  while (++k < j)
    if (Var(k))
      rs = Format("%s, %s", rs, Var(k));
      
  return(rs);
}
