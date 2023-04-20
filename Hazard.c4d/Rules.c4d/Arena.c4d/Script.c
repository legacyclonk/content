/*-- Arena --*/

#strict 2

protected func Activate(player) {
  MessageWindow(GetDesc(),player);
}

/* Zeugs, das herumliegt, soll verschwinden */

protected func Timer()
{
  // Zeugs suchen
  for(var stuff in FindObjects(Find_Category(C4D_Object),
                               Find_NoContainer(),
                               Find_Or(Find_Func("IsWeapon"),
                                       Find_Func("IsEquipment"),
                                       Find_Func("IsUpgrade")),
                               Find_Not(Find_Func("IsMine")),
                               Find_Not(Find_Func("NoArenaRemove")))
     )
  {
    var fadeoutTime = stuff->~GetArenaRemoveTime();
    if(!fadeoutTime) fadeoutTime = GetDefaultArenaRemoveTime();
    AddEffect("Arena_Remove", stuff, 1, fadeoutTime, this);
  }
}

public func GetDefaultArenaRemoveTime()
{
  return 170;
}

public func FxArena_RemoveTimer(object target)
{
  //Abbruch bei neuer Aktion (Drohne)
  if(!target->ActIdle())
    return -1;
  //Oder wenn aufgehoben
  if(target->Contained())
  {
    return -1;
  }

  target->RemoveObject();
  return -1;
}

public func FxArena_RemoveEffect(string szNewEffectName)
{
  // Nochmal entfernen brauchen wir nicht
  if(szNewEffectName == "Arena_Remove")
    return -1;
}

/* Zeugs aus dem Inventar soll beim Tod verschwinden */

public func OnClonkRecruitment(object pClonk) {
  // Effekt erzeugen
  AddEffect("RemoveInvOnDeath", pClonk, 99, 0, this());
}

public func FxRemoveInvOnDeathStop(target, no, reason) {
  if(reason == 4)
    {
    for(var i = 0 ; Contents(i, target) ; i++)
      if(Contents(i, target)->~IsWeapon() || Contents(i, target)->~IsEquipment())
        Schedule("RemoveObject();", 1, 0, Contents(i, target));
    for(var pGear in LocalN("aGear", target))
      if(pGear)
      {
        pGear->GearUnbuckle(target);
        pGear->RemoveObject();
      }
    }
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
