/*-- Topf --*/

#strict

// Feuer, Wasser enthalten, Funkenfarbe
local pFire, fWater, iColor;

/* Initialisierung */

protected func Initialize()
{
  pFire = CreateObject(CPFR,0,+25);
  // Alle Komponenten löschen
  var i, idType;
  while(idType = GetComponent(0, i++, 0, GetID()) )
    SetComponent(idType, 0, this() );
}

/* TimerCall */

private func CheckContents()
{
  if(GetAction(FindObject(CPFR,-50,0,100,100)) eq "Burn")
    if(Contents())
      if(ActIdle())
        if(IsHot())
          for(var i=0;i<ContentsCount();++i)
            if(Contents(i)->~CanBeCooked())
        {
              Contents(i)->~Cook();
              Sound("Cook");
          return();
        }
}

public func ControlThrow(pClonk)
{
  var iObj;
  // Nur wenn inaktiv
  if(GetAction()S="Brew") return();
  // Wasser einfüllen
  if(!fWater)
    if(!CookingContents())
      if(FillInWater(pClonk))
        return();
  // Trank drinne
  if(GetAction() eq "Potion") {
    if(Contents(0,pClonk))
      // Clonk will Trank holen
      if(Contents(0,pClonk)->~IsPotionContainer()) return(Fill(Contents(0,pClonk)));
    return(1);
  }
  // Essen reinlegen
  // Wenn der Spieler kein Essen in der Hand hat, Inhalt anzeigen
  if(Contents(0,pClonk))
    if(!Contents(0,pClonk)->~CanBeCooked())
      return(SetCommand(pClonk, "Get", this(), 0, 0, 0, 1));
}

public func FillInWater(pClonk)
{
  var iObj = Contents(0,pClonk);
  var iID = GetID(iObj);
  var i;
  // Von nun an wird das Wasser, das in das Topf gefüllt wird, nicht herrauskommen
  if(iID == BOWL) {
    if(GetAction(iObj) ne "Water") return(0);
    ObjectSetAction(iObj, "Idle");
    LocalN("iWater", iObj) = 0;
    SetGraphics("",iObj);
    SetColorDw(0,iObj);
    Sound("Splash*");
    if(iObj) ShiftContents(pClonk, 0, GetID(iObj)); 
    return(fWater = true);
  }
  if(iID == WDWB || iID == WBRL) {
    if(LocalN("iFillLevel", iObj) < 50) return(0);
    LocalN("iFillLevel", iObj) = 0;
    ChangeDef(iObj->~EmptyBarrelID(), iObj);
    Sound("Splash*");
    if(iObj) ShiftContents(pClonk, 0, GetID(iObj)); 
    return(fWater = true);
  }
  if(iID == WSKI) {
    if(LocalN("iWater", iObj) < 30) return(0);
    LocalN("iWater", iObj) -= 30;
    Contents(0,pClonk)->~UpdateGraphics();
    Sound("Splash*");
    return(fWater = true);
  }
  // Sonstige andere Wasserbehälter sind auch erwünscht
  // Diese können per appendto hinzugefügt werden
  return(_inherited(pClonk));
}


// Inhalt vernichten
private func DestroyMenu(pClonk)
{
  CreateMenu(CPFR, pClonk, this(), 0, "$DestroyMenu$", 0, 1);
  ExtraInformation(pClonk);
  AddMenuItem("$CmdCancel$", "Cancel", GetID(), pClonk, 0, 0, "$CancelDesc$");
  AddMenuItem("$CmdDestroyContents$", "DestroyContents", MCMX, pClonk, 0, 0, "$DestroyContentsDesc$");
  return(1);
}

private func Cancel() { return(); }

private func ExtraInformation(pClonk)
{
  _inherited(pClonk);
  var i, sText;
  if(GetAlchem(0,0,this())) {
    sText = "  ";
    for(i = 0; GetAlchem(0,i,this()); i++) {
      if(GetAlchem(0,i,this())) sText = Format("%s%d{{%i}}", sText, GetAlchem(GetAlchem(0,i,this()),0,this()), GetAlchem(0,i,this()));
      if(GetAlchem(0,i+1,this())) sText = Format("%s ", sText);
    }
    AddMenuItem("$CauldronContains$", 0, ALC_, pClonk);
    AddMenuItem(sText, 0, TIM1, pClonk);
  }
  return(1);
}



protected func DestroyContents()
{
  // Alles was drin ist, muss weg
  fWater = 0; // Wasser
  // Alchemiezeugs
  var idAlchem;
  while(idAlchem = GetAlchem(0,0,this()))
    DoAlchem(idAlchem, -GetAlchem(idAlchem,0,this()), this());
  // Geräusch
  Sound("Fuse");
  // Aktion zurücksetzen
  SetAction("Idle");
  // Ok
//  CheckPotionContents();
  return(1);
}

