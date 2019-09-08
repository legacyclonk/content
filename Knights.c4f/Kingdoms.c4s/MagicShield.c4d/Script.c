/* Magieschild */

#strict

local time, obj_cnt;

public func Activate(caster, real_caster)
  {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;
  if (!clonk) return(0, RemoveObject());

  // Nur Ritter
  if (!clonk->~IsKnight()) return();

  // Anderer Schild vorhanden? Abnehmen
  var pPrevShield = clonk->~HasShield();
  if (pPrevShield) pPrevShield->~Unbuckle();

  // Magieschild erzeugen
  return(CreateObject(_SHM, 0,0, GetOwner(clonk))->Activate(clonk));
  }
