/*-- Kleines Handgemenge --*/

#strict

/*static g_iCrewCount;*/
static g_iSpawnCount;

protected func Initialize() 
{
  ScriptGo(1);  
  return(1);
}

/*protected func InitializePlayer(iPlr)
{
  // Clonks auch für Spätjoiner
  if (g_iCrewCount) InitializePlayer2(iPlr);
}

protected func InitializePlayer2(iPlr)
{
  // Clonks an dieselbe Stelle wie den Erstclonk. Fällt am wenigsten auf
  var pHiRank = GetHiRank(iPlr);
  if (pHiRank) for (var i=1; i<g_iCrewCount; ++i)
    {
    var pClonk = CreateObject(CLNK, GetX(pHiRank), GetDefBottom(pHiRank), iPlr);
    ObjectSetAction(pClonk, "Walk");
    MakeCrewMember(pClonk, iPlr);
    DoEnergy(+500, pClonk);
    SetDir(GetDir(pHiRank), pClonk);
    SetCommand(pClonk, "Follow", pHiRank); // geht nicht :C
    }
}

protected func Script1()
{
  // Jetzt gibts erstmal Clonknachschub für alle: Nach Durchschnittsrang der besten Clonks
  var pHiRank, n, iRank;
  for (var i=0; i<GetPlayerCount(); ++i)
    if (pHiRank = GetHiRank(GetPlayerByIndex(i)))
      {
      iRank += GetRank(pHiRank);
      ++n;
      }
  iRank = (iRank+n/2)/n;
  g_iCrewCount = BoundBy(iRank/3, 1, 5);
  // Zusätzliche Clonks erzeugen
  if (g_iCrewCount)
    for (var i=0; i<GetPlayerCount(); ++i)
      InitializePlayer2(GetPlayerByIndex(i));
}*/

protected func Script100()
{
  // Mehr Spieler brauchen mehr Waffen
  var n_failures = 0;
  for (var i=0; i<GetPlayerCount(); ++i)
    {
    // Platz im Erdreich finden
    var x = 10 + Random(LandscapeWidth() - 20);
    var h = GetHorizonHeight(x);
    var y = h + 10 + Random(LandscapeHeight() - h - 20);
    if (GetMaterial(x, y) != Material("Earth"))
    {
      // Keine Erde? Macht nichts; bis zu 20 Versuche
      ++n_failures;
      if (n_failures < 20) --i;
      continue;
    }
    // Objekt per verzögertem Effekt erzeugen
    var effect = AddEffect("SpawnSparkle", 0, 1, 5, 0, 0);
    EffectVar(0, 0, effect) = getSpawnType(); EffectVar(1, 0, effect) = x; EffectVar(2, 0, effect) = y;
    }
  // Spawns mitzählen
  g_iSpawnCount++;    
}

protected func Script150()
{
  goto(100);
}
  
private func getSpawnType()
{
  var id = [STFN, SFLN, EFLN, FBMP, ZAPN];
  return (id[Random(1 + Min(g_iSpawnCount/5, GetLength(id)))]);
}  

private func GetHorizonHeight(x)
{
  var y;
  while (y < LandscapeHeight() && !GBackSemiSolid(x, y))
    y += 10;
  while (y > LandscapeHeight() && GBackSemiSolid(x, y))
    --y;
  return(y);
}

global func FxSpawnSparkleTimer(obj, effect, time)
{
  // Variablen auswerten
  var id = EffectVar(0, 0, effect); var x = EffectVar(1, 0, effect); var y = EffectVar(2, 0, effect);
  // Funken erzeugen
  CreateParticle("NoGravSpark", x + RandomX(-6,+6), y + RandomX(-6,+6), 0, -3, RandomX(30,50), RGB(64,64,255));
  // Objekt erzeugen
  if (time == 100)
    EffectVar(3, 0, effect) = CreateObject(id, x, y);
  // Fertig
  if (time >= 150) return(-1);
}
