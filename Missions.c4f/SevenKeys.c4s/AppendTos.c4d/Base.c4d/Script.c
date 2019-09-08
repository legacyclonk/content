
#strict

// So gut wie alle relevanten Basen haben eine Tür. 
#appendto DOOR

// Ist eine Basis? Die aktuelle Fahne wird zwischengespeichert
local pMyFlag;
private func GetBase() {
	// Keine Flagge?
	if(!pMyFlag) return(-1);
	// Flagge weht noch?
	if(pMyFlag->GetAction() eq "FlyBase" && 
		 pMyFlag->GetActionTarget() == this())
	  return(pMyFlag->GetOwner());
	// Fahne weg...
	pMyFlag = 0;
	return(-1);
}
private func IsBase() { return GetBase() >= 0; }

// Wird von Flaggen aufgerufen
public func CheckFlag(object obj) {
	// Bereits eine Basis?
	if(GetBase() >= 0) return();
	// Kann eine Basis sein?
	if(!GetDefCoreVal("Base")) return();
	// Kommt in Frage?
	if(GetPlayerName(obj->GetOwner())) {
		pMyFlag = obj;
		pMyFlag->Exit();
		pMyFlag->SetAction("FlyBase", this());
		Sound("Trumpet");
		return();
	}
}

// Wird von Flagge aufgerufen
local iEnergyAvailable;
public func BaseTimer(object pFlag) {
	// Nicht unsere Flagge?
	if(pFlag != pMyFlag) return();
	// Inhalt prüfen
	for(var i = 0, obj; obj = Contents(i); i++)
		// Lore?
		if(obj->~IsLorry())
			// Inhalt holen
			GrabContents(obj);
		// Lebewesen?
		else if(obj->GetOCF() & OCF_Alive) {
			// Löschen
			if(obj->OnFire())
				obj->Extinguish();
			// Heilen
			if(obj->GetEnergy() < obj->GetPhysical("Energy") / 1000) {
				// Keine Energie da?
				if(!iEnergyAvailable) {
					// Kein Geld da?
					if(GetWealth(GetBase()) < 5)
						continue;
					// Kaufen
					Sound("Cash", true, 0, 100, GetBase() + 1);
					DoWealth(GetBase(), -5);
					iEnergyAvailable += 50;
				}
				// Auffüllen
				var iFillup = Min(Min(3, obj->GetPhysical("Energy") / 1000 - obj->GetEnergy()), iEnergyAvailable);				
				obj->DoEnergy(iFillup); iEnergyAvailable -= iFillup;
			}
		}
}

// Verkaufbare Objekttypen
private func IsSellable(id idItem) {
  // Nur alles womit die Zivilisation noch was anfangen kann, also z.B. seltene Rohstoffe
  return(idItem == GOLD || idItem == OBRL || idItem == CRYS ||
         idItem == WIPF || // als Haustier beliebt
         idItem == FMEG || idItem == MEGG || // Werden für geheime Klonierungssexperimente benötigt
         idItem == GIDL || idItem == IDOL || // Der Schlager auf jedem Flohmarkt
         idItem == SHEL); // shiny
}

// Preis, zu dem ein bestimmtes Gut gekauft werden kann (-1 = nicht kaufbar)
private func GetBuyPrice(id idItem, int iPlayer) {
  // Was verkaufbar ist, ist automatisch nicht kaufbar
  if(IsSellable(idItem)) return(-1);
  // Standardpreis
  var iValue = GetValue(0, idItem, 0, iPlayer);
  // Noch vorrätig?
  var fHomebase = (GetHomebaseMaterial(iPlayer, idItem) > 0);
  // Preis verdoppeln, wenn nicht vorrätig
  if(!fHomebase) iValue *= 2;
  return(iValue);
}

// Preis, zu dem ein bestimmtes Objekt verkauft werden kann (-1 = nicht verkaufbar)
private func GetSellPrice(object pObj, int iPlayer) {
  var idItem = pObj->GetID();
  // Verkaufbar?
  if(!IsSellable(idItem)) return(-1);
  // Standardpreis
  var iValue = GetValue(0, idItem, this(), iPlayer);
  // Inhalt dazuzählen
  for(var pCnt, i = 0; pCnt = pObj->Contents(i); i++)
    iValue += GetSellPrice(pCnt, iPlayer);
  // Und dabei bleibt es. Vielleicht mal Preis bei viel Verkaufsvolumen senken? Wäre mächtig fies :)
  return(iValue);
}

// Erzeuge Kaufmenü
private func OpenBuyMenu(object pClonk, id idSelect) {
  // Menü erzeugen
  CreateMenu(GetID(), pClonk, 0, 2, "$TxtNothingToBuy$");
  // Homebase-Materialien durchgehen
  var idItem, j = 0;
  for(var i = 0; idItem = GetHomebaseMaterial(GetBase(), 0, i, C4D_All()); i++) {
    // Kaufpreis bestimmen
    var iValue = GetBuyPrice(idItem, GetBase());
    if(iValue < 0) continue;
    // Menüeintrag erzeugen
    AddMenuItem("$TxtBuyIt$", "BuyIt", idItem, pClonk, 
      GetHomebaseMaterial(GetBase(), idItem), pClonk, 0,
      128, 0, iValue);
    j++;
    // Selektieren?
    if(idSelect == idItem) SelectMenuItem(j-1, pClonk);
  }
}

