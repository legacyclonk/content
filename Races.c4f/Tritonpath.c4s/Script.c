/* Rennen */

#strict

// WICHTIG: damit man das Rennen auf Liga spielen kann
// muss Wettstreit(RVLR) aktiviert sein

// -- Callbacks des Rennen-Spielziels --
// wenn diese Funktionen nicht im Szenarioscript sind
// oder 0 zurück geben, wird der Default-Wert verwendet

// Richtung:
// 1: links -> rechts
// 2: rechts -> links
// 3: untern -> oben
// 4: oben -> unten
// default: links -> rechts
func GetRACEDirection() { return(2); }

// Start: Anzahl an Pixeln, ab dem Rand, von dort beginnt die Zählung
// default: 50 px
func GetRACEStartOffset() { return(20); }

// Ende: Anzahl an Pixeln, ab dem Rand, bis dorthin geht die Zählung
// default: 50 px
func GetRACEEndOffset() { return(30); }

// Gewinnbedingung
//  1: Spieler hat gewonnen
//  0: gewinnen erlaubt wenn am Rand
// -1: kein gewinnen mögliche
func CheckRACEGoal(iPlr)
{
  // Der Spieler muss Reiten
  if(GetAction(GetCursor(iPlr)) S= "Ride" && GetID(GetCursor(iPlr)->GetActionTarget()) == HISD)
     return(0); // Dann kann der Spieler gewinnen
  // Sonst muss er sich erst mal ein Pferd suchen
  return(-1);
}

func GetRACEWayPercent(iPlr)
{
  if(GetAction(GetCursor(iPlr))S="Ride" && GetID(GetCursor(iPlr)->GetActionTarget()) == HISD)
    return(BoundBy((LandscapeWidth()-GetX(GetCursor(iPlr))-20)*50/(LandscapeWidth()-30-20)+50, 1, 99));
  else
    return(BoundBy((GetX(GetCursor(iPlr))-20)*50/(LandscapeWidth()-20-30), 1, 99));
}

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
 var pClonk = CreateObject(HCLK, 10, 10, iPlr);
 MakeCrewMember(pClonk, iPlr);
 DoEnergy(100, pClonk);

 // Den Spieler neu starten lassen
 return(JoinPlayer(iPlr));
}

func JoinPlayer(iPlr)
{ 
  // Anwählen und in Position bringen
  SetCursor(iPlr,GetHiRank(iPlr));
  SelectCrew(iPlr, GetHiRank(iPlr), 1);
  SetPosition(10,260, GetCursor(iPlr)); // Hier die Koordinaten für die Relaunchposition anpassen!
  var pHut = FindObject(_HUT);
  if(pHut) Enter(pHut, GetHiRank(iPlr));
  FindObject(ANVL)->CreateContents(KSDL);
//  var obj = FindObjectOwner(HISD);
//  if(obj) obj->Kill();
//  CreateObject(HISD, 10, 260, iPlr)->~ActivateEntrance(GetCursor(iPlr));
//  SetDir(1, FindObjectOwner(HISD));
}
