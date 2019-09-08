/*-- Zelt --*/

#strict

/* Initialisierung */

public func Initialize()
{
  // Eingang aufmachen
  SetEntrance(1);
}

/* Aufbau */

public func BeginCon()
{
  // Zelt wird aufgebaut: Nicht betretbar
  SetEntrance(0);
  // Animation & Effekt setzen
  SetAction("Con");
  AddEffect("IntCon", this(), 1, 1, this() );
}

protected func FxIntConTimer(object target, int number)
{
  // Arbeiter suchen
  var workers = ObjectCount(0, 0,0,0,0, OCF_CrewMember(), "Push", target);
  // Keine Arbeiter: wieder zusammenfallen
  if(!workers && GetActTime() > 5)
  {
    Pack();
    return(-1);
  }
  // Maximal drei Arbeiter
  if(workers > 3) workers = 3;
  // Aufbauen
  var iLastPhase = GetPhase();
  EffectVar(0, target, number) += workers;
  if (EffectVar(0, target, number) > 450) // Türöffnen geht schnell
    EffectVar(0, target, number) += 20;
  SetPhase(EffectVar(0, target, number) * 20 / (GetMass() * 3 / 2) );
  // Geräusch  
  if (iLastPhase != GetPhase())
    if (EffectVar(0, target, number) < 450)
      Sound("TentPacking*");
  // Fertig
  if(EffectVar(0, target, number) >= GetMass() * 3 / 2)
  {
    ConComplete();
    return(-1);
  }
}

/* Aufbau fertig */

private func ConComplete()
{
  // Eingang oeffnen
  SetEntrance(1);
  // Feedback fuer Spieler: Ist der Bau fertig laesst der Clonk los
  var obj;
  while(obj = FindObject(0, 0,0,0,0, OCF_CrewMember(), "Push", this(), 0, obj) )
    AddCommand(obj, "UnGrab");
  SetAction("Idle");
}

/* Kann man das Zelt einpacken? */

public func CanPack()
{
  // Inhalt prüfen
  while(Contents())
  {
    // Tote Clonks rauswerfen
    if (GetOCF(Contents()) & OCF_Living() && !GetAlive(Contents()))
      Exit(Contents());
    // Sonst nicht
    else
      return(false);
  }
  return(true);
}

/* Zelt einpacken */

public func Pack()
{
  
  // Geht es ueberhaupt?
  if(!CanPack() )
  {
    // Meldung + Sound
    Message("$MsgTentMustBeEmpty$", this());
    Sound("Error");
    return(false);
  }
  
  // Eingang dicht machen
  SetEntrance(0);
  
  // Attachte Objekte (Flaggen, etc.) loswerden
  var obj;
  while(obj = FindObject(0, 0,0,0,0, 0, 0, this(), 0, obj) )
    if(obj->GetProcedure() S= "ATTACH")
      obj->SetAction("Idle");
  
  // Zu setzende Phase bestimmen, falls das Zelt
  // mitten im Aufbau wieder eingepackt werden muss
  var phase = 0;
  if(GetAction() S= "Con") phase = 19 - GetPhase();
  
  // Action aendern
  SetAction("Pack");
  SetPhase(phase);
  
  // Akustik
  Sound("TentPack");
}

/* Einpacken fertig */

private func PackComplete()
{
  // Zeltpaket erzeugen
  var iyOut; if (!GetR()) iyOut = GetCon()/5;
  CreateObject(TENP, 0, iyOut, GetOwner());
  // Zelt loeschen
  RemoveObject();
}

/* Steuerung: Einpacken */

protected func ControlDigDouble()
{
  [$CtrlPack$]  
  // Kann nicht während des Auf- oder Abbaus eingepackt werden
  if (!(GetAction() S= "Idle"))
    return(false);
  // Wieder einpacken
  Pack();
  return(1);
}

protected func SellTo() { return(TENP); }
