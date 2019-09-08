/* Symbol */

#strict

local idThisSpell, pMagician;

protected func AttachTargetLost() { return(RemoveObject()); }

public func SetSymbolGfx(string szAction, id idSpell, object pTarget, object pMagi)
  {
  // Standardaktivität: Symbol
  if (!szAction) szAction="Symbol";
  // Grafik und Namen setzen
  if (idThisSpell = idSpell)
    {
    SetGraphics(0, this(), idSpell, GFX_Overlay, GFXOV_MODE_IngamePicture);
    SetName(GetName(0, idSpell));
    // Anwählbar
    SetCategory(GetCategory() | C4D_MouseSelect());
    // Zauberer sichern fürs Symbolupdate
    pMagician = pMagi;
    }
  // Aktivität setzen
  if (!SetAction(szAction, pTarget)) return (0, RemoveObject());
  // Auswahlgröße: Immer 35x35
  SetShape(-18,-18,35,35);
  // Fertig
  return(1);
  }

public func GetSpellID() { return(idThisSpell); }
public func GetControlChar()
  {
  if (GetAction() eq "Throw") return("4");
  if (GetAction() eq "Up"   ) return("5");
  if (GetAction() eq "Dig"  ) return("6");
  if (GetAction() eq "Left" ) return("L");
  if (GetAction() eq "Down" ) return("2");
  if (GetAction() eq "Right") return("R");
  }

protected func MouseSelection(iByPlr) { return(GetActionTarget()->MouseSelection(this())); }

protected func RecheckSymbol()
  {
  // Magiesymbol-Timer: Färbung danach, ob der Magier genug Zauberenergie besitzt
  // Wenn unbekannt (Magier nicht gesetzt, etc.) als erlaubt annehmen
  var iCanCast = MCLK_UnlimitedCast;
  if (pMagician && idThisSpell)
    if (!idThisSpell->~IsSpellClass()) iCanCast = pMagician->~CheckMagicRequirements(idThisSpell, true);
  if (iCanCast)
    {
    SetGraphics(0, this(), 0, 2);
    SetClrModulation(16777215, 0, GFX_Overlay);
    SetObjectBlitMode(0, 0, GFX_Overlay);
    }
  else
    {
    SetGraphics(0, this(), idThisSpell, 2, GFXOV_MODE_IngamePicture);
    SetClrModulation(RGBa(255,0,0,160), 0, 2);
    SetObjectBlitMode(GFX_BLIT_Mod2, 0, 2);
    SetClrModulation(1, 0, GFX_Overlay);
    SetObjectBlitMode(GFX_BLIT_Mod2, 0, GFX_Overlay);
    }
  if (iCanCast && idThisSpell && !idThisSpell->~IsSpellClass() && iCanCast<MCLK_UnlimitedCast)
    CustomMessage(Format("@x%d", iCanCast), this(), GetOwner(), 15,40, 0xffffff);
  else
    CustomMessage("", this(), GetOwner());
  return(true);
  }
  