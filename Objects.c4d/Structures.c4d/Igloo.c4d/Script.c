#strict

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,0);
}
protected func CheckTemp()  { if(GetTemperature()>10) Melt(); }
protected func Completion() { SetEntrance(1); }
protected func MaxDamage()  { return(50); }

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if (GetDamage() > MaxDamage())
  {
    // Bestandsmaterial schleudern
    CastObjects(ICE1,6,35);
    // Zerstören
    RemoveObject(0,1);
  }
}