protected func BuyIt(id idItem, object pClonk, bool fRight, int iValue) {
  // Noch Basis?
  if(GetBase() < 0) return();
  // Verfeindete dürfen nichts kaufen
  var iPlayer = GetBase(), iBuyPlayer = pClonk->GetOwner();
  if(Hostile(iPlayer, iBuyPlayer)) return();
  // Wert neu berechnen, nur zur Sicherheit (da könnte wer tricksen...)
  iValue = GetBuyPrice(idItem, iPlayer);
  // Heimatbasismaterial vorhanden?
  var iHomeBase = GetHomebaseMaterial(iPlayer, idItem);
  // Anzahl der zu kaufenden Elemente
  var iCount = 1;
  if(fRight) {
    // So viele wie auf Lager, bzw. wenn kein Lager vorhanden als Standard 5 Stück.
    if(iHomeBase) iCount = iHomeBase; else iCount = 5;
  }
  // Entsprechende Anzahl kaufen
  for(var i = 0; i < iCount; i++) {
    // Differenzbetrag entfernen
    var iDiff = Min(iValue - GetValue(0, idItem, 0, iPlayer), GetWealth(iPlayer));
    DoWealth(iPlayer, -iDiff);
    // Sicherstellen, dass das Objekt kaufbar ist
    if(!iHomeBase) DoHomebaseMaterial(iPlayer, idItem, 1);
    // Kaufen!
    var pObj = Buy(idItem, iBuyPlayer, GetBase(), this(), i == 0);
    // Nicht kaufbar?
    if(!pObj) {
      // Differenz wieder gutschreiben
      DoWealth(GetBase(), iDiff);
      // Heimatbasismaterial entfernen
      if(!iHomeBase) DoHomebaseMaterial(iPlayer, idItem, -1);
      break;
    }
  }
  // Kaufmenü wieder öffnen
  OpenBuyMenu(pClonk, idItem);
}

protected func ContainedUp(object pClonk) {
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmenü öffnen
  OpenBuyMenu(pClonk);
  return(1);
}

protected func ContextBuy(object pClonk) {
  [$TxtBuy$|Condition=IsBase]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmenü öffnen
  OpenBuyMenu(pClonk);
  return(1);    
}

// Erzeuge Verkaufmenü
private func OpenSellMenu(object pClonk, id idSelect) {
  // Menü erzeugen
  CreateMenu(GetID(), pClonk, 0, 2, "$TxtNothingToSell$");
  // Alle Inhaltsobjekte durchgehen
  var iIDCnt = 0, k = 0;
  for(var pObj, i = 0; pObj = Contents(i); i++) {
    var idItem = pObj->GetID();
    // Bereits in Liste?
    for(var j = 0; j < iIDCnt; j++)
      if(Var(j) == idItem)
        break;
    if(j < iIDCnt) continue;
    // Speichern
    Var(iIDCnt++) = idItem;
    // Preis bestimmen
    var iValue = GetSellPrice(pObj, GetBase());
    if(iValue < 0) continue;
    // Inhalt?
    var strPrompt = "$TxtSellIt$";
    if(pObj->Contents()) strPrompt = "$TxtSellItWithContents$";
    // Menüeintrag erzeugen
    AddMenuItem(strPrompt, "SellIt", idItem, pClonk, 
      ContentsCount(idItem), pClonk, 0,
      128, 0, iValue);
    k++;
    // Selektieren?
    if(idSelect == idItem) SelectMenuItem(k-1, pClonk);
  }
}

protected func SellIt(id idItem, object pClonk, bool fRight, int iValue) {
  // Noch Basis?
  if(GetBase() < 0) return();
  // Verfeindete dürfen nichts verkaufen (wer weiß was sich damit für Schabernack treiben ließe....?)
  var iPlayer = GetBase();
  if(Hostile(iPlayer, pClonk->GetOwner())) return();
  // Solange verkaufen bis nichts mehr da ist
  while(true) {
    // Erstbestes Objekt
    var pObj = FindContents(idItem);
    if(!pObj) break;
    // Nochmal Verkaufspreis bestimmen (kann sich durchaus von Objekt zu Objekt ändern...)
    iValue = GetSellPrice(pObj, iPlayer);
    if(iValue < 0) break;
    // Alten Geldwert speichern, für Korrekturen
    var iOldWealth = GetWealth(iPlayer);
    // Verkaufen
    if(!Sell(iPlayer, pObj)) break;
    // Geldwert ggf. korrigieren
    SetWealth(iPlayer, iOldWealth + iValue);
    // Nur wiederholen falls gewünscht
    if(!fRight) break;
  }
  // Verkaufsmenü wieder öffnen
  OpenSellMenu(pClonk, idItem);
}

protected func ContainedDig(object pClonk) {
  [$TxtSell$]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmenü öffnen
  OpenSellMenu(pClonk);
  return(1);
}

protected func ContextSell(object pClonk) {
  [$TxtSell$|Condition=IsBase]
  // Keine Basis?
  if(GetBase() < 0) return(_inherited());
  // Kaufmenü öffnen
  OpenSellMenu(pClonk);
  return(1);    
}
