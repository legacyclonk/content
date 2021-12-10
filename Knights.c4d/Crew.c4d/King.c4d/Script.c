/*-- König --*/

#strict

#include KNIG // Der König ist ein Ritter

local iLastSpell; // Letzter Spruchindex - wird im Zaubermenü wieder ausgewählt
local szKingdomName; // Name des Königreiches seiner Hoheit (für Spezialszenarien)

local idCurSpell;
local iCurPhase;

/* Hat nur ein Portrait */

public func StdPortraitCount() { return(1); }

/* Zauberkraft geben (Aufgerufen vom Aufwertungszauber) */

public func MagicEnergy()
{
  // Normalerweise wird die Physical schon im Redefine2 gesetzt - dies ist ein Workaround für Aufwertungen von Clonks,
  // die kein #include CLNK im Script haben
  if (!GetPhysical("Magic")) SetPhysical("Magic", GetPhysical("Magic", 0, 0, GetID()), PHYS_Temporary);
  // Und gleich auffüllen
  for (var i = 0; i < 10; i++)
    DoMagicEnergy(10);
  return(1);
}
 
/* Kontextmenü */  
  
public func ContextMagic(pCaller)
{
  [$CtxMagicDesc$|Image=MCMS|Condition=ReadyToMagic]
  // Nicht bereit
  if (!ReadyToMagic()) return(0);
  // Clonk soll anhalten
  SetComDir(COMD_Stop(), pCaller);
  // Menü öffnen
  var iExtra;
  if(ObjectCount(ALCO)) { if(ObjectCount(NMGE)) iExtra=1; else iExtra=5; }
  else                  { if(ObjectCount(NMGE)) iExtra=0; else iExtra=3; }
  CreateMenu(MCMS, 0, 0, iExtra, "$MnuNoSpells$", GetMagicEnergy());
  var idSpell, i;
  while (idSpell = GetPlrMagic(GetOwner(), 0, i++)) 
    AddMenuItem("$MnuCast$: %s", "DoMagic", idSpell);
  // Letzten Spruch auswählen
  SelectMenuItem(iLastSpell);
  return(1);
}

protected func OnMenuSelection(iMenuIndex)
{
  // Nur Zaubermenü: Menüindex speichern (Für Wiederauswahl beim Schließen und Öffnen des Menüs)
  if (GetMenu() == MCMS) iLastSpell = iMenuIndex;
  return();
}

/* Zaubern */  

// Der König kann immer zaubern
public func ReadyToMagic()  { return(1); }
  
protected func DoMagic(idSpell)
{
  // Nicht bereit
  if (!ReadyToMagic()) return();

  // Nicht genug Zauberenergie
  if(!CheckMana(idSpell))
  {
    Sound("Error");
    Message("$MsgNotEnoughEnergy$",this());
    return();
  }
  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idSpell,this()))
  {
    Sound("Error");
    Message("$MsgNotEnoughIngredients$",this(),GetNeededAlcStr(idSpell,this()));
    return();
  }

  if(IsConvertReady() )
  {
    idCurSpell = idSpell;
    SetMagicAction();
  }
  else
    ExecMagic(idSpell);
}
  

/* Status */

public func & GetKingdomName() { return (szKingdomName); }


// Zur Sicherheit, damit Überladungen keinen Fehler beim Starten geben
protected func Scaling()
{
  var szDesiredAction;
  if (GetYDir() > 0) szDesiredAction = "ScaleDown"; else szDesiredAction = "Scale";
  if (GetAction() ne szDesiredAction) SetAction(szDesiredAction);
  return(1);   
}

protected func CheckStuck()
{                   
  // Verhindert Festhängen am Mittelvertex
  if(!GetXDir()) if(Abs(GetYDir()) < 5)
    if(GBackSolid(0, 3))
      SetPosition(GetX(), GetY() + 1);
}

/* Inhalt prüfen: Kristalle verarbeiten */

