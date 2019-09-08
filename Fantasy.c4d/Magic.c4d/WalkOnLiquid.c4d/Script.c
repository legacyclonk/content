/*-- Wasserlaufen --*/

#strict

protected func Activate(object pCaster, object pRealCaster)
{
	// Effekte (mehr?)
	Sound("Magic*");
	// Effekt erzeugen
	if(!pRealCaster) pRealCaster = pCaster;
  if(!AddEffect("WalkOnLiquidSpell", pRealCaster, 100, 1, 0, GetID(), 1000))
		return(false);
  return(RemoveObject());
}

protected func CheckWalkOnLiquid(object pTarget, int iVtx, int iVtxY, bool fActive)
{
	// Wasserlaufen ist eine empfindliche Angelegenheit...
	if(!Inside(pTarget->GetYDir(), -30, 30)) return(false);
	if(!Inside(pTarget->GetXDir(), -50, 50)) return(false);
	// Richtige Tiefe?
	var iVtxX = pTarget->GetVertex(iVtx);
	if(!fActive)
	{
		if(!GBackLiquid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY - 2)) return(false);
		if(!GBackLiquid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY - 1)) return(false);
	}
	else
	{
		if(GBackSolid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY - 2)) return(false);
		if(GBackSolid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY - 1)) return(false);
	}
	if(!GBackLiquid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY)) return(false);
	if(GBackLiquid(pTarget->GetX() + iVtxX, pTarget->GetY() + iVtxY - 6)) return(false);
	return(true);
}

protected func FxWalkOnLiquidSpellStart(pTarget, iEffectNumber, fTemp, iDuration)
{
  if(!fTemp) EffectVar(0, pTarget, iEffectNumber) = iDuration;
	// Suche untersten Vertex, speichere Position (Laufvertex?)
	var iBest = -1;
	for(var i = 0; i < pTarget->GetVertexNum(); i++)
		if(!i || pTarget->GetVertex(iBest, true) < pTarget->GetVertex(i, true))
			iBest = i;
	// Nicht gefunden?
	if(iBest < 0) return(-1);
	// Speichern
	EffectVar(1, pTarget, iEffectNumber) = iBest;
}

protected func FxWalkOnLiquidSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
	// Effekt entfernen
	if(!fTemp && EffectVar(2, pTarget, iEffectNumber) > 0)
	{
		RemoveEffect(0,pTarget,EffectVar(2, pTarget, iEffectNumber));
		EffectVar(2, pTarget, iEffectNumber) = 0;
	}
}

protected func FxWalkOnLiquidSpellTimer(pTarget, iEffectNumber, iEffectTime)
{
	// Effekt zuende?
  var iDuration=EffectVar(0, pTarget, iEffectNumber);
	if(iEffectTime > iDuration) return(-1);
	// Prüfe, ob Effekt aktiviert werden soll
	var iVtx = EffectVar(1, pTarget, iEffectNumber), iActive = EffectVar(2, pTarget, iEffectNumber);
	if(iActive > 0) if(!GetEffect(0, pTarget, iActive)) iActive = 0;
	// Aktivieren?
	if(iActive <= 0)
		if(CheckWalkOnLiquid(pTarget, iVtx, pTarget->GetVertex(iVtx, true)))
			EffectVar(2, pTarget, iEffectNumber) = AddEffect("ActiveWalkOnLiquidSpell", pTarget, 100, 1, 0, GetID(), iVtx);
  return(1);
}

protected func FxWalkOnLiquidSpellEffect(szEffect, pTarget, iEffectNumber)
{
	// Übernehmen?
	if(szEffect eq "WalkOnLiquidSpell") return(-3);
	return(0);
}

protected func FxWalkOnLiquidSpellAdd(pTarget, iEffectNumber, szNewEffect, iNewEffectTimer, var1)
{
	// Verlängern
	EffectVar(0, pTarget, iEffectNumber) += var1;
	return(1);
}

protected func FxWalkOnLiquidSpellInfo() { return("{{WOLI}} $SpellInfo$"); }

protected func FxActiveWalkOnLiquidSpellStart(pTarget, iEffectNumber, fTemp, iVtx)
{
	if(!fTemp)
		EffectVar(0, pTarget, iEffectNumber) = iVtx;
	else
		iVtx = EffectVar(0, pTarget, iEffectNumber);
	// Speichere Y-Position und Physicals
	var iVtxY = EffectVar(1, pTarget, iEffectNumber) = pTarget->GetVertex(iVtx, true),
	    iJumpPhys = pTarget->GetPhysical("Jump"),
			iWalkPhys = pTarget->GetPhysical("Walk");
	// Setze Vertex hoch und Contact-Density runter
	pTarget->SetVertex(iVtx, 1, iVtxY - 3);
	pTarget->SetContactDensity(C4M_Liquid);
	// Wasserspringen geht nicht so gut
	pTarget->SetPhysical("Jump", iJumpPhys * 3 / 4, PHYS_StackTemporary);
	pTarget->SetPhysical("Walk", iWalkPhys * 3 / 4, PHYS_StackTemporary);
	// Gerade aus dem Wasser gesprungen? Auf Oberfläche setzen
	if(pTarget->GetYDir() <= 0) if(pTarget->GetAction() eq "Dive")
	{
		pTarget->SetAction("KneelUp");
		pTarget->SetXDir();
	}
}

protected func FxActiveWalkOnLiquidSpellTimer(pTarget, iEffectNumber, fTemp, iVtx)
{
  // Effekt
  CastParticles("PSpark", 3, 2, GetX(pTarget)+Random(10)-5, GetY(pTarget)+10, 20, 50, RGB(20,20,50), RGB(20,20,100));
	// Prüfe Deaktivierung
	if(!CheckWalkOnLiquid(pTarget, EffectVar(0, pTarget, iEffectNumber), EffectVar(1, pTarget, iEffectNumber), true))
		return(-1);
}

protected func FxActiveWalkOnLiquidSpellStop(pTarget, iEffectNumber, iReason, fTemp)
{
	var iVtx = EffectVar(0, pTarget, iEffectNumber), iVtxY = EffectVar(1, pTarget, iEffectNumber);
	// Alles zurück setzen
	pTarget->SetVertex(iVtx, 1, iVtxY);
	pTarget->SetContactDensity(C4M_Solid);
	pTarget->ResetPhysical(0, "Jump");
	pTarget->ResetPhysical(0, "Walk");
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(WATR); }
public func GetSpellCombo(pMage) { return ("555"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
