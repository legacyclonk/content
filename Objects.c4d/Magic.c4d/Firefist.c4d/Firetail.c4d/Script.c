#strict

protected func Initialize()
  {
  // An die Erdoberfläche bewegen
  var i;
  while(Stuck() && (i++) < 5)
    SetPosition(GetX(), GetY() - 1);

  // Immernoch festhängend?
  if(Stuck() ) RemoveObject();

  // Hit()-Aufruf verursachen
  SetYDir(10);
  
  // Effekt anhängen, damit Entzauberung den Feuerschweif entfernt
  AddEffect("MFiretailNSpell", this(), 130, 0, this());

  return(1);
  }

protected func Hit()
  {
  if(GetAction() S= "Left") Jumpada(-20);
  if(GetAction() S= "Right") Jumpada(20);
  return(1);
  }

protected func Jumpada(xdir)
  {
  if (!GBackSolid(xdir, 0) || !Random(5)) {
    SetXDir(xdir);
    SetYDir(-10);
    SetController(GetController(),CreateObject(FLAM,0,0,GetOwner()));
  }
  return(1);
  }

protected func Timer()
  {
  if(GetActTime()>200) return(Explode(20));

  var c;  CreateParticle("MSpark",Random(11)-5,Random(11)-5,0,-Random(10),c=20+Random(41),RGBa(255,Random(100),0,c*4,128));
  return(1);
  }

protected func FxMFiretailNSpellStop(object pTarget, int iEffectNumber, int iReason)
  {
  // Effekt per Script entfernt: Feuerschweif in die Luft jagen
  if (iReason) return(1);
  Explode(20, pTarget);
  return(1);
  }
