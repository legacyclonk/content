/*-- Konto --*/

#strict 2

local aAlliancePlayers;
local aAllianceAccounts;

local iValue; // Geld auf dem Konto
local iScore; // Punkte
local iTick;  // Zähler
local fSkipNextScore;
local iMyValue;

local fFirstAlliance;

/* Zerstörung */

protected func Destruction()
{
  // Befreundete Accounts benachrichtigen
  for(var pAccount in aAllianceAccounts)
      pAccount->AllianceDie(this);
}

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  iValue = GetWealth(GetOwner());
  iScore = GetPlrValueGain(GetOwner());

  // Anfangsbasismaterial
  var id;
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
    SetComponent(id, GetHomebaseMaterial(GetOwner(), id));

  // Zähler und Array initialisieren
  iTick = 1;
  aAlliancePlayers  = [];
  aAllianceAccounts = [];
}

/* Überprüfung */
// Mit der Funktion TACC_BlockTeamExchange(int iType) im Szenarioscript kann der Austausch einzelner Tpyen geblockt werden

private func Execute()
{
  // Gehört niemandem (Spieler eliminiert?) - löschen
  if (GetOwner() < 0) return RemoveObject();

  var iChange;
  // Basismaterial
  var id, i;
  if(!GameCall("TACC_BlockTeamExchange", TACC_TypeBaseMat))
  while(id = GetHomebaseMaterial (GetOwner(), 0, i++, C4D_All))
  {
    // Basismaterialänderung?
    if (GetHomebaseMaterial(GetOwner(), id) != GetComponent(id))
    {
      // Änderung berechnen und anrechenen
      iChange = GetHomebaseMaterial(GetOwner(), id)-GetComponent(id);
      SetComponent(id, GetComponent(id)+iChange);
      // Änderung auf verbündete Konten anrechnen
      for(var pAccount in aAllianceAccounts) pAccount->UpdateValue(TACC_TypeBaseMat, iChange, id);
    }
  }
  // Alle 35 Aufrufe CheckAlliances durchführen
  if (!--iTick) AllChk();
}

/* Wert ändern */

static const TACC_TypeValue   = 1;
static const TACC_TypeScore   = 2;
static const TACC_TypeBaseMat = 3;
static const TACC_TypePlans   = 4;
static const TACC_TypeSpells  = 5;

public func UpdateValue(iType, iByAmount, idTypeID)
{
  // Basismaterial
  if(iType == TACC_TypeBaseMat && !GameCall("TACC_BlockTeamExchange", TACC_TypeBaseMat))
  {
    if(!idTypeID) return;
    SetComponent(idTypeID, GetComponent(idTypeID)+iByAmount);
    DoHomebaseMaterial(GetOwner(), idTypeID, iByAmount);
    return;
  }
}

public func GetValue(iType, idTypeID)
{
  // Basismaterial
  if(iType == TACC_TypeBaseMat)
    return GetComponent(idTypeID);
}

protected func CalcValue()
{
  fSkipNextScore = 0;
  return iMyValue;
}


/* Allianzänderungen prüfen und Konten entsprechend umkonfigurieren */

private func AllChk()
{
  // Zähler zurücksetzen
  iTick=35;
  // Prüfen, ob Allianzen gebrochen wurden
  var i,iOwner = GetOwner(), iPlr;
  // Alle Konten der Teammitglieder durchgehen
  for (iPlr in aAlliancePlayers)
    // Konto vorhanden und verfeindet?
    if(Hostile(iOwner,iPlr))
      // Konto löschen
      BreakAlliance(iPlr);

  // Prüfen, ob neue Allianzen geschlossen wurden
  for(i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spieler ermitteln
    iPlr = GetPlayerByIndex(i);
    // Nicht verfeindet und noch kein Konto reserviert?
    if(!Hostile(iOwner,iPlr) && GetIndexOf(iPlr, aAlliancePlayers)==-1)
      // Neues Konto erstellen
      CreateAlliance(iPlr);
  }
}

/* Konto aus den Teamkonten löschen */

private func BreakAlliance(iPlr)
{
  // Alle Allianzen brechen, dabei alles aufteilen
  var iCount = GetLength(aAlliancePlayers)+1, id;

  for(var pAccount in aAllianceAccounts)
  {
    // Material anpassen
    for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
      pAccount->UpdateValue(TACC_TypeBaseMat, -GetComponent(id)/iCount, id);

    // Und diesen Spieler aus der Allianz löschen
    pAccount->RemoveAlliance(this);
    // Uns selbst natürlich auch löschen :)
    RemoveAlliance(pAccount);
  }
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
      UpdateValue(TACC_TypeBaseMat, GetComponent(id)/iCount - GetComponent(id), id);
}

/* Neuen Spieler in die Allianz aufnehmen */
// Wenn TACC_BlockFirstAllianceAdd im Szenarioscript 1 zurückgibt,
// wird das Anfangsguthaben des Teams als das vom ersten Mitglied genommen,
// statt aller Mitglieder zusammen

private func CreateAlliance(iWithPlr)
{
  // Verbündete Spieler überprüfen: Alle müssen verbündet sein
  var pAccount = FindObjectOwner(BACN, iWithPlr);
  if(!pAccount) return;
  if(!CheckAlliances(pAccount)) return;
  // Werte anpassen
  var iNewValue;
  if(!GameCall("TACC_BlockFirstAllianceAdd")) fFirstAlliance = 1;
  // Basismaterial
  var id;
  for(var i=0; id = GetHomebaseMaterial (GetOwner(), 0, i, C4D_All); i++)
  {  
    iNewValue = pAccount->AddAllianceValue(TACC_TypeBaseMat, GetComponent(id), id, !fFirstAlliance);
    AddAllianceValue(TACC_TypeBaseMat, iNewValue-GetComponent(id), id);
  }
  for(var i=0; id = GetHomebaseMaterial (iWithPlr, 0, i, C4D_All); i++)
  {
    if(GetHomebaseMaterial(GetOwner(), id)) continue; // Wurde schon behandelt
    iNewValue = pAccount->AddAllianceValue(TACC_TypeBaseMat, GetComponent(id), id, !fFirstAlliance);
    AddAllianceValue(TACC_TypeBaseMat, iNewValue-GetComponent(id), id);
  }
  fFirstAlliance = 1;
  // Allianz hinzufühen
  pAccount->AddAlliances(this);
  AddAlliances(pAccount);

  return 1;
}

