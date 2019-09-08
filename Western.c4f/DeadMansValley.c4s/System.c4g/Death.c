#strict

#appendto BNDT

public func & idDamage() { return(Local(57)); }

public func Initialize()
{
  if(!GetEffect("DamageCounter", this())) AddEffect("DamageCounter", this(), 1, 0, 0, 0);
  return(_inherited());
}

global func FxDamageCounterDamage (object pTarget, int iEffectNumber, int iDmgEngy, int iCause)
{
  var idID = 0;
  if(iCause==33) idID = BLST; // Explosion
  if(iCause==35) idID = FIRE; // Feuer
  if(iCause==37) idID = SUFO; // Ersticken
  if(iCause==38) idID = ACID; // Säure
//  if(iCause==40) idID = CLNK; // Clonk Kampf
  if(idID) pTarget->~NewDamageID(idID);
  return(iDmgEngy);
}

public func QueryCatchBlow(pBy)
{
  idDamage() = GetID(pBy);  
  return(_inherited(pBy));  
}

public func NewDamageID(id, pObj)
{
  idDamage() = id;
}

global func Punch(object pObj, int iX)
{
  if(this() && pObj) pObj->~NewDamageID(GetID(this()));
  return(_inherited(pObj, iX));
}

global func DoEnergy (int iChange, object pObj)
{
  if(this() && pObj) pObj->~NewDamageID(GetID(this()));
  return(_inherited(iChange, pObj));
}

public func ControlThrow()
{
  // Beim Werfen Besitzer setzen (für genauere Fragzuordnung)  
  if(Contents()) SetOwner(GetOwner(), Contents());
  return(_inherited());
}

protected func Death(int iDeathCausedBy)
  {
  CountDeath(iDeathCausedBy);
  if(GetOwner()!=-1) return(_inherited(iDeathCausedBy));
  }
  
protected func Destruction()
  {
  if(GetOwner()!=-1) Kill();
  }

protected func CountDeath(iDeathCausedBy)
{
  // Kein Punkte für eliminierte Spieler
  if(!GetPlayerName(GetOwner())) return(); 
  if(Local(49)) return(); // Nur einmal Zählen
  else Local(49) = 1;
  var iPlr = GetOwner();
  Deaths(iPlr)++;
  // ggf. Kill zählen
  if ( (iDeathCausedBy != iPlr) && GetPlayerName(iDeathCausedBy) && (iPlr != -1))
  { 
    Kills(iDeathCausedBy)++;
//    if(GetEffect("Chief", GetCursor(iDeathCausedBy))) { Kills(iDeathCausedBy)++; } // Hauptmann tötet
    if(pChief && (this() == pChief)) // Hauptmann wird getötet
    { 
      Kills(iDeathCausedBy)++; // insgesamt 2 Punkte für Hauptmannskill
      // ExtraLog("$MsgChiefKill$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      if(idDamage() == BLST) ExtraLog("$MsgCExplo$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == FIRE) ExtraLog("$MsgCFire$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == BNDT) ExtraLog("$MsgCFight$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy)); 
      else if(idDamage() == CSHO) ExtraLog("$MsgCShot$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy)); 
      else ExtraLog("$MsgCItem$", GetPlayerName(iPlr), GetName(0,idDamage()),GetPlayerName(iDeathCausedBy)); 
      DoWealth(GetPlayerByIndex(iDeathCausedBy),40);
    }
    else
    {
 /* // alternative Meldungen, wenn keine Todesursache angezeigt werden soll
    var rnd = Random(5);
    if(rnd==0) ExtraLog("$MsgKill0$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(rnd==1) ExtraLog("$MsgKill1$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(rnd==2) ExtraLog("$MsgKill2$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(rnd==3) ExtraLog("$MsgKill3$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(rnd==4) ExtraLog("$MsgKill4$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));*/
    if(idDamage() == BLST) ExtraLog(RandomItem("$MsgExplo1$", "$MsgExplo2$", "$MsgExplo3$"), GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(idDamage() == FIRE) ExtraLog("$MsgFire$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
    else if(idDamage() == BNDT) ExtraLog(RandomItem("$MsgFight1$", "$MsgFight2$"), GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy)); 
    else if(idDamage() == CSHO) ExtraLog(RandomItem("$MsgShot1$", "$MsgShot2$", "$MsgShot3$", "$MsgShot4$"), GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy)); 
    else ExtraLog(RandomItem("$MsgItem1$", "$MsgItem2$", "$MsgItem3$"), GetPlayerName(iPlr), GetName(0,idDamage()),GetPlayerName(iDeathCausedBy));
    if(GetEffect("Chief", GetCursor(iDeathCausedBy))) DoWealth(GetPlayerByIndex(iDeathCausedBy),10); // Hauptmann bekommt weniger
    else DoWealth(GetPlayerByIndex(iDeathCausedBy),20);
    }
    Sound("Cash",1);
  }
  else
  {
    if(pChief && (this() == pChief)) // Hauptmann stirbt
    { 
      Kills(iPlr)--;    
      if(idDamage() == BLST) ExtraLog("$MsgSCExplo$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == FIRE) ExtraLog("$MsgSCFire$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == SUFO) ExtraLog("$MsgSCAir$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == ACID) ExtraLog("$MsgSCAcid$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else ExtraLog(RandomItem("$MsgSCElse1$", "$MsgSCElse2$"), GetPlayerName(iPlr));
    }
    else
    {   
      Kills(iPlr)--;    
      if(idDamage() == BLST) ExtraLog("$MsgSExplo$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == FIRE) ExtraLog("$MsgSFire$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == SUFO) ExtraLog("$MsgSAir$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else if(idDamage() == ACID) ExtraLog("$MsgSAcid$", GetPlayerName(iPlr), GetPlayerName(iDeathCausedBy));
      else ExtraLog(RandomItem("$MsgSElse1$", "$MsgSElse2$"), GetPlayerName(iPlr));
      /*
      var rnd = Random(3);
      if(rnd==0) ExtraLog("$MsgSuicide0$", GetPlayerName(iPlr));
      else if(rnd==1) ExtraLog("$MsgSuicide1$", GetPlayerName(iPlr));
      else if(rnd==2) ExtraLog("$MsgSuicide2$", GetPlayerName(iPlr));*/
    }
  }
  var i=-1;
  while(Clonk(GetOwner(), ++i)) if(i>4) break;
  if(i<5) Clonk(GetOwner(), i) = this();
  UpdateScoreboard(true);
}

// Zufallsfunktion
public func RandomItem()
{
  var i=-1;
  while(Par(++i));
  return(Par(Random(i)));
}

// Nicht als Bandit deklariert, damit er auch einkaufen kann
public func IsBandit() { return(0); }
