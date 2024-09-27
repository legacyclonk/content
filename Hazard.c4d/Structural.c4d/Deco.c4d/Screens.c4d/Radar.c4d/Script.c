/*-- Radarschirm --*/

#strict

func Initialize() {
  SetAction("Animate");
  SetClrModulation(RGB(0,255,0));

  AddFrame();
  Local(0) = AddLight(75,-1); //Licht nur sicherheitshalber speichern, für Änderungen.
  //falls die Farbe per Script erst nacher wo gesetzt wird
  ScheduleCall(Local(0),"Init",1,0,0,-1);
}
