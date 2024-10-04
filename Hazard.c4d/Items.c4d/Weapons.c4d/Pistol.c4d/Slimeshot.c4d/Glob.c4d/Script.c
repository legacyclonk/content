/*-- Schleimklumpen --*/

#strict

local target, // Klebezielobjekt
      damage; // Evtl. Schaden

func Initialize() {
  SetR(Random(360));
  AddAlienFlareEffect(this(),100);
}

/* Kleben am Clonk */

public func AttachTo(pClonk)    // An einem Clonk heften
{
  //SetVertexXY(0, GetX()-GetX(pClonk)+GetVertex(0, 0, pClonk), GetY()-GetY(pClonk)+GetVertex(0, 1, pClonk));
  SetAction("AGlobbing", pClonk);

  if(!DecreasePhysical(pClonk)) return(RemoveObject());
}

private func DecreasePhysical(object pClonk)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  if(GetPhysical("Walk", 2, pClonk) <= GetPhysical("Walk", 1, 0, GetID(pClonk))*7/10)
    return(0);
  // Effekt starten
  AddEffect("Slimed", pClonk, 50, 800, 0, GetID());
  return(1);
}

/* Kleben am Boden */

public func Glob2Ground(iDamage)
{
  damage = iDamage;
  SetAction("Globbing");
}

protected func Check()
{
  var living;
  while(living = FindObject(0, -5,-5, 10,10, OCF_Living(), 0,0, NoContainer(), living))
    if(GetAlive(living) && !(living)->~IsAlien())
      return(HitLiving(living));
  while(living = FindObject(0, 1,1, 0,0, OCF_Living(), 0,0, NoContainer(), living))
    if(GetAlive(living) && !(living)->~IsAlien())
      return(HitLiving(living));

  if(GetActTime() >= 600) return(Boom());

}

private func HitLiving(target)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  if(!(GetPhysical("Walk", 2, target) <= GetPhysical("Walk", 1, 0, GetID(target))*7/10))
    // Effekt starten
    AddEffect("Slimed", target, 101, 1500, 0, GetID());

  // Schaden roflol
  DoDmg(damage, DMG_Bio, target);
  // Weg
  Boom();
}

/* Schleimdingsis */

public func Boom()
{
  // Weg
  RemoveObject();
}

protected func AttachTargetLost()
{
  RemoveObject();
}

/* Schleim-Effekt */

public func FxSlimedStart(target, no, temp,int iStrength)
{
  if(temp)
    if(GetPhysical("Walk", 2, target) <= GetPhysical("Walk", 1, 0, GetID(target))*7/10)
      return(0);

  // Variablen
  // 0: Walk
  // 1: Jump
  // 2: Scale
  // 3: Hangle
  EffectVar(0, target, no) = GetPhysical("Walk", 1, 0, GetID(target))*1/10;
  EffectVar(1, target, no) = GetPhysical("Jump", 1, 0, GetID(target))*1/10;
  EffectVar(2, target, no) = GetPhysical("Scale", 1, 0, GetID(target))*1/10;
  EffectVar(3, target, no) = GetPhysical("Hangle", 1, 0, GetID(target))*1/10;

  SetPhysical("Walk", GetPhysical("Walk", 0, target)-EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)-EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)-EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)-EffectVar(3, target, no), 2, target);

  // blar
  SetClrModulation(RGB(200,255,200), target);
}

public func FxSlimedTimer()
{
  return(-1);
}

public func FxSlimedStop(target, no, reason, temp)
{
  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(3, target, no), 2, target);

  // blar
  if(!GetEffect("*Slimed*", target)) SetClrModulation(RGB(0,0,0), target);

  if(temp) return(0);

  // Schleimklumpen entfernen
  var glob = FindObject(GLOB, 0,0,0,0,0, "AGlobbing", target);
  if(glob) glob->~Boom();
}

// Feuer! :o
protected func Incineration() { Boom(); }
