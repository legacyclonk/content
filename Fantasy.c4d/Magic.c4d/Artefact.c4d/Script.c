#strict

local pObj;
local pClonk;
local iEffect;
local iMagicAmount;

func Activate(pCaster, pRealCaster) { 
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("ArtefactNSpell", 0, 75)) return(iResult!=-1 && RemoveObject());
	
  // Clonk und Objekt speichern
  pClonk = pCaster;
  if(pRealCaster) pClonk = pRealCaster;
  pObj = Contents(0,pCaster);
  
  // Ohne Objekt geht nichts
  if(!pObj) return(0, Message("$MsgNoObject$", pCaster));

  // Wenn schon ein Artefakt vorliegt
  if(GetEffect("ArtefactNSpell", pObj))
  {
    iEffect = GetEffect("ArtefactNSpell", pObj);
    if(!EffectVar(8, pObj, iEffect))
    {
    	// Menü mit Optionen öffnen
	CreateMenu(MCMS,pCaster, 0, 0, "$MsgTitle$", GetMagicEnergy(pCaster), 1);
    	var idMagic, i = 0;
    
    	AddMenuItem("$MsgAnalyse$", "DoChange", CXIN, pCaster,0,2);
    	AddMenuItem("$MsgLoad$"   , "DoChange", MCMQ, pCaster,0,0);
    	AddMenuItem("$MsgUnload$" , "DoChange", MCMX, pCaster,0,1);
    	AddMenuItem("$MsgModifiy$", "DoChange", RSR1, pCaster,0,3);	
    	AddMenuItem("$MsgHide$"   , "DoChange", CMFG, pCaster,0,4);
        AddMenuItem("$MsgStack$"  , "DoChange", FXV1, pCaster,0,5);

	// Zauber effekt
    	AddEffect("IntDoMagic", pCaster, 1, 1, this());
    }
    // Gleich eine Analyse erstellen
    else DoChange(0, 2);
    return(1);	  
  }
  // Menü öffnen
  MagicMenu(pCaster);
  // Zauberenergie speichern für späteres Abbrechen
  iMagicAmount += GetValue();
  // Nicht bewegen
  SetCategory(1);
  // Zauber effekt, der das Menü abbricht, wenn der Zauberer gestört wird
  AddEffect("IntDoMagic", pCaster, 1, 1, this());
  return(1);
}

