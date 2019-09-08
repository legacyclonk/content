/* Feuerfaust */
#strict

public func Activate(caster, real_caster)
{
  var clonk = caster;
  if (real_caster) clonk = real_caster;
  
  Sound("Magic1");

  var iChkEff;
  if (iChkEff = CheckEffect("FirefistNSpell", 0, 130)) return(iChkEff!=-1 && RemoveObject());

  var obj, nocrcnt;
  if (InLiquid(clonk) && (obj = FindContents(METL, clonk)))
    {
    var torpedo;
    SetDir(DIR_Left(),  torpedo=CreateObject(TRP1,-15,+10,-1));
    if (torpedo)
      {
      torpedo->Launch(torpedo); // Kleiner Trick damit das Torpedo in die richtige Richtung schwimmt
      torpedo->SetController(GetOwner(clonk)); // Damit eventuelle Tötungen auch gezählt werden
      }
    else ++nocrcnt;
    SetDir(DIR_Right(), torpedo=CreateObject(TRP1,+15,+10,-1));
    if (torpedo)
      {
      torpedo->Launch(torpedo); 
      torpedo->SetController(GetOwner(clonk));
      }
    else ++nocrcnt;
    if (nocrcnt < 2) RemoveObject(obj);
    }
  else
    {
    var firefist=CreateObject(FSHW,-15,0,GetOwner(clonk));
    if (firefist)
      {
      firefist->SetController(GetOwner(clonk));
      ObjectSetAction(firefist,"Left");
      }
    else ++nocrcnt;
    
    firefist=CreateObject(FSHW,+15,0,GetOwner(clonk));
    if (firefist)
      {
      firefist->SetController(GetOwner(clonk));
      ObjectSetAction(firefist,"Right");
      }
    else ++nocrcnt;
  }
  
  RemoveObject();
  return(nocrcnt < 2);
}

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("444"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
