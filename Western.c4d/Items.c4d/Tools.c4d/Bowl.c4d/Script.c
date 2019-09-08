/*-- Schale --*/

#strict 2

local iWater;

protected func Initialize()
{
  // Damit es sich nicht von anderen Schalen unterscheidet (sie nicht einzeln im Menü gezeigt werden)
  SetColorDw();
  return(1);
}

protected func Hit()
{
  if(GetAction() == "Potion") {
    RemoveEffect("Potion", this());
    var iColor = GetColorDw();
    if(!iColor) iColor = RGB(0,0,255);
    CastParticles("LiquidSpark",RandomX(30,50),30,0,0,10, 10,iColor,iColor);
    SetAction("Idle");
    SetGraphics("");
    SetColorDw();
    Sound("Splash*");
  }
  if(GetAction() == "Water") {
    LocalN("iWater", this()) = 0;
    CastPXS("Water",30,30);
    SetAction("Idle");
    SetGraphics("");
    SetColorDw();
    Sound("Splash*");
  }
  Sound("WoodHit*"); 
}

/* TimerCall */

private func FillCheck()
{
  if(GetAction() != "Idle") return(1);
  if(GetMaterial() == Material("Water")) {
    iWater += ExtractMaterialAmount(0,0,Material("Water"),30-iWater);
    if(iWater<30) return(0);
    SetAction("Water");
    SetGraphics("Full");
//    SetColorDw(RGB(0,0,255));
    SetColorDw(RGB(0,40,150));
  }
}

public func Activate(object pClonk) { return(Drink(pClonk)); }

public func Drink(pClonk)
{
  if(GetAction() == "Idle") return(0);
  if(GetAction() == "Potion") {
    if(GetEffect("ActivePotion",pClonk))
      RemoveEffect("Potion",this());
    else
      EffectCall(this(),GetEffect("Potion",this()),"Activate",pClonk);
  }
  pClonk->~Drink(this(), 100);
  ObjectSetAction(pClonk,"DrinkBowl");
  iWater = 0;
  SetAction("Idle");
  SetGraphics("");
  SetColorDw();
  Sound("Drink");
  return(1);
}

public func GetDrinkValue()
{
  if(GetAction() != "Idle") return(100);
  return(0);
}

public func OnOwnerChanged(int iNewOwner, int iOldOwner)
{
  // Farbwert nochmal anpassen
  if(GetAction() == "Water")
    SetColorDw(RGB(0,40,150));
  else if(GetAction() == "Potion")
    SetColorDw(EffectCall(this(),GetEffect("Potion",this()),"GetColor"));
  // Damit es sich leer nicht von anderen unterscheidet
  else SetColorDw();
}

public func PotionFilled(object pCauldron)
{
  SetAction("Potion");
  SetGraphics("Full");
  SetColorDw(EffectCall(this(),GetEffect("Potion",this()),"GetColor"));
}

public func IsPotionContainer() { return(GetAction() == "Idle"); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }