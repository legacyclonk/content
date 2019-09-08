/*-- Alchemiebeutel --*/

#strict

protected func Activate(pCaller)
{
  [$CtrlActivateBag$]
  var pBag = pCaller;
  // Clonk hat schon eine Tasche?
  if(AlchemBag(pBag)) 
  {
    // ist da was drin? Dann ausleeren (aber nicht unter Wasser)
    if(AlchemCount() && pBag->~Ready2Decompose()) {
      pBag->Transfer(this());
      return(1);
    }
    // ansonsten Meldung dass er schon einen hat
    PlayerMessage(GetOwner(pCaller), "$MsgAlreadyHasBag$", pCaller, pCaller->GetName());
    return(1);  
  }   
  // an den Clonk heften
  BelongTo(pCaller);
  Sound("Connect",0,0,0,GetOwner(pCaller)+1);
  PlayerMessage(GetOwner(pCaller),"$MsgPuton$",pCaller);
  return(1);
}

public func Exhaustion()
{
  // angeheftet muss der Beutel dableiben
  if(GetAction()eq"Belongs") return(true);
  // im Inventar, etc
  return(true);
}

public func BelongTo(object pObject) { SetAction("Belongs",pObject); }

// Menü für Alchemie, das im Clonk auftaucht, an den der Beutel gehaengt ist
protected func AttachContextAlchem(object pActionTarget, object pObj, int iSelection)
{
  [$CtxAlchemy$|Image=ALC_]

  // Clonk soll anhalten
  pActionTarget->SetComDir(COMD_Stop());

  var i, j, idAlchem;
  // Kontextmenü erzeugen
  CreateMenu(ALC_,pActionTarget,0,0,"$CtxAlchemy$",0,1,0);

  // zerlegen/entleeren (nur wenn nicht Contained())
  if(!Contained(pActionTarget) && Ready2Decompose()) {
    var pAlchemContainer, idAlchem, szName;
    for(;pAlchemContainer=AlchemContainer(i,pActionTarget);++i) {
      if(pAlchemContainer->~AlchemProcessTime() == 0) { // was zum rausnehmen
        if(AlchemCount(pAlchemContainer)) // Inhalt?
          AddMenuItem(Format("$MnuExhaust$",GetName(pAlchemContainer)),"Decompose",GetID(pAlchemContainer),pActionTarget,0,pAlchemContainer);
      } else {
        AddMenuItem(Format("$MnuDecompose$",GetName(pAlchemContainer)),"Decompose",GetID(pAlchemContainer),pActionTarget,0,pAlchemContainer);
      }
    }
  }

  AddMenuItem("$MnuIndex$","Index",ALC_,pActionTarget);
  AddMenuItem("$MnuDeposit$","Loose",BMSD,pActionTarget);
  AddMenuItem("$MnuHelp$","HelpTxt",CXIN,pActionTarget);

  return(1);
}



protected func SomethingAlchem()
  { return(AlchemCount() || AlchemContainerCount(GetActionTarget())); }

private func Decompose(id idType, object pObject) {
  // sofort zerlegen
  if(pObject->~AlchemProcessTime() == 0) Transfer(pObject);
  else AddEffect("IntDecompose",GetActionTarget(),1,1,0,GetID(),pObject,this());
}

public func Loose() {
  PlayerMessage(GetOwner(GetActionTarget()),"$MsgDeposit$",GetActionTarget());
  var pEnter = Contained(GetActionTarget());
  SetAction("Idle");
  SetActionTargets();
  if(pEnter) Enter(pEnter);
}

public func HelpTxt() {
  MessageWindow("$MsgHelpTxt$",GetOwner(GetActionTarget()),CXIN,"$MnuHelp$");
}

public func Index() {
  // keine Zutaten vorhanden
  if(!AlchemCount()) {
    MessageWindow("$MsgEmpty$",GetOwner(GetActionTarget()),ALC_,"$MnuIndex$");
  }
  // Zutaten vorhanden
  else {
    CreateMenu(ALC_,GetActionTarget(),0,0,"$MnuIndex$",0,1,1);
    var idAlchem;
    for(var i=0; idAlchem=GetAlchem(0,i); ++i)
      AddMenuItem(GetName(0,idAlchem),"",idAlchem,GetActionTarget(),GetAlchem(idAlchem));
  }
}

