/* --- Paxhill - Krone der Ewigkeit --- */

#strict 2

/* -- Variablen -- */

static const G_MaxRejoins = 3;
static const TEAM_Left = 1, TEAM_Right = 2;

static g_rejoin_count; // Anzahl der verbleibenden Rejoins für den Spieler (Array)
static g_leftjoined, g_rightjoined;


/* -- Spielinitialisierung -- */

protected func Initialize()
  {
  // Capture the crown
  if (!ObjectCount(_CTC)) CreateObject(_CTC,50,50,NO_OWNER);
  // Melee
  if (!ObjectCount(MELE)) CreateObject(MELE,50,50,NO_OWNER);
  // Team accounts
  if (!ObjectCount(TACC)) CreateObject(TACC,50,50,NO_OWNER);
  // Rejoin-Array
  if (!g_rejoin_count) g_rejoin_count = CreateArray();
  // Fahrstuhlschächte bohren
  for (var elev in FindObjects(Find_ID(CPEL)))
    elev->CreateShaft(250);
  return true;
  }


/* -- Spielerinitialisierung -- */

protected func InitializePlayer(int plr)
  {
  // Rejoins
  g_rejoin_count[plr] = G_MaxRejoins;
  // Baupläne
  WPPL->SetKnowledge(plr);
  CPPL->SetKnowledge(plr);
  // Direkter Beitritt
  return DoPlrJoin(plr);
  }

private func DoPlrJoin(int plr, bool is_rejoin)
  {
  // Rejoinnachricht
  if (is_rejoin)
    {
    Log("$MsgRejoin$", GetPlayerName(plr), g_rejoin_count[plr]);
    }
  // Spieler in der Burg plazieren: Linke Burg
  if (GetPlayerTeam(plr) == TEAM_Left)
    {
    // Linke Burg noch nicht zugewiesen?
    if (!g_leftjoined) { AquireBase(plr,    0, 0, 550, 550); g_leftjoined = true; }
    // Plazieren
    PlacePlr(plr,   40, 190);
    }
  // Rechte Burg
  if (GetPlayerTeam(plr) == TEAM_Right)
    {
    // Rechte Burg noch nicht zugewiesen?
    if (!g_rightjoined) { AquireBase(plr, 1370, 0, 550, 550); g_rightjoined = true; }
    // Plazieren
    PlacePlr(plr, 1820, 180);
    }
  // Feddich
  return true;
  }

private func AquireBase(int plr, int x, int y, int wdt, int hgt)
  {
  // Alles im Zielrechteck...                             ...wenn es nicht lebt...
  for (var obj in FindObjects(Find_InRect(x,y,wdt,hgt), Find_Not(Find_OCF(OCF_Living)),
   // ...und noch keinem gehört......und keine internen Objekte...
   Find_Owner(NO_OWNER), Find_Not(Find_Category(C4D_Rule|C4D_Goal|64 /* C4D_Environment */))))
    {
    SetOwner(plr, obj); // ...in Besitz nehmen
    }
  // Fertig =)
  return true;
  }

private func PlacePlr(int plr, int x, int y)
  {
  // Alle Clonks des Spielers...
  var i, crew;
  while (crew = GetCrew(plr, i++))
    {
    // ...in etwa an der Zielposition plazieren
    crew->SetPosition(x-19+Random(40), y);
    }
  // Eine Flagge gibts auch gleich noch
  Enter(GetHiRank(plr), CreateObject(FLAG, x,y, plr));
  // OK, fertig
  return true;
  }

public func RelaunchPlayer(int plr)
  {
  // Sicherheit
  if (plr == NO_OWNER) return -1;
  // Noch ein Rejoin übrig?
  if (!g_rejoin_count[plr]) return;
  // Dann abziehen und rejoinen
  --g_rejoin_count[plr];
  // Neuen Clonk erzeugen
  var crw,obj;
  MakeCrewMember(crw=CreateObject(KNIG,0,0,plr),plr);
  DoEnergy(1000, crw);
  SetCursor(plr, crw);
  SelectCrew(plr, crw, 1);
  // Sofortiger Rejoin
  return DoPlrJoin(plr, true);
  }
