/*-- Drachenfels --*/

#strict

static FOW_dgstate, iDifficulty;
static g_pEndboss, g_pDragon, g_iDragonHomeX, g_iDragonHomeY, g_pPrincess, g_pKing;
static g_iEndbossX, g_iEndbossY;
static g_fIntroRun;

static const EVIL_MAGE_OBJ = 1758; // Nummer in der Objects.txt
static const SKIP_INTRO = false;
static const TEST_ENDBOSS = false;

/* DBG */

public func aED()
  {
  FOW_dgstate = (FOW_dgstate+1)%4;
  
  if (FOW_dgstate == 1)
    {
    FOW_Deactivate();
    FOW_Hide();
    }
  else if (FOW_dgstate == 2)
    {
    FOW_Show();
    }
  else if (FOW_dgstate == 3)
    {
    FOW_ShowRange();
    }
  else // 0
    {
    FOW_Hide();
    FOW_Activate();
    }
  }
  
public func aSAVE()
  {
  FOW_Hide();
  FOW_Activate();
  FOW_dgstate=0;
  }
  
  
  
/* Initialisierung */

protected func Initialize()
  {
  if (!ObjectCount(_EAI)) CreateObject(_EAI, 50,50, -1); // KI
  if (!ObjectCount(_ETG)) CreateObject(_ETG, 50,50, -1); // Löscher
  if (!ObjectCount(COAN)) CreateObject(COAN, 50,50, -1); // Einsammelbare Tiere
  // Holz aus dem König
  var pWood;
  while (pWood = FindContents(WOOD, FindObject(KING)))
    Exit(pWood, GetX(pWood)-75, GetY(pWood));
  return(1);
  }
  
protected func InitializePlayer(int iPlr, int tx, int ty, object pBase, int iTeam)
  {
  // In der Liga immer schwer.
  if (GetLeague() && !iDifficulty) SetDifficulty(2);
  // Schwierigkeitsstufen-Initialisierung für den Spieler (für den Laufzeitbeitritt)
  if (iDifficulty) InitializePlayer2(iPlr);
  // Immer FoW, damit FoW-Generatoren funzen
  Schedule(Format("SetFoW(true, %d)", iPlr), 1);
  
  // ggf. Intro starten
  if (!g_fIntroRun) { g_fIntroRun = true; ScriptGo(true); }

  // Der Spieler bekommt alle Waffen- und Burgbaupläne
  WPPL->SetKnowledge(iPlr);
  CPPL->SetKnowledge(iPlr);

  // Von links reinlaufen, dann rumlaufen
  var pCrew = GetCrew(iPlr);
  pCrew->SetPosition(20 + Random(100), pCrew->GetY());
  for (var i=0; i<10; i++)
  {
    pCrew->AddCommand(0, "MoveTo", 0, 160 + Random(100), pCrew->GetY());
    pCrew->AddCommand(0, "Wait", 0,0,0,0, 20 + Random(100));
  }
  
  var pClonk = GetHiRank(iPlr), i;
  if (pClonk)
    {
    // Der Host muss erst einmal den Schwierigkeitsgrad einstellen...
    if (!iPlr && !iDifficulty)
      {
      CreateMenu(WIPF, pClonk, pClonk, 0, "$MnuChooseDifficulty$");
      AddMenuItem("$MnuLevelItmNormal$", "SetDifficulty(1)", WIPF, pClonk, 0,0, "$DescLevelNormal$");
      AddMenuItem("$MnuLevelItmHard$", "SetDifficulty(2)", MONS, pClonk, 0,0, "$DescLevelHard$");
      return();
      }

    // ...während die anderen Spieler ihren Clonktyp wählen
    CreateMenu(CLNK, pClonk, pClonk, 0, "$MnuChooseClonk$");
    AddClonkChooseMnuItem(pClonk, KNIG, i++);
    AddClonkChooseMnuItem(pClonk, MAGE, i++);
    }
  }

protected func InitializePlayer2(int iPlr)
  {
  if (iDifficulty == 1) CreateContents(FLAG, GetHiRank(iPlr));
  }