private func DoChange(idID, iMode)
{
  // Kein Effekt mehr? aufhören	
  if(GetEffect("IntDoMagic", pClonk)) RemoveEffect("IntDoMagic", pClonk);
  // Aufladen
  if(iMode == 0)
  {	  
    var idMagic = EffectVar(0, pObj, iEffect);  
    // Nicht genug Zauberenergie
    if(!CheckMana(idMagic,pClonk))
    {
      Sound("Error");
      Message("$TxtNotEnoughEnergy$",this());
      return(RemoveObject());
    }

    // Nicht genug alchemische Komponenten
    if(ObjectCount(ALCO)) if(!CheckAlchem(idMagic,pClonk))
    {
      Sound("Error");
      Message("$TxtNotEnoughIngredients$",this(),GetNeededAlcStr(idMagic,pClonk));
      return(RemoveObject());
    }

    // Energie abziehen
    DoMagicEnergy(-Value(idMagic),pClonk);

    // Anzahl der Zauber hochzählen
    EffectVar(1, pObj, iEffect)++;

    // Effekte!
    CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pClonk)));

    // Nachricht!
    PlayerMessage(GetOwner(pClonk),"$MsgSpellSet$", pClonk, GetName(pObj), GetName(0,idMagic));

    // Aktion für den Clonk setzen
    pClonk->SetAction("Magic");
  
    // Fertig!
    return(RemoveObject());
  }
  // Entladen
  if(iMode == 1)
  {
    // Wert abfragen	  
    iMagicAmount = GetValue(0,EffectVar(0, pObj, iEffect));	  
    // und dem Clonk geben
    while(iMagicAmount--) DoMagicEnergy(1, pClonk);	
    // Weg mit dem Effekt
    RemoveEffect("ArtefactNSpell", pObj);
    // Weg mit dem Bild
    SetGraphics(0,pObj,0,1,GFXOV_MODE_Picture, 0);
    // Effekte!
    CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pObj)));
    // Fertig!
    return(RemoveObject());	  
  }
  // Analyse
  if(iMode == 2)
  {
    // Getarntes Artefakt! beenden	  
    if(EffectVar(8, pObj, iEffect)) return(Message("$MsgAnalysingHidden$", pClonk));	  
    // Nachricht zusammenbauen
    var string1; var iMode = EffectVar(2, pObj, iEffect);
    if(!iMode--) string1 = "$MsgHit$"; if(!iMode--) string1 = "$MsgStill$"; if(!iMode--) string1 = "$MsgCollect$";
    if(!iMode--) string1 = "$MsgDamage$"; if(!iMode--) string1 = "$MsgBurn$"; if(!iMode--) string1 = "$MsgEnemy$";
    if(!iMode--) string1 = "$MsgEat$"; if(!iMode--) string1 = "$MsgDrink$";
    var string2; var iMode2 = EffectVar(6, pObj, iEffect);
    if(!iMode2--) string2 = "$MsgAlly$"; if(!iMode2--) string2 = "$MsgObj$"; if(!iMode2--) string2 = "$MsgTarEnemy$";
    // Nachricht ausgeben
    PlayerMessage(GetOwner(pClonk), "$MsgAnalysing$|Besitzer: %d", pClonk, GetName(0,EffectVar(0, pObj, iEffect)),
		  EffectVar(1, pObj, iEffect)+1,string1,string2, EffectVar(4, pObj, iEffect));
    // Effekte!
    CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pObj)));
    // Fertig!
    return(RemoveObject());	  
  }
  // Modifizieren
  if(iMode == 3)
  {
    // Menü öffnen	  
    MenuMode(pClonk);
    // Fertig!
    return(RemoveObject());	  
  }
  // Tarnen
  if(iMode == 4)
  {
    // Bild löschen	  
    SetGraphics(0,pObj,0,1,GFXOV_MODE_Picture, 0);
    // Effekt!
    CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pObj)));
    // Tarnvariable setzten
    EffectVar(8, pObj, iEffect) = 1;
    // Fertig!
    return(RemoveObject());	  
  }
  // Stapeln
  if(iMode == 5)
  {
    // Menü öffnen
    MagicMenu(pClonk);
    // Zauberenergie speichern für späteres Abbrechen
    iMagicAmount += GetValue();
    // Nicht bewegen
    SetCategory(1);
    // Zauber effekt, der das Menü abbricht, wenn der Zauberer gestört wird
    AddEffect("IntDoMagic", pClonk, 1, 1, this());
    // Fertig!
    return();	  
  }
}

func FxIntDoMagicTimer(pCaster, iEffectNumber, iEffectTime)
{	
  var pClonk=EffectVar(0,pCaster,iEffectNumber);
  var iMLength=GetActMapVal("Length",GetAction(pCaster),GetID(pCaster));

  // magische Aktion vorhanden
  if(GetActMapVal("Name","Magic",GetID(pCaster)))
  {
    // z.B. während des Zauberns durch einen Stein getroffen
    if(!DoesMagic(pCaster)) 
    {	
      // Menü schließen	    
      CloseMenu(pCaster);
      MenuQueryCancel();
      return(-1);
    }
    // Zauberaktion für das zaubernde Objekt: Es wird angenommen, dass diese
    // in einem Raum von etwa 2/6 bis 5/6 der Zauberaktion spielt
    if(GetPhase(pCaster) >= iMLength*5/6) SetPhase(iMLength*2/6,pCaster);
    // Kein Menü mehr? schließen
    if(!GetMenu(pCaster)) return(-1);
  }
  return(1);
}

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }

private func MagicMenu(object pByClonk) {
  // Menü öffnen
  CreateMenu(MCMS,pByClonk, 0, 0, "$MnuNoSpells$", GetMagicEnergy(pByClonk));
  var idMagic, i = 0;
  // Mit Elementsymbolen füllen
  while (idMagic = GetDefinition(i++, C4D_Object))
    if(idMagic->~IsSpellClass())	  
    AddMenuItem("$MnuEnchant$: %s", "MagicMenuB", idMagic, pByClonk,0,pByClonk);
}

