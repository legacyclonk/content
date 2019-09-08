/*-- Toter Fisch --*/

#strict

protected func Construction() 
{
  var pEnv;
  if (pEnv=FindObject(CLFS))
    pEnv->CLFS::Colorize(this());
  else
    SetColorDw(RGB(255,255,255));
}

private func Decaying() 
{ 
  DoCon(-4); 
}

public func Eat(object pByObject)
{
  pByObject->~Feed(50*GetCon()/100);
  RemoveObject();
  return(1);
}

public func Cook()
{
  // Verwesende Fische können nicht mehr gekocht werden
  if (GetAction() S= "Decay") return(0);
  // Kochen
  ChangeDef(CFSH);
  return(1);
}

/* Aufwertungszauberkombo: Mit Fisch wird der Clonk zum Aquaclonk */
public func GetRevaluationCombo(object pClonk) {
  if (GetAction() S= "Decay") return(0);
  return(ACLK);
}

/* Zerlegen */

public func Activate(pClonk) {
  [$TxtEmbowel$|Image=KNFE]
  // Aquaclonks (und damit Hydroclonks) können Fische direkt essen
  if (pClonk->~IsAquaClonk()) return(Eat(pClonk));
  // Der Clonk soll dieses Objekt zerlegen, wenn er kann
  return(ObjectSetAction(pClonk,"Embowel",this()));
}

/* Wird zerlegt */

public func Embowel(object pClonk) {
  // In Bestandteile zerfallen
  if (pClonk) pClonk->Split2Components(this()); else Split2Components(this());
  return(1);
}

protected func GetCustomComponents(object pClonk)
  {
  if (pClonk)
    {
    // Jedem seine Extrawürste
    if (pClonk->~IsTrapper() || pClonk->~IsIndian()) return ([FSHM, FSHM, FSHB]);
    if (pClonk->~IsInuk()) return([MEAT, FAT1, FSHB]);
    //if (pClonk->~IsJungleClonk()) return([MEAT, FAT1, FSHB]); // Jungelclonk kann (noch) nicht zerlegen
    }
  }
