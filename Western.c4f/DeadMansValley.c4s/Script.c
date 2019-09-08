/*-- Dead Man's Valley --*/

#strict

static FragLimit;

// Ein paar Spieler Arrays
global func & Kills(int iPlr)   { return (Global(iPlr*8+0)); }
global func & Deaths(int iPlr)  { return (Global(iPlr*8+1)); }
global func & Clonk(int iPlr, iIndex)  { return (Global(iPlr*8+2+iIndex)); }

func Initialize() {
  FragLimit = 10;
        
  // Regeln und Ziel   
  CreateObject(_ETG, 100, 100, -1); // Löscher
  CreateObject(DRSI, 100, 100, -1); // Angebot Drugstore
  CreateObject(REAC, 100, 100, -1); // Entlassen
  
  CreateObject(FRAG, 100, 100, -1); // Einstellungen
    
  CreateObject(BKIL, 100, 100, -1); // Spielziel
              
  // Fahrstühle aufbauen und Schächte graben 
  CreateConstruction(ELEV, 60, 151, NO_OWNER, 100, 1)->CreateShaft(240);
  CreateConstruction(ELEV, 689, 450, NO_OWNER, 100, 1)->CreateShaft(180);
  CreateConstruction(ELEV, 900, 210, NO_OWNER, 100, 1)->CreateShaft(250);
  CreateConstruction(ELEV, 1181, 330, NO_OWNER, 100, 1)->CreateShaft(150);
  CreateConstruction(ELEV, 1480, 330, NO_OWNER, 100, 1)->CreateShaft(240);
 
  // Telepraphenmasten unzerstörbar
  var obj;
  while(obj = FindObject(TEL1, 0, 0, 0, 0, 0, 0, 0, 0, obj)) { AddEffect("NoBurning", obj, 300,0); SetObjectLayer(obj,obj);}
 
  // Nachlieferung Waffenlager
  AddEffect("Supply", FindObject(AMRY, 1302, 306, -1, -1), 1, 4000, 0, 0, AMBO, 5);
  AddEffect("Supply", FindObject(AMRY, 1302, 306, -1, -1), 1, 10000, 0, 0, DYNB, 2);

  // Kanonenkugeln und Schießpulver für die Kanonen
  AddEffect("Supply", FindObject(CCAN, 1294, 266, -1, -1), 1, 6000, 0, 0, CABL, 4);
  AddEffect("Supply", FindObject(CCAN, 1294, 266, -1, -1), 1, 6000, 0, 0, GUNP, 4);
  AddEffect("Supply", FindObject(CCAN, 178, 149, -1, -1), 1, 6000, 0, 0, CABL, 4);
  AddEffect("Supply", FindObject(CCAN, 178, 149, -1, -1), 1, 6000, 0, 0, GUNP, 4);

  // Geldsäcke für die Kisten
  AddEffect("SupplyChest", FindObject(CHST, 45, 477, -1, -1), 1, 60*32, 0, 0, 15);
  AddEffect("SupplyChest", FindObject(CHST, 539, 527, -1, -1), 1, 60*32, 0, 0, 10);
  AddEffect("SupplyChest", FindObject(CHST, 761, 528, -1, -1), 1, 60*32, 0, 0, 10);
  AddEffect("SupplyChest", FindObject(CHST, 996, 588, -1, -1), 1, 60*32, 0, 0, 10);
  AddEffect("SupplyChest", FindObject(CHST, 1488, 666, -1, -1), 1, 60*32, 0, 0, 15);

  // Gebäude sollen mehr aushalten
  var obj;
  while(obj = FindObject(0, 0, 0, -1, -1, OCF_Exclusive, 0, 0, 0, obj))
  {
    if(GetID(obj)==ELEV) { AddEffect("LessDamage", obj, 400, 0, 0, 0, 25); SetCategory(1, obj);}
    else  AddEffect("LessDamage", obj, 400, 0, 0, 0, 50);
  }
  obj = 0;
  while(obj = FindObject(BRDG, 0, 0, -1, -1, 0, 0, 0, 0, obj))
  {
    AddEffect("LessDamage", obj, 400, 0, 0, 0, 50);
  }
  obj = 0;
  while(obj = FindObject(ELEC, 0, 0, -1, -1, 0, 0, 0, 0, obj))
  {
    AddEffect("LessDamage", obj, 400, 0, 0, 0, 25);
  }

  // Scorboard
  SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("$MsgFraglimit$", FragLimit),                    SBRD_Caption);
  SetScoreboardData(SBRD_Caption, ScoreboardCol(KILL),       "{{REVR}}",              ScoreboardCol(KILL));  
  SetScoreboardData(SBRD_Caption, ScoreboardCol(DEAD),       "{{SKUL}}",              ScoreboardCol(DEAD));

  return(1);
}

