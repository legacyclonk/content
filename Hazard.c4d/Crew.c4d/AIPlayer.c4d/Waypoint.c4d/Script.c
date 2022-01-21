/* Waypoint */

#strict

/* Global */

static WAYP_visibleWaypoints;

global func CreateWP(int x, int y) {
  var WP = CreateObject(WAYP,AbsX(x),AbsY(y+8),-1);
  return WP;
}

// Flags sind nicht kombinierbar
static const Path_MoveTo = 0; // ;)
static const Path_Jump = 1;
static const Path_Backflip = 2;
static const Path_Lift = 3;
//... weitere denkbar

// Zündungsrichtungen für das Jetpack
static const Jetpack_Left = 1;
static const Jetpack_UpLeft = 2;
static const Jetpack_UpRight = 3;
static const Jetpack_Right = 4;

static const WAYP_PathErr = "Path %d does not exist.";

// macht alle Wegpunkte sichtbar/unsichtbar
global func WaypointsVisible(bool visu) {
  for(var obj in FindObjects(Find_ID(WAYP)))
    obj->SetVisible(visu);
  WAYP_visibleWaypoints = visu;
}

// alle Wegpunkte testen (deren Pfade)
global func WaypointsTest() {
  var time = 0;
  for(var obj in FindObjects(Find_ID(WAYP))) {
    ScheduleCall(obj,"Test",time);
    time += 80;
  }
}

// Schreibt ein Wegpunktscript in den Enginelog
global func LogWaypoints() {
  var aWaypoints = FindObjects(Find_ID(WAYP));
  
  var i = 0;
  for(var pWp in aWaypoints)
    Log("  var wp%d = CreateWP(%d,%d);", ++i, pWp->GetX(), pWp->GetY());
  
  i = 0;
  for(pWp in aWaypoints)
  {
    i++;
    for(var j = 0,eo ; j < pWp->WAYP::GetPathCount() ; j++)
    {
      eo = Waypoints_GetArrayPosition(aWaypoints, pWp->WAYP::GetPathTarget(j));
      if(i < 0)
      {
        Log("// Path error: no target!");
        continue; // Pfad kann man nicht auswerten
      }

      var flag = pWp->WAYP::GetPathFlag(j);
      var strFlag = Format("%d",flag);
      if(flag == 0) strFlag = "Path_MoveTo";
      if(flag == 1) strFlag = "Path_Jump";
      if(flag == 2) strFlag = "Path_Backflip";
      if(flag == 3) strFlag = "Path_Lift";

      if(pWp->WAYP::GetPathJetpack(j))
        Log("  wp%d -> AddPath(wp%d, %s, %d, %d, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j), pWp->WAYP::GetPathJetpack(j), pWp->WAYP::GetPathJetpackFlag(j));
      else
        Log("  wp%d -> AddPath(wp%d, %s, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j));
      
      var cmd;
      for(var z = 0; cmd = pWp->WAYP::GetArriveCommand(j,z,0,true);z++)
      {
        Log("  wp%d -> SetArriveCommand(%d, %d, \"%s\", Hier Objekt einfügen [%s], %d, %d);",i, j,z,cmd, GetName(pWp->WAYP::GetArriveCommand(j,z,1,true)), pWp->WAYP::GetArriveCommand(j,z,2,true), pWp->WAYP::GetArriveCommand(j,z,3,true));
      }
    }
  }
}

/* Serialisierung */

public func Serialize(array& extra)
{
	var cmd;
	for (var i=0;i<GetLength(aPath);++i) {
		if(GetPathFlag(i) == 0) cmd = "Path_MoveTo";
		else if(GetPathFlag(i) == 1) cmd = "Path_Jump";
		else if(GetPathFlag(i) == 2) cmd = "Path_Backflip";
		else if(GetPathFlag(i) == 3) cmd = "Path_Lift";
		if (GetPathJetpack(i)) {
			extra[GetLength(extra)] = Format("AddPath(Object(%%d),%s,%d,%d,%d)", GetPathTarget(i), cmd, GetPathDir(i), GetPathJetpack(i), GetPathJetpackFlag(i));
		} else {
			extra[GetLength(extra)] = Format("AddPath(Object(%%d),%s,%d)", GetPathTarget(i), cmd, GetPathDir(i));
		}
		for (var j=0;cmd=GetArriveCommand(i,j,0);++j) {
			if (GetArriveCommand(i,j,4)) {
				// TODO: any Data für AddCommand in der KI kann u.U. auch Objektzeiger beinhalten.. auch serialisieren. :D
				extra[GetLength(extra)]  = Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,Object(%%d),%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,4), GetArriveCommand(i,j,5));
			} else {
				extra[GetLength(extra)]  = Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,0,%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,5));
			}
		}
	}
}