protected func MagicMenuB(def) {
  // Menü öffnen
  OpenEmptySpellMenu(MCMS, pClonk, this, pClonk, "$MnuNoSpells$");
  var idMagic, i = 0;
  // Mit Zaubern des Elements füllen
  while (idMagic = GetPlrMagic(GetOwner(pClonk), 0, i++))
    if(idMagic->~GetSpellClass()==def)	  
    AddMenuItem("$MnuEnchant$: %s", "SetMagic", idMagic, pClonk,0,pClonk);
}

private func SetMagic(id idMagic, object pByClonk) {
  // Nicht genug Zauberenergie
  if(!CheckMana(idMagic,pByClonk))
  {
    Sound("Error");
    Message("$TxtNotEnoughEnergy$",this());
    return(RemoveObject());
  }

  // Nicht genug alchemische Komponenten
  if(ObjectCount(ALCO)) if(!CheckAlchem(idMagic,pByClonk))
  {
    Sound("Error");
    Message("$TxtNotEnoughIngredients$",this(),GetNeededAlcStr(idMagic,pByClonk));
    return(RemoveObject());
  }

  // Energie abziehen
  DoMagicEnergy(-Value(idMagic),pByClonk);
  iMagicAmount += Value(idMagic);

  // alchemische Zutaten abziehen
  if(ObjectCount(ALCO)) ReduceAlchem(idMagic,pByClonk);

  // Aktuelle Zauberkraft des zaubernden Clonks trainieren, sowie sonstige, szenarienspezifische Aufrufe
  OnClonkSucceededSpell(idMagic, pByClonk);

  SetSpell(idMagic,pByClonk);

  // Effekte!
  CastParticles("MSpark", 20, 40, 0,0, 30, 45, GetPlrColorDw(GetOwner(pByClonk)));

  // Nachricht!
  PlayerMessage(GetOwner(pByClonk),"$MsgSpellSet$", pByClonk, GetName(pObj), GetName(0,idMagic));

  Sound("Magic*");
}

private func SetSpell(idMagic,pByClonk)
{
  // Effekt erzeugen	
  iEffect = AddEffect("ArtefactNSpell", pObj, 75, 1, 0, GetID(), idMagic);	
  // Mit Infos füttern
//  EffectVar(0, pObj, iEffect) = idMagic;
  EffectVar(7, pObj, iEffect) = -1;
  // Bild auf das Objekt machen
  SetGraphics(0,pObj,idMagic,iEffect,GFXOV_MODE_Picture, 0);
  SetObjDrawTransform(800,0,0,0,800,0, pObj, 1);
  // Weitere Einstellungen
  MenuMode(pByClonk, idMagic);
}

private func MenuMode(pByClonk, idMagic)
{
  // Menü öffenen
  CreateMenu(MCMS,pByClonk, 0, 0, "$MsgActTitle$", GetMagicEnergy(pByClonk), 1);

  // Mit Aktivierungsmöglichkeinen füllen
  AddMenuItem("$MsgHit$"     , "SetMode", FXQ1, pByClonk,0,0);
  AddMenuItem("$MsgStill$"   , "SetMode", FXP1, pByClonk,0,1);
  AddMenuItem("$MsgCollect$" , "SetMode", WIPF, pByClonk,0,2);
  AddMenuItem("$MsgDamage$"  , "SetMode", METO, pByClonk,0,3);
  AddMenuItem("$MsgBurn$"    , "SetMode", DFLM, pByClonk,0,4);
  AddMenuItem("$MsgEnemy$"   , "SetMode", MELE, pByClonk,0,5);
  // Essbare Objekte
  if( (GetDefCoreVal("Edible", "DefCore", GetID(pObj)) || GetID(pObj)==WMPF ) && !(pObj->~IsPotion()) )
    AddMenuItem("$MsgEat$"     , "SetMode", WMPF, pByClonk,0,6);
  // Tränke
  if(pObj->~IsPotion())
    AddMenuItem("$MsgDrink$"   , "SetMode", PHEA, pByClonk,0,7);//*/
}

