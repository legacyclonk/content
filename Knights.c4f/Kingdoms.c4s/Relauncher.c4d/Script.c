/* Relauncher */

#strict

local iRelaunchProcess; // Relaunchfortschritt
local fSacrificeMade; // Flagge geopfert?

/* Relaunch */

public func & SacrificeMade() { return(fSacrificeMade); }

public func GetRelaunchProcess() { return(iRelaunchProcess); }

public func DoRelaunchProgress()
  {
  if (++iRelaunchProcess < 100) return();
  return(DoRelaunch());
  }

public func DoRelaunch()
  {
  // Einfach den Inhalt auswerfen
  while (Contents()) Exit(Contents());
  // Effekt
  Sound("Ding", 1);
  Log("$MsgRelaunch$", GetPlayerName(GetOwner()));
  Sparkle(10);
  // Thron benachrichtigen
  if (Contained()) Contained()->~RelaunchDone(this());
  // Fertig; entfernen
  return(RemoveObject());
  }


/* Steuerung */

protected func ContainedUp(pClonk)
  {
  [$CtrlQueryStatus$]
  // Status anzeigen
  var szMsg;
  if (fSacrificeMade)
    szMsg = Format("$MsgRelaunchProcess$", iRelaunchProcess);
  else
    szMsg = "$MsgNoRelaunch$";
  return(PlayerMessage(GetOwner(pClonk), szMsg, this()));
  }

protected func ContainedLeft(pClonk) { [$CtrlQueryStatus$] return(ContainedUp(pClonk)); }
protected func ContainedRight(pClonk) { [$CtrlQueryStatus$] return(ContainedUp(pClonk)); }
protected func ContainedThrow(pClonk) { [$CtrlQueryStatus$] return(ContainedUp(pClonk)); }

