/*-- Kajak --*/

/* Einstieg */

#strict

protected func Initialize() { SetAction("Stop"); }

protected func ControlUpDouble(pClonk) {
  [Einsteigen]
  //nur einsteigen, wenn er nicht rotiert
  if(GetR()) return();
  // Clonk geht ins Kajak
  SetCommand(pClonk,"Enter",this());
}

protected func ActivateEntrance(pClonk) {
  // Nur Inuks
  if (GetID(pClonk)!=INUK) return(0);
  // Clonk ist noch zu klein
  if (GetCon(pClonk)<100)
    return(Message("$CantRide$",pClonk,GetName(pClonk)));
  // Einsteigen
  Enter(this(),pClonk);
  // Verwandeln
  ChangeDef(KAJO);
  SetAction("Stop");
  SetDir(GetDir(pClonk));
  // Anorak?
  if(pClonk->INUK::IsUndressed()) SetGraphics("Undressed");
  else SetGraphics();
  // Farbe
  SetColorDw(GetColorDw(pClonk));  
  return(1);
}
  
/* Aufrichten */

private func FloatUpright() {
  if (!InLiquid()) return(0);
  if (Inside(GetR(),-3,+3) || Inside(GetR(),357,363)) return(SetR(0),SetRDir(0));
  if (Inside(GetR(),2,180)) SetRDir(-3);
  if (Inside(GetR(),180,355) || Inside(GetR(),-180,-5)) SetRDir(+3);
}

/* Automatisches Ausleeren in Gebäuden */

protected func Entrance(object pNewContainer)
  {
  // Nur in Gebäuden (auch Burgteile)
  if (GetCategory(pNewContainer) & (C4D_StaticBack | C4D_Structure))
    // Nicht, wenn es das Gebäude verbietet (verwendet der Einfachheit halber auch den Lorrycallback)
    if (!pNewContainer->~NoLorryEjection(this()) && !pNewContainer->~IsStaircase())
      {
      // Kajak entleeren
      pNewContainer->GrabContents(this());
      }
  SetR(0);
  SetRDir(0);
  }
