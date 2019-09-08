/*-- Konto --*/

#strict
#appendto ACNT

/* Initialisierung */

protected func Initialize()
{
  // Anfangsgeldwert
  value=GetWealth(GetOwner());
  if(Local(CheckTeam(GetOwner()), FindObject(TACC))) value = Local(CheckTeam(GetOwner()), FindObject(TACC));
  else Local(CheckTeam(GetOwner()), FindObject(TACC)) = value;
  SetWealth(GetOwner(), value);
}

/* Überprüfung */

private func Execute()
{
  // Gehört niemandem (Spieler eliminiert?) - löschen
  if (GetOwner() < 0) return(RemoveObject() );

  // Geldwertänderung?
  if (GetWealth(GetOwner()) != value)
  {
    // Änderung berechnen und anrechenen
    var d=GetWealth(GetOwner())-value;
    value+=d;
    // Änderung auf verbündete Konten anrechnen
    Local(CheckTeam(GetOwner()), FindObject(TACC)) += d;
    var obj;
    while(obj = FindObject(ACNT, 0, 0, 0, 0, 0, 0, 0,0, obj)) if(obj!=this()) if(CheckTeam(GetOwner(obj))==CheckTeam(GetOwner())) obj->UpdateValue(d);
  }
}

/* Teamänderung */

public func TeamChanged()
{
  Initialize();    	
}