global func Waypoints_GetArrayPosition(array aWps, object pFind)
{
  var i=0, gotit = false;
  for(var pWp in aWps)
  {
    if(pWp == pFind)
    {
      gotit = true;
      break;
    }
    i++;
  }
  if(!gotit) i=-1;
  else i += 1;
  return(i);
}

/* Effekt */
// Der Effekt speichert Pfaddaten (Kanteneigenschaften)

// EffectVars
// 0: nächster Knoten
// 1: Flags
// 2: X-Richtung des Pfades
// 3: Jetpack-Zündungs-Delay
// 4: Jetpack-Zündungsrichtung
// ab 5 bis ...: Kommandos

/* Lokal */

local aPath;

// Setters

// iNo ist die entsprechende Nummer (wer hätte das gedacht?) - beginnend bei 0! :o
public func SetArriveCommand(int pId, int iNo, string command, object pTarget, int iX, int iY, object pTarget2, Data) {
  EffectVar(iNo + 5, this(), aPath[pId]) = [command,pTarget,iX,iY,pTarget2,Data];
}

// wenn fReconnect true ist, wird die Strecke zurückverbunden
public func AddPath(object target, int flag, int dir, int jetpdelay, int jetpflag, bool fReconnect) {
  // ein Pfad ohne Target ist nix
  if(!target || target == this()) return(false);
  // Pfad eintragen
  aPath[GetLength(aPath)] = AddEffect("PathData", this(), 1, 0, this());

  var pId = GetLength(aPath)-1;

  SetPathTarget(pId, target);
  SetPathDir(pId, dir);
  SetPathFlag(pId,flag);
  SetPathJetpack(pId,jetpdelay);
  SetPathJetpackFlag(pId,jetpflag);

  if(WAYP_visibleWaypoints)
     CreateObject(VIPU,0,0,-1)->Set(this(),target,flag,jetpdelay,pId);

  if(fReconnect)
    target->AddPath(this(), flag, dir*-1, jetpdelay, jetpflag);

  return(pId);
}

public func RemovePath(int pId) {
  RemoveEffect(0,this(),aPath[pId]);
  var aNewArray = CreateArray(), i;
  for(var path in aPath)
  {
    if(i != pId) aNewArray[GetLength(aNewArray)] = path;
    i++;
  }
  aPath = aNewArray;
}

public func SetPathTarget(int pId, object target) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));

  // Vorsicht: wenn auf 0 gesetzt wird, zeigt der Pfad ins nix
  EffectVar(0,this(),aPath[pId]) = target;
}

public func SetPathDir(int pId, int dir) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));

  // dir nicht angegeben: automatisch aus Position von target ermitteln
  if(!dir) {
    var target = GetPathTarget(pId);
    if(!target) return();
    var diff = GetX(target)-GetX();
    dir = diff/Abs(diff);
  }
  EffectVar(2,this(),aPath[pId]) = dir;
}

public func SetPathFlag(int pId, int flag) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));

  EffectVar(1,this(),aPath[pId]) = flag;
}

public func SetPathJetpack(int pId, int jetpack) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));

  EffectVar(3,this(),aPath[pId]) = jetpack;
}

public func SetPathJetpackFlag(int pId, int flag) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));

  if(!flag && GetPathJetpack(pId))
  {
    var angle = Angle(GetX(), GetY(), GetPathTarget(pId)->GetX(), GetPathTarget(pId)->GetY());
    if(Inside(angle, 180, 310)) flag = Jetpack_Left;
    if(Inside(angle, 311, 360)) flag = Jetpack_UpLeft;
    if(Inside(angle, 0, 49)) flag = Jetpack_UpRight;
    if(Inside(angle, 50, 179)) flag = Jetpack_Right;
  }

  EffectVar(4,this(),aPath[pId]) = flag;
}