protected func Collection2(pItem)
{
  if (FindContents(CRYS)) ConvertCrystal();
  return(1);
}

/* Energiegewinnung */

private func ConvertCrystal(pClonk)
{
  if(!IsConvertReady())   return(0);
  
  var crystal = FindContents(CRYS);
  if(!crystal) return(0);
  
  if(!crystal->~Apply(this()) )
    if(DoMagicEnergy(+10) )
      RemoveObject(crystal);
    else
      return(0);  
  
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);

  SetMagicAction();
  Sound("Electric");
  return(1);
}

private func Riding()
{
  ConvertCrystal();
  return(_inherited());
}

protected func AfterMagic()
{
  // Callback nach Magie-Animation: Nach weiteren Kristallen suchen
  if (FindContents(CRYS)) ConvertCrystal();

  idCurSpell = 0;
  iCurPhase = 0;

  return(1);
}
  
private func AfterMagicJump()
{
  if(idCurSpell && GetAction() S= "Walk")
  {
    SetAction("Magic");
    SetPhase(iCurPhase);
  }
  else AfterMagic();
}

public func LightningStrike()
{
  // Wenn der Clonk tot ist, zerstört ihn der erste Blitz
  if (GetAction() S= "Dead")
  {
    CastObjects(SPRK, 5, 20);
    CreateParticle("MSpark", 0,0, 1000, RGBa(50,50,255,150));
    return(RemoveObject());
  }
  // Clonk soll anhalten
  SetComDir(COMD_Stop());
  // Energiegewinnung
  var iEnergyGain = 10;
  // Extra-Energiegewinnung durch die Krone?
  if (FindContents(_CRN)) iEnergyGain += 10;
  while (iEnergyGain--) DoMagicEnergy(1);
  // Sound
  Sound("Electric");
  // Effekt-Aktion
  if(IsConvertReady() ) SetMagicAction();
  return(1);
}
  
public func IsConvertReady()
{
  // Im Laufen und Reiten können Kristalle umgewandelt werden
  return(GetAction() S= "Walk" || GetAction() S= "Ride" || GetAction() S= "RideStill");
}

/* Kann keine Rüstung tragen */

public func WearArmor(pArmor)
{
  Sound("KnightFailure");
  Message("$MsgCantWearArmor$", this());
  return(0);
}

private func SetMagicAction()
{ 
  // Zaubern im Sprung
  if(GetAction() S= "Jump") return(SetAction("JumpMagic") );
  
  // Aktion setzen
  if(!IsRiding() ) return(SetAction("Magic") );
  // Beritten? Aufs Zaubern mögen wir dennoch nicht verzichten.
  else SetAction("RideMagic");
}

private func CheckMagic()
{
  // Nur in Phase 5 zaubern
  iCurPhase = GetPhase();
  if(iCurPhase != 5) return();
  
  // Kein Zauber? :[
  if(!idCurSpell) return();
 
  // Zauberspruch ausführen
  ExecMagic(idCurSpell);
}

private func ResetMagic()
{
  if(GetAction() eq "Magic") SetAction("Walk");
  if(GetAction() eq "JumpMagic") SetAction("Jump");
  if(GetAction() eq "RideMagic") SetAction("Ride");
}

private func ExecMagic(idSpell)
{ 
  // Zauberobjekt erschaffen und aktivieren
  if (!CreateObject(idSpell,0,0,GetOwner())->~Activate(this()))
    // Zauber fehlgeschlagen
    return(ResetMagic() );

  // Zauberenergie abziehen
  DoMagicEnergy(-Value(idSpell));

  // alchemische Zutaten reduzieren
  if(ObjectCount(ALCO)) ReduceAlchem(idSpell,this());

  // Aktuelle Zauberkraft trainieren
  if (GetPhysical("Magic",1))
    SetPhysical("Magic", Min(GetPhysical("Magic")+Value(idSpell), 100000), 0);

  // Fertig
  return(1);
}

