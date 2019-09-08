/* Tarnzauber */

#strict

public func Activate(caster, real_caster) {
  // Zaubernden Clonk ermitteln
  var clonk = caster;
  if (real_caster) clonk = real_caster;

  // Zaubernder Clonk nicht im Freien -> Abbruch
  if (Contained(clonk)) {
    NoCamouflage(caster, "$must_be_in_free$");
    return(0);
  }

  // Sonst den zaubernden Clonk draußen tarnen
  return(Camouflage(clonk));
}

protected func Camouflage(clonk) {
  Sound("Magic1");
  // Der Zauberer wird in den magischen Stein hineinversetzt
  var container = CreateObject(MRCK, 0,0, GetOwner());
  Enter(container, clonk);
  // Aktivitaet auf Walk zuruecksetzen, wenn attacht (wuerde sonst sofort wieder aus dem Stein rauskommen)
  if(clonk->GetProcedure() S= "ATTACH") clonk->SetAction("Walk");
  // Clonk trägt Pfeil -> Stein wird geschleudert
  var comb_obj;
  comb_obj = FindContents(ARRW, clonk);
  if(!comb_obj) if(FindContents(ARWP, clonk))
	  comb_obj = FindContents(ARWP, clonk)->GetItem();
  if(comb_obj) {
    var dir_sign = -1;
    if (GetDir(clonk) == DIR_Right()) dir_sign = +1;
    SetXDir(dir_sign * 35, container);
    SetYDir(-40, container);
    RemoveObject(comb_obj);
  }
  RemoveObject();
  return(1);
}

protected func NoCamouflage(caster, error_string) {
  Sound("Error");
  Message(error_string, caster);
  RemoveObject();
  return(1);
}

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("166"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
