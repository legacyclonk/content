/*-- Pfeilpaket --*/

#strict

/* Initialisierung */

protected func Initialize()
{
  // Bild anpassen
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit");
}

/* Paketeigenschaften */

public func UnpackTo() { return(ARRW); }
public func IsArrowPack() { return(1); }
public func MaxPackCount() { return(10); }
public func IsAnvilProduct() { return(1); }

/* Packet Zerlegung */
public func Unpack(pTo)
{
  if(!pTo) pTo = Contained(this());
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking", pTo, 1);
  // Pfeile erzeugen
  for(var i = 0; i < PackCount(); i++)
    CreateContents(UnpackTo(), pTo);
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", pTo);
  // Fertig
  RemoveObject();
  return(1);
}

/* Manuelle Zerlegung */

protected func Activate(pClonk)
{
  [$TxtUnpack$]
  return (Unpack(pClonk));
}

/* Pfeilverwaltung */
local iUsedItems;

public func PackCount() { return(MaxPackCount()-LocalN("iUsedItems")); }

public func SetPackCount(iAmount) { iUsedItems = PackCount()-iAmount; UpdatePicture(); UpdateMass(); }

public func DoPackCount(iChange) 
{
  iUsedItems-=iChange;
  if(PackCount()<=0) return(RemoveObject());
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func GetItem()
{
  // Effekt zum Deaktivieren des Pfeilpackens
  AddEffect("IntNoPacking", Contained(), 1);
  // Pfeil ausgeben
  var obj = CreateContents(UnpackTo(), Contained());
  // Effekt wieder entfernen
  RemoveEffect("IntNoPacking", Contained());
  // Einer weniger
  DoPackCount(-1);
  return(obj);
}

public func SplitPack(int iAmount, object pClonk)
{
  // Nur wenn überhaut so viele Teile da sind
  if(iAmount >= MaxPackCount()-iUsedItems) return(0);
  // 0 Absplitten? nee
  if(!iAmount) return(0);
  // Vom Paket werden iAmount Teile abgetrennt und in ein neues Pack gesteckt
  var pSplit;
  if(Contained()) {
    pSplit = CreateContents(GetID(),Contained());
    SetOwner(GetOwner(),pSplit);
  }
  else pSplit = CreateObject(GetID(),0,GetDefBottom()-GetY(),GetOwner());
  pSplit->SetPackCount(iAmount);
  DoPackCount(-iAmount);
  return(1);
}

public func IsNotFull() { return(MaxPackCount() > PackCount()); }

/* Einsammeln */
public func Entrance(object pContainer)
{
  if(!(GetOCF(this()) & OCF_Fullcon)) return(false);
  // Ist dies überhaupt ein Pfeilpaket?
  if(IsArrowPack()) {
    // Hat der Clonk einen Köcher bei sich?
    var pObj;
    if(pObj = FindContents(QUIV,pContainer))
      // Keine Außnahmen vorhanden?
      if(pObj->~CanPack(pContainer))
        pObj->~AddPack(this());
    // Pfeilpaket noch vorhanden?
    if(!PackCount()) return(1);
  }
  // In vorhandene Packte integrieren
  if(JoinPack(pContainer)) return(1);
  // Grafik anpassen
  UpdatePicture();
  // Masse anpassen
  UpdateMass();
}

public func JoinPack(object pContainer)
{
  // In vorhandene Pfeilpakete integrieren
  var pObj, iChange, iAmount, iList;
  // Alle gleichen Pakete im Clonk durchsuchen
  iList = FindObjects(Find_Exclude(this()),Find_Container(pContainer),Find_ID(GetID()),Find_OCF(OCF_Fullcon));
  for(pObj in iList) {
    iAmount = PackCount();
    // Wie viel Platz ist im neuen Paket?
    iChange = MaxPackCount() - pObj->PackCount();
    // Wir können aber höchstens so viele abgeben wie wir haben
    if(iChange>iAmount) iChange = iAmount;
    // Pfeile abgeben
    pObj->DoPackCount(iChange);
    // Wenn das Paket leer ist aufhören
    if(iAmount-iChange<=0) return(RemoveObject());
    DoPackCount(-iChange);
  }
  return(0);
}


/* Grafik anpassen */
public func UpdatePicture()
{
  if(PackCount()>9)
  {
    SetGraphics(0,0,GetNumberID(PackCount()/10),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-12000,0,400,+10000, this(), 1);
  }
  else SetGraphics(0,0,0,1,0);  

  SetGraphics(0,0,GetNumberID(Mod(PackCount(),10)),2,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,-5000,0,400,+10000, this(), 2);
}

/* Masse anpassen */
public func UpdateMass()
{
  SetMass(GetMass(0,UnpackTo())*Max(PackCount(),1));
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}

/* Wert */

public func CalcValue(object pInBase, int iForPlayer)
{
  // Je nach Anzahl
  return(GetValue(0, GetID())*PackCount()/MaxPackCount());
}

protected func SellTo(int iPlr)
{
  // Nicht volle Pfeilpakete werden zunächst nur zu einem Pfeile Heimatbasismaterial verkauft
  // Im Sale-Callback werden zusätzliche Pfeile angerechnet und ggf. HomebaseMaterial zusammengefasst
  if (PackCount() < MaxPackCount()) return UnpackTo();
  // Volle Pakete sind kein Problem
  return GetID();
}
  
protected func Sale(int iPlr)
{
  // Heimatbasismaterial anpassen
  // Paket war voll? Dann stimmt der normale Verkauf
  var cnt = PackCount(), cntpack = MaxPackCount(), idArrow;
  if (cnt == cntpack) return();
  // Ansonsten wurde nur zu einem Pfeil dem Heimatbasismaterial angerechnet. Zusätzliche Pfeile ins Heimatbasismaterial
  idArrow = UnpackTo();
  if (cnt > 1) DoHomebaseMaterial(iPlr, idArrow, cnt-1);
  // Und Heimatbasispfeile zu Paketen zusammenfassen
  if (cntpack)
    {
    cnt = GetHomebaseMaterial(iPlr, idArrow) / cntpack;
    if (cnt)
      {
      DoHomebaseMaterial(iPlr, idArrow, -cnt*cntpack);
      DoHomebaseMaterial(iPlr, GetID(), cnt);
      }
    }
}
