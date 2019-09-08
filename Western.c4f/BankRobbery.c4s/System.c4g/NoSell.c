#strict

#appendto DRST

protected func ContainedDig(object pClonk)
{
  [$SellMenu$]
  Sound("Error");
  PlayerMessage(GetOwner(pClonk),"$MsgNoSell$",this());
}
