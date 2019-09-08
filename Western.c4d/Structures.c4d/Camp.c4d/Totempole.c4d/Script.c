/*-- Totempfahl --*/

#strict 2

protected func Initialize()
{
  SetAction("Mystic");
  // Irgendein Besitzer vorhanden?
  if(GetOwner(this()) != -1)
    Message("+%d{{MYMS}}",this(),GetMystic());
  return(1);
}

protected func Construction(object pByObj)
{
  // Kein Besitzer? Kein Check!
  if(GetOwner(this()) == -1) return(true);
  // Der Clan kann nur eine bestimmte Anzahl davon bauen    
  if(RejectResearch(this(),0)) SetClanKnowledge(GetClan(pByObj),GetID(),1);
  // Zur Sicherheit
  if(RejectResearch(this(),-1)) {
    if(pByObj) AddMessage("$MsgNotBuildable$",pByObj,GetName(0,GetID()));
    RemoveObject(this());
  }
  return(true);
}

public func RejectConstruction(int iX, int iY, object pClonk)
{
  // Davon lässt sich nur eine begrenzte Anzahl bauen
  if(RejectResearch(pClonk,0)) {
    if(pClonk) AddMessage("$MsgNotBuildable$",pClonk,GetName(0,GetID()));
    SetClanKnowledge(GetClan(pClonk),GetID(),1);
    return(true);
  }
  return(false);
}

protected func Destruction()
{
  // Der Clan bekommt den Bauplan zurück
  if(!RejectResearch(this(),-1)) SetClanKnowledge(GetClan(),GetID(),0);
}

public func RejectResearch(object pClonk, int iAdd)
{
  // Verhindert das erneute Erforschen
  return(ObjectCount2(Find_Or(Find_ID(GetID()),Find_ID(GetDefBurnTo(GetID()))),Find_Clan(GetClan(pClonk)))+iAdd >= GetBuildLimit());
}

/* Gibt an wie viele Objekte der Spieler davon bauen kann */
public func GetBuildLimit() { return(1); } // nur ein Totem

/* Gibt an wie viele Mystikpunkte der Spieler davon erhält */
public func GetMystic() { return(50); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Ohne Scharmanenzelt lassen sich Mystikpunkte nicht einsetzen */
public func GetResearchBase() { return(NTIP); }