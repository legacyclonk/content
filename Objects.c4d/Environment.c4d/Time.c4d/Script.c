/*-- Tageszeiten --*/

#strict

/* Locals */

//   0: Geschwindigkeit 1-10
//   1: Tageszeit 0-TimeResolution (Mittag-Mittag)
//   2: Helligkeit 0-100
// 4,5: Hilfsobjekte (Abwärtskompatiblität)
// 6-25: Himmelsfarben (Abwärtskompatiblität)

local SkyAdjustOrig;

private func TimeResolution() { return(10000); }
private func DayTimeLength() { return(TimeResolution() / 2); }

/* Verzögerte Initialisierung */

protected func Initialize()
{
  SetAction("Initialize");
  // Starthelligkeit
  Local(2) = 100;
  return(1);
}

private func Initialized()
{
  // Geschwindigkeit ermitteln
  Local() = ObjectCount(GetID()) + 1;
  // Andere Objekte des gleichen Typs entfernen
  var pOther;
  while(pOther = FindObject(GetID()))
    RemoveObject(pOther);
  // Himmelsmodulation speichern
  SkyAdjustOrig = GetSkyAdjust();
  // Steuerungsaktion
  SetAction("Time");
  // Fertig
  return(1);  
}
  
/* Helligkeitsberechnung */
private func GetLightIntensity(iTime)
{
  return BoundBy(Cos((iTime * 360) / TimeResolution(), 250) - Cos((DayTimeLength() * 180) / TimeResolution(), 300), -50, 50) + 50;
}
  
/* Konfiguration */

public func SetTime(iTime) // 0 bis 100 = Mittag bis Mittag
{
  // Zeit setzen
  Local(1) = iTime * TimeResolution() / 100;
  // Helligkeit anpassen
  RestoreSkyColors(Local(2) = GetLightIntensity(Local(1)));
  return(1);
}
  
/* Zeitverlauf */  

private func Timing()
{
  // Zeitfortschritt
  Local(1) = (Local(1) + Local(0)) % TimeResolution();
  // Helligkeit nach Tageszeit berechnen
  var iIntensity = GetLightIntensity(Local(1));
  // Helligkeit und Himmelsfarbe anpassen
  if (iIntensity > Local(2)) RestoreSkyColors(++Local(2));
  if (iIntensity < Local(2)) RestoreSkyColors(--Local(2));
  // Fertig
  return(1);
}
  
protected func UpdateTransferZone()
{
  // Kleiner Trick, damit der Himmel nach dem Laden aktualisiert wird
  if (GetAction() S= "Time") 
    RestoreSkyColors(Local(2));
  return(0);  
}
  
/* Status */

global func IsDay()
{
  var pTime; 
  if (!(pTime = FindObject(TIME))) return(1);
  if (Local(2, pTime)) return(1);
  return(0);
}
  
global func IsNight()
{
  return(!IsDay());
}
    
/* Himmelsfarbe */

private func RestoreSkyColors(iPercent)
{
  // Alte Speicherung? Übertragen
  if (Local (4)) GetOldSkyColors();
  if (Local (6)) 
  {
    var i;
    // ehemaliges OldGfx: Normales SetSkyColor
    while(i < 20) RestoreSkyColor(i++, 100);
  }
  // NewGfx: Einfach SetSkyAdjust
  // Minimale Gammakontrolle (Rampe 3)
  var lt = iPercent / 2 + 78;
  SetGamma(0, RGB(lt, lt, 128), 16777215, 3);
  SetSkyAdjust(RGBa(
    iPercent * GetRGBValue(SkyAdjustOrig,1) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,2) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,3) / 100,
    iPercent * GetRGBValue(SkyAdjustOrig,0) / 100  ), GetSkyAdjust(1));
  return(1);
}
  
private func RestoreSkyColor(int iColor, int iPercent)
{
  SetSkyColor(iColor,
              ((Local(iColor+6)>>16 & 255) * iPercent)/100,
              ((Local(iColor+6)>> 8 & 255) * iPercent)/100,
              ((Local(iColor+6)     & 255) * iPercent)/100);
  Local(iColor + 6) = 0;
  return();
}

private func GetOldSkyColors()
{
  var i;
  i=-1; while (++i<11) Local(i+ 6)=Local(i,Local(4));
  i=-1; while (++i<11) Local(i+16)=Local(i,Local(5));
  // Alte Hilfsobjekte entfernen
  RemoveObject(Local(4));
  RemoveObject(Local(5));
  return(1);
}
