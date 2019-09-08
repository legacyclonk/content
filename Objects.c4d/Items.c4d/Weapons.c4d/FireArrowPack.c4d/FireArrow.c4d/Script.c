/*-- Brandpfeil --*/

#strict

#include ARRW

public func Launch()
{
  Incinerate();
  return(_inherited() );
}

public func HitTarget(pObject)
{
 if(!_inherited(pObject))return(0);
 // Objekt moeglicherweise anzuenden?
 var iContactInc=GetDefCoreVal("ContactIncinerate","DefCore",GetID(pObject));
 if(iContactInc && !Random(iContactInc) && !Random(3))
 {
  Incinerate(pObject);
 }
 return(1);
}

/* Pfeileigenschaften */
public func PackTo() { return(FARP); }
public func ArrowStrength() { return(2+Random(2)); }
public func ArrowTumbleRandom() { return(3); }
