/*-- Hauptmann mit Bestellung --*/

#strict
#appendto TGST

static fNoChiefBuy;
static fDontUseChief;

local temp1, temp2;

protected func ContainedUp(object pClonk)
{
  [$SendMsg$]
  CreateMenu(GetID(), pClonk, this(), 2, "$SendMsg$", 2);
  var pLine, i = 1;
  // Leitungen zum Testen
  while(pLine = FindObject(CNNC, 0,0,0,0,0, "Connect", this(), 0, pLine))
  {
    AddMenuItem(Format("$TestWire$", i++), Format("TestWire(%d, %d)", ObjectNumber(pClonk), ObjectNumber(GetActionTarget(0,pLine))), WIRE, pClonk, 0, pClonk, "$TestWireDesc$", 128, 0, 0);
    Local(i-1) = pLine;
  }
  // Hauptmann bestellen
  if(!fDontUseChief)
    AddMenuItem("$MnCallSupp$", "CallSupply", CHIF, pClonk, 0, pClonk, 0,128,0,80);
}

protected func CallSupply(id dummy, object pClonk)
{
  // Nur einen Boss	
  if(fNoChiefBuy) return(PlayerMessage(GetOwner(pClonk), "$MsgOnlyOneChief$", pClonk));
  // Hat der Spieler genug Geld?
  if(GetWealth(GetOwner(pClonk)) < 80)
    {
    PlayerMessage(GetOwner(pClonk), "$MsgNoMoney$", pClonk);
    Sound("Error", 1, 0,0, GetOwner(pClonk));
    return();
    }
  // Geld abziehen, bestellen und Geräusch
  DoWealth(GetOwner(pClonk), -80);

  // Keinen weiteren bauen, bis der alte tot ist
  fNoChiefBuy = 1;

  // Funken losschicken
  temp1 = ObjectNumber(pClonk);
  temp2 = ObjectNumber(GetActionTarget(0,Local(1)));
  // Nur wenn Energie da ist
  SetAction("TestWire");
  ScheduleCall(this(), "DoSupply", 35*5, 1, pClonk);
}

private func LaunchSpark(pClonk, pLine)
{
  // Alte Funken entfernen
  var oldSpark;
  if(oldSpark = FindObjectOwner(TGTS, GetOwner(pClonk))) RemoveObject(oldSpark);
  // Funken erzeugen
  var Spark = CreateObject(TGTS, 27, -21, GetOwner(pClonk));
  // Startpunkt ermitteln
  var Start = GetActionTarget(0, pLine);
  if(GetActionTarget(1, GetActionTarget(1, pLine)) == this()) Start = GetActionTarget(1, pLine);
  // Losschicken
  Spark->Set(pLine, Start);
}

protected func DoSupply(pClonk)
{
  // Luftschiffeffekt starten
  AddEffect("BlimpArrive", 0, 1, 2, 0, GetID(), GetOwner(pClonk));
  return(Sound("Uncash", 1, 0,0, GetOwner(pClonk)));
}