global func SetDifficulty(int iSetDifficulty)
  {
  // Schwierigkeitsstufe setzen
  iDifficulty = iSetDifficulty;
  var i = GetPlayerCount(C4PT_User);
  while (i--) GameCall("InitializePlayer2", GetPlayerByIndex(i, C4PT_User));

  // Message über dem Clonk ausgeben
  if (iSetDifficulty == 1) var strDifficulty = "$MnuLevelItmNormal$";
  if (iSetDifficulty == 2) var strDifficulty = "$MnuLevelItmHard$";
  Message("$MsgDifficulty$", GetHiRank(0), strDifficulty);

  // Nun darf der Host auch seinen Clonktyp auswählen
  var pClonk = GetHiRank(0), i;
  CreateMenu(CLNK, pClonk, pClonk, 0, "$MnuChooseClonk$");
  GameCall("AddClonkChooseMnuItem", pClonk, KNIG, i++);
  GameCall("AddClonkChooseMnuItem", pClonk, MAGE, i++);
  
  // Nachinitialisierung der Physicals schon erzeugter Gegner
  var o;
  while (o = FindObject(0, 0,0,0,0, OCF_Alive, 0,0, 0, o))
    o->~AI_SetPhysicals();
  }
  
public func RemovePlayer(int plr)
  {
  // Wenn der letzte Spieler tot ist, ist das Spiel vorbei
  if (!GetPlayerCount(C4PT_User) || (GetPlayerCount(C4PT_User)==1 && GetPlayerByIndex(0, C4PT_User)==plr))
    Schedule("GameOver(-10)", 1, 1);
  }

private func AddClonkChooseMnuItem(object pClonk, id idNewType, int i)
  {
  // Menüeintrag hinzufügen, und auswählen wenn es der gegenwärtige Clonktyp ist
  AddMenuItem("$MnuChooseClonkItm$", Format("Redefine3(%i)", idNewType), idNewType, pClonk, 0, 0, Format("$MnuChooseClonkDesc$", GetName(0, idNewType)), 0, GetColorDw(pClonk));
  if (idNewType == GetID(pClonk)) SelectMenuItem(i, pClonk);
  }
  
global func Redefine3(id idTo)
  {
  // Redefine durch Neuerzeugung
  var pNew;
  if (GetID() != idTo)
    {
    var iPlr = GetOwner();
    pNew = CreateObject(idTo, 0, 0, iPlr);
    pNew->GrabContents(this());
    pNew->SetPosition(GetX(), GetY());
    pNew->SetAction(GetAction(), GetActionTarget(), GetActionTarget(1));
    pNew->SetDir(GetDir());
    pNew->SetComDir(GetComDir());
    MakeCrewMember(pNew, iPlr);
    if (GetCursor(iPlr) == this()) SetCursor(iPlr, pNew);
    // Fertig; Diesen Clonk nun entfernen
    RemoveObject();
    }
  // Achtung: pNew=0 (lokaler Aufruf), wenn GetID()==idTo war
  // Magier kriegen Zauberenergie
  if (idTo == MAGE) while (DoMagicEnergy(5, pNew));
  // Volle Startenergie
  DoEnergy(100, pNew);
  // Endhöhle testen?
  if (TEST_ENDBOSS)
    {
    SetPosition(1950, 500, pNew);
    AddEffect("Divinity", pNew, 200, 3);
    }
  return(true);
  }
  
private func AssignLocalizedStrings()
  {
  // Wandtexte - Runtime, damit beim Editieren nicht die Namen verloren gehen!
  AssignWScriptText("ScriptElev", "$ScrElev$");
  AssignWScriptText("ScriptLava", "$ScrLava$");
  AssignWScriptText("ScriptWipf", "$ScrWipf$");
  AssignWScriptText("ScriptSwitch", "$ScrSwitch$");
  // Twonkys Bogen
  var pBow = FindContents(BOW1, Contained(FindObject(_TAP)));
  if (pBow) pBow->SetName("$ItmTwonkysBow$"); else Log("INTERNAL ARROW: TWONKY#S BOW NOT FOUND!");
  }
  
private func GetEndboss()
  {
  var o;
  return Object(EVIL_MAGE_OBJ);
  }
  
// -- AI-Encountercallbacks
// Aufruf, sobald der entsprechende KI-Gegnerclonk einen Spielerclonk "sieht"

