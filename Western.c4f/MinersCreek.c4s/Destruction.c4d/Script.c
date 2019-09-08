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
 // Geldeffekte erstellen
 AddEffect("Money", FindObject(HEDQ, 203, 149, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgHEDQ0$", "$MsgHEDQ1$"), 125);
 AddEffect("Money", FindObject(AMRY, 417, 212, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgAMRY0$", "$MsgAMRY1$"), 110);
 AddEffect("Money", FindObject(AMRY, 257, 213, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgAMRY2$", "$MsgAMRY3$"), 110);
 
 AddEffect("Money", FindObject(HEDQ,1926, 167, -1, -1), 200, 0, this(), 0, 1, RandomItem("$MsgHEDQb0$", "$MsgHEDQb1$"), 125, 1);
 AddEffect("Money", FindObject(AMRY,1945, 291, -1, -1), 200, 0, this(), 0, 1, RandomItem("$MsgAMRYb0$", "$MsgAMRYb1$"), 110, 1);
 
AddEffect("Money", FindObject(SAWW, 999, 295, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgSAWW0$", "$MsgSAWW1$"),  50, 1);
AddEffect("Money", FindObject(DRST, 804, 352, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgDRST0$", "$MsgDRST1$"),  70, 1);
AddEffect("Money", FindObject(SALO,1019, 456, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgSALO0$", "$MsgSALO1$"),  60, 1);
AddEffect("Money", FindObject(TGST,1071,  69, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgTGST0$", "$MsgTGST1$"),  40, 1);
AddEffect("Money", FindObject(SHOF, 396, 409, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgSHOF0$", "$MsgSHOF1$"),  70, 1);
AddEffect("Money", FindObject(WRKW, 355, 534, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgWRKW0$", "$MsgWRKW1$"),  65, 1);
AddEffect("Money", FindObject(BANK, 245, 417, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgBANK0$", "$MsgBANK1$"), 100, 1);
 
 AddEffect("Money", FindObject(TEL1,1291,  72, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgTEL10$", "$MsgTEL11$"),  20);
 AddEffect("Money", FindObject(TEL1,1180,  60, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgTEL12$", "$MsgTEL13$"),  20);
 AddEffect("Money", FindObject(TEL1,1023,  60, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgTEL14$", "$MsgTEL15$"),  20);
 
AddEffect("Money", FindObject(PUMW, 817, 492, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgPUMW0$", "$MsgPUMW1$"),  30, 2);
AddEffect("Money", FindObject(WMLW, 672, 322, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgWMLW0$", "$MsgWMLW1$"),  25, 2);
AddEffect("Money", FindObject(WTOW, 962,  50, -1, -1), 200, 0, this(), 0, 2, RandomItem("$MsgWTOW0$", "$MsgWTOW1$"),  35, 2);
 // 2 = Team 2 ; "Das Haupt..." = Nachricht ; 150 = Prämie in Geld
}

public func RandomItem()
{
  var i=-1;
  while(Par(++i));
  return(Par(Random(i)));
}
 
public func IsFulfilled() { return(); } // Kooperativ geht nicht, return(1) für immer erfüllt

public func IsFulfilled()
{
  // Eine Seite muss alle Gebäude zestören  
  if(destright>=maxright && destutil>=maxutil) return(1);
  if(destleft>=maxleft) return(1);

  // Wird auch erfüllt falls eine Seite nur über das Ziel "Teammelee" gewinnt.
  if(FindObject(MELE)) return(FindObject(MELE)->IsFulfilled()); 
}

protected func Activate(iPlayer)
{
  var szString = "", iColor = RGB(255, 255,255);
  SetPosition(0,0);
  szString = Format("%s<c %x>$MsgBanditAims$ (%d/%d)</c>|", szString, RGB(255, 255,255), destright, maxright);
  szString = GetString(szString, SAWW, 999, 295, 50);
  szString = GetString(szString, DRST, 804, 352, 70);
  szString = GetString(szString, SALO,1019, 456, 60);
  szString = GetString(szString, TGST,1071,  69, 40);
  szString = GetString(szString, SHOF, 396, 409, 70);
  szString = GetString(szString, WRKW, 355, 534, 60);
  szString = GetString(szString, BANK, 245, 417, 100);
    
  /*
  //Fortgebäude der Kavallerie, für Wertung eine "1" bei "Geldeffekte erzeugen" nach der Prämie setzen
  szString = GetString(szString, HEDQ, 203, 149, 125); 
  szString = GetString(szString, AMRY, 417, 212, 110);
  szString = GetString(szString, AMRY, 257, 213, 110);
  */   
       
  if(maxutil)
  {
    szString = Format("%s<c %x>|$MsgUtilAims$ (%d/%d)</c>|", szString, RGB(255, 255,255), destutil, maxutil);
    szString = GetString(szString, PUMW, 817, 492, 30);
    szString = GetString(szString, WMLW, 672, 322, 25);
    szString = GetString(szString, WTOW, 962,  50, 40);
  }
    
  szString = Format("%s|<c %x>$MsgCalvalAims$ (%d/%d)</c>|", szString, RGB(255, 255,255), destleft, maxleft);
  szString = GetString(szString, HEDQ, 1926, 167, 125);
  szString = GetString(szString, AMRY, 1945, 291, 110);
  
  // Nachricht ausgeben
  return(MessageWindow(szString, iPlayer));
}

private func GetString(szString, idString, x, y, value)
{
  var iColor = RGB(200, 200,200);
  if(!FindObject(idString,x, y)) iColor = RGB(255, 0,0);
  return(Format("%s<c %x>%s (%d)</c>|", szString, iColor, GetName(0, idString), value));
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
  if(EffectVar(3, pTarget, iNumber)==1)
  {
    if(EffectVar(0, pTarget, iNumber)==1) destleft++;
    else destright++;
    if(destleft >=maxleft) Team2Lose();
    if(destright>=maxright && destutil>=maxutil) Team1Lose();
  }
  else if(EffectVar(3, pTarget, iNumber)==2)
  {
    destutil++;
    if(destright>=maxright && destutil>=maxutil) Team1Lose();
  }
  // Nachricht ausgeben
  if(EffectVar(3, pTarget, iNumber))
  {
    if(EffectVar(0, pTarget, iNumber)==1) ExtraLog("%s", EffectVar(1, pTarget, iNumber), destleft);
    else ExtraLog("%s", EffectVar(1, pTarget, iNumber), destright+destutil);
    ActualizeScoreboard();
  }
  else  ExtraLog("%s", EffectVar(1, pTarget, iNumber));
}
  
  
 /*
 // Zusätzliche Punkteanzeige. Deaktiviert, da Scoreboard ausreichen sollte.
  
  // Nachricht ausgeben
  if(EffectVar(3, pTarget, iNumber))
  {
    if(EffectVar(0, pTarget, iNumber)==1) ExtraLog("%s ($MsgPoints$ %d)", EffectVar(1, pTarget, iNumber), destleft);
    else ExtraLog("%s ($MsgPoints$ %d)", EffectVar(1, pTarget, iNumber), destright+destutil);
    ActualizeScoreboard();
  }
  else  ExtraLog("%s", EffectVar(1, pTarget, iNumber));
}
*/
