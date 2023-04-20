/*-- Neonlicht --*/

#strict

local pLight, // Das Lichtobjekt.
      on, // Ob das Licht gezündet wurde
      iH, iS, iL; // Die Farbwerte.

func Initialize() 
{
  //Kein Besitzer
  SetOwner(-1);
  
  //Zufällige Farbgebung
  iH = Random(256);
  iS = 255;
  iL = 128;
  
  //Nicht an, etwas dunkler
  SetColorDw(HSL(iH, iS/2, iL-50));
}

public func Activate(object pByObj)
{
  if (on)  // Nur einmal aktivierbar.
    return;
  else
    on = true;
  
  AddEffect("NeonGlow", this(), 100, 5, this()); // Effekt starten und aus dem Clonk schmeißen
  if(!(Contained()->~Throw()))
  	Exit(this(), GetDir(Contained())*12-6, 0, 0, GetDir(Contained())*6-3, -3, GetDir(Contained())*60-30);
  return(1);
}

protected func FxNeonGlowStart(pTarget, iEffectNumber, fTemp)
{
	if(fTemp) return;
	
  var iColor;
  iColor = HSLa(iH,iS,iL,30); // Alpha einrechnen
  SetColorDw(HSL(iH, iS, iL));
  pLight = AddLight(400, iColor, this());
  
  // Effekte
  CastParticles("PxSpark", 0, 0, 10, 40, 40, 90, GetColorDw(), GetColorDw());
  CastParticles("Smoke", 0, 0, 10, 40, 40, 90, GetColorDw(), GetColorDw());
}

protected func FxNeonGlowTimer(pTarget, iEffectNumber, iEffectTime)
{
  CreateParticle("PxSpark", RandomX(-3,3), RandomX(-3,3), RandomX(-5,5), RandomX(-4,-9), RandomX(40,90), GetColorDw());
  if(!Random(3)) CreateParticle("Smoke", RandomX(-3,3), RandomX(-3,3), RandomX(-5,5), RandomX(-4,-9), RandomX(140,190), GetColorDw());
  if(iEffectTime >= 4000 || !pLight) return(-1);
  pLight->ChangeSize(Max(400-iEffectTime/10, 0));
}

protected func FxNeonGlowStop(pTarget, iEffectNumber, iReason, fTemp)
{
	if(fTemp) return;
	if(pLight) pLight->RemoveObject(); // Licht entfernen, wenn noch nicht geschehen
	SetColorDw(HSL(iH, iS/3, 30)); // Licht ist ausgebrannt.
	FadeOut(); // ausfaden
}

public func RejectEntrance()
{
	// Nicht mehr sammelbar wenn an
	return(on);
}

func IsEquipment() { return(1); }