public func AI_EncounterBAND(object pBandit, object pPlayer)
  {
  MessageEx("$MsgBandits1$", pBandit);
  var szMsg;
  if (GetPlayerCount(C4PT_User) <= 1) szMsg = "$MsgBandits2a$"; else szMsg = "$MsgBandits2b$";
  Schedule(Format("MessageEx(\"%s\", Object(%d))", szMsg, ObjectNumber(pPlayer)), GetLastMsgWaitTime()); // GetLastMsgWaitTime nicht syncsicher!
  return(true);
  }
  
public func AI_EncounterMAGE(object pMage, object pPlayer)
  {
  MessageEx("$MsgMage1$", pMage);
  Schedule("MessageEx(\"$MsgMage2$\")", GetLastMsgWaitTime()); // GetLastMsgWaitTime nicht syncsicher!
  return(true);
  }
  
  
/* Thx2Guenther4greatASCII */
/**********************************************************************\
 *                                                                    *
 *  %**************************************************************%  *
 *  *                                                              *  *
 *  *  %********************************************************%  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                   I    N    T    R    O                *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  *                                                        *  *  *
 *  *  %********************************************************%  *  *
 *  *                                                              *  *
 *  %**************************************************************%  *
 *                                                                    *
\**********************************************************************/

protected func Script1()
  {
  // Texte zuweisen
  AssignLocalizedStrings();
  // Endboss-Physicals
  g_pEndboss = GetEndboss();
  if (!g_pEndboss) { Log("INTERNAL ERROR: ENDBOSS NOT FOUND!"); return(GameOver(2005)); }
  g_pEndboss->AI_SetPhysicals();
  g_iEndbossX = GetX(g_pEndboss); g_iEndbossY = GetY(g_pEndboss);
  g_pDragon = FindObject(DRGN);
  if (!g_pDragon) { Log("INTERNAL ERROR: DRAGON NOT FOUND!"); return(GameOver(2005)); }
  g_pKing = FindObject(KING);
  if (!g_pKing) { Log("INTERNAL ERROR: KING NOT FOUND!"); return(GameOver(2005)); }
  g_pPrincess = FindObject(_PRC);
  if (!g_pPrincess) { Log("INTERNAL ERROR: PRINCESS NOT FOUND!"); return(GameOver(2005)); }
  g_iDragonHomeX = GetX(g_pDragon); g_iDragonHomeY = GetY(g_pDragon);
  
  // Intro überspringen? (test)
  if (SKIP_INTRO) return(IntroSkip());

  // Intro: Drache+Magier
  SetPosition(1000, 800, g_pEndboss);
  SetPosition(1000, 800, g_pDragon);
  g_pDragon->SetAction("Fly");
  g_pEndboss->AI_Disable();
  g_pEndboss->SetAction("Ride", g_pDragon);
  g_pEndboss->SetObjectLayer(g_pEndboss);
  g_pPrincess->SetObjectLayer(g_pPrincess);
  g_pEndboss->SetCategory(4);
  g_pEndboss->SetObjectBlitMode();
  // König+Königin
  MessageEx("$MsgIntro0$", g_pKing, GetName(g_pPrincess)); // Oh, %s!
  //MessageEx("$MsgIntro0$", g_pPrincess, GetName(g_pKing)); // Oh, %s
  var mx = (GetX(g_pKing) + GetX(g_pPrincess))/2, my = GetY(g_pPrincess);
  SetCommand(g_pKing, "MoveTo", 0, mx-7, my);
  SetCommand(g_pPrincess, "MoveTo", 0, mx+7, my);
  }

protected func Script3()
  {
  g_pDragon->IntroControl(400, 1050);
  }

protected func Script15()
  {
  g_pPrincess->SetCommand(0, "MoveTo", 0, g_pPrincess->GetX()+10, g_pPrincess->GetY());
  MessageEx("$MsgIntro1$", g_pPrincess); // Was ist das?  
  return(ScriptGo(0));
  }
  
// Scriptausführung fortsetzen, sobald der Drache das Flugziel erreicht  
public func OnDragonReachTarget() { goto(20); return(ScriptGo(1)); }

protected func Script21()
  {
  // Drache hat das Lager erreicht
  g_pDragon->IntroControl(320, 1111, true);
  g_pDragon->SetXDir(); g_pDragon->SetYDir();
  g_pDragon->SetPhysical("Float", 5, PHYS_Temporary);
  g_pDragon->SetComDir(COMD_Stop());
  g_pDragon->DoFireBreath();
  g_pDragon->Sound("DragonRoar");
  }
  
