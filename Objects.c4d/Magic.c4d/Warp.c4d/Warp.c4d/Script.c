/*-- Warploch --*/

#strict

local active;
local warped;

private func WarpRange() { return(GetCon() ); }
private func WarpWhirlRange() { return(GetCon()/2); }
private func WarpPower() { return(GetCon()/4); }
private func WarpSpeed() { return(3); }
private func WarpLimit() { return(200); }

private func WarpRed() { if(GetActionTarget(1) ) return(96); return(255); }
private func WarpGreen() { if(GetActionTarget(1) ) return(128); return(255); }
private func WarpBlue() { return(255); }

func Initialize () {
  // Kompatibilität zum prä-CE-Verhalten
  SetAction("Warp");
  // Verhindere diverse Lebewesen-Effekte
  SetAlive(false);
}

protected func Connect(pEnd, pHome)
{
  // Aktionen setzen
  SetAction("Warp", pEnd, pHome);
  ObjectSetAction(pEnd, "Warp", 0, pHome);
  LocalN("active", pEnd) = false;
  SetCon(50, pEnd);
  pEnd->WARP::Shrink();
  // Warp-Effekt anfügen
  AddEffect("WarpUSpell", this(), 150, 2, this() );
  // Einfaden
  active = false;
  SetCon(50);
  Shrink();
}

protected func Destruction()
{
  // Endloch löschen
  if(GetActionTarget() ) RemoveObject(GetActionTarget() );
}

private func Shrink()
{
  if(!active)
  {
    // Größer werden, wenn nicht aktiv
    DoCon(8);
    if(GetCon() >= 100) active = true;
  }
  else
  {
    // Immer kleiner werden
    DoCon(-5);
    if(GetCon() < 50) return(RemoveObject() );
  }
  // Ausfaden
  SetClrModulation(RGBa(WarpRed(), WarpGreen(), WarpBlue(), (100 - BoundBy(GetCon(), 0, 100)) * 3) );
}

// ----------------------------------------------------------------------------

protected func FxWarpUSpellStop(pTarget, iEffect, iReason, fTemp)
{
  // Alle Objekte aus der Liste wieder resetten
  while(EffectVar(0, pTarget, iEffect) )
    DeleteWarpObject(EffectVar(1, pTarget, iEffect), pTarget, iEffect);
}

protected func FxWarpUSpellTimer(pTarget, iEffect)
{
  // Noch nicht aktiv?
//  if(!active) return(0);
  if(warped > WarpLimit() ) return(RemoveObject() );

  var obj;
  // Alle Objekte im Umkreis durchsuchen
  for(obj in FindObjects(Find_Distance(WarpRange()), Find_NoContainer(), Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Object))))
      // Keine attachten Objekte (Flagge von Hütte wegwarpen oder so wäre extrem böse) und
      // keine feststeckenden Objekte (explodierende Feuersteine im Erdreich)
      if (GetProcedure(obj) ne "ATTACH" &&
         ((GetCategory(obj) & C4D_Living()) || !Stuck(obj)))
        // Objekt noch nicht vorhanden oder nicht bereits von einem anderen Warp eingesaugt?
        if(!GetEffect("WarpUSpellData", obj, 0, 1) )
          // Keine Warplöcher wegwarpen oder Kraftfelder ;)
          if(GetID(obj) != WARP && !obj->~IsForceField())
          {
            // Distanz bestimmen.
            var d = ObjectDistance(obj);
            // In Reichweite und aktiv?
            if(d < WarpWhirlRange() && (EffectVar(0, pTarget, iEffect) < 3) && active)
              // Wegwarpen
              AddWarpObject(obj, pTarget, iEffect);
            // Bisschen weiter weg oder inaktiv? Bisschen anziehen.
            else if(d < WarpRange() && !Random(3) )
            {
              var fac = (GetCon() - 50) * 2;
              var angle = Angle(GetX(), GetY(), GetX(obj), GetY(obj) );
              // Stärker anziehen, wenn das Objekt läuft, da es durch die Reibung am Boden sonst soviel Geschwindigkeit
              // verliert, dass es sich gar nicht erst bewegt.
              SetXDir(GetXDir(obj, 10000) - Sin(angle, 65 * fac + (120 * fac * (GetProcedure(obj) S= "WALK")) ), obj, 10000);
              SetYDir(GetYDir(obj, 10000) + Cos(angle, 180 * fac ), obj, 10000);
              AdjustObjectBorderSpeed(obj);
            }
          }

  // Schon aktiv?
  if(!active) return(0);

  // Alle Objekte durchgehen
  for(var i = 0; i < EffectVar(0, pTarget, iEffect); ++ i)
  {
    // Objekt holen
    var obj = EffectVar(i + 1, pTarget, iEffect);
    // Objekt nicht mehr vorhanden (gelöscht oder so?)
    if(!obj) { DeleteWarpObjectNum(i, pTarget, iEffect); --i; continue; }
    // ExecWarpObject gibt 0 zurück, wenn das Objekt so nah am WarpKern ist, dass es zum Ziel gewarpt wurde.
    if(!ExecWarpObject(obj, pTarget, iEffect) )
      { DeleteWarpObjectNum(i, pTarget, iEffect); --i; continue; }
  }
}

// ----------------------------------------------------------------------------

private func GetWarpObject(pObj, pTarget, iEffect)
{
  // Alle Objekte durchgehen
  for(var i = 0; i < EffectVar(0, pTarget, iEffect); ++ i)
    // Dieses gesucht?
    if(EffectVar(i + 1, pTarget, iEffect) == pObj)
      // Ke, das wars auch schon
      return(i);
  // Nicht gefunden. :(
  return(-1);
}

