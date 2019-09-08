/*-- Gebäudezerstörung --*/

#strict
#include GOAL

static destleft, destright;

protected func Initialize()
{
  // Warten
  SetAction("Initializing");
  return(_inherited());
}

protected func Initialized()
{
 // Geldeffekte erstellen für die linke Burg..
 AddEffect("Money", FindObject(CPOF,182,360,-1,-1), 200, 0, this(), 0, 2, "$MsgPart1$", 120, 1);  //Kontor unten links
 AddEffect("Money", FindObject(CPOF,103,180,-1,-1), 200, 0, this(), 0, 2, "$MsgPart2$", 120, 1);  //Kontor oben links
 AddEffect("Money", FindObject(CPWK,219,237,-1,-1), 200, 0, this(), 0, 2, "$MsgPart3$", 100, 1);  //Werkstatt links
 AddEffect("Money", FindObject(CPAL,344,298,-1,-1), 200, 0, this(), 0, 2, "$MsgPart4$", 90, 1);  //Labor links
 AddEffect("Money", FindObject(CPR1,123,481,-1,-1), 200, 0, this(), 0, 2, "$MsgPart5$", 75, 1);  //Waffenkammer links
 
 // ..und die rechte
 AddEffect("Money", FindObject(CPOF,1523,357,-1,-1), 200, 0, this(), 0, 1, "$MsgPart6$",  120, 1);  //Kontor unten rechts
 AddEffect("Money", FindObject(CPOF,1603,173,-1,-1), 200, 0, this(), 0, 1, "$MsgPart7$",  120, 1);  //Kontor oben rechts
 AddEffect("Money", FindObject(CPWK,1484,235,-1,-1), 200, 0, this(), 0, 1, "$MsgPart8$",  100, 1);  //Werkstatt rechts
 AddEffect("Money", FindObject(CPAL,1361,295,-1,-1), 200, 0, this(), 0, 1, "$MsgPart9$",  90, 1);  //Labor rechts
 AddEffect("Money", FindObject(CPR1,1583,477,-1,-1), 200, 0, this(), 0, 1, "$MsgPart0$",  75, 1);  //Waffenkammer rechts
 
 // 2 = Team 2 ; "Das Haupt..." = Nachricht ; 150 = Prämie in Geld
}

/*
// für Zufallsnachrichten
public func RandomItem()
{
  var i=-1;
  while(Par(++i));
  return(Par(Random(i)));
}
*/
 
public func IsFulfilled() { return(); } // Kooperativ geht nicht, return(1) für immer erfüllt

public func IsFulfilled()
{
  // Eine Seite muss alle Gebäude zestören  
  if(destright>=4) return(1);
  if(destleft>=4) return(1);

  // Wird auch erfüllt falls eine Seite nur über das Ziel "Teammelee" gewinnt.
  if(FindObject(MELE)) return(FindObject(MELE)->IsFulfilled()); 
}





protected func Activate(iPlayer)
{
  var szString = "", iColor = RGB(255, 255,255);
  SetPosition(0,0);
  szString = Format("%s<c %x>$MsgShadowAims$ (%d/%d)</c>|", szString, RGB(255, 255,255), destleft, 4);
  szString = GetString(szString, CPOF,1523,357,120);
  szString = GetString(szString, CPOF,1603,173,120);
  szString = GetString(szString, CPWK,1484,235,100);
  szString = GetString(szString, CPAL,1361,295,90);
  szString = GetString(szString, CPR1,1583,477,75);
    
  szString = Format("%s|<c %x>$MsgMantisAims$ (%d/%d)</c>|", szString, RGB(255, 255,255), destright, 4);
  szString = GetString(szString, CPOF,182,360, 120);
  szString = GetString(szString, CPOF,103,180, 120);
  szString = GetString(szString, CPWK,219,237, 100);
  szString = GetString(szString, CPAL,344,298,90);
  szString = GetString(szString, CPR1,123,481,75);
    
  // Nachricht ausgeben
  return(MessageWindow(szString, iPlayer));
}

private func GetString(szString, idType, x, y, value)
{
  var iColor = RGB(200, 200,200);
  if(!FindObject(idType,x, y)) iColor = RGB(255, 0,0);
  return(Format("%s<c %x>%s (%d)</c>|", szString, iColor, GetName(0, idType), value));
}

/*-- Effekt für Geld bei der Zerstörung von Gebäuden --*/

func FxMoneyStart(pTarget, iNumber, fTmp, iTeam, szMessage, iAmount, fPoint)
{
  if(fTmp) return();    
  EffectVar(0, pTarget, iNumber) = iTeam;
  EffectVar(1, pTarget, iNumber) = szMessage;
  EffectVar(2, pTarget, iNumber) = iAmount;
  EffectVar(3, pTarget, iNumber) = fPoint;
}

func FxMoneyEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber)
{
  // Bei Feuer Effekt beenden
  if (WildcardMatch(szNewEffect, "*Fire*")) { RemoveEffect("Money", iEffectTarget); return();}
}

func FxMoneyStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();    
  // Spieler aus dem Team suchen
  var i = -1;
  while( i++ < GetPlayerCount()) if(CheckTeam(GetPlayerByIndex(i))==EffectVar(0, pTarget, iNumber)) break;
  // Geld überweisen
  DoWealth(GetPlayerByIndex(i),EffectVar(2, pTarget, iNumber));
  Sound("Cash",1);
  // Punkte vergeben
  if(EffectVar(3, pTarget, iNumber))
  {
    if(EffectVar(0, pTarget, iNumber)==1) destleft++;
    else destright++;
    if(destleft >=4) Team2Lose();
    if(destright>=4) Team1Lose();
  }
  
  // Nachricht ausgeben
  if(EffectVar(3, pTarget, iNumber))
  {
    if(EffectVar(0, pTarget, iNumber)==1) Log("%s ($MsgPoints$ %d)", EffectVar(1, pTarget, iNumber), destleft);
    else Log("%s ($MsgPoints$ %d)", EffectVar(1, pTarget, iNumber), destright);
    ActualizeScoreboard();
  }
  else  Log("%s", EffectVar(1, pTarget, iNumber));
}