/* Prüfen, ob dieser Spieler mit der Allianzgruppe pWith verbündet ist */

private func CheckAlliances(pWith)
{
  // Allianzen für alle verbündeten Konten prüfen...
  for (var pAccount in aAllianceAccounts)
    if (!CheckAlliance(pAccount, pWith)) return false;
  // ...und für sich selbst
  return CheckAlliance(this, pWith);
}

/* Prüfen, ob der Account pAcc mit pWith verbündet ist */

public func CheckAlliance(pAcc, pWith)
{
  // Besitzer von pAcc ermitteln 
  var iOwner = GetOwner(pAcc);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var iPlr = GetPlayerByIndex(i);
    // Konto für diesen Spieler in pWith vorhanden?
    if( pWith->CheckPlayer() )
      // Verfeindet?
      if(Hostile(iOwner, iPlr) )
        // Dann war's wohl nix.
        return 0;
  }
  // Zuletzt noch prüfen, ob der Spieler mit pWith selbst verbündet ist.
  return !Hostile(iOwner, GetOwner(pWith));
}

public func CheckPlayer(iPlr)
{
  // Prüft, ob ein Spieler schon in der Allianz ist
  if(GetIndexOf(iPlr, aAlliancePlayers) == -1) return false;
  return true;
}

public func GetAlliancePlayerCount()
{
  return GetLength(aAlliancePlayers)+1;
}

public func GetPlayerAccount(iPlr)
{
  // Sucht das Konto eines Spielers
  for (var pAccount in aAllianceAccounts)
    if(GetOwner(pAccount) == iPlr)
      return pAccount;
}

// Fügt einer ganzen Allianz einen Wert von einem bestimmten Typen zu
public func AddAllianceValue(iType, iAddValue, idTypeID, fSet)
{
  if(fSet) iAddValue -= GetValue(iType, idTypeID);
  // Bei Verbündeten hinzufügen...
  for (var pAccount in aAllianceAccounts)
    pAccount->UpdateValue(iType, iAddValue, idTypeID);
  // ...und bei sich selbst
  UpdateValue(iType, iAddValue, idTypeID);
  return GetValue(iType, idTypeID);
}

public func AddAlliances(pGroup)
{
  // Bei Verbündeten hinzufügen...
  for (var pAccount in aAllianceAccounts)
    // Jo, also hinzufügen
    pAccount->AddAlliance(pGroup);
  // ...und bei sich selbst
  AddAlliance(pGroup);
}

public func AddAlliance(pGroup)
{
  var o = GetOwner(pGroup);
  // Jeden Spieler durchgehn
  for(var i = 0; i < GetPlayerCount(); ++ i)
  {
    // Spielernummer ermitteln
    var iPlr = GetPlayerByIndex(i);
    // Verbündet mit dem neuen, aber nicht mit uns?
    if(pGroup->CheckPlayer(iPlr) && iPlr != GetOwner() && !CheckPlayer(iPlr))
    {
      // Dazufügen
      aAllianceAccounts[GetLength(aAllianceAccounts)] = pGroup->GetPlayerAccount(iPlr);
      aAlliancePlayers[GetLength(aAlliancePlayers)] = iPlr;
    }
  }
  
  // Auch hier neue Allianz hinzufügen
  aAllianceAccounts[GetLength(aAllianceAccounts)] = pGroup;
  aAlliancePlayers[GetLength(aAlliancePlayers)] = GetOwner(pGroup);
}

/*  Allianz stirbt: Geldwert belassen und Zähler verringern */

public func AllianceDie(pDeadAccount)
{
  RemoveArrayValue(pDeadAccount, aAllianceAccounts);
  RemoveArrayValue(GetOwner(pDeadAccount), aAlliancePlayers);
}

public func RemoveAlliance(pDeadAccount)
{
  RemoveArrayValue(pDeadAccount, aAllianceAccounts);
  RemoveArrayValue(GetOwner(pDeadAccount), aAlliancePlayers);
}


/* Info */

public func GetTeam()
{
  // Kein Team? :'(
  if (!GetLength(aAlliancePlayers)) return "$NoAccount$";

  // Alle Allianzmitglieder speichern
  var szResult, i = GetLength(aAlliancePlayers)-1;
  for(var iPlr in aAlliancePlayers)
  {
    if(!szResult) szResult = Format("$AccountWith$ %s", GetPlayerName(iPlr));
    else if(!i) szResult = Format("%s $And$ %s", szResult, GetPlayerName(iPlr));
    else szResult = Format("%s, %s", szResult, GetPlayerName(iPlr));
    i--;
  }
      
  return szResult;
}

// Sucht einen Wert im Array und löscht diesen
private func RemoveArrayValue(Test, array &aArray)
{
  // Aus der Liste löschen
  var i = GetLength(aArray), iLength = GetLength(aArray);
  while(i--)
  	if(aArray[i] == Test)
   	{
      		aArray[i] = 0;
      		while(++i < iLength) aArray[i-1] = aArray[i];
      		SetLength(aArray, iLength-1);
      		return 1;
    	}
  return 0;
}
