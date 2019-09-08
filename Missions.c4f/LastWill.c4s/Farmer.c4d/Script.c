
#strict

protected func Hit()
{
  if(GetAction() ne "RumbleFall") return();
  var cnt = 21;
  while(cnt--) Smoke(Random(31)-15,Random(36)-30,Random(61));
  ShakeFree(GetX(),GetY(),30);
  Sound("Earthquake");Sound("DeathGrowl");
  RemoveObject();
  return(1);
}
protected func NeutralRotate() { return(SetR()); }
protected func Ghost()
{
  if(LessThan(GetY(),10))
  {
    CastObjects(SPRK,20,30);
    Sound("Magic1");
    SetAction("SeaSound");
    SetPosition(3600,240);
    return();
  }
  SetPosition(GetX(),GetY() - 3);
  return(SetR());
}
protected func LiquidCheck()
{
  if(fShowdown) return();
  if(GBackLiquid() && GetX() > 3600) Finale();
  return(1);
}
protected func Finale()
{
  SetR();SetRDir();SetXDir();SetYDir();
  SetAction("WithoutFarmer");
  ObjectSetAction(pFarmer = CreateObject(_LEI),"GhostMove");
  GameCall("End");
  Sound("Magic1");
  return(1);
}