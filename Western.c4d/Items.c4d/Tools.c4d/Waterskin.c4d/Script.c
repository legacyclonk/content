/* Trinkschlauch */

#strict

local iWater, iPotion;

public func Initialize() {
  SetColorDw();
}

protected func Hit() 
{
  Sound("WoodHit*"); 
}

/* TimerCall */

private func FillCheck()
{
  if(iPotion==1) return();
  if(iWater>=90) return();
  if(GetMaterial()==Material("Water"))
  {
    iWater += ExtractMaterialAmount(0,0,Material("Water"), 90-iWater);
    UpdateGraphics();
  }
}

private func UpdateGraphics()
{
  if(iPotion==1) return();
  if(iWater<30) SetGraphics("");
  else SetGraphics(Format("%d", BoundBy(iWater/30, 1, 3)));
  SetColorDw(RGB(0,0,0));
}

public func Activate(pClonk)
{
  if((iWater>=30) || (iPotion==1))
  {
   Drink(pClonk);
  }
 return(1);
}

public func Drink(pClonk)
{
   if(iPotion==1) {
     if(GetEffect("ActivePotion",pClonk))
       RemoveEffect("Potion", this());
     else
       EffectCall(this(), GetEffect("Potion", this()), "Activate", pClonk);
     Contained()->~Drink(this, 100);
     ObjectSetAction(pClonk,"DrinkWaterskin");
     iPotion = 0;
     UpdateGraphics();
     Sound("Drink");
     return();
   }
   if(iWater<30) return();
   Contained()->~Drink(this, 100);
   ObjectSetAction(pClonk,"DrinkWaterskin");
   iWater -= 30;
   UpdateGraphics();
   Sound("Drink");
}

public func GetDrinkValue()
{
  if( iWater >= 30 ) return(100);
  if( iPotion==  1 ) return(100);
  return(0);
}

// Hier kommt die Trank-Erweiterung:
public func PotionFilled(pCauldron)
{
  iPotion = 1;
  iWater  = 0;
  SetGraphics("Potion");
  var ef = GetEffect("Potion", this());
  SetColorDw(EffectCall(this(), ef, "GetColor"));
}

// Und die Funktion, den Trinkschlauch zu entleeren
protected func ContextEmptyWSKI(pCaller)
{
  [$TxtEject$|Image=WSKI|Condition=GetDrinkValue]
  EmptyWSKI(pCaller);
}

public func EmptyWSKI(pCaller)
{
  if(!GetDrinkValue()) return(0);
  if(iPotion==1) {
    if(GetEffect("Potion", this())) RemoveEffect("Potion", this());
    var iColor = GetColorDw();
    if(!iColor) iColor = RGB(0,0,255);
    CastParticles ("LiquidSpark",RandomX(30,50),20,0,0,10, 10,iColor,iColor);
    iPotion = 0;
    UpdateGraphics();
    Sound("Splash*");
  }
  else {
    CastPXS("Water", iWater, 20);
    iWater = 0;
    UpdateGraphics();
    Sound("Splash*");
  }
  return(1);
}

public func IsPotionContainer() { return((iPotion != 1) && (iWater < 30)); }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }