/*-- Köcher --*/

#strict

public func MaxArrowCount() { return(30); }

public func UnpackTo()      { return(0); }
public func PackCount()     { return(GetArrowCount()); }

private func & ArwCount(i)  { return(Local(i*2  )); }
private func & ArwID(i)     { return(Local(i*2+1)); }

public func GetArrowCount()
{
  var iCount, i;
  while(ArwID(i)) iCount += ArwCount(i++);
  return(iCount);
}

// Objekt darf nur einmal im Inventar sein
public func CarryLimit() { return(1); }

/* Einsammeln */

public func Entrance(object pClonk)
{
  var i, pObj;
  // Nur Clonks packen Pfeile
  if(CanPack(pClonk))
    for(i; pObj = Contents(i,pClonk); i++) {
      if(AddPack(pObj,1)) i--;
      if(MaxArrowCount() <= GetArrowCount()) break;
    }
  MakePicture();
}

public func AddPack(object pObj, fNoPicture)
{
  if(!pObj) return(0);
  var i, iAmount, iDif, idType;
  // Wurde der Pfeil bereits irgendwie eingepackt?
  if(!pObj) return(0);
  // Sind überhaupt noch Pfeile einzupacken?
  iDif = MaxArrowCount()-GetArrowCount();
  if(iDif <= 0) return(0);
  // Ist dies ein ganzes Paket oder nur ein Pfeil?
  if(pObj->~IsArrowPack()) {
    // Wieviele Pfeile können überhaupt eingepackt werden?
    iAmount = Min(pObj->~PackCount(),iDif);
    idType = pObj->~UnpackTo();
    // Ist diese Pfeilart bereits vorhanden?
    for(i = 0; ArwID(i); i++) if(ArwID(i) == idType) break;
    // Pfeile hinzufügen
    ArwID(i) = idType;
    ArwCount(i) += iAmount;
    // Jeweilige Anzahl vom Paket abziehen
    pObj->~DoPackCount(-iAmount);
    // Gegebenenfalls Graphik anpassen
    if(!fNoPicture) MakePicture();
    // Fertig!
    return(true);
  }
  if(!pObj->~IsArrow()) return(false);
  // Pfeil einpacken
  idType = GetID(pObj);
  for(i = 0; ArwID(i); i++) if(ArwID(i) == idType) break;
  ArwID(i) = idType;
  ArwCount(i)++;
  RemoveObject(pObj);
  // Gegebenenfalls Graphik anpassen
  if(!fNoPicture) MakePicture();
  // Fertig!
  return(true);
}

public func MakePicture()
{
  if(GetArrowCount()) SetPicture(0,6,64,64);
  else SetPicture(18,6,64,64);
  for(var i = 0; i < 3; i++) {
    if(!ArwID(i)) break;
    SetGraphics(0,0,ArwID(i)->~PackTo(),1+i*3,GFXOV_MODE_Picture);
    SetObjDrawTransform(400,0,3000,0,400,-10000+i*13000,this(),1+i*3);
    
    if(ArwCount(i)/10) {
      SetGraphics(0,0,GetNumberID(ArwCount(i)/10),2+i*3,GFXOV_MODE_Picture);
      SetObjDrawTransform(400,0,-12000,0,400,-10000+i*13000,this(),2+i*3);
    }
    else SetGraphics(0,0,0,2+i*3);

    SetGraphics(0,0,GetNumberID(ArwCount(i)%10),3+i*3,GFXOV_MODE_Picture);
    SetObjDrawTransform(400,0,-5000,0,400,-10000+i*13000,this(),3+i*3);
  }
}

/* ID des passenden Zahlobjektes ausgeben */

private func GetNumberID(int iNumber) { return(C4Id(Format("SNB%d",iNumber))); }

public func FetchArrow()
{
  if(!GetArrowCount()) return(0);
  ArwCount()--;
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking",this(),1);
  // Pfeil erzeugen
  var pObj = CreateContents(ArwID());
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking",this());
  if(!ArwCount()) {
    Shift(1);
    if(Contained())
      if(ArwID())
        PlayerMessage(GetOwner(Contained()),"%s",this(),GetName(0,ArwID()));
  }
  MakePicture();
  return(pObj);
}

public func Shift(fNoPicture)
{
  
  var i, iCount = ArwCount(), idType = ArwID(); 
  for(i = 0; ArwID(i+1); i++) {
    ArwCount(i) = ArwCount(i+1);
    ArwID(i) = ArwID(i+1);
  }
  if(iCount) {
    ArwCount(i) = iCount;
    ArwID(i) = idType;
  }
  else {
    ArwCount(i) = 0;
    ArwID(i) = 0;
    SetGraphics(0,0,0,1+i*3,GFXOV_MODE_Picture);     
    SetGraphics(0,0,0,2+i*3,GFXOV_MODE_Picture);
    SetGraphics(0,0,0,3+i*3,GFXOV_MODE_Picture);
  }
  if(!fNoPicture) MakePicture();
}

