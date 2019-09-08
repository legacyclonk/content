/*-- Monster --*/

#strict
#appendto MONS

// Nicht mit Doppelgraben den Effekt entfernen
public func ContainedDigDouble(pClonk)
{
  return(1);
}

// Aus dem Wasser springen
protected func ContainedUpDouble() 
{
  // nur wenn an Meeresoberfläche
  if(!InLiquid()) return(0);
  if(GBackSemiSolid(0,-1)) return(0);
  // herausspringen
  SetPosition(GetX(),GetY()-2);
  SetAction("Jump");
  SetSpeed(GetXDir(),-38);
}

protected func ContainedDig(object caller)
{
 SetCommand(this(), "None");

 if (GetAction() eq "Walk")
  SetAction("LookUp");
  
 return(1);
}

protected func ContainedDigDouble(object caller)
{
 if(GetID(Contents(0,caller))==1346983760) { RemoveObject(Contents(0,caller)); Sound("Drink"); ChangeDef(CastC4ID(1179666775)); SetAction("Walk"); }
  
 return(1);
}

protected func ContainedDown(object caller)
{
 SetCommand(this(), "None");

 if (GetAction() eq "Swim")
 {
  if(!GetPlrJumpAndRunControl(caller->GetController()))
   SetComDir(COMD_Down());
  return(1);
 }

 if (GetAction() eq "Walk")
  SetComDir(COMD_Stop());
  
 return(1);
}