protected func SetMode(idMagic, iMode)
{
  // Aktivierungmodus speichern	
  EffectVar(2, pObj, iEffect) = iMode;
  // Zusatzparamente abfragen
  if(iMode==3)
  {CreateMenu(MCMS,pClonk, 0, 0, "$MsgActTitle$", GetMagicEnergy(pClonk), 1);
    AddMenuItem("10%% $MsgEnergy$" , "SetParam", ERTH, pClonk,0,10);
    AddMenuItem("20%% $MsgEnergy$" , "SetParam", ASHS, pClonk,0,20);
    AddMenuItem("30%% $MsgEnergy$" , "SetParam", SAND, pClonk,0,30);
    AddMenuItem("40%% $MsgEnergy$" , "SetParam", LOAM, pClonk,0,40);
    AddMenuItem("50%% $MsgEnergy$" , "SetParam", COAL, pClonk,0,50);
    AddMenuItem("60%% $MsgEnergy$" , "SetParam", ROCK, pClonk,0,60);
    AddMenuItem("70%% $MsgEnergy$" , "SetParam", ORE1, pClonk,0,70);
    AddMenuItem("80%% $MsgEnergy$" , "SetParam", METL, pClonk,0,80);
    AddMenuItem("90%% $MsgEnergy$" , "SetParam", GOLD, pClonk,0,90);
    return();	  
  }
  if(iMode==5)
  {CreateMenu(MCMS,pClonk, 0, 0, "$MsgActTitle$", GetMagicEnergy(pClonk), 1);
    AddMenuItem(" 50 Px $MsgEnemy$" , "SetParam", ERTH, pClonk,0,50);
    AddMenuItem("100 Px $MsgEnemy$" , "SetParam", ROCK, pClonk,0,100);
    AddMenuItem("200 Px $MsgEnemy$" , "SetParam", ORE1, pClonk,0,200);
    AddMenuItem("500 Px $MsgEnemy$" , "SetParam", GOLD, pClonk,0,500);
    return();	  
  }
  // Weitere Einstellungen
  MenuDelay(pClonk);
}

protected func SetParam(idDummy, iParam)
{ 
  // Aktivierungmodus speichern	
  EffectVar(9, pObj, iEffect) = iParam;
  // Weitere Einstellungen
  MenuDelay(pClonk);
}

private func MenuDelay()
{
  CreateMenu(MCMS,pClonk, 0, 0, "$MsgDelay$", GetMagicEnergy(pClonk), 1);
  AddMenuItem("$MsgNo$ $MsgDelay$", "SetDelay", FXP1, pClonk,0,  0);
  AddMenuItem("   5 fr $MsgDelay$" , "SetDelay", ERTH, pClonk,0,   5);
  AddMenuItem("  10 fr $MsgDelay$" , "SetDelay", SAND, pClonk,0,  10);
  AddMenuItem("  20 fr $MsgDelay$" , "SetDelay", LOAM, pClonk,0,  20);
  AddMenuItem("  50 fr $MsgDelay$" , "SetDelay", COAL, pClonk,0,  50);
  AddMenuItem(" 100 fr $MsgDelay$" , "SetDelay", ROCK, pClonk,0, 100);
  AddMenuItem(" 200 fr $MsgDelay$" , "SetDelay", ORE1, pClonk,0, 200);
  AddMenuItem(" 500 fr $MsgDelay$" , "SetDelay", METL, pClonk,0, 500);
  AddMenuItem("1000 fr $MsgDelay$" , "SetDelay", GOLD, pClonk,0,1000);
  return();	 	
}

protected func SetDelay(idDummy, iParam)
{ 
  // Aktivierungmodus speichern	
  EffectVar(10, pObj, iEffect) = iParam;
  // Weitere Einstellungen
  MenuTarget(pClonk);
}