// Getters

// iElement
// 0: szCommand
// 1: pTarget
// 2: iX
// 3: iY
// 4: pTarget2
// 5: Data
public func GetArriveCommand(int pId, int iNo, int iElement, bool NoLog) {
  if(GetPathCount() <= pId)
  { 
    if(NoLog)
      return();
    return(ErrorLog(WAYP_PathErr, pId));
  }
  if(iElement < 0 || iElement > 5) return();
  if(!EffectVar(iNo + 5, this(), aPath[pId])) return();
  return(EffectVar(iNo + 5,this(),aPath[pId])[iElement]);
}

public func GetArriveCommandCount(int pId) {
  var i = 0;
  while(EffectVar(i + 5,this,aPath[pId]))
    i++;
  return i;
}

public func GetPathCount() {
  return(GetLength(aPath));
}

public func GetPathTarget(int pId) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(0,this(),aPath[pId]));
}

public func GetPathDir(int pId) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(2,this(),aPath[pId]));
}

public func GetPathFlag(int pId) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(1,this(),aPath[pId]));
}

public func GetPathJetpack(int pId) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(3,this(),aPath[pId]));
}

public func GetPathJetpackFlag(int pId) {
  if(GetPathCount() <= pId) return(ErrorLog(WAYP_PathErr, pId));
  return(EffectVar(4,this(),aPath[pId]));
}

// Visualization

public func SetVisible(bool vis) {
  if(!vis) {
    SetAction("Idle");
    Message("", this());
    // alle Pfad-Anzeige-Objekte löschen
    RemovePathDisplays();
  }
  else {
    SetAction("Vis");
    Message("@%d", this(), ObjectNumber());
    // alle Pfad-Anzeige-Objekte erstellen
    // (von Pfaden die vom Knoten wegführen)
    CreatePathDisplays();
  }
}

public func Test() {
  // alle Pfade
  for(var i; i<GetLength(aPath); ++i) {
	var hzck = CreateObject(HZCK,0,10,-1);
	var jtpk = hzck->CreateContents(JTPK);
	hzck->DoAmmo(GSAM,500);
	jtpk->Activate(hzck);
	hzck->SetLocal(13,GetPathTarget(i));
	//hzck->SetMacroCommand(this(),"Wait",0,0,0,40);
	hzck->SetMacroCommand(this(),"MoveTo",GetPathTarget(i));
  }
}

public func OnMacroCommandFailed(clonk, iReason) {
  Log("Failed for <c ff0000>%d</c>(%d,%d)-><c ff0000>%d</c>(%d,%d). Reason: %d",ObjectNumber(),GetX(),GetY(),ObjectNumber(Local(13,clonk)),GetX(Local(13,clonk)),GetY(Local(13,clonk)),iReason);
}

public func OnMacroCommandSuccess(clonk) {
  if(clonk) 
  {
    clonk->LocalN("aGear") = CreateArray();
    while(clonk->Contents())
      RemoveObject(clonk->Contents());
    RemoveObject(clonk);
  }
}

/* Intern */

protected func Initialize()
{
  aPath = CreateArray();
  SetObjectBlitMode(GFX_BLIT_Additive);
  if(WAYP_visibleWaypoints) { Message("@%d", this(), ObjectNumber()); SetAction("Vis"); }
}

private func CreatePathDisplays() {
  // alle Pfade
  for(var i; i<GetLength(aPath); ++i) {
     var visu = CreateObject(VIPU,0,0,-1);
     visu->Set(this(),GetPathTarget(i),GetPathFlag(i),GetPathJetpack(i),i, GetPathDir(i));
  }
}

private func RemovePathDisplays() {
  for(var obj in FindObjects(Find_ID(VIPU),Find_ActionTarget(this())))
    if(obj)
      RemoveObject(obj);
}


// visu Set(anfangsknoten, endknoten, flag, jetpack)

