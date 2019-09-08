/*-- Milestone Woods --*/

#strict

func Initialize() {
  
  FixElevator(FindObject(ELEV,  391, 532, -1, -1), 180);
  FixElevator(FindObject(ELEV, 1146, 295, -1, -1), 0);
  
  CreateObject(TBMT); // TeamBasismaterial
  CreateObject(CNSW); // Bäume zerlegen
  
  CreateObject(REPR);  // Nachwuchs
  CreateObject(SCRG);  // Spielziel
  
  // Tafeltexte
  SignpostText("Sgnp1", "$SgnpTemple1$");
  SignpostText("Sgnp2", "$SgnpTemple2$");
  SignpostText("Sgnp3", "$SgnpFortville$");
  
  // Banditen
  var pBandit = CreateObject(BNDT, 2053, 239, -1);
  RemoveEffect("*", pBandit);
  pBandit->SetAI("Bandit", 3, REVR);
  pBandit->SetColorDw(RGB(150));
  pBandit = CreateObject(BNDT, 2079, 220, -1);
  RemoveEffect("*", pBandit);
  pBandit->SetAI("Bandit", 3, WINC);
  pBandit->SetColorDw(RGB(150));
  pBandit = CreateObject(BNDT, 2116, 187, -1);
  RemoveEffect("*", pBandit);
  pBandit->SetAI("Bandit", 3, WINC);
  pBandit->SetColorDw(RGB(150));
  pBandit->MakeBoss();
  pBandit = CreateObject(BNDT, 1804, 138, -1);
  RemoveEffect("*", pBandit);
  pBandit->SetAI("Bandit", 3, WINC);
  pBandit->SetColorDw(RGB(150));

  ai_idFirstEncounterCB = BNDT;

  ScriptGo(1); 

}

protected func InitializePlayer(player)
{
  // Den Spieler ausrüsten
  CreateContents(MLLT, GetHiRank(player));
  CreateContents(KNFE, GetHiRank(player));
  return(1);
}

protected func Script1()
{
  StartMovie("Intro", GetCrew(GetPlayerByIndex()), GetCrew(GetPlayerByIndex(), 1));
  ScriptGo(0);
}

protected func OnGoalsFulfilled()
{
  StartMovie("Extro", GetCrew(GetPlayerByIndex()), GetCrew(GetPlayerByIndex(), 1)); 
  return(1);
}

public func AI_EncounterBNDT(object pBandit, object pPlayer)
  {
  StartMovie("Blackhat", pPlayer, pBandit); 
  return(true);
  }

global func FixElevator(object elev, iDig)
  {
  var x=GetX(elev), y=GetDefBottom(elev);
  var elev2 = CreateConstruction(ELEV, x,y, -1, 100, true); // Fürs Fundament...
  elev2->RemoveObject();
  elev->CreateShaft(iDig);
  }
