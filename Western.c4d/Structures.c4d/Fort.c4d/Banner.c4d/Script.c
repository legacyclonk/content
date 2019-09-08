/*-- Fahne --*/

#strict

protected func Initialize()
{
  SetAction("HangUp");
  return(1);
}

/* TimerCall */

private func Wind()
{
    //Richtung der Windgeschwindigkeit anpassen
    if(GetAction() S= "Fly") SetDir(BoundBy(GetWind() / 15 + 3, 0, 6));
}

/* Steuerung */

protected func ControlUp()
{
    //Fahne ist schon oben?
    if(GetAction() S= "Fly" || GetAction() S= "HangUp") return(0);
    //Häng die Fahne unten oder geht sie erst runter?
    if(GetAction() S= "HangDown")
    {
        if(GetPhase() < 4) return(0);
    }
    //Richtung Setzten
    SetDir(0);
    SetAction("HangUp");
    return(1);
}

protected func ControlDownSingle()
{
    //Fahne ist schon oben?
    if(GetAction() S= "HangUp" || GetAction() S= "HangDown") return(0);
    //Richtung setzten
    SetDir(0);
    SetAction("HangDown");
    return(1);
}

