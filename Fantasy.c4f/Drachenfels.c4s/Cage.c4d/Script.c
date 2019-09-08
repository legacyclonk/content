/*-- Käfig --*/

#strict

/* Öffnung */

protected func ControlThrow(object pClonk)
  { [$CtrlOpen$|Image=_KEY]
  // Schon offen?
  if (GetAction() eq "Open") return();
  // Schlüssel dabei?
  var pKey = FindContents(_KEY, pClonk);
  if (!pKey) return(Message("$MsgNoKey$", pClonk, GetName(pClonk)), Sound("CommandFailure*"));
  // Aufschließen
  Sound("Connect", this());
  Sound("ChestOpening", this());
  SetAction("Open");
  SetSolidMask();
  GameCall("OnCageOpened", this());
  }
