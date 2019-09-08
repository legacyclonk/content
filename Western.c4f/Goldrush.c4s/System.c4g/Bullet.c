/* Kein FriendlyFire für Banditen */

#strict

#appendto CSHO

/* Fliegt */
private func Travel ()
{
  if(GetOwner()>=0) return(_inherited());
  // Bewegung
  if(xdir != 0) // Revolverschuß
  {
    SetXDir(xdir);
    SetYDir(0);
    if(GetActTime() > 20)
      SetYDir(5);
  }
  // Treffer-Überprüfung
  var living;
  if(GetActTime() > 1)
  {
    if (living = FindObject(0, -7, -7, 14, 14, OCF_Living(), 0, 0, NoContainer()))
      if(GetAlive(living) && GetOwner(living)>=0)
        return(HitLiving(living));
    if (living = FindObject(0, 0, 1, 0, 0, OCF_Living(), 0, 0, NoContainer()))
      if(GetAlive(living) && GetOwner(living)>=0)
        return(HitLiving(living));
    // Winchesterprojektil fliegt so schnell, dass es die zwischen Stellen gesondert angeschaut werden müssen,
    // da sonst der Schuss durch Ziele hindurchfliegt
    if(!xdir)
    {
      if (living = FindObject(0, -7-GetXDir()/20, -7-GetYDir()/20, 14, 14, OCF_Living(), 0, 0, NoContainer()))
        if(GetAlive(living) && GetOwner(living)>=0)
          return(HitLiving(living));
      if (living = FindObject(0, -GetXDir()/20, 1-GetYDir()/20, 0, 0, OCF_Living(), 0, 0, NoContainer()))
        if(GetAlive(living) && GetOwner(living)>=0)
          return(HitLiving(living));
    }
  }
}
