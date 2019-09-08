#strict

local iTempSel, iSelection;

private func MaxSpells() { return(5); }

protected func CalcValue()
{
  var iValue = GetValue(0,GetID());
  // Bei aktivierter Alchemieregel wird den Zaubern ein gewisser Wert zugesprochen
  if (ObjectCount(ALCO))
    for(var i=0; i<MaxSpells(); ++i)
      if(Local(i) && GetName(0, Local(i)))
        iValue += GetValue(0,Local(i))*2/3;
  return(iValue);  // *1/2 wäre 1:1 für Preis Kristall
}

protected func RejectEntrance()
{
  if(GetAction() eq "WannaRemove") return(1);
}

public func Selection()
{
  var szMessage="", idSpell;
  for(var i=0; i<MaxSpells(); ++i) {
    if(!(idSpell=Local(i))) idSpell = FXU1;
    szMessage = Format("%s{{%i}}",szMessage,idSpell);
  }
  PlayerMessage(GetOwner(Contained()),szMessage,Contained());
}

protected func Entrance(object pContainer) { if(GetOCF(pContainer) & OCF_CrewMember()) Selection(); }

protected func Activate(object pByClonk)
{
  MagicMenu(pByClonk);
  return(1);
}

protected func OnMenuSelection(int iItemIndex, object pMenuObject) { iTempSel=iItemIndex; }
protected func MenuQueryCancel(int iItemIndex, object pMenuObject) { SetGraphics(); }

private func MagicMenu(object pByClonk) {
  var szCommand = "ChooseMagic";

  //SetGraphics("Open");

  // Nicht bereit
  if(!(pByClonk->~ReadyToMagic())) szCommand = "Nothing";
  if(GetAction(pByClonk) ne "Walk") szCommand = "Nothing";

  // Clonk soll anhalten
  SetComDir(COMD_Stop(), pByClonk);
  // Menü
  CreateMenu(GetID(),pByClonk);
  for(var i=0; i<MaxSpells(); ++i) {
    // Zauber belegt: Zaubern
    if(Local(i)) AddMenuItem(Format("$MnuCast$: %s",GetName(0,Local(i))),"DoMagic",Local(i),pByClonk,0,pByClonk);
    // Zauber nicht belegt: Belegen
    else AddMenuItem("$MnuEmpty$",szCommand,FXU1,pByClonk,0,pByClonk);
  }

  SelectMenuItem(MaxSpells()/2,pByClonk);
}

private func ChooseMagic(something_useless, object pByClonk) {
  iSelection = iTempSel;

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

  SetSpell(idMagic,iSelection);

  // Effekte!
  CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pByClonk)));

  // Nachricht!
  PlayerMessage(GetOwner(pByClonk),"$MsgSpellSet$", pByClonk, GetName(0,idMagic));

  // Aktion für den Clonk setzen
  pByClonk->SetAction("Magic");

  Sound("Magic*");
}

public func SetSpell(id idMagic, int iSel) {

  // Aktion für das Buch setzen
  SetAction("Be");

  Local(iSel) = idMagic;
}


private func Nothing() {}

private func DoMagic(id idSpell, object pByClonk) {
  iSelection = iTempSel;

  // Richtung
  SetDir(GetDir(pByClonk));
  // In den Besitz des Clonk-Controllers, damit der Verfeindungscheck diverser Zauber funktioniert
  SetOwner(GetController(pByClonk));
  SetController(GetOwner());
  // Zauber
  if(CreateObject(idSpell,0,0,GetController(pByClonk))->~Activate(this(),pByClonk)) Local(iSelection) = false;
  else PlayerMessage(GetOwner(Contained()),"$MsgNothingHappened$",Contained());

}

public func SpellFailed(id idSpell, object pByClonk)
{
  // Zauber abgebrochen: Zauber rückbelegen
  if (!Inside(iTempSel, 0, 4)) return();
  if (Local(iTempSel)) return();
  Local(iTempSel) = idSpell;
  return(true);
}

public func IsMagicProduct() { return(true); }

