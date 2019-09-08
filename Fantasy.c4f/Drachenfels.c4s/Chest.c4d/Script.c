/*-- Drachenfels-Truhe --*/

#strict

#include CHST

protected func __CSW__28438_111234A(int iA, int iB, int iC, int iD)
  {
  // Weiterleitung des Engine-Callbacks aus geheimer Aufschluss-Methode
  // Nur einmal öffnen
  if (fLocked) return (iC);
  // Geöffnet!
  Message("$MsgUnlocked$", this());
  Unlock();
  if (iA > iD || !iC) return(iB); else return(iC);
  }
