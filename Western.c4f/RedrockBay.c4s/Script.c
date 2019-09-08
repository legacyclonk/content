/*-- Redrock Bay --*/

#strict

func Initialize() {

 // Ziele erschaffen
 CreateObject(CROB);  //Siedlungsbau
 CreateObject(MONE);  //Geld erwirtschaften..
 CreateObject(MONE);  //..viel Geld..
 CreateObject(MONE);  //..sehr viel Geld

 CreateObject(REPR);  // Nachwuchs

 var pGoalTele = CreateObject(TSCG);  //Telegraphenbau  

 LocalN("pStartPole", pGoalTele) = FindObject(TEL1);
 LocalN("pEndPole", pGoalTele) = FindObject(TEL1,0, 0, 0, 0, 0, 0, 0, 0, LocalN("pStartPole", pGoalTele));
 AddEffect("NoDmg", LocalN("pStartPole", pGoalTele), 101, 0, 0, TSCG);
 AddEffect("NoDmg", LocalN("pEndPole", pGoalTele), 101, 0, 0, TSCG);

 //Gebäudebau
 FindObject(CROB)->AddType(DRST, 1);
 FindObject(CROB)->AddType(BANK, 1);
 FindObject(CROB)->AddType(SALO, 1);
 FindObject(CROB)->AddType(SAWW, 1);
 FindObject(CROB)->AddType(SHOF, 1);
 FindObject(CROB)->AddType(TGST, 1);
 FindObject(CROB)->AddType(DRST, 1);
 FindObject(CROB)->AddType(WRKW, 1);


 // Schildtexte
 SignpostText("SgnpBay", "$SgnpBay$");

 // Planwagen erstellen
 var pWagon=CreateObject(COAC,466,410);

 // Startmaterial
 for(var i=0;i<6;i++)
 CreateContents(MEAC, pWagon);
 for(var i=0;i<2;i++)
 CreateContents(WSKI, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(WHKY, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(IGNB, pWagon);
 for(var i=0;i<2;i++)
 CreateContents(DYNB, pWagon);
 for(var i=0;i<4;i++)
 CreateContents(METL, pWagon);
 for(var i=0;i<3;i++)
 CreateContents(CNKT, pWagon);
 for(var i=0;i<6;i++)
 CreateContents(WOOD, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(WINC, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(AMBO, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(MLLT, pWagon);
 for(var i=0;i<1;i++)
 CreateContents(KNFE, pWagon);
 for(var i=0;i<2;i++)
 CreateContents(LNKT, pWagon);
 for(var i=0;i<2;i++)
 CreateContents(ROPE, pWagon);

  // Geräuschkulisse
  //SeaSound(350);
  //SeaSound(400);
  //SeaSound(450);
  //SeaSound(500);
  //SeaSound(550);
  SeaSound(600);
  //SeaSound(650);
  Seagulls(400);
  //Seagulls(450);
  //Seagulls(500);
  //Seagulls(550);
  
  ScriptGo(1); 
}

protected func Script1()
{
  StartMovie("Intro",GetCrew(GetPlayerByIndex()),GetCrew(GetPlayerByIndex(),1));
  ScriptGo(0);
}

protected func OnGoalsFulfilled()
{
  StartMovie("Extro", GetCrew(GetPlayerByIndex()), GetCrew(GetPlayerByIndex(), 1));
  GainMissionAccess("WestRB");
  return(1);    
}

// Meeresrauschen
func SeaSound() {
return(ObjectSetAction(CreateObject(SL9Z,Par(),400,420),"Sea"  ));
}

// Mowen
func Seagulls() {
return(ObjectSetAction(CreateObject(SL9Z,Par(),400,420),"Seagulls"));
}
