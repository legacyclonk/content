/*-- Bodenlucke --*/

#strict

#include GBDR

public func Initialize() 
{
   SetAction("Closed");
   _inherited();
   SetMaxDamage(400);
}

public func OnDmg(int iDmg, int iType)
{
  // diese Tür ist besonders dick
  if(iType == DMG_Projectile)	return(60);
  if(iType == DMG_Melee)	return(80);
  if(iType == DMG_Energy)	return(30);
  if(iType == DMG_Explosion)	return(0);

  return(100);
}

public func OnOpen()
{
  if(GetAction() eq "Open" || GetAction() eq "Opened") return();
  SetAction("Open");
  Sound("Airlock1");
}

public func OnClose()
{
  if(GetAction() eq "Closed" || GetAction() eq "Close") return();
  SetAction("Close");
  Sound("Airlock2");
  SetSolidMask(0, 0, 20, 20,3);
}

public func SetPathFree()
{
  SetSolidMask();
}

public func OnDestroyed(iType)
{
    //Kleinen Explosionseffekt
    CreateObject(ROCK)->Explode(20);
    SetAction("Destroyed");
}

private func SomeonesApproaching()// such ankommene Clonks
{
  var aClonks = CreateArray();

  // Suchen wir mal rum.
  aClonks = FindObjects(Find_InRect(-(GetObjWidth()/2),0,GetObjWidth(),20),
 		        Find_NoContainer(),
 		        Find_OCF(OCF_Alive),
 		        Find_Not(Find_Func("IsAlien")) );//Das mit dem Alien ändert sich später noch. -> Hazard 2.0

  if(!closed)
  {
    if(!GetLength(aClonks))
    {
      aClonks = FindObjects(Find_InRect(-(GetObjWidth()/2),-20,GetObjWidth(),20),
   		          Find_NoContainer(),
 		            Find_OCF(OCF_Alive),
 		            Find_Not(Find_Func("IsAlien")) );
    }
  }

  // Irgendwas gefunden?
  if(GetLength(aClonks) > 0)
    return(true);
  return(false);
}

public func ControlUp(object pByObj)
{
  if(GetAction() ne "Destroyed")
    if(!lock)
      OnOpen();
  return(1);
}

public func IsBulletTarget(id idBullet){return(false);}//kann nur von explosionen schaden bekommen

//KI
public func UpdateTransferZone()
{
  //SetTransferZone(-15,-GetObjWidth()/2,30,GetObjWidth());
  SetTransferZone(-GetObjWidth()/2,-GetObjHeight()/2,GetObjWidth(),GetObjHeight());
}

public func ControlTransfer(object obj, int x, int y)
{
  if(lock && closed)
    return(false);
  
  var dir = 1;
  if(obj->GetY() < GetY()+GetObjHeight())
    dir = -1;
  
  if(!closed) return(false);
  
  if(dir == -1)
  {
    if(GetProcedure(obj) ne "PUSH")
    {
      if(GetCommand(obj) ne "Grab")
        return(AddCommand(obj,"Grab",this(),0,-10));
    }
    else
    {
      ControlUp(obj);
        return(true);
    }
  }
  else
  {
    return(AddCommand(obj,"MoveTo",0,GetX(),GetY()+10));
  }
}


