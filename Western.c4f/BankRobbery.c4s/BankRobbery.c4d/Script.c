/*-- Bankraubziel --*/

#strict
#include GOAL

// Wer die Kisten selber aufstellen will, muss nur die Funktion 'PlaceBanditChests' im Szenarioscript benutzen
// Für Teams s.u.

protected func Initialize()
{
  // Warten
  SetAction("Initializing");
  return(_inherited());
}

protected func InitializePlayer(iPlr)
{
  // Spieler hat Banditen?
  if(PlayerHasBandits(iPlr))
    // Banditenspieler
    Local(iPlr) = 1;
}

protected func Initialized()
{
  // Dieses Spielziel geht nicht kooperativ
  if(!FindObject(RVLR)) CreateObject(RVLR, 0,0, -1);
  // Hier kann man jetzt eigene Banditenkisten platzieren und Teams festlegen (oder hier im Spielziel 'AllyPlayers' aufrufen
  if(GameCall("PlaceBanditChests")) return();
  // Spieler werden in zwei Teams aufgeteilt
  AllyPlayers();
  // Jeden Spieler im Spiel durchlaufen
  for(var i=0, hirank, chest ; i < GetPlayerCount() ; i++)
    // Spieler vorhanden
    if(GetPlayerByIndex(i) > -1)
      {
      // Spieler ist ein Bandit
      if(!Local(i)) continue;
      // Dem Spieler eine Kiste geben, sonfern ein Verbündeter noch keine hat
      if(PlayerHasChest()) continue;
      hirank = GetCrew(i, 0);
      chest = CreateObject(_TCH, AbsX(GetX(hirank)), AbsY(GetY(hirank)), i);
      }
}

public func AllyPlayers()   // Teilt die Spieler in zwei Teams auf. Die Banditen und die Städter
{
  for(var i=0,j ; i < GetPlayerCount() ; i++)
    for(j=0 ; j < GetPlayerCount() ; j++)
     if(GetPlayerByIndex(i) > -1)
      if(GetPlayerByIndex(j) > -1)
       {
       if(i == j) continue;
       if(Local(i) && !Local(j))
         SetHostility(i,j, true, true);
       if(!Local(i) && Local(j))
         SetHostility(i,j, true, true);
       }
}

private func PlayerHasBandits(int iPlr)
{
  // Hat der Spieler einen Banditen in der Crew?
  for(var i=0, bandit ; i < GetCrewCount(iPlr) ; i++)
    if(GetCrew(iPlr, i)->~IsBandit())
      bandit = true;
  return(bandit);
}

private func PlayerHasChest(int iPlr)
{
  var chest;
  // Hat der Spieler selber eine Kiste?
  if(chest = FindObjectOwner(_TCH, iPlr)) return(chest);
  // Alle Spieler durchgehen, befreundet prüfen und schauen, ob die eine Kiste haben
  var i=0;
  while(i < GetPlayerCount())
    {
    if(GetPlayerByIndex(i) > -1)
      if(!Hostile(iPlr, i))
        if(FindObjectOwner(_TCH, i))
          chest = FindObjectOwner(_TCH, i);
    i++;
    }
  // Keine Kiste
  return(chest);
}

public func IsFulfilled() { return(); } // Kooperativ geht nicht

public func IsFulfilledforPlr(int iPlr)
{
  // Spieler ist ein Bandit
  if(Local(iPlr)) return(BanditWin(iPlr));
  // Spieler ist ein Städter
  else return(TownspeopleWin(iPlr));
}

private func BanditWin(int iPlr)
{
  // Banditen gewinnen, wenn sie mehr Geld als die Städter haben
  var tmoney, bmoney, chest;
  // Geld der Städter zählen
  tmoney = GetWealth(GetPlrByTeam(1));
  // Geld der Banditen zählen
  bmoney = GetWealth(GetPlrByTeam(-1));
  // Mehr Geld als Städter? :OO
  if(bmoney > tmoney) return(1);
  return();
}

private func TownspeopleWin(int iPlr)
{
  // Städter gewinnen, wenn alle Banditen tot oder eingesperrt sind
  var bandit = true;
  for(var i=0,j ; i < GetPlayerCount() ; i++)
   if(GetPlayerByIndex(i) > -1)
    {
    if(Local(i))
      for(j=0 ; j < GetCrewCount(i) ; j++)
        {
        if(!Contained(GetCrew(i,j)))
          bandit = false;
        else
          if(! (Contained(GetCrew(i,j))->~IsPrison()))
            bandit = false;
        }
    }
  return(bandit);
}
