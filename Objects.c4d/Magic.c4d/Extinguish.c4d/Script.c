/* Löschen */

#strict

public func Activate(oCaller, oClonk)
{                            
  if(!oClonk) oClonk = oCaller;
  // Effekt prüfen
  var iChkEff;
  if (iChkEff = CheckEffect("ExtinguishPSpell", oExtiObj, 180)) return(iChkEff!=-1 && RemoveObject());
  // Lavafässer zu Granit werden lassen
  var obj;
  if (obj = FindContents(LBRL, oClonk))
    if (!CheckEffect("ExtinguishPSpell", obj, 180))
    {
      var mat = "Granite";
      // Granit nicht geladen -> Stein casten
      if (!MaterialLoaded(Material(mat))) mat = "Rock";
      // Stein nicht geladen -> Kohle casten
      if (!MaterialLoaded(Material(mat))) mat = "Coal";
      // Material schleudern
      var amount = Min(400, obj->GetAmount()*3);
      CastPXS(mat, amount, 40, AbsX(GetX(oClonk)), AbsY(GetY(oClonk)));
      // Objekt entfernen
      RemoveObject(obj);
    }
  // Nächstes brennendes Objekt finden
  var oExtiObj, iExtiCount;
  // Den Clonk selbst?
  if(OnFire(oClonk)) 
    iExtiCount += DoExtinguish(oClonk, oCaller);
  // Nächstliegende Objekte
  SetPosition(GetX(oCaller), GetY(oCaller));
  // Suchradius ist abhängig von Höhe des Aufrufenden - ein Zauberturm hat einen größeren Radius
  var iSearchRadius = GetObjHeight(oCaller) * 3;
  while(oExtiObj = FindObject(0, 0, 0, -1, -1, 0, 0, 0, 0, oExtiObj))
    if(ObjectDistance(oCaller, oExtiObj) > iSearchRadius)
      break;
    else if(OnFire(oExtiObj))
      iExtiCount += DoExtinguish(oExtiObj, oCaller);
    else if (oExtiObj->~MagicExtinguish(oCaller))
      iExtiCount += DoExtinguish(oExtiObj, oCaller);
  // Irgendwas gelöscht?
  if (!iExtiCount) return(0, Message("$NoExtinguish$", oClonk), RemoveObject());
  // Einmaliger Effekt pro Zauber
  Sound("Splash1");
  return(true);
}

private func DoExtinguish(object oExtiObj, object oCaller)
  {
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

private func MaterialLoaded(mat) {
  if (GetMaterialVal("Name", "Material", mat)) return(1);
  else return(0);
}

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("666"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
