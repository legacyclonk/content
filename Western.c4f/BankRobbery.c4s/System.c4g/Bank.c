/*-- Bank --*/

#strict
#appendto BNKB

// Script kommt normalerweise mit dem Teamkonten durcheinander
// auserdem soll ein Mindestbetrag ausgegeben werden
protected func Incineration()
{
  if(burned) return();
  burned = true;
  // Wenn das Bankraubspielziel aktiviert ist, lässt die Bank Geldsäcke fallen
  if(!FindObject(BNKR)) return();
  // Geld der Nicht-Banditen-Spieler zusammenzählen
  var factor = ObjectCount(BANK, 0,0,0,0, OCF_Fullcon())+1;
  var money = GetWealth(GetPlrByTeam(2))/factor;
  // Mindestens 90 Clunker oder ganzen Restbetrag
  if(money<90)
  {
    if(GetWealth(GetPlrByTeam(2))>90) money = 90;
    else money = GetWealth(GetPlrByTeam(2));
  }
  DoWealth(GetPlrByTeam(2), -money);
  // Geld in Form von Säcken rausschmeißen
  var bag;
  while(money > 0)
    {
    bag = CreateObject(MBAG, RandomX(-20,20), 25, -1);
    if(money >= 30) { bag->~SetValue(30); money-=30; }
    else { bag->~SetValue(money); money=0; }
    }
  // Löschen
  AddEffect("Extinguish", this(), 99, 50, 0, SHOB);
  DoDamage(-GetDamage());
}

protected func IncinerationEx() { Incineration(); }
