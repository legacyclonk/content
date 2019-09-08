/*-- Hauptzelt --*/

#strict

// Das Zelt hat extra eine zu kleine Grafik, und wird mit einer Aktion voll angezeigt
// so kann es unter Palisaden errichtet werden und blockiert höhere Palisadenebenen nicht

// Bauanimation wie beim Tipi
#include TIPI

protected func Initialize()
{
  _inherited(...);
  SetAction("Exist");
  return(1);
}

/* Kann nicht von Indianern erforscht werden */
public func IsIndianResearch() { return(0); }

/* Kann nicht vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(0); }

/* Kann vom Trapper hergestellt werden */
public func IsTrapperHandcraft() { return(1); }
