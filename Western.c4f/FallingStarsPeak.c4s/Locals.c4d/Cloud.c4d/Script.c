/*-- Wolke --*/

local fFadingOut, fRaining;

#strict

public func ActivateRain()
  {
  // Vorne oder hinten
  if (Random(10)) SetCategory(C4D_Background); else SetCategory(C4D_Foreground);
  // Zufallsussehen
  SetAction(Format("Cloud%d", Random(2)+1));
  SetPhase(Random(2));
  DoCon(Random(500)+500);
  SetMass(1); // Um Platschen zu Verhindern
  SetDir(Random(2));
  fRaining = true;
  // Einfaden
  var iMod = BoundBy(255 - GetY() / 3, 0, 255);
  SetClrModulation(RGBa(iMod, iMod, iMod, 255));
  FadeIn(1, 4);
  // Startgeschwindigkeit
  //SetXDir(g_CD69_xdir+Random(5)-2);
  SetXDir(0);
  // Startdrehung - Drehung 0 hat(te?) Clippingfehler in der Engine
  var iR = Random(61)-30; if (!iR) ++iR;
  SetR(iR);
  // k
  return (true);
  }

static g_CD69_xdir,g_CD69_ydir,g_CD69_iCalcFrame;

protected func Timer()
  {
  if (fFadingOut) return();
  // Globale Wolkenbewegung
  if (g_CD69_iCalcFrame != FrameCounter())
    {
    g_CD69_iCalcFrame = FrameCounter();
    var wind = GetWind(0,0,true)/4;
    g_CD69_xdir = BoundBy(g_CD69_xdir+Random(5)-2, wind-10, wind+10);
    g_CD69_ydir = BoundBy(g_CD69_ydir+Random(5)-2, -5,5);
    }
  // Noch nicht eingefadet?
  if (GetClrModulation()>>24) return();
  // Rand erreicht?
  if (GetXDir()>=0 && GetX()>=LandscapeWidth()-50) return(DoFadeout());
  if (GetXDir()<=0 && GetX()<=50) return(DoFadeout());
  if (GetY()<0) return(DoFadeout());
  // Solid: Aufwärts
  if (GBackSolid()) return(SetXDir(GetXDir()*2/3), SetYDir(g_CD69_ydir-10+Random(5)-2));
  // Sonst nach Wind
  SetXDir(g_CD69_xdir+Random(5)-2);
  SetYDir(g_CD69_ydir+Random(5)-2);
  // Wenn frei, dann regnen
  if (!fRaining) return();
  return();
  var sz = GetCon()/2, i = Random(10);
  while (i--)
    {
    var x = Random(sz+1) - sz/2, y = Random(sz/2);
    if (GetMaterial(x,y)>=0) continue; // Nur auf Sky
    InsertMaterial(Material("Water2"), x, y, GetXDir(), GetYDir());
    }
  // Zum Regen gehört auch Gewitter!
  if (!Random(500)) DoThunder();
  }
  
public func DoThunder()
  {
  Sound("Thunder*");
  var o,l = 10+Random(26);
  while (o = FindObject(GetID(), 0,-1, 0,0, 0, 0,0, NoContainer(), o))
    AddEffect("IntThunder", o, 250, l, o);
  return(AddEffect("IntThunder", this(), 250, l, this()));
  }
  
protected func FxIntThunderStart(object pTarget, int iEff, int iTemp)
  {
  var dwMod = EffectVar(0, pTarget, iEff) = GetClrModulation(pTarget);
  EffectVar(1, pTarget, iEff) = GetObjectBlitMode(pTarget);
  //SetClrModulation(RGB(255,255,200) + (dwMod & 255 << 24), pTarget);
  SetObjectBlitMode(GFX_BLIT_Additive, pTarget);
  }
  
protected func FxIntThunderStop(object pTarget, int iEff)
  {
  SetClrModulation(EffectVar(0, pTarget, iEff), pTarget);
  SetObjectBlitMode(EffectVar(1, pTarget, iEff), pTarget);
  }

private func DoFadeout()
  {
  fFadingOut = true;
  FadeOut(1, 2);
  SetXDir(); SetYDir();
  }