private func MenuTarget(pByClonk)
{
  if(IsAimerSpell(EffectVar(0, pObj, iEffect)))
  {
    // Menü öffnen	
    CreateMenu(MCMS,pByClonk, 0, 0, "$MsgTarTitle$", 0, 0);
    // Mit Zielen füllen
    AddMenuItem(" ", "", NONE, pByClonk);
    AddMenuItem("$MsgAlly$"  , "SetTarget", WIPF, pByClonk,0,0);
    AddMenuItem("$MsgRandom$" , "SetTarget", CXIN, pByClonk,0,1);
    AddMenuItem("$MsgTarEnemy$" , "SetTarget", MONS, pByClonk,0,2);
    AddMenuItem(" ", "", NONE, pByClonk);
    // Verschiedene Richtungen
    AddMenuItem("$MnuArrowLeft$"      , "SetTarget", LMM2, pByClonk,0,2+1, "", 2, 0);
    AddMenuItem("$MnuArrowLeftDiag$"  , "SetTarget", LMM2, pByClonk,0,2+2, "", 2, 1);
    AddMenuItem("$MnuArrowStraight$"  , "SetTarget", LMM2, pByClonk,0,2+3, "", 2, 2);
    AddMenuItem("$MnuArrowRightDiag$" , "SetTarget", LMM2, pByClonk,0,2+4, "", 2, 3);
    AddMenuItem("$MnuArrowRight$"     , "SetTarget", LMM2, pByClonk,0,2+5, "", 2, 4);
    AddMenuItem(" ", "", NONE, pByClonk);
    AddMenuItem("$MnuArrowLeftDown$"  , "SetTarget", LMM2, pByClonk,0,2+6, "", 2, 5);
    AddMenuItem("$MnuArrowDown$"      , "SetTarget", LMM2, pByClonk,0,2+7, "", 2, 6);
    AddMenuItem("$MnuArrowRigthDown$" , "SetTarget", LMM2, pByClonk,0,2+8, "", 2, 7);
    SelectMenuItem(2, pByClonk); 
    return();
  }
  // Menü öffnen	
  CreateMenu(MCMS,pByClonk, 0, 0, "$MsgTarTitle$", GetMagicEnergy(pByClonk), 1);
  // Mit Zielen füllen
  AddMenuItem("$MsgAlly$"  , "SetTarget", WIPF, pByClonk,0,0);
  AddMenuItem("$MsgObj$"   , "SetTarget", ROCK, pByClonk,0,1);
  AddMenuItem("$MsgTarEnemy$" , "SetTarget", MONS, pByClonk,0,2);
  // Eintrag "Objekt" auswählen
  SelectMenuItem(1, pByClonk); 
}

private func SetTarget(idMagic, iMode)
{
  // Einspeichern  	
  EffectVar(6, pObj, iEffect) = iMode;
  if(FindObject(0, -10,-10,20,20,OCF_Collectible,0,0,NoContainer()))
    MenuCombo();
  else SetCombo();
}

private func MenuCombo()
{
  CreateMenu(MCMS,pClonk, 0, 0, "$MsgCombo$", GetMagicEnergy(pClonk), 1);
  var obj, id;
  while(obj = FindObject(0, -10,-10,20,20,OCF_Collectible,0,0,NoContainer(),obj))
  {
    if(GetID(obj)==id) continue;
    id = GetID(obj);
    AddMenuItem("%s", "SetCombo", GetID(obj), pClonk,0,  obj);
  }
  return();	 	
}

private func SetCombo(idMagic, pCombo)
{
  // Einspeichern  	
  Enter(pObj, pCombo);
  // Effekt entfernen
  if(GetEffect("IntDoMagic", pClonk)) RemoveEffect("IntDoMagic", pClonk);
  // Owner sezten und Effekt freigeben
  EffectVar(4, pObj, iEffect) = GetOwner(pClonk);
  EffectVar(7, pObj, iEffect) = 0;
  // Endlich fertig!
  RemoveObject();
}

protected func MenuQueryCancel()
{
  if(!iMagicAmount) return();	
  // Bei Schließen des Menüs Artefakt abbrechen	
  while(iMagicAmount--) DoMagicEnergy(1, pClonk);	
  if(iEffect) RemoveEffect("ArtefactNSpell", pObj);
  if(GetEffect("IntDoMagic", pClonk)) RemoveEffect("IntDoMagic", pClonk);
  SetGraphics(0,pObj,0,1,GFXOV_MODE_Picture, 0);
}

// 0 ZauberID
// 1 Anzahl Ladungen
// 2 Aktivierungsmodus
// 3 Aktiviert?
// 4 Besitzer
// 5 Richtung
// 6 Ziel
// 7 WarteTimer
// 8 Getarnt
// 9 Parameter zum Aktivierungsmodus
//10 Wartezeit
//11 Delaytimer

