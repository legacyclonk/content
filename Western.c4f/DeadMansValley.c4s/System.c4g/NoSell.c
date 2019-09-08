#strict

#appendto DRST

protected func ContainedDig(object pClonk)
{
  [$SellMenu$]
  Sound("Error");
  PlayerMessage(GetOwner(pClonk),"$MsgNoSell$",this());
}

// Geldgeschäfte sollen nicht durch Vandalismus gestört werden
global func GetBank(object pMessage, bool silent)
{
  return(1);
}