func FxIntDecomposeStart(object pClonk, int iNumber, int iTemp, object pAlchemContainer, object pAlchemManager)
{
  var szAct;
  EffectVar(0,pClonk,iNumber) = pAlchemContainer;
  EffectVar(1,pClonk,iNumber) = pAlchemManager;
  // Process vorhanden? toll. Nicht vorhanden? nich so toll.
  // ...aber möglich!! :)
  if(GetActMapVal("Name","Process",GetID(pClonk)))
    szAct = "Process";
  else
    szAct = "FlatUp";
  pClonk->SetAction(szAct);
  EffectVar(2,pClonk,iNumber) = szAct;
  // Objekt "virtuell" vor sich hinlegen
  var iOvID = EffectVar(3,pClonk,iNumber) = GetUnusedOverlayID(GFXOV_ProcessTarget, pClonk);
  SetGraphics(0, pClonk, 0, iOvID, GFXOV_MODE_Object, 0, GFX_BLIT_Parent, pAlchemContainer);
  SetObjDrawTransform(1000, 0, GetDir(pClonk)*20000-10000, 0, 1000, 12000 - 1000 * (GetDefBottom(pAlchemContainer) - GetY(pAlchemContainer)), pClonk, iOvID);
}

func FxIntDecomposeTimer(object pClonk, int iNumber, int iEffectTime) {
  var pAlchemContainer = EffectVar(0,pClonk,iNumber),
      pAlchemManager = EffectVar(1,pClonk,iNumber),
      szAction = EffectVar(2,pClonk,iNumber);

  // Objekt weg
  if( !pAlchemContainer || Contained(pAlchemContainer) != pClonk)
        return(-1,pClonk->SetAction("KneelUp"));

  // Beutel weg
  if(!pAlchemManager || !GetAlchemBag(pClonk)) return(-1,pClonk->SetAction("KneelUp"));

  // Zeit abgelaufen
  if(iEffectTime >= pAlchemContainer->~AlchemProcessTime())
    return(-1,pAlchemManager->Transfer(pAlchemContainer));

  // Aktion
  if(szAction eq "FlatUp")
    if(GetPhase(pClonk)!=4) SetPhase(4,pClonk);
  if(GetAction(pClonk) ne szAction) return(-1);
}

func FxIntDecomposeStop(object pClonk, int iNumber, int iReason, bool fTemp)
{
  if (fTemp) return();
  // Process fertig: Overlay immer entfernen
  // (bei Erfolg wird es ohnehin schon entfernt sein, da das Objekt gelöscht wurde)
  SetGraphics(0, pClonk, 0, EffectVar(3,pClonk,iNumber));
  return (FX_OK);
}

public func Transfer(object pObject) {

  // Clonk wieder hinstellen
  if(GetAction(GetActionTarget()) ne "Walk")
    GetActionTarget()->SetAction("KneelUp");

  // bei leerem Behälter bestimmte Nachricht ausgeben
  if(!GetAlchem(0,0,pObject))
    PlayerMessage(GetOwner(GetActionTarget()),"$MsgContainerEmpty$",this());
  else
    PlayerMessage(GetOwner(GetActionTarget()),Format("$MsgReceived$.",TransferAlcStr(pObject)),this());
  // und nun den Inhalt rüberschieben
  TransferAlchem(pObject,this());
 }

private func TransferAlcStr(object pObject, int iMul) {
  var idAlchem, iAlchem, szMessage;

  if(!iMul) iMul = 1;
  szMessage = "";

  // alle Zutaten
  for(var i=0; idAlchem=GetAlchem(0,i,pObject); ++i)
  {
    iAlchem = GetAlchem(idAlchem,0,pObject)*iMul;

    // tatsächlich gar keine Zutat sondern ein weiterer Behälter?
    if(idAlchem->~IsAlchemContainer()) {
      var pObj = CreateObject(idAlchem,0,0,-1);
      szMessage = Format("%s%s",szMessage,TransferAlcStr(pObj,iAlchem));
      if(pObj) RemoveObject(pObj);
    }

    else {
      szMessage = Format("%s|%dx{{%i}}%s",szMessage,iAlchem,idAlchem,GetName(0,idAlchem));
    }
  }
  return(szMessage);
}

protected func Ready2Decompose() { if(GetActionTarget()) if(GetAction(GetActionTarget()) eq "Walk") return(1);  }

protected func RejectEntrance() { if(GetAction()eq"Belongs") return(1); }

public func IsAlchemContainer() { return(true); }

protected func Hit() { Sound("WoodHit*"); }
