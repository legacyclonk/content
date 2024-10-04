/*-- Marker --*/

#strict

local iTeam, mID, iOrder;
local light1, light2, progress;
local iCaptureTeam, iCaptureStartTime;

public func Set(int iID, int iNumber, id sourceID)
{
  mID = iID;
	iOrder = iNumber;
  SetGraphics(Format("%d", iID), this(), sourceID, 1, 2, "Std");
  SetObjDrawTransform(1000,0,0,0,1000,-20000,0,1);
  light1 = AddLightCone(700, RGB(50,50,50), this());
  light2 = AddLightCone(700, RGB(50,50,50), this());
  light1->ChangeOffset(-10,3);
  light1->ChangeR(-30);
  light1->ChangeColor(RGB(50,50,50));
  light2->ChangeOffset(10,3);
  light2->ChangeR(30);
  light2->ChangeColor(RGB(50,50,50));
	progress = CreateObject(EBAR);
	progress->Set(this);
	progress->SetVisibility(VIS_None);
	progress->SetVertex(0,1,43);
}

protected func Timer()
{
	var team = 0;
	var heldByOnlyOneTeam = false;
	var heldByNoone = true;
  for(var clnk in FindObjects(Find_InRect(-25,-10, 50,20), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive)))
	{
		heldByNoone = false;
		if(!team)
		{
			team = GetPlayerTeam(clnk->GetOwner()); 
			heldByOnlyOneTeam = true;
		}
		else if(team != GetPlayerTeam(clnk->GetOwner()))
		{
			heldByOnlyOneTeam = false;
			break;
		}
	}
	
	// neues Team erobert
	if(heldByOnlyOneTeam && team != iTeam && team != iCaptureTeam)
	{
		SetCaptureTeam(team);
	}
	else if(team == iTeam && iTeam)
	{
		SetNoCaptureTeam();
	}
	// gleiches Team erobert weiter
	else if(iCaptureTeam)
	{
		// undo capture progress wenn niemand mehr da
		if(heldByNoone) iCaptureStartTime+=3;
		// Capture progress anhalten wenn nicht der einzige dort
		else if(!heldByOnlyOneTeam) iCaptureStartTime++;

		DoCaptureProgress();
	}
}

private func SetCaptureTeam(int team)
{
	iCaptureTeam = team;
	SetClrModulation(GetTeamColor(team),progress,1);
	progress->Update(0);
	progress->SetVisibility(VIS_All);
	iCaptureStartTime = FrameCounter();
	GameCallEx("MarkerTeamChange",iTeam,iCaptureTeam);
}

private func SetNoCaptureTeam()
{
	iCaptureTeam = 0;
	progress->Update(0);
	progress->SetVisibility(VIS_None);
	GameCallEx("MarkerTeamChange",iTeam,iCaptureTeam);
}

private func DoCaptureProgress()
{
	var timeCapturing = FrameCounter() - iCaptureStartTime;
	if(timeCapturing >= GetCaptureTime())
	{
		progress->SetVisibility(VIS_None);
		ChangeTeam(iCaptureTeam);
	}
	else if(timeCapturing < 0)
	{
		progress->SetVisibility(VIS_None);
		SetNoCaptureTeam();
	}
	progress->Update(100 * timeCapturing / GetCaptureTime());
}

public func ChangeTeam(int team)
{
  // Gleiches Team?
  if(iTeam == team) return();
	
  // Team ändern
  iTeam = team;
	iCaptureTeam = 0;
  // Farbe ändern
  SetClrModulation(GetTeamColor(iTeam), 0, 1);
  // Lichter ändern
  light1->ChangeColor(GetTeamColor(iTeam));
  light2->ChangeColor(GetTeamColor(iTeam));
  // Symbole benachrichtigen
  GameCallEx("MarkerTeamChange",iTeam,iCaptureTeam);
  // Testen, ob Punkt gemacht
  CheckPoint();
}

public func GetCaptureTime() { return 220; }
public func GetTeam() { return iTeam; }
public func GetCaptureTeam() { return iCaptureTeam; }
public func GetMarkerID() { return mID; }
public func GetOrder() { return iOrder; }

private func CheckPoint()
{
  var count, mark = FindObjects(Find_ID(GetID()), Find_Exclude(this()));
  for(var pMarker in mark)
    if(pMarker->GetTeam() == iTeam)
      count++;
  // score
  if(ObjectCount(GetID()) == count)
  {
    var team = iTeam;
    SetNoTeam();
    for(pMarker in mark)
      pMarker->SetNoTeam();
		GameCallEx("Dominating",team);
  }
}

public func SetNoTeam()
{
  iTeam = 0;
	SetNoCaptureTeam();
  SetClrModulation(RGB(255,255,255), 0, 1);
  // Lichter
  light1->ChangeColor(RGB(50,50,50));
  light2->ChangeColor(RGB(50,50,50));
}
