/* Schadenssystem */

#strict

//Schadens-Konstanten
static const DMG_Melee      = 1;
static const DMG_Projectile = 2;
static const DMG_Fire       = 3;
static const DMG_Explosion  = 4;
static const DMG_Energy     = 5;
static const DMG_Bio        = 6;

global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer)
{
  if(!pTarget)
    if(!(pTarget = this()))
      return(0);
  if(!iPrecision)
    iPrecision = 10;

  var dmg;
  var dmgdealer = dmgplayer-1;
  if(dmgdealer < 0)
  {
    dmgdealer = GetController(this());
    if(!this()) dmgdealer = -1;
  }

  var red = pTarget->~OnDmg(iDmg, iType); //reduction
  // reduction
  dmg = iDmg*(100-red)*iPrecision;
  if(!dmg) return;
  
  // Killer setzen
  if(this() && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
  {
    DebugLog("DoDmg sets killer: %d, owner of %s", "damage", this()->GetOwner(), this()->GetName());
    SetKiller(dmgdealer, pTarget);
  }

  //Schaden machen
  var pFrom = this();
  pTarget->~LastDamageType(iType);
  pTarget->~OnHit(dmg/1000, iType, pFrom);
  
  // testweise: Schadensdings
  //var rgb = RGB(255,0,0);
  //if(dmg/1000 != 0){
  //	if(dmg < 0) rgb = RGB(0,255,0);
  //	CreateObject(CMBT,AbsX(GetX(pTarget)),AbsY(GetY(pTarget)),-1) -> Set(0, Format("%d",dmg/1000), rgb);
  //}
  
  if(GetCategory(pTarget) & C4D_Living)
    return(DoEnergy(-dmg,pTarget, true, 0, dmgdealer + 1));
  return(DoDamage(dmg/1000,pTarget));
}
