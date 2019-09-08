/*-- Verbrannte Bank --*/

#strict

#include BAS9

local burned;

protected func Incineration()
{
  if(burned) return();
  burned = true;
  // Wenn das Bankraubspielziel aktiviert ist, lässt die Bank Geldsäcke fallen
  if(!FindObject(BNKR)) return();
  // Geld der Nicht-Banditen-Spieler zusammenzählen
  var factor = ObjectCount(BANK, 0,0,0,0, OCF_Fullcon())+1;
  for(var i=0, money, pmoney ; i < GetPlayerCount() ; ++i)
    if(GetPlayerByIndex(i) > -1)
      if(!IsBanditPlayer(i))
        {
        pmoney = GetWealth(i) / factor;
        money += pmoney;
        SetWealth(i, GetWealth(i) - pmoney);
        }
  // Geld in Form von Säcken rausschmeißen
  var bag;
  while(money > 0)
    {
    bag = CreateObject(MBAG, RandomX(-20,20), 25, -1);
    if(money >= 50) { bag->~SetValue(50); money-=50; }
    else { bag->~SetValue(money); money=0; }
    }
  // Löschen
  AddEffect("Extinguish", this(), 99, 50, 0, SHOB);
}

protected func IncinerationEx() { Incineration(); }
