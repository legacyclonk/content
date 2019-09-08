/*-- Verbranntes Zauberkastell --*/

#strict

#include WTRB

local entr;

/* Niedergang bei Zerstoerung oder Brand loeschen */

protected func Destruction()
{
         if(entr) entr->RemoveObject();
  return(_inherited() );
}

protected func Incineration()
{
  if(entr) entr->RemoveObject();
  return(_inherited() );
}

protected func IncinerationEx()
{
  if(entr) entr->RemoveObject();
  return(_inherited() );
}