/* Aufnahme */

protected func Collection() { Sound("Clonk"); }

/* Aufschlag */

protected func Hit() { Sound("ClonkHit*"); }

/* Campfeuer brennt */
protected func IsHot() {
  if(!pFire) return();
  return(GetAction(pFire) eq "Burn");
}

// Nur essbare Sachen aufnehmen, 
// verhindert z.B. die Aufnahme vorbeifliegender Speere
public func RejectCollect(idObj, pObj)
{
  if(!ActIdle()) return(1);
  if(!idObj->~CanBeCooked()) return(1); 
}

/* Tränke brauen */

// Inhalt vernichten
public func ControlDigDouble(object pCaller)
{
  [$TxtProduce$|Image=BOWL]
  // Ist ein Trank vorhanden?
  if(GetAction() eq "Potion")
    return(DestroyMenu(pCaller));
  // Wird noch zum Kochen benutzt
  if(CookingContents()) return(Message("$MsgNotEmpty$", this()));

  var pBag = FindObject(ALC_, 0, 0, 0, 0, 0, "Belongs", pCaller);
  if(!pBag) return(Message("$MsgNoBag$", this()));

  return(ProductionMenu(pCaller, 0, pBag));
}

/* Produktion */

public func ProductionMenu(object pWorker, int iSelection, object pBag) {
  // Nur wenn inaktiv
  if (!ActIdle()) return(0);
  // Effekt an den Clonk hängen
  if(!GetEffect("AlchemMenu", pWorker)) AddEffect("AlchemMenu", pWorker, 99, 20, 0, GetID());
  // Ansonsten Menü nach Clonkvorgabe erzeugen
//  else return(ProductionMenuOrdered(pWorker, iSelection, pBag));
  // Menü schließen
  CloseMenu(pWorker);
  // Menü erzeugen
  CreateMenu(CLD2, pWorker, this(), 1, "");
  // alle Zutaten auflisten
  var idAlchem, iAlchem;

  // Komponenten aus dem Clonk
  for(var i=0, j=0, k=0; idAlchem=GetAlchem(0,i,pBag); ++i)
  {
    iAlchem = GetAlchem(idAlchem,0,pBag);
    AddMenuItem(Format("(%d) %s [%d]", GetAlchemCount(0, true), GetName(0,idAlchem), iAlchem), "AddComponent", idAlchem, pWorker, GetAlchemCount(idAlchem), ObjectNumber(pWorker), 0, 128, 0, k);
    // id speichern für MenuQueryCancel
    EffectVar(j, pWorker, GetEffect("AlchemMenu", pWorker)) = idAlchem;
    Var(j++) = idAlchem;
    k++;
  }
  // Komponenten aus dem Topf
  idAlchem = 0;
  for(var i=0, l,m ; idAlchem = GetAlchem(0,i,this()) ; ++i)
  {
    m = false;

    for(l=0 ; Var(l) ; l++)
      if(Var(l) == idAlchem)
        m = true;
    if(m) continue;

    // id speichern für MenuQueryCancel
    EffectVar(j, pWorker, GetEffect("AlchemMenu", pWorker)) = idAlchem;
    Var(j++) = idAlchem;
    AddMenuItem(Format("(%d) %s", GetAlchemCount(0, true), GetName(0,idAlchem)), "Void", idAlchem, pWorker, GetAlchemCount(idAlchem), ObjectNumber(pWorker), 0, 128, 0, k);
    k++;
  }

  // Andere Aktionen
  AddMenuItem("$CmdStart$","StartProduction", CXCN, pWorker, 0, ObjectNumber(pWorker), "$StartProductionDesc$");
  AddMenuItem("$CmdCancel$","EndMenu", MCMX, pWorker, 0, ObjectNumber(pWorker), "$CancelDesc$");
  AddMenuItem("$CmdHelp$","Help", CXIN, pWorker, 0, ObjectNumber(pWorker), "$HelpDesc$");

  SelectMenuItem(iSelection, pWorker);
  // Fertig
  return(1);
}

// Hilfe
public func Help(id dummy, int pWorkerNo)
{
  // Hilfstext anzeigen
  CreateMenu(CXIN, Object(pWorkerNo), this(), 0, "$CmdHelp$", 0, 2);
  AddMenuItem("$CmdHelp$", "", TIM1, Object(pWorkerNo),0,0,"$MsgPotionHelp$");
}

public func EndMenu(id dummy, int pWorkerNo)
{
  var pWorker = Object(pWorkerNo);
  if(!pWorker) return();
  return(CloseMenu(pWorker));
}

