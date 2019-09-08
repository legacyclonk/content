/*-- Whisky --*/

#strict

private func Initialize()
{
  SetAction("Full");
}

public func Activate(pClonk)
{
  [$Drink$|Condition=IsFull]
  if (GetAction(0) eq "Full")
    Drink(pClonk);
  else
    Message("$BtlEmpty$", Contained(this()));
 return(1);
}

public func IsFull() { return(GetAction(0) eq "Full"); }

public func Drink(pClonk)
{
   if(!(GetAction() S= "Full")) return();
   if(GetEffect("Potion", this()))
     if(GetEffect("ActivePotion",pClonk)) RemoveEffect("Potion", this());
     else EffectCall(this(), GetEffect("Potion", this()), "Activate", pClonk);
   else
   {
     DoEnergy(GetPhysical("Energy", 0, Contained(this()))*100/100000, Contained(this()));
     DoAlcohol(Contained(this()),1000);
// Eine Funktion im Clonk namens "Eat" gibt es nicht.
// Bestimmt wollte man "Feed" nehmen aber ganz sicher bin ich mir nicht...
// Doch wenn es schonmal als "Eat" vorhanden ist so sollte es bestimmt einen Zweck erfüllen
//     Contained()->~Feed(60);
// Damit kein Ess-Geräusch kommt
//     if(ObjectCount(NDFO)) LocalN("iHunger",pClonk) = BoundBy(LocalN("iHunger",pClonk)-iValue*60-500+5*GetCon(),0,MaximumHunger());
// Da ich mir aber noch nicht ganz sicher bin...
   }
   Contained()->~Drink(this, 70);
   SetAction("Empty");
   SetGraphics("Empty");
   SetName("$NameEmpty$");
   Sound("Drink");
}

private func Hit()
{
  Split2Components();
}

public func PotionFilled()
{
  SetAction("Full");
  SetName(GetName(0,GetID(this())));
  SetGraphics("");
}

public func IsPotionContainer() { return(GetAction()S="Empty"); }

// Damit die KI dies als Getränk ansieht
public func GetDrinkValue() { if(IsFull()) return(70); }
// Da ich mir aber noch nicht ganz sicher bin...
//public func GetFeedValue()  { if(IsFull() && (!GetEffect("Potion",this()))) return(60); }

/* Wert */

public func CalcValue(object pInBase, int iForPlayer)
{
  if(GetAction() eq "Full") return(GetValue(0,GetID(),pInBase,iForPlayer));
  // Pfand gibbet nicht
  return();
}

/* Verkauf */

public func SellTo()
{
  // Leere Flaschen verschwinden
  if(GetAction() ne "Full") return();
  return(GetID());
}