protected func Script25()
  {
  MessageEx("$MsgIntro2a$", g_pEndboss); // Komm mit mir!
  var fx=GetX(g_pPrincess), fy=GetY(g_pPrincess), tx=2145, ty=485;
  DrawParticleLine("NoGravSpark", fx,fy, tx,ty, 30, 150, RGBa(127, 255, 255, 63), RGBa(127, 255, 255, 63));
  Sound("PrincessScream", false, g_pKing);
  Sparkle(5, fx, fy);
  //Sparkle(5, tx, ty); - sieht man eh nicht
  SetPosition(tx, ty, g_pPrincess);
  g_pPrincess->SetAction("Walk");
  g_pPrincess->SetDir(DIR_Left);
  g_pEndboss->SetAction("RideMagic", g_pDragon);
  Sound("Magic1", true);
  }
  
protected func Script30()
  {
  MessageEx("$MsgIntro2$", g_pKing); // Oh nein,|wo ist sie hin?
  g_pKing->Jump();
  g_pKing->SetXDir(g_pKing->GetXDir()/2);
  g_pKing->SetYDir(g_pKing->GetYDir()/2);
  }
  
protected func Script40()
  {
  MessageEx("$MsgIntro3$", g_pEndboss); // Harr harr!|Die verarbeite ich jetzt zu Haargel!
  }
  
protected func Script52()
  {
  MessageEx("$MsgIntro4$", g_pKing); // Du Schurke!
  }
  
protected func Script56()
  {
  g_pEndboss->SetAction("RideMagic");
  MessageEx("$MsgIntro5a$", g_pEndboss); // Nichts wirst Du!
  Sound("Magic1", true);
  }
  
protected func Script58()
  {
  MessageEx("$MsgIntro5$", g_pKing); // Aaaahh
  g_pEndboss->SetAction("RideMagic", g_pDragon);
  g_pKing->Incinerate();
  g_pKing->DoEnergy(-g_pKing->GetEnergy() + 7);
  g_pKing->Fling(g_pKing, -1, -3);
  g_pDragon->SetPhysical("Float", 500, PHYS_Temporary);
  g_pDragon->ResetPhysical();
  g_pDragon->IntroControl(1350, 450);
  }
  
protected func Script70()
  {
  g_pDragon->SetPosition(2060, 480);
  g_pDragon->IntroControl();
  g_pDragon->SetXDir(); g_pDragon->SetYDir();
  g_pDragon->SetAction("Sleep");
  g_pDragon->SetR();
  g_pEndboss->AI_Enable();
  g_pEndboss->SetAction("Walk");
  g_pEndboss->SetPosition(g_iEndbossX, g_iEndbossY-20);
  g_pEndboss->SetObjectLayer();
  g_pEndboss->SetCategory(8);
  g_pEndboss->SetObjectBlitMode(1);
  }
  
protected func Script80()
  {
  var szMsg;
  if (GetPlayerCount(C4PT_User) > 1) szMsg = "$MsgIntro6b$"; else szMsg = "$MsgIntro6a$"; // Oh nein, wir müssen sie retten
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  szMsg = Format("%s:|%s", GetName(pClonk), szMsg);
  Message(szMsg, pClonk);
  ScriptGo(0);
  }
  
private func IntroSkip()
  {
  Script25();
  Script58();
  Script70();
  goto(81);
  ScriptGo();
  }
  
// Schaden abblocken
global func FxDivinityDamage() { return(0); }

// Mana nachliefern
global func FxDivinityTimer(object o) { DoMagicEnergy(1, o); }
    
  
/**********************************************************************\

 **********************************************************************
 
 **********************************************************************
 
 * * *                                                            * * *
 
 * * *                                                            * * *
 
 * * *                     I    N    T    R    O                  * * *
 
 * * *                                                            * * *
                              E   N   D   E
 * * *                                                            * * *
 
 * * *                                                            * * *
 
 **********************************************************************
 
 **********************************************************************
 
\**********************************************************************/



/* Magier tot? */