static fScoreboard;

protected func InitializePlayer(int iPlr)
  {
  // mit 0 Kills anfangen
  Kills(iPlr) = 0;

  // Scoreboard-Überschrift initialisieren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       SBRD_Caption, GetTaggedPlayerName(iPlr), GetPlayerID(iPlr));
  
  // Anfangswerte einsetzen
  UpdateScoreboard(iPlr, false);

  // Spieler mit allen anderen verfeinden
  var iHostilePlr; 
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iHostilePlr = GetPlayerByIndex(i);   
    if(iHostilePlr==iPlr) continue;
    SetHostility(iPlr, iHostilePlr, 1, 1); 
    SetHostility(iHostilePlr, iPlr, 1, 1); 
  }
  
  // Startausrüstung geben
  CreateContents(DYNA, GetCrew(iPlr));
  CreateContents(AMBO, GetCrew(iPlr)); 
  CreateContents(AMBO, GetCrew(iPlr));
  CreateContents(REVR, GetCrew(iPlr));
  return(1);
  }

global func UpdateScoreboard(bool fShow)
  {
  var iPlr;
  var iWinner;
  // alle Spieler durchgehen
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iPlr = GetPlayerByIndex(i);   
    // Daten eintragen
    SetScoreboardData(GetPlayerID(iPlr),       ScoreboardCol(KILL),       Format("%d", Kills(iPlr)), Kills(iPlr));
    SetScoreboardData(GetPlayerID(iPlr),       ScoreboardCol(DEAD),       Format("%d", Deaths(iPlr)), Deaths(iPlr));
    // Hat einer schon das Fraglimit erreicht?
    if(Kills(iPlr)>=FragLimit) iWinner = iPlr+1;
  }
  // Sortieren nach Frags
  SortScoreboard(ScoreboardCol(KILL), true);
  // Scoreboard zeigen
  if (fShow)
    {
    DoScoreboardShowAll(1, 5);
    }
  // Gewinner?
  if(iWinner) Win(iWinner-1);
  return(true);
  }

protected func RemovePlayer(iPlr)
  {
  // Relaunchzähler ium Scoreboard ausleeren
  var iPlrID = GetPlayerID(iPlr);
  SetScoreboardData(iPlrID,       ScoreboardCol(KILL),       0, -1);
  SetScoreboardData(iPlrID,       ScoreboardCol(DEAD),       0, -1);
  SortScoreboard(ScoreboardCol(KILL), true);
  }

public func RelaunchPlayer(int iPlr, int iDeathCausedBy)
  {
  // Kein Relaunch für eliminierte Spieler
  if(!GetPlayerName(Par())) return();
  
  // Spieler neu beitreten lassen
  var pNewClonk = CreateObject(BNDT, 0,0, iPlr);
  // Entweder neue Infosection hohlen oder eine alte nehmen,
  // so dass nur die 5 besten Clonks mitspielen verwendet werden
  if(!Clonk(iPlr, 4)) MakeCrewMember(pNewClonk, iPlr);
  if(Clonk(iPlr, 4))
  {
    GrabObjectInfo(Clonk(iPlr,0), pNewClonk);
    var i;
    while(Clonk(iPlr, i+1)) 
    {
      if(i>3) break;	    
      Clonk(iPlr, i) = Clonk(iPlr, i+1);
      i++;
    }
    Clonk(iPlr, 4) = 0;
  }
  // Cursor und Sicht
  SetCursor(iPlr,  GetCrew(iPlr));
  SetPlrView(iPlr, GetCrew(iPlr));
  // Ausrüstung
  CreateContents(DYNA, GetCrew(iPlr));
  CreateContents(AMBO, GetCrew(iPlr));
  CreateContents(REVR, GetCrew(iPlr));

  var pBuild = GetRandomBuilding();
  // Jetzt muss der Spieler noch etwas warten
  CreateObject(_PLA, GetX(pBuild), GetY(pBuild), iPlr)->RelaunchPlayer(iPlr);
  
  // Keine weiteren Einstellungen mehr
  LocalN("fNoMoreSettings", FindObject(FRAG)) = 1;
  }

static iScoreboardShow;

global func DoScoreboardShowAll(iAmount, iTime)
{
    // Scoreboard wird angezeigt	
    ResetScoreboard();
    DoScoreboardShow(iAmount);
    iScoreboardShow += iAmount;
    if(iTime) // Timer, damit das Scoreboard nicht ewig bleibt
    {
        goto(Max(1000-iTime*3, 0));
	ScriptGo(1);
    }
}

