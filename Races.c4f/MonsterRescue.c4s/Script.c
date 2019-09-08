/* Rennen */

#strict

// WICHTIG: damit man das Rennen auf Liga spielen kann
// muss Wettstreit(RVLR) aktiviert sein

func Initialize()
{
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
func GetRACEStartOffset() { return(30); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(30); }


// Spielerbeitritt
func InitializePlayer(iPlr)
{
  return(JoinPlayer(iPlr));
}

// Relaunch
protected func RelaunchPlayer(int iPlr)
{
 // Verkünden
 Log("$MsgRestart$", GetPlayerName(iPlr));

 // Ein neuer Clonk für den Spieler mit voller Energie
 var pClonk = CreateObject(MAGE, 10, 10, iPlr);
 MakeCrewMember(pClonk, iPlr);
 DoEnergy(100, pClonk);

 // Den Spieler neu starten lassen
 return(JoinPlayer(iPlr,pClonk));
}

func JoinPlayer(iPlr, pClonk)
{ 
  if(!pClonk) pClonk = GetHiRank(iPlr);
  // Anwählen und in Position bringen
  SetCursor(iPlr,pClonk);
  SelectCrew(iPlr, pClonk, 1);
  SetPosition(34,183, pClonk); // Hier die Koordinaten für die Relaunchposition anpassen!
  DoMagicEnergy(30, pClonk);
  SetPhysical("Magic", 30000, 2, pClonk);
  // altes Monster evtl. entfernen
  if(FindObjectOwner(MONS, iPlr))
    RemoveObject(FindObjectOwner(MONS, iPlr));
  var pMonster = CreateObject(MONS, 34, 183, iPlr);
  Enter(pMonster, pClonk);
  SetPortrait("1", pClonk, MONS);
  AddEffect("PossessionSpell", pMonster, 1, 0);
}

global func FxPossessionSpellStop()
{
  return(-1);
}
