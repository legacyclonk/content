#strict
#appendto EXTG

private func DoExtinguish(object oExtiObj, object oCaller)
  {
  //Keine Gebäude
  if(GetCategory(oExtiObj) & C4D_Structure) return(0);  
  
  // Nicht wenn gegnerischer CrewMember
  if (Hostile(GetOwner(oExtiObj), GetOwner(oCaller)))
    if (GetOCF(oExtiObj) & OCF_CrewMember())
      if (!ObjectCount(NTMG)) return(0);
  // Lösche es
  Extinguish(oExtiObj);
  // Eine paar Effekte...
  CastObjects(MSTB,10,25, GetX(oExtiObj)-GetX(), GetY(oExtiObj)-GetY());
  CastParticles("MSpark", 200, 50, GetX(oExtiObj)-GetX(),GetY(oExtiObj)-GetY(), 10, 50, RGBa(100,100,255,128), RGBa(0,0,255,0));
  // 1 Objekt gelöscht
  return(1);
  }