global func ResetScoreboard()
{
    // Scoreboardtimer resetten	
    DoScoreboardShow(-iScoreboardShow);
    iScoreboardShow=0;
    ScriptGo(0);
}

protected func Script1000() { ResetScoreboard(); }

// Spieler plazieren
global func DoPlace(pObj, fInside, fGoOut, fNear)
{
  // In einem Gebäude	
  if(fInside)
  {
    // Zufällig oder das nächste	  
    var pBuilding = GetRandomBuilding();
    if(fNear) pBuilding = GetNearBuilding();
    // Wenn gefunden, dann rein damit
    if(pBuilding)
    {
      Enter(pBuilding, pObj);
      if(fGoOut) SetCommand(pObj, "Exit");
      return(1);
    }
  }
  // Sonst zufällige Landschaftsposition
  var pWipf = PlaceAnimal(WIPF);
  SetPosition(GetX(pWipf),GetY(pWipf), pObj);
  RemoveObject(pWipf);
}

global func GetRandomBuilding()
{
  // Liste mit allen Gebäuden machen	
  var obj, count;
  while(obj = FindObject(0, 0, 0, 0, 0, OCF_Entrance, 0, 0, 0, obj)) 
    if(GetID(obj)!=WGTW && GetID(obj)!=CSTE) // Keine Türme oder Treppen nehmen
     { Var(count++) = obj; }
  // Zufällig eines auswählen
  return(Var(Random(count)));
}

global func GetNearBuilding()
{
  // Nähestes Gebäude finden	
  var obj, count;
  while(obj = FindObject(0, 0, 0, -1, -1, OCF_Entrance, 0, 0, 0, obj)) 
    if(GetID(obj)!=WGTW && GetID(obj)!=CSTE) // Keine Türme oder Treppen nehmen
     { return(obj); }
  return(0);
}

global func Win(iWinner)
{
  // Alle Verlierer eliminieren
  var iPlr = 0; 
  for(var i = 0; i < GetPlayerCount(); i++)
  {
    iPlr = GetPlayerByIndex(i);   
    if(iPlr!=iWinner) EliminatePlayer(iPlr);
  }
  fWon = 1; // Spielziel ist erfüllt
//  GameOver(); // Ende
}

// Geldgeschäfte sollen nicht durch Vandalismus gestört werden
global func GetBank(object pMessage, bool silent)
{
  return(1);
}

// Nachschub
global func FxSupplyStart(pTarget, iNumber, fTmp, idID, iAmount)
{
  if(fTmp) return();    
  EffectVar(0, pTarget, iNumber) = idID;
  EffectVar(1, pTarget, iNumber) = iAmount;
}

global func FxSupplyTimer(pTarget, iNumber)
{
  if(ContentsCount(EffectVar(0, pTarget, iNumber), pTarget)<EffectVar(1, pTarget, iNumber)) CreateContents(EffectVar(0, pTarget, iNumber), pTarget);    
}

// Geldsäcke in Kisten respawnen
global func FxSupplyChestStart(pTarget, iNumber, fTmp, iValue)
{
  if(fTmp) return();    
  if(!iValue) iValue = 10;
  EffectVar(0, pTarget, iNumber) = iValue;
}

global func FxSupplyChestTimer(pTarget, iNumber)
{
  // Wenn kein Geldsack da ist und keiner hinschaut, einen neuen erstellen und Truhe schließen    
  if(!ContentsCount(MBAG, pTarget) && !FindObject(0, 0, 0, 0, 0, 0, "Push", pTarget))  
  { 
    CreateContents(MBAG, pTarget)->~SetValue(EffectVar(0, pTarget, iNumber));
    if(!(GetAction(pTarget) S= "Closed")) ObjectSetAction(pTarget, "Closing");  
  }
}

// Schutz vor verbrennen
global func FxNoBurningEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber, var1, var2, var3)
  {
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return(-1);
  // Alles andere ist OK
  return();
  }

global func FxNoBurningEffect(string szNewEffect)
  {
  if(szNewEffect eq "NoBurning") return(-1); // Einer reicht
  }

// Weniger Schaden
global func FxLessDamageStart(pTarget, iNumber, fTmp, iAmount)
{
  if(fTmp) return();    
  if(!iAmount) iAmount = 50;
  EffectVar(0, pTarget, iNumber) = iAmount;
}

global func FxLessDamageDamage (pTarget, iNumber, iDmgEngy, iCause)
{
  if(iCause == 1) return(iDmgEngy*EffectVar(0, pTarget, iNumber)/100);
  return(iDmgEngy);   
}