public func OnClonkDeath(object pClonk)
  {
  if (!pClonk) return();
  if (pClonk == g_pPrincess)
    {
    Message("$MsgPrincessDead$");
    var i = GetPlayerCount(C4PT_User);
    while (i--)
     EliminatePlayer(GetPlayerByIndex(i, C4PT_User));
    }
  if (pClonk == g_pEndboss)
    {
    // Evtl. Schlüssel erschaffen
    if (!ObjectCount(_KEY))
      CreateObject(_KEY, GetX(pClonk), GetY(pClonk), -1);

    // Alle Duplikate des Endmagiers werden gelöscht
    var pMage;
    while (pMage = FindObject(MAGE, 0,0,0,0, 0, 0,0, 0, pMage))
      if (pMage->GetOwner() == GetOwner(g_pEndboss))
        if (GetEffect("ReplicationSpell", pMage))
          RemoveEffect("ReplicationSpell", pMage);
     // Der Drache stirbt
     Kill(g_pDragon);
     
     // Der Endboss-Spieler ist tot
     EliminatePlayer(GetOwner(pClonk));
    }
  }

/* Prinzessin befreit */

public func OnCageOpened()
  {
  ScriptGo(1);
  return(goto(1000));
  }

protected func Script1006()
  {
  SetCommand(g_pPrincess, "MoveTo", 0, GetX(g_pPrincess)-40, GetY(g_pPrincess));
  MessageEx("$MsgOutro1$", g_pPrincess); // Ihr habt mich gerettet,|vielen Dank!
  }

protected func Script1018()
  {
  var szMsg;
  if (GetPlayerCount(C4PT_User) > 1) szMsg = "$MsgOutro2b$"; else szMsg = "$MsgOutro2a$"; // Wir konnten doch nicht zulassen,|dass dieser Schurke Euch zu Haargel verarbeitet!
  var pClonk = GetCursor(GetPlayerByIndex(0, C4PT_User));
  MessageEx(Format("%s",szMsg), pClonk);
  }

protected func Script1040()
  {
  MessageEx("$MsgOutro3$", g_pPrincess); // Nun sollten wir aber weg|von diesem schrecklichen Ort!
  }

protected func Script1042()
  {
  var pBlimp = CreateObject(BLMP, g_pPrincess->GetX()-40, g_pPrincess->GetY(), -1);
  pBlimp->SetObjDrawTransform(1,0,0,0,1);
  pBlimp->RemoveVertex(1);
  pBlimp->RemoveVertex(0);
  AddEffect("IntGrow", pBlimp, 300, 2);
  }

global func FxIntGrowTimer(object pTarget, int iEffectNumber, int iEffectTime)
  {
  pTarget->SetObjDrawTransform(iEffectTime/2 * 25, 0,0,0, iEffectTime/2 * 25);
  if (iEffectTime >= 80) return(-1);
  }

protected func Script1047()
  {
  SetCommand(g_pPrincess, "Grab", FindObject(BLMP));
  }

protected func Script1054()
  {
  MessageEx("$MsgOutro4$", g_pPrincess); // Steigt auf!
  }
  
protected func Script1060()
  {
  SetCommand(g_pPrincess, "MoveTo", 0, 2000,430);
  }
  
protected func Script1065()
  {
  SetCommand(g_pPrincess, "MoveTo", 0, 1500,500);
  }

protected func Script1080()
  {
  if (GetPlayerCount(C4PT_User) > 1) Message("$MsgOutro5b$"); else Message("$MsgOutro5a$");
  }

protected func Script1111()
  {
  GameOver(2000);
  return(ScriptGo());
  }
  
  
  
/* KI-Konvertierung */

public func ConvertAI()
  {
  var iAIPlr = GetPlayerByIndex(0, C4PT_Script), i;
  if (iAIPlr == NO_OWNER) return;
  for (var obj in FindObjects(Find_OCF(OCF_Alive), Find_Owner(NO_OWNER)))
    if (GetEffect("AI", obj) || GetID(obj) == KING)
      {
      var dwClr = GetColorDw(obj);
      var szName = GetName(obj);
      SetOwner(iAIPlr, obj);
      MakeCrewMember(obj, iAIPlr);
      SetColorDw(dwClr, obj);
      SetName(szName, obj);
      SetName(szName, obj, 0, true, true);
      ++i;
      }
  Log("%d Objekte umgewandelt!", i);
  return true;
  }