// Auswahl einer Inhaltskomponente -> Menü wieder öffnen
private func Void(id dummy, int pWorkerNo, bool dummy2, int MenuItem)
{
  if(!Object(pWorkerNo)) return();
  var pBag = FindObject(ALC_, 0, 0, 0, 0, 0, "Belongs", Object(pWorkerNo));
  if(pBag) ProductionMenu(Object(pWorkerNo), MenuItem, pBag);
}

// Auswahl einer Taschenkomponente
private func AddComponent(id idAlchem, int pWorkerNo, bool Right, int MenuItem)
{
  var pWorker = Object(pWorkerNo);
  var pBag = FindObject(ALC_, 0, 0, 0, 0, 0, "Belongs", pWorker);
  var iAlchem = GetAlchem(idAlchem,0,pBag);

  if(!pWorker || !pBag) return();
  // Alles scheint rechtens
  // Alles reintun, wenn gefordert
  if(GetAlchemCount(0,true) < GetMaxAlchemCount()) Sound("Grab");
  else Message("$MsgFull$", this());
  while(iAlchem > 0)
    {
    // Topf voll
    if(GetAlchemCount(0,true) >= GetMaxAlchemCount())
      break;
    // Dem Beutel ein Stück abziehen und hier reinlegen
    DoAlchem(idAlchem, -1, pBag);
    DoAlchem(idAlchem, +1, this());
    if(!Right) iAlchem = 0;
    else iAlchem--;
    }
  // Inhalt testen
//  CheckPotionContents();
  // Menü wieder aufrufen
  ProductionMenu(pWorker, MenuItem, pBag);
  return(1);
}

// Rausnehmen
public func MenuQueryCancel(int MenuItem, object pWorker)
{
  // Hilfsmenü weiterblättern	
  if(GetMenu(pWorker) == CXIN)
  {
    CreateMenu(BOWL,pWorker, this(), 0, "$CmdHelp$", 0, 2);
    AddMenuItem("$CmdHelp$", "", TIM1, pWorker,0,0,"$MsgPotionHelp2$");	  
    return(1);
  }
	
  // Falsches Menü
  if(GetMenu(pWorker) != CLD2) return();

  var pBag = FindObject(ALC_, 0, 0, 0, 0, 0, "Belongs", pWorker);
  if(!pBag) return();
  // pWorker hat keinen Effekt
  if(!GetEffect("AlchemMenu", pWorker)) return(ProductionMenu(pWorker, MenuItem, pBag));
  // ID herausfinden
  var idAlchem = EffectVar(MenuItem, pWorker, GetEffect("AlchemMenu", pWorker));
  if(!idAlchem) return(ProductionMenu(pWorker, MenuItem, pBag));
  // Nicht mehr vorhanden?
  if(!GetAlchem(idAlchem,0,this())) return(ProductionMenu(pWorker, MenuItem, pBag));
  // Einen rausnehmen und dem Clonk geben
  DoAlchem(idAlchem, -1, this());
  DoAlchem(idAlchem, +1, pBag);
  Sound("Grab");
  // Inhalt testen
//  CheckPotionContents();
  // Neues Menü
  return(ProductionMenu(pWorker, MenuItem, pBag));
}

// Prüft auf Komponenten
private func GetAlchemCount(id idAlchem, bool All)
{
  if(All)
    return(AlchemCount(this()));
  else
    return(GetAlchem(idAlchem,0,this()));
}

public func GetMaxAlchemCount() { return(9); }

/* Brauen */

protected func StartProduction(id dummy, int iPotion)
{	
  // Zu wenig Zutaten vorhanden?
  var count, i, id;
  while(id = GetComponent(0, i++)) count += GetComponent(id, 0);
  if(count<3) return(Message("$MsgNotEnoughIngredients$", this()));
  // Kein Feuer? Kein Wasser?
  if(!IsHot() && !fWater) return(Message("$MsgNoWaterFire$", this()));
  if(!IsHot()) return(Message("$MsgNoFire$", this()));
  if(!fWater) return(Message("$MsgNoWater$", this()));
  fWater = false;
  // Farbe zusammenstellen
  var j, r, g, b;
  i = id = count = 0;
  while(id = GetComponent(0, i++) )
  {
    j = GetComponent(id, 0);	  
    while(j--)
    {
      SplitRGBaValue(DefinitionCall(id,"PotionColor"), Var(0), Var(1), Var(2), Var(3)); 
      r+=Var(0);
      g+=Var(1);
      b+=Var(2);
      count++;
    }
  }
  iColor = RGB(r/count, g/count, b/count);
  // Brauen beginnen
  SetAction("Brew");
}

