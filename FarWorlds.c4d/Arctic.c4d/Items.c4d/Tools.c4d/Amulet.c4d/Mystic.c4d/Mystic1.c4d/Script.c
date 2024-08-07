/*-- Mystikamulett --*/

#strict
#include AMD1

public func Activate(pClonk) {
  [$TxtMagic$]
  if(GetPhysical("Magic",2,pClonk)<100000) return(0,Message("$TxtsisnoAngakok$",this(),GetName(pClonk)));
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Men� �ffnen
  CreateMenu(MCMS,pClonk,this(),MenuExtraMode(),"$TxtNoMagic$",ObjectNumber(pClonk));
  if(Inside(Global(),0,5)) AddMenuItem( "$TxtMagic$: %s", "ConjureUp", FIRG, pClonk,0,pClonk );
  if(Inside(Global(),1,5)) AddMenuItem( "$TxtMagic$: %s", "ConjureUp", AIRG, pClonk,0,pClonk );
  if(Inside(Global(),2,5)) AddMenuItem( "$TxtMagic$: %s", "ConjureUp", SNWG, pClonk,0,pClonk );
  if(Inside(Global(),3,5)) AddMenuItem( "$TxtNextGrade$", "DoMagic",   NXGD, pClonk,0,pClonk );
  return(1);
}

/* Geister beschw�ren */

protected func ConjureUp(idSpirit,pClonk) {
  // nur beim Gehen
  if(GetAction(pClonk)ne"Walk") return(0);
  // Schauen ob Landmasse vorhanden ist
  if (GBackLiquid(-30+60*GetDir(Contained()),+20)) return( Message("$TxtTooCloseToWater$",this(),GetName(pClonk)), Sound("Error") );
  if (!GBackSolid(-30+60*GetDir(Contained()),+15)) return( Message("$TxtNoGoodPos$",this()), Sound("Error") );
  // Ist der gew�nschte Geist schon vorhanden, abbruch
  if (FindObject(idSpirit)) return( Message("$TxtAlreadyThere$",this()), Sound("Error") );
  // Energie pr�fen und abziehen
  if (!DoMagicEnergy(-Value(idSpirit),pClonk))
    return( Message("$TxtErrMagic$",this(),GetName(pClonk)), Sound("Error") );
  // Aktion starten
  ObjectSetAction(pClonk,"Conjuration");
  Sound("InukConjuration");
  // Geisterfunken erschaffen und aktivieren
  CreateObject(GHSP,-30+60*GetDir(Contained()),-GetY(),-1)->GHSP::Activate(idSpirit,pClonk);
  return(1);
}