/* Aufschlag */

protected func Hit() { Sound("WoodHit"); }

/* Verkauf von Pfeilen */

protected func Sale(int iPlr) {
  if(!ArwCount()) return(0);
  var i, iID, iCount, idPack, iPackCount, iSellCount, pObj;
  // Jede Pfeilart einzeln durchgehen
  for(i = 0; iID = ArwID(i); i++) {
    // Sind Pfeile dieser Art im Köcher vorhanden?
    iCount = ArwCount(i);
    if(!iCount) continue;
    // Das Pfeilpaket soll mitsamt dem Packverhältnis her
    idPack = iID->~PackTo();
    // Kein Paket dieser Art vorhanden?
    if(!idPack) {
      // Alle Pfeile einzeln verkaufen
      while(iCount--)
        Sell(iPlr,CreateContents(iID));
      continue;
    }
    iPackCount = idPack->~MaxPackCount();
    // Wieviele Pakete können aus dieser Pfeilanzahl erstellt werden?
    iSellCount = iCount/iPackCount;
    // Pfeilpakete gleich verkaufen
    while(iSellCount--)
      // Um es etwas einfacher zu gestalten
      Sell(iPlr,CreateContents(idPack));
    // Ist noch etwas übrig geblieben?
    if(iCount %= iPackCount) {
      // Der Rest wird zu einem Pack zusammengefasst und verkauft
      pObj = CreateContents(idPack);
      pObj->~SetPackCount(iCount);
      Sell(iPlr,pObj);
    }
    // Nächstes Pfeilpaket!
  }
}

/* Pfeile wechseln */

protected func Activate(pClonk)
{
  [$TxtChange$]
  Shift();
  if(ArwID()) PlayerMessage(GetOwner(pClonk),"%s",this(),GetName(0,ArwID()));
  return(1);
}

/* Pfeile aufnehmen */

protected func ContextMenu(object pClonk)
{
  [$MnuTakeArrows$|Image=QUIV]
  ExtraMenu(pClonk);
}

private func ExtraMenu(object pClonk)
{
  // Clonk soll anhalten
  SetComDir(COMD_Stop(),pClonk);
  // Das Menü
  CreateMenu(QUIV, pClonk, 0, 0, "$TxtNoArrows$");
  for(var i = 0; ArwCount(i); i++)
    AddMenuItem(GetName(0,ArwID(i)),"TakeArrow",ArwID(i),pClonk,ArwCount(i),pClonk);
  for(i = 0; Contents(i, pClonk) != 0; i++)
    if(Contents(i, pClonk)->~IsArrow() || Contents(i, pClonk)->~IsArrowPack()) {
      AddMenuItem("$TxtPutIn$","PutInArrow",QUIV,pClonk,0,pClonk,"$DescPutIn$");
      break;
    }
  return();
}

private func PutInArrow(id iID, object pClonk) {
  var i;
  while(ArwCount(i)) i++;
  Entrance(pClonk);
  ExtraMenu(pClonk);
  if(!ArwCount(i)) i--;
  SelectMenuItem(i,pClonk);
}

private func TakeArrow(id iID, object pClonk, bool fSpecial)
{
  var i, iSelect, idPack;
  var iTake = 1;
  for(i = 0; ArwID(i); i++) if(ArwID(i) == iID) break;
  iSelect = i;
  idPack = iID->~PackTo();
  if(fSpecial) iTake = idPack->~MaxPackCount();
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking",this(),1);
  // Pfeil(e) erzeugen und gleich per Pfeil-Script zusammenpacken
  CreateContents(iID,pClonk,iTake);
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking",this());
  ArwCount(i) -= iTake;
  if(!ArwCount()) Shift(1);
  else if(!ArwCount(i)) {
    for(i; ArwID(i); i++) {
      ArwCount(i) = ArwCount(i+1);
      if(!ArwCount(i)) {
        ArwID(i) = 0;
        break;
      }
      ArwID(i) = ArwID(i+1);
    }
    SetGraphics(0,0,0,1+i*3,GFXOV_MODE_Picture);
    SetGraphics(0,0,0,2+i*3,GFXOV_MODE_Picture);
    SetGraphics(0,0,0,3+i*3,GFXOV_MODE_Picture);
  }
  MakePicture();
  ExtraMenu(pClonk);
  SelectMenuItem(iSelect, pClonk);
}

public func CanPack(object pClonk)
{
  // Nur Clonks packen Pfeile
  if(!(GetOCF(pClonk) & OCF_Alive)) return(0);
  if(GetEffect("IntNoPacking",this())) return(0);
  if(pClonk) if(GetEffect("IntNoPacking",pClonk)) return(0);
  else if(Contained()) if(GetEffect("IntNoPacking",Contained())) return(0);
  return(1);
}

/* Der Köcher dient als Extra um Pfeile besser zu tragen */
public func GetResearchBase() { return(BOW2); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }