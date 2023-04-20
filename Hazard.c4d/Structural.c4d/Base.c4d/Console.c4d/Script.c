/*-- Konsole --*/

#strict

local smashed, target;

// Effekte
protected func Lights()
{
  if(smashed || GetEffect("EMPShock", this()))
    {
    if(!Random(10)) CastObjects(SPRK, 1, 30, RandomX(-15,15));
    }
  else
    {
    if(!Random(8)) CreateParticle("NoGravSpark", 3, 1, 0, 0, 50, RGBa(250, 10, 10, 50));
    if(!Random(8)) CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 10, 250, 50));
    }
}

// Bumm :o
protected func Damage()
{
  if(smashed) return(CastObjects(SPRK, 1, 30, RandomX(-15,15)));
  if(GetDamage() >= 40)
    {
    CastObjects(SPRK, 4, 50);
    Sparks(15, RGB(210, 210, 0));
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    Sound("Blast1");
    Sound("CrystalHit*");
    SetGraphics("Broken");
    smashed = true;
    }
}

/* Bedienung */

protected func ControlDig(pClonk)
{
  [$Control$]
  if(smashed || GetEffect("EMPShock", this()))
    {
    Sound("Electric");
    Fling(pClonk, RandomX(-2,2), -2);
    return();
    }
    
  if(!target) return(Sound("CatapultSet"));

  CreateMenu(GetID(target), pClonk, target, 0, Format("$Control$: %s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i) ; i++)
  {
    //Aufrufenden Clonk und uns selbst dem Zielobjekt übergeben. Dies kann es in ConsoleController abrufen
//    target->~ConsoleController(this(), pClonk);
    // Zielobjekt muss für ConsoleControl(i) jeweils den Namen der Aktion zurückgeben, wichtig: i beginnt bei 1
    AddMenuItem(desc, Format("ConsoleControlled(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this())), GetID(), pClonk, 0, 0, "$Control$");
    // Zielobjekt bekommt Aufruf: ConsoleControll(i) und muss das dann verarbeiten
  }
  return(1);
}

//EMP-Angriff
public func EMPShock()
{
  //Nock Schockbar?
  if(GetEffect("EMPShock", this()) || smashed) return(0);
  
  //Ansonsten knallt's
  AddEffect("EMPShock", this(), 180, 650, this(), GetID());
}

// ist ne Steuereinheit!
public func IsConsole() { return(1); }
public func IsMachine() { return(1); }

public func Set(pTarget)
{
  target = pTarget;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[GetLength(extra)] = ["Set(Object(%d))", target];
	if (smashed)
		extra[GetLength(extra)] = "LocalN(\"smashed\")=true";
}