public func Brewing()
{
  Sound("Alchem*");
  CreateParticle("MSpark",RandomX(-8,8),-8,RandomX(-5,5),RandomX(0,-28),RandomX(10,40),iColor);
  CreateParticle("MSpark",RandomX(-8,8),-8,RandomX(-5,5),RandomX(-8,-15),RandomX(20,50),iColor);
}

// Füllen
public func Fill(pBowl)
{
  var i, id;
  while(id = GetComponent(0, i++) )
  { 
    if(GetComponent(id))
      AddPotionAlchem(pBowl, id, GetComponent(id));
  }
  pBowl->~PotionFilled(this());
  Sound("Splash1");
//  SetAction("Idle");
//  DestroyContents();
}

/* Inhaltscheck */

public func CookingContents()
{
  for(var i=0 ; Contents(i) ; i++)
    if(Contents(i)->~GetFeedValue())
      return(1);
}
/*
public func CheckPotionContents()
{
  // Schaut nach, ob ein Zaubertrank enthalten ist
  if(fWater) { potion = true; return(1); }
  if(potionnumber) { potion = true; return(1); }
  if(GetAlchemCount(0,true)) { potion = true; return(1); }
  // Nix drin
  potion = false;
}*/

/* Effekt */

public func FxAlchemMenuTimer(target, no)
{
  // Clonk hat das Menü nicht mehr offen
  if(GetMenu(target) != CLD2) return(-1);
}

// Ist alchemistischer Behälter
public func IsAlchemContainer() { return(1); }
// Wenn leer, nicht verschwinden
public func Exhaustion() { return(1); }

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

// ohne Tipi keinen Kochtopp
public func GetResearchBase() { return(TIPI); }

//----- Zaubertrank ----//

public func Physical(i) {
  if(i==0) return("Walk");
  if(i==1) return("Jump");
  if(i==2) return("Scale");
  if(i==3) return("Hangle");
  if(i==4) return("Dig");
  if(i==5) return("Breath");
  if(i==6) return("Swim");
  if(i==7) return("Throw");
  if(i==8) return("Push");
  if(i==9) return("Fight");
}

func FxPotionGetColor(pTarget, iPotionEffect)
{
  var i=-1, j, color, id, count, iAlpha;
  while(id = EffectVar(++i*2, pTarget, iPotionEffect))
  {
    j = EffectVar(i*2+1, pTarget, iPotionEffect);
    while(j--)
    {   
    SplitRGBaValue(id->~PotionColor(), Var(count*3), Var(count*3+1), Var(count*3+2), iAlpha);
    count++;
    }
  }
  var iR, iG, iB;
  var i=0;
  while(i<count)
  {   
    iR += Var(i*3);
    iG += Var(i*3+1);
    iB += Var(i*3+2);
    i++;            
  }
  return(RGB(iR/count, iG/count, iB/count));
}

func FxPotionGetHeal(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", IBLO, ILOA, iMystic));    
}

func FxPotionGetFire(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", IBON, IROC, iMystic));    
}

func FxPotionGetWipf(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", IHAI, IBLO, iMystic));    
}

func FxPotionGetFish(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", ILOA, IHAI, iMystic));    
}

func FxPotionGetMonster(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", IPOI, IBON, iMystic));    
}

func FxPotionGetStone(pTarget, iPotionEffect, iMystic)
{
  return(EffectCall(pTarget, iPotionEffect, "GetXXX", IROC, IPOI, iMystic));    
}

func FxPotionGetXXX(pTarget, iPotionEffect, idFirst, idSecond, iMystic)
{
  var i=-1, j, id, count, iFirst, iSecond, iAmount;
  while(id = EffectVar(++i*2, pTarget, iPotionEffect))
  {
    j = EffectVar(i*2+1, pTarget, iPotionEffect);
    if(id == idFirst) iFirst = j;
    if(id == idSecond) iSecond = j;
    count += j;
  } 
  if(!iFirst || !iSecond) return();
  // Beide sollen im Verhältnis 2:1 da sein. Mehr von einer Sorte zählt nichts
  if(iFirst>iSecond*2) iAmount = iSecond*3;
  else iAmount = iFirst / 2 * 3;
  return(iAmount*100/9+(iMystic*iAmount/9)); // Die wirkenden Zutaten in Prozent + Mystikbonus
}

// Gibt die Anzahl einer einzigen Zutat zurück
func FxPotionGetYYY(pTarget, iPotionEffect, idAlchem)
{
  var i=-1, id;
  while(id = EffectVar(++i*2, pTarget, iPotionEffect))
  {
    if(id == idAlchem) return(EffectVar(i*2+1, pTarget, iPotionEffect));
  }
}

