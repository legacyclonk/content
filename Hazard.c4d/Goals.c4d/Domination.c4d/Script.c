/*-- Dominaton --*/

#strict
#include TEAM

static DOM_markerNumber;

global func CreateDominationPoint(int iX, int iY, int iID, id sourceID)
{
  if(!sourceID) sourceID = DBAS;
  var point = CreateObject(DBAS, iX, iY, -1);
  point->Set(iID, DOM_markerNumber++, sourceID);
  return(point);
}

protected func Initialize() {
  //Anzahl Punkte wird durch Anzahl Aktivierungen eingestellt
  var pGoal;
  if (pGoal = FindObject(GetID()))
  { 
    LocalN("iWinScore", pGoal)++;
    return(RemoveObject()); 
  }
  iWinScore = 1;
  return(_inherited());
}

public func Dominating(int iTeam)
{
  Sound("Domination", 1);
  Log("$Dominating$", GetTeamName(iTeam));
  DoTeamScore(iTeam, 1);
  var iX, iY;
  for(var pClonk in FindObjects(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive)))
  {
    var pos = GameCall("RelaunchPosition",GetPlayerTeam(pClonk->GetOwner()));
    var iX = pos[0];
    var iY = pos[1];
    pClonk->SetPosition(iX, iY);
    pClonk->SetComDir(COMD_Stop);
    if(GetPlayerTeam(pClonk->GetOwner()) == iTeam) // Geld!
      DoWealth(pClonk->GetOwner(), 30);
  }
}

public func GetHUDInfo(int player, object hud) {
	
	var team = GetPlayerTeam(player);
	if(team < 1) return(inherited(player));
	
	var markerCount = ObjectCount(DBAS);
	
	for(var marki in FindObjects(Find_ID(DBAS))) {
		var mID = marki->GetMarkerID();
		var color = GetTeamColor(marki->GetTeam());
		if(!marki->GetTeam()) color = RGB(255,255,255);
		var overlay = 2+marki->GetOrder();
		var order = marki->GetOrder();
		
		SetDOMHudGraphics(hud,Format("%d",mID), color, overlay, order);
		
		if(marki->GetCaptureTeam()) color = GetTeamColor(marki->GetCaptureTeam());
		overlay += markerCount;
		
		SetDOMHudGraphics(hud,Format("Capture%d",mID), color, overlay, order);
	}
	
	return(inherited(player));
}

private func SetDOMHudGraphics(object hud, string name, int color, int overlay, int position)
{
	hud->SetGraphics(name,0,DBAS,overlay,2,"Std");
	hud->SetObjDrawTransform(1000,0,35000+position*28000,0,1000,-66000,0,overlay);
	hud->SetClrModulation(color,0,overlay);
}

public func MarkerTeamChange() {
	UpdateHUDs();
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }

public func GetFriendlyMarkers(owner) {
	var ret = CreateArray();
	for(var pMarker in FindObjects(Find_ID(DBAS)))
	{
		if(pMarker->GetTeam() == GetPlayerTeam(owner))
			ret[GetLength(ret)] = pMarker;
	}
	return(ret);
}

public func GetNotFriendlyMarkers(owner) {
	var ret = CreateArray();
	for(var pMarker in FindObjects(Find_ID(DBAS)))
	{
		if(pMarker->GetTeam() != GetPlayerTeam(owner))
			ret[GetLength(ret)] = pMarker;
	}
	return(ret);
}

public func MaxMarkerGuardingDistance() { return(300); }


// KI Taktik
public func AITactic(object pAIPlayer) {

	var owner = pAIPlayer->GetOwner();
	var FM = GetFriendlyMarkers(owner);
	var EM = GetNotFriendlyMarkers(owner);
	var commandedCrew = CreateObject(L_OS,1,1,-1);
	
	// Clonks die in was drin sind wollen wir nicht befehligen
	var commandableCrewcount = ObjectCount2( Find_OCF(OCF_CrewMember),Find_Owner(owner), Find_NoContainer());

	// so lange Befehle erteilen bis alle bedient sind...
	while(commandedCrew->GetItemCount() < commandableCrewcount) {
		// das heisst also wenn es 4 marker gibt aber 12 KI-Clonks, stürmen
		// auf alle Marker etwa 3 Leute zu

		// Diese Taktik ist ein bischen anders als die anderen: Hier wird nicht
		// pro Clonk sondern pro Marker geplant
		for(var pMarker in EM) {
			var markerX = AbsX(GetX(pMarker));
			var markerY = AbsY(GetY(pMarker));
			// suche das nächste Crewmitglied
			// und gehe zum nächsten Marker der mir nicht gehört und zu dem nicht gegangen wird
			var crewArr = FindObjects( Find_OCF(OCF_CrewMember),
			                           Find_Owner(owner),
			                           Find_NoContainer(),
			                           Sort_Distance(markerX, markerY)
			                         );
			if(crewArr) {
				for(var crew in crewArr ) {
					// merke zu welchem gegangen wird
					var notcommanded = commandedCrew->AddItem(crew);
					// noch nicht befehligt? gut.
					if(notcommanded) {
						crew->CheckInventory();
						pAIPlayer->Run(crew,pMarker,Format("Running to marker at %d,%d!",markerX,markerY));
						break;
					}
					// ansonsten den nächsten Clonk befehligen dort hinzugehen...
				}
			}
			// wenn es mehr zu erobernde Marker gibt als Clonks zu befehligen,
			// kriegt ein Marker halt erstmal kein Clonk zugeteilt
		}
		// wenn allerdings ncoh Clonks übrig sind...
		// alle Clonks die nahe an eroberten Punkten sind, können das mal bewachen
		for(var pMarker in FM) {
			var markerX = AbsX(GetX(pMarker));
			var markerY = AbsY(GetY(pMarker));
			
			var crewArr = FindObjects( Find_OCF(OCF_CrewMember),
			                           Find_Owner(owner),
			                           Find_NoContainer(),
			                           Sort_Distance(markerX, markerY)
			                         );
			if(crewArr) {
				for(var crew in crewArr ) {
					// merke zu welchem gegangen wird
					var notcommanded = commandedCrew->AddItem(crew);
					if(ObjectDistance(crew,pMarker) > MaxMarkerGuardingDistance()) continue;
					// noch nicht befehligt? gut.
					if(notcommanded) {
						crew->CheckInventory();
						pAIPlayer->Guard(crew,Format("Guarding marker at %d,%d!",markerX, markerY));
						break;
					}
				}
			}
		}
		
	}
	return true;
}

// Es könnte Probleme geben mit Find_Owner da wir das nicht überladen
// können - vielleicht werden KI-Spieler sonst nicht gefunden
global func CheckOwner(int owner) { return(owner == GetOwner()); }