func FxArtefactNSpellStart(pObj, iEffect, fTmp, idMagic)
{
  if(idMagic) EffectVar(0, pObj, iEffect) = idMagic;
  SetGraphics(0,pObj, EffectVar(0, pObj, iEffect),iEffect,GFXOV_MODE_Picture, 0);
  SetObjDrawTransform(800,0,0,0,800,0, pObj, iEffect);
}

func FxArtefactNSpellTimer(pObj, iEffect)
{
  if(EffectVar(11, pObj, iEffect))
  {	
	if(EffectVar(11, pObj, iEffect)++>EffectVar(10, pObj, iEffect)) CastSpell(pObj, iEffect);	
	return();
  }
  // Warten	
  if(EffectVar(7, pObj, iEffect)) return(0, EffectVar(7, pObj, iEffect)--);
  // Passende Aktion ausführen
  DefinitionCall (MART, Format("Mode%d", EffectVar(2, pObj, iEffect)), pObj, iEffect);
}

func FxArtefactNSpellStop(pObj, iEffect, iReason)
{
  // Wird das Objekt entfernt Spruch auslösen (z.B. explodierendes Flint oder aufgegessene Beere)	
  if(iReason==3) CastSpell(pObj, iEffect, 0, 1);
  SetGraphics(0,pObj,0,iEffect,GFXOV_MODE_Picture, 0);
}

func HasContact(pObj)
{
  // Berührt der Gegenstand was?	
  if(GetContact(pObj, -1, CNAT_Left)) return(1);
  if(GetContact(pObj, -1, CNAT_Right)) return(1);
  if(GetContact(pObj, -1, CNAT_Top)) return(1);
  if(GetContact(pObj, -1, CNAT_Bottom)) return(1);
}

func Mode0(pObj, iEffect)
{
  // Wenn jemand es in der Hand hat wird er zum Besitzer
  if(Contained(pObj)) 
  {
    if(EffectVar(3, pObj, iEffect)) return(CastSpell(pObj, iEffect));
    EffectVar(4, pObj, iEffect) = GetOwner(Contained(pObj)); return(); 
  }
  // Bewegt sich das Objekt nicht und hat Kontakt?
  if( Abs(GetXDir(pObj))+Abs(GetYDir(pObj))<15 )
  {
    if( HasContact(pObj) && EffectVar(3, pObj, iEffect))
    {
     CastSpell(pObj, iEffect);
    }
  }
 else
 {   
     // Aktivieren und Richtung anpassen	 
     EffectVar(3, pObj, iEffect) = 1; 
     if(GetXDir(pObj)<0) EffectVar(4, pObj, iEffect) = 0; 
     else EffectVar(5, pObj, iEffect) = 1;
 }
}

func Mode1(pObj, iEffect)
{
  // Wenn jemand es in der Hand hat wird er zum Besitzer
  if(Contained(pObj)) {EffectVar(4, pObj, iEffect) = GetOwner(Contained(pObj)); return(); }
  // Ist das Objekt zum Stillstand gekommen?
  if(Abs(GetXDir(pObj))+Abs(GetYDir(pObj))<2)
  {
    if(EffectVar(3, pObj, iEffect))
    {
     CastSpell(pObj, iEffect);
    }
  }
 else
 {   // Aktivieren und Richtung anpassen
     EffectVar(3, pObj, iEffect) = 1; 
     if(GetXDir(pObj)<0) EffectVar(4, pObj, iEffect) = 0; 
     else EffectVar(5, pObj, iEffect) = 1;
 }
}

func Mode2(pObj, iEffect)
{
  // Eingesammelt und aktiviert? dann Spruch ab!	
  if(Contained(pObj))
  {
	if(!EffectVar(3, pObj, iEffect))  {EffectVar(4, pObj, iEffect) = GetOwner(Contained(pObj)); return(); }
	else CastSpell(pObj, iEffect);
  }
  else
  {  // Aktivieren und Richtung anpassen
     EffectVar(3, pObj, iEffect) = 1; 
     if(GetXDir(pObj)<0) EffectVar(4, pObj, iEffect) = 0; 
     else EffectVar(5, pObj, iEffect) = 1;
  }
}

func Mode3(pObj, iEffect)
{
  // Enthalten und Schaden genommen?	
  if(Contained(pObj))
  {
	if(GetEnergy(Contained(pObj))>EffectVar(9, pObj, iEffect)*GetPhysical("Energy", 0, Contained(pObj))/100000)  { return(); }
	else { EffectVar(4, pObj, iEffect) = GetOwner(Contained(pObj)); CastSpell(pObj, iEffect); }
  }
}

