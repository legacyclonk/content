/*-- Polarnacht --*/

#strict

local SkyAdjust, SkyBack, OldSkyAdjust, OldSkyBack;

protected func Initialize() {
  SetAction("Initialize");
  SetPosition(50,80);
}

protected func Initialized() {
  //Himmelsmodulation speichern
  SkyAdjust = GetSkyAdjust ();
  // Steuerungsaktion
  SetAction("PolarNight");
}

private func PolarNight() {
  // Helligkeit
  SetDaylight(Sin(GetSeason() * 360/100,50)+50);
}
  
private func SetDaylight (iPercent) {
  // Minimale Gammakontrolle (Rampe 3)
  var lt = iPercent / 2 + 78;
  SetGamma (0, RGB (lt, lt, 128), 16777215, 3);
  SetSkyAdjust (RGBa (
    iPercent * GetRGBValue(SkyAdjust,1) / 100,
    iPercent * GetRGBValue(SkyAdjust,2) / 100,
    iPercent * GetRGBValue(SkyAdjust,3) / 100,
    iPercent * GetRGBValue(SkyAdjust,0) / 100
  ), GetSkyAdjust (1));
}
  
/* Status */

global func IsNight() {
  if (Inside(GetSeason(),60,90)) return(1);
}
  
global func IsDay() {
  return(!IsNight());
}

/* Skyfade */

public func StartSkyFade (rgb0, rgb1) {
  SkyAdjust = rgb0;
  OldSkyAdjust = GetSkyAdjust ();
  SkyBack = rgb1;
  OldSkyBack = GetSkyAdjust (1);
  SetAction ("SkyFade");
}

func Fading () {
  var iPercent = GetPhase ();
  SetSkyAdjust (RGBa (
    Mod (SkyAdjust, OldSkyAdjust, iPercent, 1),
    Mod (SkyAdjust, OldSkyAdjust, iPercent, 2),
    Mod (SkyAdjust, OldSkyAdjust, iPercent, 3),
    Mod (SkyAdjust, OldSkyAdjust, iPercent, 0)
  ), RGB (
    Mod (SkyBack, OldSkyBack, iPercent, 1),
    Mod (SkyBack, OldSkyBack, iPercent, 2),
    Mod (SkyBack, OldSkyBack, iPercent, 3)
  ));
  // Gamma
  SetGamma(RGB(
    0,
    0,
    0
  ),       RGB(
    Mod(SkyAdjust,OldSkyAdjust,iPercent,1)/6 + 128*5/6,
    Mod(SkyAdjust,OldSkyAdjust,iPercent,2)/6 + 128*5/6,
    Mod(SkyAdjust,OldSkyAdjust,iPercent,3)/6 + 128*5/6
  ),       RGB(
    Mod(SkyAdjust,OldSkyAdjust,iPercent,1)/4 + 255*3/4,
    Mod(SkyAdjust,OldSkyAdjust,iPercent,2)/4 + 255*3/4,
    Mod(SkyAdjust,OldSkyAdjust,iPercent,3)/4 + 255*3/4
  ));
}

func Mod (rgb0, rgb1, pct, n) { return ((pct * GetRGBValue(rgb0,n) + (100 - pct) * GetRGBValue(rgb1,n)) / 100); }