func FxBlimpArriveStart(pTarget, iNumber, fTmp, iPlr)
{
  if(fTmp) return(0);
  // Alle Variablen füllen
  EffectVar(0, pTarget, iNumber) = iPlr;
  EffectVar(1, pTarget, iNumber) = 0;
  EffectVar(2, pTarget, iNumber) = CreateObject(BLM2, 1466, 0,-1);
  EffectVar(3, pTarget, iNumber) = CreateObject(BNDT, 1471,-2,-1); // FühererSetCommand(this(), "MoveTo", 0, 963, 141);
  EffectVar(4, pTarget, iNumber) = CreateObject(BNDT, 1456,-2,-1); // Neuer Clonk

  // Grapihien anpassen
  SetGraphics(0, EffectVar(2, pTarget, iNumber), BLMP);
  SetGraphics("Boss", EffectVar(4, pTarget, iNumber), BNDT);
  EffectVar(2, pTarget, iNumber)->~SetSolidMask(16,57,36,3,16,57);

  // Layer anpassen
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(2, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(3, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(4, pTarget, iNumber));
  // Commando geben
  SetCommand(EffectVar(2, pTarget, iNumber), "MoveTo", 0, 963, 131);
  // Farbe und Sicht setzen
  SetColorDw(GetPlrColorDw(iPlr), EffectVar(4, pTarget, iNumber)); 
  SetPlrView(iPlr, EffectVar(4, pTarget, iNumber));

  // Noch passende Aktionen
  ObjectSetAction(EffectVar(3, pTarget, iNumber), "Push", EffectVar(2, pTarget, iNumber));
  ObjectSetAction(EffectVar(2, pTarget, iNumber), "FloatPropel");
}

func FxBlimpArriveTimer(pTarget, iNumber, iTime)
{
  // Variablen hohlen   
  var pBlimp = EffectVar(2, pTarget, iNumber), 
      pLeader = EffectVar(3, pTarget, iNumber), 
      pClonk = EffectVar(4, pTarget, iNumber),
      iPlr = EffectVar(0, pTarget, iNumber),
      fBack = EffectVar(1, pTarget, iNumber),
      iShift = EffectVar(5, pTarget, iNumber);
  // Ohne Luftschiff geht nichts
  if(!pBlimp) return(-1);   
  // Auf der Hinfahrt
  if(!fBack)
  {
    // Sind wir schon da?	  
    if(Distance(GetX(pBlimp), GetY(pBlimp), 963, 131)<20 || iTime>575)
    {
      // Status umsetzen	    
      EffectVar(1, pTarget, iNumber) = 1;
      // Abspringen
      SetDir(0, pClonk);
      pClonk->Jump();
      // Layer zurücksetzen und Owner geben
      SetObjectLayer(pClonk, 0);
      SetOwner(iPlr, pClonk);
      
      // Entweder einen neuen CrewMember hohlen oder eine alte Info verwenden
      // (es sollen nur 5 InfoSections verwendet werden
      if(!Clonk(iPlr, 4)) MakeCrewMember(pClonk, iPlr);
      if(Clonk(iPlr, 4))
      {
        GrabObjectInfo(Clonk(iPlr,0), pClonk);
        var i;
        while(Clonk(iPlr, i+1)) 
        {
          if(i>3) break;	    
          Clonk(iPlr, i) = Clonk(iPlr, i+1);
          i++;
        }
        Clonk(iPlr, 4) = 0;
      }
      // Rückfahrt
      SetCommand(pBlimp, "MoveTo", 0, 1466, -28);
      // Bosseffekt vergeben
      AddEffect("Chief", pClonk, 1, 0, 0, TGST);
      return();
    }
  }
  // Und jetzt zurückfahren
  else
  {
    // Schon da?	  
    if(Distance(GetX(EffectVar(2, pTarget, iNumber)), GetY(EffectVar(2, pTarget, iNumber)), 1466, -28)<20)
    {
      RemoveObject(pBlimp);
      RemoveObject(pLeader);
      return(-1);
    }
    // Zu lang im Spiel (z.B. blockiert) dann ausfaden und ...
    if(iTime>1150 && pLeader)
    {
      if(!GetEffect("ReleaseClonkFadeOut", pLeader))
      {AddEffect ("ReleaseClonkFadeOut", pLeader, 320, 1, 0, REAC);
       Message("", pLeader);
      }
    }
    // ... Luftschiff brennt kurz darauf ab
    if(iTime>1160)
    {
      Incinerate(pBlimp);
      return(-1);
    }
  }
}

static pChief;

func FxChiefStart(pTarget, iEffect)
{
  pChief = pTarget; 
  // Ein bischen Ausrüstung
  CreateContents(AMBO,pTarget);
  CreateContents(AMBO,pTarget);
  CreateContents(DYNB,pTarget);
  CreateContents(WINC,pTarget);
  CreateContents(REVR,pTarget);
  // Wird zum Boss
  pTarget->~MakeBoss();

  // Scoreboard und Nachricht
  var iPlr = GetOwner(pTarget);
  SetScoreboardData(GetPlayerID(iPlr),       SBRD_Caption, Format("%s {{STAR}}", GetTaggedPlayerName(iPlr)), GetPlayerID(iPlr));
  DoScoreboardShowAll(1, 5);
  ExtraLog("$MsgNewChief$", GetPlayerName(iPlr));
}

func FxChiefStop(pTarget, iEffect, iReason, fTmp)
{
  if(fTmp) return();
  // Wieder neuer Boss kaufbar
  fNoChiefBuy = 0;

  // Scoreboard anpassen
  var iPlr = GetOwner(pTarget);
  SetScoreboardData(GetPlayerID(iPlr),       SBRD_Caption, Format("%s", GetTaggedPlayerName(iPlr)), GetPlayerID(iPlr));
}

// Nur Objekte im selben Layer finden
// (das ist wichtig damit z.B. Vögel das Luftschiff im anderen Layer nicht angreifen)
global func FindObject (id idDef, int iX, int iY, int iWdt, int iHgt, int dwOCF, string szAction, object pActionTarget, pContainer, object pFindNext)
{
  // Bei Globalen Aufrufen nichts machen    
  if(!this()) return(_inherited(idDef, iX, iY, iWdt, iHgt, dwOCF, szAction, pActionTarget, pContainer, pFindNext)); 
  // Sonst nur Objekte in dem selben Layer finden
  while(pFindNext = _inherited(idDef, iX, iY, iWdt, iHgt, dwOCF, szAction, pActionTarget, pContainer, pFindNext)) if(GetObjectLayer(this())==GetObjectLayer(pFindNext)) return(pFindNext);  
}