private func AddWarpObject(pObj, pTarget, iEffect)
{
  // Masse aufaddieren
  warped += GetMass(pObj);
  // Objekt zur Liste hinzufügen
  EffectVar(++EffectVar(0, pTarget, iEffect), pTarget, iEffect) = pObj;
  // Effekt an dieses Objekt als simplen Datenspeicher dranhängen
  AddEffect("WarpUSpellData", pObj, 150, 0, this(), 0, ObjectDistance(pObj) );
}

private func DeleteWarpObject(pObj, pTarget, iEffect)
{
  // Effekt aus der Liste austragen
  var pos = GetWarpObject(pObj, pTarget, iEffect);
  // Objekt gar nicht vorhanden
  if(pos == -1) return(0);
  return(DeleteWarpObjectNum(pos, pTarget, iEffect) );
}

private func DeleteWarpObjectNum(iPos, pTarget, iEffect)
{
  // Objekt holen
  var obj = EffectVar(1 + iPos, pTarget, iEffect);
  // Alle anderen Effekte aufrücken lassen
  for(var i = iPos + 1; i < EffectVar(0, pTarget, iEffect); ++ i)
    EffectVar(i, pTarget, iEffect) = EffectVar(i + 1, pTarget, iEffect);
  // Objektlistencounter um eins verringern
  EffectVar(0, pTarget, iEffect)--;
  // Effekt löschen
  if(obj) RemoveEffect("WarpUSpellData", obj);
}

private func ExecWarpObject(pObj, pTarget, iEffect)
{
  // Kein USpellData da?
  if(!GetEffect("WarpUSpellData", pObj, 0, 1) )
    // Das Objekt scheint also schon irgendwie aus der Liste  entfernt zu sein.
    return(0);
  // Effektnummer vom WarpUSpellData ergattern
  var effectnum = GetEffect("WarpUSpellData", pObj);
  // Stärke abholen
  var strength = EffectVar(0, pObj, effectnum);
  EffectVar(0, pObj, effectnum) -= WarpSpeed();

  // Winkel berechnen
  var agl = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj) );
  // Lebewesen werden geflingt
  if(GetCategory(pObj) & C4D_Living() && GetAlive(pObj) )
  {
    var phase = GetPhase(pObj);
    Fling(pObj, Sin(agl - 115, strength/10), -Cos(agl - 115, strength/10) );
    SetPhase(phase, pObj);
  }
  // Objekte gesetspeedet
  else

    SetSpeed(Sin(agl - 115, strength), -Cos(agl - 115, strength), pObj);

  // Objektbewegungen anpassen, damit es nicht zum Rand rausfällt
  AdjustObjectBorderSpeed(pObj);

  // Objekt im Mittelpunkt? Dann mal beamen.
  if(strength <= 25)
    return(TransferWarpObject(pObj) );

  // Ist das Objekt irgendwie wieder aus dem Einflussradius rausgekommen?
  // Egal wie, auf jeden Fall mal aus der Liste werfen.
  if(ObjectDistance(pObj) > WarpWhirlRange() * 4 / 3) return(0);
  return(1);
}

private func AdjustObjectBorderSpeed(object pObj)
  {
  // Gestrudelte Objekte nicht aus der Karte schleudern
  if( GetX(pObj) + GetXDir(pObj,2) < 5)
    SetXDir(0, pObj);
  else if( GetX(pObj) + GetXDir(pObj, 2) > LandscapeWidth() - 5)
    SetXDir(0, pObj);
  if( GetY(pObj) + GetYDir(pObj, 2) > LandscapeHeight() - 5)
    SetYDir(0, pObj);
  }

private func TransferWarpObject(pObj)
{
  // Nach Möglichkeit ins Zielobjekt
  if(GetActionTarget(1) )
    // Zielobjekt muss noch zugänglich sein (#2080)
    if(GetActionTarget(1)->GetOCF() & OCF_Entrance)
    {
      Enter(GetActionTarget(1), pObj);
      return(0);
    }

  // Sonst einfach ans Ziel transferieren
  SetPosition(GetX(GetActionTarget()), GetY(GetActionTarget()), pObj);
  // Delete from list
  return(0);
}

// ----------------------------------------------------------------------------

protected func FxWarpUSpellDataStart(pTarget, iEffect, iTemp, iDist)
{
  // WarpUSpellData-Start: Warp-Abstand zwischenspeichern
  EffectVar(0, pTarget, iEffect) = (iDist * WarpPower() * 10 / WarpRange() );
  // Anzahl an Vertices speichern
  EffectVar(1, pTarget, iEffect) = GetVertexNum(pTarget);
  // Einzelne Vertices löschen, damit das Objekt durch Material gewirbelt werden kann, Vertices aber speichern
  var vtxcnt;
  while(GetVertexNum(pTarget) )
  {
    EffectVar(2 + vtxcnt*2, pTarget, iEffect) = GetVertex(0, 0, pTarget);
    EffectVar(2 + vtxcnt*2 + 1, pTarget, iEffect) = GetVertex(0, 1, pTarget);
    RemoveVertex(0, pTarget);
    vtxcnt++;
  }
}

protected func FxWarpUSpellDataStop(pTarget, iEffect, iReason, fTemp)
{
  // Einzelne Vertices wieder zurückspeichern
  for(var i = 0; i < EffectVar(1, pTarget, iEffect); ++ i)
    AddVertex(EffectVar(2 + i*2, pTarget, iEffect), EffectVar(2 + i*2 + 1, pTarget, iEffect), pTarget);
}
