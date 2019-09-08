/*-- OceanEater --*/

#strict

private func EatRes() { return(50); }
private func Margin() { return(10); }

protected func Initialize() { SetAction("OceanEater"); }

private func Advancing() { for(var i=0;i<3;++i) Advance(); }

private func Advance() {
  // Out of landscape
  if (GetY()>LandscapeHeight()) 
    return(RemoveObject());
  // Eat this
  if(GBackLiquid(Margin(),Margin()))
    if(GBackLiquid(EatRes()-Margin(),Margin()))
      if(GBackLiquid(Margin(),EatRes()-Margin()))
        if(GBackLiquid(EatRes()-Margin(),EatRes()-Margin())) {
          FreeRect   (GetX(),GetY(),EatRes(),EatRes());
          DigFreeRect(GetX(),GetY(),EatRes(),EatRes());
        }
  // Advance
  if(GetX()+EatRes()<LandscapeWidth())
    return(SetPosition(GetX()+EatRes(),GetY()));
  SetPosition(0,GetY()+EatRes()-Margin());
}