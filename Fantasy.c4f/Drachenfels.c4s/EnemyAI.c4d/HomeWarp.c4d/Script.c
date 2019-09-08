/* Homewarp für KI-Zauberer */

#strict 2

public func Activate(caster, real_caster)
  {
  // Zaubernden Clonk finden
  var clonk = caster;
  if (real_caster) clonk = real_caster;
    
  if (Contained(clonk))
    if (Contained(clonk)->~IsGolem())
      clonk = Contained(clonk);

  if (!clonk) return;

  // Die akustische Kulisse darf nicht fehlen
  clonk->Sound("Magic1");

  // Und warpen.
  SetPosition();
  var fx=GetX(clonk), fy=GetY(clonk);
  if (!clonk->~AI_WarpHome()) return;
  var tx=GetX(clonk), ty=GetY(clonk);
  DrawParticleLine("NoGravSpark", fx,fy, tx,ty, 10, 20, RGBa(127, 255, 255, 63), RGBa(127, 255, 255, 63));
  Sparkle(5, fx, fy);
  Sparkle(5, tx, ty);
  
  RemoveObject();
  return true;
  }