func Mode4(pObj, iEffect)
{
  // Entalten und bernnend?	
  if(Contained(pObj))
  {
	EffectVar(4, pObj, iEffect) = GetOwner(Contained(pObj));
	if(OnFire(Contained(pObj))) CastSpell(pObj, iEffect);
  }
  else if(OnFire(pObj)) CastSpell(pObj, iEffect);
}

func Mode5(pObj, iEffect)
{
  // Gegner in der Nähe?	
  var pClonk;
  var iRange = EffectVar(9, pObj, iEffect);
  while(pClonk=FindObject(0,GetX(pObj)-iRange/2,GetY(pObj)-iRange/2,iRange,iRange,OCF_CrewMember(),0,0,NoContainer(),pClonk))
    if(ObjectDistance(pClonk,pObj) <= iRange)
      if(Hostile(GetOwner(pClonk),EffectVar(4, pObj, iEffect)))
      {
         if(GetX(pObj) < GetX(pClonk)) EffectVar(5, pObj, iEffect) = 1;
	 CastSpell(pObj, iEffect, pClonk);
      }
}


func CastSpell(pObj, iEffect, pTarget, fNoDelay)
{
  // Steht erst noch ein Delay aus?
  if(EffectVar(11, pObj, iEffect)<=EffectVar(10, pObj, iEffect) && !fNoDelay) return(0, EffectVar(11, pObj, iEffect)++);
    // So jetzte wird gecastet!
    // Erstmal ein Casterobjekt machen, dass soll später das Zielen übernehmen
//    var pCaster = CreateObject(GetID(), GetX(pObj),GetY(pObj), EffectVar(4, pObj, iEffect));
    var pCaster = pObj;
    if(fNoDelay) pCaster = CreateObject(MART, GetX(pObj),GetY(pObj), EffectVar(4, pObj, iEffect));
    var pCaster2;
    // passende Aktion
    ObjectSetAction(pCaster, "Cast", pObj);
    // passende Richtung
    SetDir(EffectVar(5, pObj, iEffect), pCaster);
    // haben wir kein Ziel? dann eines suchen!
    if(!pTarget && EffectVar(6, pObj, iEffect)<3)
    {
      while(pTarget=FindObject(0,GetX(pObj),GetY(pObj),-1,-1,OCF_CrewMember(),0,0,NoContainer(),pTarget))
      { // Unterscheidung ob wir Freune oder Feinde suchen sollen	      
	if(EffectVar(6, pObj, iEffect)==2)
	{
          if( ObjectDistance(pTarget,pObj) >= 300 ) break;
	  if( Hostile( GetOwner(pTarget),EffectVar(4, pObj, iEffect) ))
             if(Contained(pObj)!=pTarget) { AddEffect("Casting", pObj, 1, 1, 0, MART, pTarget); break; }
	}
        if(EffectVar(6, pObj, iEffect)==0)
	{
          if( ObjectDistance(pTarget,pObj) >= 150 ) break; 
	  if(!Hostile( GetOwner(pTarget),EffectVar(4, pObj, iEffect) ))
          { pCaster2 = pTarget; break; }
	}
      }
    }
    else
    {
      if(EffectVar(6, pObj, iEffect)>2) //pCaster->Aim(0, (EffectVar(6, pObj, iEffect)-3)*(45)-90);
        AddEffect("Casting", pObj, 1, 1, 0, MART, 0, (EffectVar(6, pObj, iEffect)-3)*(45)-90);
      if(EffectVar(6, pObj, iEffect)==2) // pCaster->Aim(pTarget);
        AddEffect("Casting", pObj, 1, 1, 0, MART, pTarget);
    }
    if(IsAimerSpell(EffectVar(0, pObj, iEffect)) && EffectVar(6, pObj, iEffect)==1) 
	    AddEffect("Casting", pObj, 1, 1, 0, MART, 0, RandomX(-180,180));
    // Zauber erstellen und aktivieren
    if(!CreateObject(EffectVar(0, pObj, iEffect),GetX(pObj),GetY(pObj),EffectVar(4, pObj, iEffect))->~Activate(pCaster, pCaster2))
	    return(EffectVar(3, pObj, iEffect)=0);
    // Effekt!!
    CastParticles("MSpark", 20, 40, GetX(pObj)+0,GetY(pObj)+0, 30, 45, GetPlrColorDw(GetOwner(pObj)));
    // Zauberzahl runterzählen
    if(!EffectVar(1, pObj, iEffect))
    {
    // War's der letzte Zauber, dann Bild und Effekt entfernen	    
    SetGraphics(0,pObj,0,iEffect,GFXOV_MODE_Picture, 0);
    RemoveEffect(0, pObj, iEffect);
    return();
    }
    // Sonst alles für eine neuse Mal vorbereiten
    EffectVar(1, pObj, iEffect)--;
    EffectVar(7, pObj, iEffect) = GetValue(0, EffectVar(0, pObj, iEffect));
    EffectVar(3, pObj, iEffect) = 0;
    EffectVar(11, pObj, iEffect) = 0;
}

