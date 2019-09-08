/*-- Neues Script --*/

#strict

#appendto XARW

//bisle heruntergedreht, da nun öfter zu haben
protected func Hit()
{
  if (Armed)
    Explode(Random(4) + 6);
}