func FxPotionActivate(pTarget, iPotionEffect, pClonk)
{
  var i=-1, count;
  while(EffectVar(++i*2, pTarget, iPotionEffect))
    count += EffectVar(i*2+1, pTarget, iPotionEffect);
  var iTime = count*4*35; // Je mehr drinnen ist desto länger hält er
  // Effekt zeitversetzt starten
//  Schedule(Format("AddEffect(\"ActivePotion\",this(),160,1,0,CLD2,Object(%d) ,%d,%d)", ObjectNumber(pTarget), iPotionEffect, iTime), 80,0,pClonk);
  AddEffect("ActivePotion",pClonk,160,1,0,CLD2,pTarget ,iPotionEffect, iTime);//", ObjectNumber(pTarget), iPotionEffect, iTime)
  return(1);    
}

global func AddPotionAlchem(pObj, idAlchem, iCount)
{
  var ef = GetEffect("Potion", pObj);   
  if(ef<=0) ef = AddEffect("Potion", pObj, 1, 0, 0, CLD2);
  var i=-1, id;
  while( id = EffectVar(++i*2, pObj, ef) ) if( id == idAlchem ) break;
  EffectVar(i*2, pObj, ef) = idAlchem;
  EffectVar(i*2+1, pObj, ef) += iCount;
}