func FxCastingStart(pObj, iNum, fTmp, pTarget, iAngle, iDir)
{
  if(!fTmp)
  {
    if(pTarget) EffectVar(0, pObj, iNum) = Angle(GetX(pObj), GetY(pObj), GetX(pTarget), GetY(pTarget));
    else EffectVar(0, pObj, iNum) = iAngle;
    while(EffectVar(0, pObj, iNum)>180) EffectVar(0, pObj, iNum)-=360;
    EffectVar(1, pObj, iNum) = iDir;
  }
}

func FxCastingTimer(pObj, iNum)
{
  if(!EffectVar(2, pObj, iNum)) return();
  if(EffectVar(2, pObj, iNum)==1) EffectVar(3, pObj, iNum)->~ActivateAngle(pObj, EffectVar(0, pObj, iNum));
  if(EffectVar(2, pObj, iNum)++>5) return(-1);
//  Message("%d", pObj, EffectVar(2, pObj, iNum));
}

func FxCastingDoSpellAim(pObj, iNum, pMagic)
{
  EffectVar(2, pObj, iNum) = 1;
  EffectVar(3, pObj, iNum) = pMagic;
  return(1);
}

func FxCastingStop(pObj, iNum, iReason)
{
  // Noch nicht gezaubert? dann Zauberobjekt erschaffen	
  if(!EffectVar(2, pObj, iNum))
  {
     var pCaster = CreateObject(MART, GetX(pObj),GetY(pObj), GetOwner(pObj));
     AddEffect("Casting", pCaster, 1, 1, 0, MART, 0, EffectVar(0, pObj, iNum));
     ObjectSetAction(pCaster, "Cast", pCaster);
  }
}

protected func WannaRemove() {
  // manche Zauber hängen gerne Effekte an den Caster
  if(!GetEffectCount(0,this())) if(!FindObject(0, 0, 0, 0, 0, 0, 0, this())) RemoveObject();
}

protected func AbortRemove() {
  RemoveObject();
}
local pAimTarget;
local pAimAngle;
local pAimMagic;
public func Aim(pTarget, iAngle) { pAimTarget = pTarget; pAimAngle = iAngle;}

global func DoSpellAim(pMagic)
{
  // Bei Bedarf an Efekt weiterleiten
  if(GetEffect("Casting", this())) return(EffectCall(this(), GetEffect("Casting", this()), "DoSpellAim", pMagic));
  return(_inherited(pMagic));	  
}

public func ArtefactSpellAim(pMagic)
{
  pAimMagic = pMagic;
  var angle;
  if(pAimTarget) angle = Angle(GetX(), GetY(), GetX(pAimTarget), GetY(pAimTarget));
  else angle = pAimAngle;
  Schedule(Format("pAimMagic->ActivateAngle(this(), %d)", angle), 1);
  return(1);
}

protected func DoAttach()
{
  SetPosition(GetX(GetActionTarget()), GetY(GetActionTarget()));
}

public func IsAimerSpell(idMagic)
{
  if(!idMagic) return(0); // Artefaktzauber ist gemeint
  if(idMagic->~IsAimerSpell()) return(1);
}

/* Zaubercombo */

public func GetSpellClass(object pMage) { return(EART); }
public func GetSpellCombo(pMage) { return ("445"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
