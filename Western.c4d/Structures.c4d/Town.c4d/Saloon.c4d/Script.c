/*-- Saloon --*/

#strict
#include BAS9
#include DOOR

/* Saloon-Menü */

protected func OpenSaloonMenu(pClonk)
{
  CreateMenu(0,pClonk);
  AddMenuItem("$Whisky$","Drink", WHKY,pClonk,0,pClonk);
  if (IsNoFightOn()) AddMenuItem("%s","Fight", SMIF,pClonk,0,pClonk);
}

protected func Drink(idObj, pClonk)
{  
  // Bank vorhanden
  if(!GetBank(this())) return();

  // Nicht während eines Kampfes
  if (IsFightOn()) return(Message("$MsgFight$", this()));

  //Kein Geld, kein Whisky
  if(GetWealth(GetOwner(pClonk)) < Value(idObj))
    {
     PlayerMessage(GetOwner(pClonk), "$NoMoney$", pClonk);
     return(0);
    }
  //Whisky erzeugen
  var Whisky = CreateContents(idObj, this());
  Collect(Whisky, pClonk);

  SetOwner(GetOwner(pClonk), Whisky);

  //Geld abziehen
  DoWealth(GetOwner(pClonk), -Value(idObj));
  Sound("UnCash", 1, 0,0, GetOwner(pClonk));
  return(1);
}

protected func Fight()
{
  // Schlägerei anzetteln
  AddEffect("SaloonFight", this(), 50, 15, this());
  return(1);
}

func FxSaloonFightStart(pSaloon, iNumber, fTmp)
{
  if(fTmp) return();
  // Sound in Schleife abspielen
  Sound("SaloonFight", 0, pSaloon, 0, 0, 1);
  Sound("SaloonMusic", 0, pSaloon, 0, 0, 1);
  EffectVar(0, pSaloon, iNumber) = 35*RandomX(45,75);
}

func FxSaloonFightTimer(pSaloon, iNumber, iTime)
{
  // Nur eine bregrenzte Zeit	
  if(iTime>EffectVar(0, pSaloon, iNumber)) return(-1); 
  // Ab und zu fliegt eine Flasche aus dem Fenster
  if(!Random(8))
    {
    var Whisky = CreateContents(WHKY, pSaloon);
    pSaloon->~Exit(Whisky, RandomX(-1,1)*25, RandomX(10,16), Random(360), RandomX(6,-6), RandomX(2,-2), RandomX(6,-6));
    }
  // Einige passende Soundeffekte
  // Manchmal löst sich ganz aus versehen ein Schuss
  if(!Random(10)) { Sound("RevolverShot", 0, pSaloon); Sound("Ricochet*", 0, pSaloon);}
  // Ein Treffer
  if(!Random(6)) { Sound("Hurt*", 0, pSaloon);}
  // Holz getroffen
  if(!Random(6)) { Sound("WoodHit*", 0, pSaloon);}
  // Ein Glas verschüttet
  if(!Random(6)) { Sound("Splash1", 0, pSaloon);}
  var obj;
  // Manchmal bekommt man einen Schlag ab
  while(obj = FindObject(0, 0, 0, 0, 0, OCF_Alive, 0, 0, pSaloon, obj)) if(!Random(4)) Punch(obj, RandomX(1,3));
}

func FxSaloonFightStop(pSaloon, iNumber, iReason, fTmp)
{
  if(fTmp) return();	
  // Schleife abbrechen
  Sound("SaloonFight", 0, pSaloon, 0, 0, -1);
  Sound("SaloonMusic", 0, pSaloon, 0, 0, -1);
}

func IsFightOn()
{
  if (GetEffect("SaloonFight", this())) return(1);
  return(0);
}

func IsNoFightOn()
{
  return(!IsFightOn());
}

/* Türsteuerung */

public func ActivateEntrance()
{
  if(ActIdle() || GetAction() eq "CloseDoor") SetAction("OpenDoor");
  return(1);
}

/* Steuerung */

protected func ContainedUp(pClonk)
{
  OpenSaloonMenu(pClonk);
}
  
protected func ContextFight(pClonk)
{
  [$StartFight$|Image=SMIF|Condition=IsNoFightOn]
  if (RequireContainedCall("ContextFight", pClonk)) return;
  Fight(pClonk);
}

protected func ContextDrink(pClonk)
{
  [$Whisky$|Image=WHKY]
  if (RequireContainedCall("ContextDrink", pClonk)) return;
  Drink(WHKY, pClonk);
}

/* Hilfsfunktionen */

private func RequireContainedCall(string strFunction, object pClonk)
{
  // Clonk befindet sich im Gebäude
  if (pClonk->Contained() == this()) return(0);
  // Clonk soll erst ins Gebäude laufen, dann den Befehl aufrufen
  SetCommand(pClonk, "Call", this(), pClonk, 0, 0, strFunction);
  AddCommand(pClonk, "Enter", this());
  return(1);
}
