#strict

local idSpell, pAimingClonk;

protected func CalcValue()
{
  var iSpell;
  if(idSpell) iSpell=GetValue(0,idSpell);
  return(GetValue(0,GetID())+iSpell*2/3);  // *1/2 wäre 1:1 für preis
}

protected func RejectEntrance()
{
  if(GetAction() eq "WannaRemove") return(1);
}

public func Selection()
{
  // wer bin ich? der da!
  if(idSpell) PlayerMessage(GetOwner(Contained()),"{{%i}} %s",Contained(),idSpell,GetName(0,idSpell));
}

protected func Entrance(object pContainer) { if(GetOCF(pContainer) & OCF_CrewMember()) Selection(); }

protected func Activate(object pByClonk)
{
  if(!idSpell) MagicMenu(pByClonk);
  else DoMagic(pByClonk);
  return(1);
}

private func MagicMenu(object pByClonk) {
  // Nicht bereit
  if(!(pByClonk->~ReadyToMagic())) return(0);
  if(GetAction(pByClonk) ne "Walk") return(0);
  // Clonk soll anhalten
  SetComDir(COMD_Stop(), pByClonk);
  // Menü öffnen
  OpenSpellMenu(pByClonk, this, "$MnuEnchant$: %s", "SetMagic", GetOwner(pByClonk), pByClonk, "$MnuNoSpells$");
}

private func SetMagic(id idMagic, object pByClonk) {
  // Nicht genug Zauberenergie
  if(!CheckMana(idMagic,pByClonk))
  {
    Sound("Error");
    Message("$TxtNotEnoughEnergy$",this());
    return();
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idMagic,pByClonk))
  {
    Sound("Error");
    Message("$TxtNotEnoughIngredients$",this(),GetNeededAlcStr(idMagic,pByClonk));
    return();
  }

  // Energie abziehen
  DoMagicEnergy(-Value(idMagic),pByClonk);

  // alchemische Zutaten abziehen
  if(ObjectCount(ALCO)) ReduceAlchem(idMagic,pByClonk);

  // Aktuelle Zauberkraft des zaubernden Clonks trainieren, sowie sonstige, szenarienspezifische Aufrufe
  OnClonkSucceededSpell(idMagic, pByClonk);

  SetSpell(idMagic);

  // Effekte!
  CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pByClonk)));

  // Nachricht!
  PlayerMessage(GetOwner(pByClonk),"$MsgSpellSet$", pByClonk, GetName(0,idMagic));

  // Aktion für den Clonk setzen
  pByClonk->SetAction("Magic");

  Sound("Magic*");
}

public func SetSpell(id idMagic) {
  // Rolle ist nun beschrieben
  SetGraphics("Full");

  // Grafik setzen
  SetGraphics(0,0,idMagic,1,GFXOV_MODE_Picture);
  SetObjDrawTransform(350,-300,3000,150,500,-2000, this(), 1);

  // Aktion für die Schriftrolle setzen
  SetAction("Be");

  idSpell = idMagic;
  
  // Name setzen
  UpdateName();
}

private func DoMagic(object pByClonk) {
  // Richtung
  SetDir(GetDir(pByClonk));
  // In den Besitz des Clonk-Controllers, damit der Verfeindungscheck diverser Zauber funktioniert
  SetOwner(GetController(pByClonk));
  SetController(GetOwner());
  // Zauber
  if(CreateObject(idSpell,0,0,GetController(pByClonk))->~Activate(this(),pByClonk))
    { Exit(); SetAction("WannaRemove",pByClonk); }
  else PlayerMessage(GetOwner(Contained()),"$MsgNothingHappened$",Contained());
}

protected func WannaRemove() {
  // manche Zauber hängen gerne Effekte an den Caster
  if(!GetEffectCount(0,this()) && !pAimingClonk) RemoveObject();
}

protected func AttachTargetLost() {
  RemoveObject();
}

public func SpellAiming(object pSpell, object pAimingClnk)
  {
  // Clonk zielt: Schriftrolle vorerst nicht entfernen
  pAimingClonk = pAimingClnk;
  }
  
public func SpellSucceeded(id idSpell, object pAimingClnk)
  {
  // Clonk hat fertig gezielt: Schriftrolle kann weg
  pAimingClonk = 0;
  }
  
public func SpellFailed(id idSpell, object pAimingClnk)
  {
  // Zauber beim Zielen abgebrochen: Schriftrolle restaurieren
  pAimingClonk = 0;
  return(SetSpell(idSpell));
  }

public func IsMagicProduct() { return(true); }

protected func UpdateTransferZone()
  {
  // Für Objects.txt-Schriftrollen
  UpdateName();
  }
  
protected func UpdateName()
  {
  // Namen nach Zauber setzen
  if (idSpell)
    SetName(Format("%s: %s", GetName(0, GetID()), GetName(0,idSpell)));
  else
    SetName();
  }
  