/*--- Funnel Valley ---*/

#strict

static Huts;

protected func Initialize()
{
  // Gamma anpassen
  SetGamma(RGB(0,20,0), RGB(128,156,128), RGB(222,255,222) );
  // Hütten erzeugen und merken
  Huts = CreateArray(4);
  Huts[0] = CreateConstruction(HUT2,120,164,-1,100,1);
  Huts[1] = CreateConstruction(HUT3,1056,176,-1,100,1);
  Huts[2] = CreateConstruction(HUT3,252,320,-1,100,1);
  Huts[3] = CreateConstruction(HUT2,768,356,-1,100,1);
  // Set mission password
  FindObject(GLDM)->SetMissionAccess("Fenella");
}

protected func InitializePlayer(iPlr)
{
  // Der erste Spieler bekommt Geld
	if (GetPlayerCount() == 1) SetWealth(iPlr, 45);
  // Flagge erzeugen
  if (GetPlayerCount() <= 4) CreateObject(FLAG,0,0,iPlr)->Enter(GetHiRank(iPlr));
  // Eine freie Hütte suchen
  var pHut, hutCount;
  var h = Random(GetLength(Huts));
  for (var i = 0; i < GetLength(Huts); i++)
  {
    if (Huts[h])
      if ((Huts[h]->GetID() == HUT2) || (Huts[h]->GetID() == HUT3))
        if (!pHut || (Huts[h]->ContentsCount() < hutCount))
          { pHut = Huts[h]; hutCount = pHut->ContentsCount(); }
    h++; if (h >= GetLength(Huts)) h = 0;
  }
  // Spieler der gefundenen Hütte starten lassen
  if (pHut)
    GetHiRank(iPlr)->Enter(pHut);
}

func TACC_BlockFirstAllianceAdd() { return 1; }