private func FxActivePotionStart (object pTarget, int iEffectNumber, int iTemp, pPotionTarget, iPotionEffect, iTime)
{
  if(iTemp) return();

  var iMystic = GetClanMystic(GetClan(pTarget));
    
  // Der Trankeffekt ist eine Mischung aus den Fantasytränken
  EffectVar(19,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetColor", iMystic);
  EffectVar(20,pTarget,iEffectNumber) += 1000/EffectCall(pPotionTarget, iPotionEffect, "GetHeal", iMystic);
  EffectVar(21,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetFire", iMystic);
  EffectVar(22,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetWipf", iMystic);
  EffectVar(23,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetFish", iMystic);
  EffectVar(24,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetMonster", iMystic);
  EffectVar(25,pTarget,iEffectNumber) += EffectCall(pPotionTarget, iPotionEffect, "GetStone", iMystic);

  var i, iSum;
  for(i = 20; i <= 25; i++)
    iSum += EffectVar(i,pTarget,iEffectNumber);

  // Keine Wirkung? Dann Nebenwirkung :-)
  if(!iSum) {
    var j, idAlchem = IBLO, idAnimal;
    i = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", IBLO);
    if( (j = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", IBON)) > i) { i = j; idAlchem = IBON; }
    if( (j = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", IHAI)) > i) { i = j; idAlchem = IHAI; }
    if( (j = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", ILOA)) > i) { i = j; idAlchem = ILOA; }
    if( (j = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", IPOI)) > i) { i = j; idAlchem = IPOI; }
    if( (j = EffectCall(pPotionTarget, iPotionEffect, "GetYYY", IROC)) > i) { i = j; idAlchem = IROC; }

    if(idAlchem == IBLO) { EffectVar(20,pTarget,iEffectNumber) = -700/(i*10); }
    else if(idAlchem == IBON) { EffectVar(28,pTarget,iEffectNumber) = IBON; }
    else if(idAlchem == IHAI) { idAnimal = WIPF; }
    else if(idAlchem == ILOA) { idAnimal = FISH; }
    else if(idAlchem == IPOI) { idAnimal = MONS; }
    else if(idAlchem == IROC) { idAnimal = ROCK; }

    if(idAnimal) {
      // Das pExtra sorgt dafür, dass man beim Verwandeln in ein Monster keinen Schaden nimmt
      var pExtra = CreateObject(TIM1,GetX(pTarget),GetY(pTarget));
      Enter(pExtra,pTarget);
      EffectVar(27,pTarget,iEffectNumber) = CreateObject(idAnimal, GetX(pTarget), GetY(pTarget)+GetDefCoreVal("Height",0,idAnimal)/2);
      SetXDir(GetXDir(pTarget), EffectVar(27,pTarget,iEffectNumber));
      SetYDir(GetYDir(pTarget), EffectVar(27,pTarget,iEffectNumber));
      Enter(EffectVar(27,pTarget,iEffectNumber), pTarget);
      if(pExtra) RemoveObject(pExtra);
      if(idAnimal != ROCK) {
        SetDir(GetDir(pTarget), EffectVar(27,pTarget,iEffectNumber));
        SetComDir(GetComDir(pTarget), EffectVar(27,pTarget,iEffectNumber));
        if(GetAction(EffectVar(27,pTarget,iEffectNumber)) eq "Turn")
          EffectVar(27,pTarget,iEffectNumber)->~SetAction(GetActMapVal("NextAction","Turn",GetID(EffectVar(27,pTarget,iEffectNumber))));
        AddEffect("PossessionSpell", EffectVar(27,pTarget,iEffectNumber), 200,1,0,CLD2);
        // Hiermit behält der Clonk seine max. Atemluft
        SetPhysical("Breath",GetPhysical("Breath",0,pTarget),2,EffectVar(27,pTarget,iEffectNumber));
        if(GetBreath(pTarget) < GetPhysical("Breath",0,pTarget)/1000)
          DoBreath(-GetBreath(pTarget),EffectVar(27,pTarget,iEffectNumber));
//          DoBreath(GetBreath(pTarget) * GetPhysical("Breath",0,EffectVar(27,pTarget,iEffectNumber)) / GetPhysical("Breath",0,pTarget) - GetBreath(EffectVar(27,pTarget,iEffectNumber)),EffectVar(27,pTarget,iEffectNumber));
      }
    }
  }

  EffectVar(26,pTarget,iEffectNumber) = iTime;
  AddEffect("Obj3D",pTarget,300,1,0,CLD2,CLD2, EffectVar(19,pTarget,iEffectNumber));

  // Jetzt werden mal alle gleich in nur einem Zug verbessert.
  EffectCall(pTarget, iEffectNumber, "AdjustPhysical", pPotionTarget, iPotionEffect, iMystic);

  RemoveEffect("Potion", pPotionTarget);
}

private func FxActivePotionAdjustPhysical(pTarget, iEffectNumber, pPotionTarget, iPotionEffect, iMystic)
{
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetWipf")) {
  if(EffectVar(22,pTarget,iEffectNumber)) {
    SetPhysical("Walk",  GetPhysical("Walk",  0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetWipf",   iMystic)* 300,3,pTarget);
    SetPhysical("Jump",  GetPhysical("Jump",  0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetWipf",   iMystic)* 180,3,pTarget);
    SetPhysical("Dig",   GetPhysical("Dig",   0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetWipf",   iMystic)* 500,3,pTarget);
  }
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetFish")) {
  if(EffectVar(23,pTarget,iEffectNumber)) {
    SetPhysical("Breath",GetPhysical("Breath",0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetFish",   iMystic)* 600,3,pTarget);
    SetPhysical("Swim",  GetPhysical("Swim",  0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetFish",   iMystic)*1000,3,pTarget);
  }
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetMonster")) {
  if(EffectVar(24,pTarget,iEffectNumber)) {
    SetPhysical("Scale", GetPhysical("Scale", 0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetMonster",iMystic)* 250,3,pTarget);
    SetPhysical("Hangle",GetPhysical("Hangle",0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetMonster",iMystic)* 200,3,pTarget);
    SetPhysical("Throw", GetPhysical("Throw", 0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetMonster",iMystic)* 100,3,pTarget);
    SetPhysical("Push",  GetPhysical("Push",  0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetMonster",iMystic)* 100,3,pTarget);
    SetPhysical("Fight", GetPhysical("Fight", 0,pTarget)+EffectCall(pPotionTarget,iPotionEffect,"GetMonster",iMystic)* 900,3,pTarget);
  }
}

private func FxActivePotionRestorePhysical(pTarget, iEffectNumber)
{
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetWipf")) {
  if(EffectVar(22,pTarget,iEffectNumber)) {
    ResetPhysical(pTarget, "Walk");
    ResetPhysical(pTarget, "Jump");
    ResetPhysical(pTarget, "Dig");
  }
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetFish")) {
  if(EffectVar(23,pTarget,iEffectNumber)) {
    ResetPhysical(pTarget, "Breath");
    ResetPhysical(pTarget, "Swim");
  }
//  if(EffectCall(pPotionTarget, iPotionEffect, "GetMonster")) {
  if(EffectVar(24,pTarget,iEffectNumber)) {
    ResetPhysical(pTarget, "Scale");
    ResetPhysical(pTarget, "Hangle");
    ResetPhysical(pTarget, "Throw");
    ResetPhysical(pTarget, "Push");
    ResetPhysical(pTarget, "Fight");
  }
}
  
private func FxActivePotionTimer(object pTarget, int iEffectNumber, int iTime) 
{
//  iPotionEffect   
  //pClonk->DoMagicEnergy(Min(PMAN->Intensity(),GetPhysical("Magic",0)/1000-GetMagicEnergy()));
  if(EffectVar(20,pTarget,iEffectNumber)>0)
  { 
    if(!Mod(iTime, EffectVar(20,pTarget,iEffectNumber)))
      DoEnergy(1, pTarget);   
  }
  if(EffectVar(20,pTarget,iEffectNumber)<0)
  { 
    if(!Mod(iTime, -EffectVar(20,pTarget,iEffectNumber)))
      DoEnergy(-1, pTarget);      
  }
  if(EffectVar(28,pTarget,iEffectNumber)==IBON)
  {
    Incinerate(pTarget); 
  }
	  
  // Zeit abgelaufen!
  if(iTime >= EffectVar(26,pTarget,iEffectNumber)) return(-1);
}

private func FxActivePotionDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {

  // Feuer: ganz
  if(iCause == 35) if(Random(100)<EffectVar(21,pTarget,iEffectNumber) || ( EffectVar(28,pTarget,iEffectNumber)==IBON && Random(5) )) return();
    
  if(iCause != 34 && iCause != 40 && iCause != 33) return(iDmgEngy);

  CreateParticle("MSpark", GetX(pTarget)+RandomX(-8,8),GetY(pTarget)+RandomX(-10,10), 0, 0, 40, RGB(128,128,128));

  // Objekttreffer: drittel
  if(iCause == 34) return(iDmgEngy-iDmgEngy*EffectVar(25,pTarget,iEffectNumber)*2/300);
  // Clonk-zu-Clonk-Kampf: halb
  if(iCause == 40) return(iDmgEngy-iDmgEngy*EffectVar(25,pTarget,iEffectNumber)/200);
  // Explosion: 2/3
  if(iCause == 33) return(iDmgEngy-iDmgEngy*EffectVar(25,pTarget,iEffectNumber)/300);
}

private func FxActivePotionEffect(string szNewEffectName)
{
  // gleichen Effekt zusammenfassen (Trank zweimal hintereinander getrunken)
  // Nein, weitere Trankeeffekte lieber verhindern, könnte sonst zu Bugs oder doppelten Verbesserungen führen
  if(szNewEffectName eq "ActivePotion") return(-1);
}

private func FxActivePotionAdd(object pTarget, int iNumber, string szNewEffectName, int iNewEffectTimer, int iTime) {
  // Effekt übernehmen
  //EffectVar(0,pTarget,iNumber) += iTime;
  // Damit keine Fehler kommen...
  return(-1);
}  

protected func FxActivePotionStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  // Ein Grund, wesshalb beim Ablegen von Objekten der Effekt einfach aus dem Indianer kam.
  if(fTemp) return();

  // Physicals wiederherstellen
  EffectCall(pTarget, iEffectNumber, "RestorePhysical");

  if(EffectVar(27,pTarget,iEffectNumber))
  {
    Exit(pTarget, GetX(EffectVar(27,pTarget,iEffectNumber)),
                  GetY(EffectVar(27,pTarget,iEffectNumber))+GetDefCoreVal("Height","DefCore",GetID(EffectVar(27,pTarget,iEffectNumber)))*GetCon(EffectVar(27,pTarget,iEffectNumber))/200);
    SetXDir(GetXDir(EffectVar(27,pTarget,iEffectNumber)), pTarget);
    SetYDir(GetYDir(EffectVar(27,pTarget,iEffectNumber)), pTarget);
    SetDir(GetDir(EffectVar(27,pTarget,iEffectNumber)), pTarget);
    SetComDir(GetComDir(EffectVar(27,pTarget,iEffectNumber)), pTarget);
    RemoveObject(EffectVar(27,pTarget,iEffectNumber),1);
  }
  if(EffectVar(28,pTarget,iEffectNumber) == IBON) Extinguish(pTarget);

  var i, iNum;
  for(i = 0; iNum = GetEffect("Obj3D",pTarget,i); ++i)
    if(EffectVar(0,pTarget,iNum) == CLD2)
      { RemoveEffect(0,pTarget,iNum); break; }
}

// Effekt für nach dem Trinken - vereinfachte (spezialisierte) Version von jsbs 3D-Engine

func FxObj3DStart(object pTarget, int iNumber, int iTemp, id idCallback, iColor) {
  if(iTemp) return();

  EffectVar(0,pTarget,iNumber) = idCallback;

  EffectVar(1,pTarget,iNumber) = Random(360);
  EffectVar(2,pTarget,iNumber) = Random(360);
  EffectVar(3,pTarget,iNumber) = Random(360);

  var iR, iG, iB, iA; SplitRGBaValue(iColor, iR, iG, iB, iA);
  EffectVar(4,pTarget,iNumber) = RGBa(iR, iG, iB, 100);
}

public func Light(iTime) { return(RGBa(Random(3)*64,64,255,150)); }
public func LightSpeed() { return(14); }
public func LightSize()  { return(60); }

func FxObj3DTimer(object pTarget, int iNumber, int iTime) {

  var idCallback = EffectVar(0,pTarget,iNumber);

  var pitch = EffectVar(1,pTarget,iNumber);
  var yaw = EffectVar(2,pTarget,iNumber);
  var roll = EffectVar(3,pTarget,iNumber);
  
  pitch += idCallback->LightSpeed();
  yaw += idCallback->LightSpeed()/2;
  roll += idCallback->LightSpeed()/6;


  // Punkte definieren
  var zoom = 14;

  var point1x = -zoom;
  var point1y = 0;
  var point1z = 0;

  var point2x = +zoom;
  var point2y = 0;
  var point2z = 0;

  var point3x = 0;
  var point3y = -zoom;
  var point3z = 0;

  var point4x = 0;
  var point4y = +zoom;
  var point4z = 0;

  var point5x = 0;
  var point5y = 0;
  var point5z = +zoom;

  var point6x = 0;
  var point6y = 0;
  var point6z = -zoom;

  // Drehung
  for(var i = 1; i <= 6; i++)
  {
    var tempx = VarN(Format("point%dx", i));
    var tempy = VarN(Format("point%dy", i));
    VarN(Format("point%dx", i)) = Cos(roll,tempx) - Sin(roll,tempy);
    VarN(Format("point%dy", i)) = Cos(roll,tempy) + Sin(roll,tempx);
  }

  for(var i = 1; i <= 6; i++)
  {
    var tempy = VarN(Format("point%dy", i));
    var tempz = VarN(Format("point%dz", i));
    VarN(Format("point%dy", i)) = Cos(pitch,tempy) - Sin(pitch,tempz);
    VarN(Format("point%dz", i)) = Cos(pitch,tempz) + Sin(pitch,tempy);
  }

  for(var i = 1; i <= 6; i++)
  {
    var tempx = VarN(Format("point%dx", i));
    var tempz = VarN(Format("point%dz", i));
    VarN(Format("point%dx", i)) = Cos(yaw,tempx) - Sin(yaw,tempz);
    VarN(Format("point%dz", i)) = Cos(yaw,tempz) + Sin(yaw,tempx);
  }

  var zbuffer = 5*zoom;
  point1z += zbuffer;
  point2z += zbuffer;
  point3z += zbuffer;
  point4z += zbuffer;
  point5z += zbuffer;
  point6z += zbuffer;

  var size = idCallback->LightSize();
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point1x/point1z, GetY(pTarget) + zbuffer*point1y/point1z,0,0,size,EffectVar(4,pTarget,iNumber)     ,pTarget,point1z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point2x/point2z, GetY(pTarget) + zbuffer*point2y/point2z,0,0,size,EffectVar(4,pTarget,iNumber)  ,pTarget,point2z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point3x/point3z, GetY(pTarget) + zbuffer*point3y/point3z,0,0,size,EffectVar(4,pTarget,iNumber)  ,pTarget,point3z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point4x/point4z, GetY(pTarget) + zbuffer*point4y/point4z,0,0,size,EffectVar(4,pTarget,iNumber)  ,pTarget,point4z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point5x/point5z, GetY(pTarget) + zbuffer*point5y/point5z,0,0,size,EffectVar(4,pTarget,iNumber) ,pTarget,point5z>zbuffer);
  CreateParticle("PSpark",GetX(pTarget) + zbuffer*point6x/point6z, GetY(pTarget) + zbuffer*point6y/point6z,0,0,size,EffectVar(4,pTarget,iNumber) ,pTarget,point6z>zbuffer);

  EffectVar(1,pTarget,iNumber) = pitch;
  EffectVar(2,pTarget,iNumber) = yaw;
  EffectVar(3,pTarget,iNumber) = roll;

}

// Dieses Script zeigt dem Clonk genau, wieviel Atem das Tier noch besitzt.
private func FxPossessionSpellTimer(object pTarget, int iEffectNumber, int iTime)
{
  if(GetBreath(pTarget) != GetPhysical("Breath", 0, pTarget)/1000) {
    Sound("Breath",0,Contents(0,pTarget),0,0,-1);
    if(GetBreath(pTarget) != 0)
      DoBreath(GetBreath(pTarget) * GetPhysical("Breath",0,Contents(0,pTarget)) / GetPhysical("Breath",0,pTarget) - GetBreath(Contents(0,pTarget)), Contents(0,pTarget));
//    PlayerMessage(GetOwner(Contents(0,pTarget)), "<c 0000ff>%d%", pTarget, GetBreath(pTarget)*100000/GetPhysical("Breath",0,pTarget));
  }
  return();
}

private func FxPossessionSpellDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {
  // Schaden an den Clonk weiterleiten
  DoEnergy(iDmgEngy, Contents(0,pTarget), 1);
  if(!GetAlive(Contents(0,pTarget))) Kill(pTarget);

  // Das Tier selbst bekommt keinen Schaden
  return(0);
}

private func FxPossessionSpellStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
  if(fTemp) return();
  if(iReason == 3) return();
  return(-1);
}
