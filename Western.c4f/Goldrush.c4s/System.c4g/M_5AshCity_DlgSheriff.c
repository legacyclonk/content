/* Dialog: Sheriff */

#strict
#appendto _TLK

static g_fSheriffSpoken;

public func DlgSheriffStart()
  {
  if (g_fSheriffSpoken==1) return(DlgSheriff8(), iTalkIndex=8);
  if (g_fSheriffSpoken==2) return(DlgSheriff16(), iTalkIndex=16);
  if (g_fSheriffSpoken==3) return(DlgSheriff18(), iTalkIndex=18);
  if (g_fSheriffSpoken==4) return(DlgSheriff19(), iTalkIndex=19);
  if (g_fSheriffSpoken==5) return(DlgSheriff31(), iTalkIndex=31);
  if (g_fSheriffSpoken==6) return(DlgSheriff34(), iTalkIndex=34);
  if (g_fSheriffSpoken==7) return(DlgSheriff37(), iTalkIndex=37);
  MessageEx("$Dlg_Sheriff0$", pPlayer);
  return(1);
  }

private func DlgSheriff1()
  {
  MessageEx("$Dlg_Sheriff1$", pNPC);
  }

private func DlgSheriff2()
  {
  MessageEx("$Dlg_Sheriff2$", pNPC);
  }

private func DlgSheriff3()
  {
  MessageEx("$Dlg_Sheriff3$", pNPC);
  }

private func DlgSheriff4()
  {
  MessageEx("$Dlg_Sheriff4$", pNPC);
  }

private func DlgSheriff5()
  {
  MessageEx("$Dlg_Sheriff5$", pPlayer);
  }

private func DlgSheriff6()
  {
  MessageEx("$Dlg_Sheriff6$", pNPC);
  }

private func DlgSheriff7()
  {
  MessageEx("$Dlg_Sheriff7$", pPlayer);
  g_fSheriffSpoken=1;
  StopDialog();
  }

private func DlgSheriff8()
  {
  MessageEx("$Dlg_Sheriff8$", pNPC);
  }

private func DlgSheriff9()
  {
  var pBandit;
  pBandit = FindObject(BNDT, 0, 0, 0, 0, OCF_Alive, 0, 0, NoContainer());
  if(pBandit)
  {
    MessageEx("$Dlg_Sheriff9b$", pPlayer);
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Sheriff9$", pPlayer);
  }

private func DlgSheriff10()
  {
  MessageEx("$Dlg_Sheriff10$", pNPC);
  g_fSheriffSpoken=1;
  }

private func DlgSheriff11()
  {
  MessageEx("$Dlg_Sheriff11$", pNPC);
  }

private func DlgSheriff12()
  {
  MessageEx("$Dlg_Sheriff12$", pPlayer);
  }

private func DlgSheriff13()
  {
  MessageEx("$Dlg_Sheriff13$", pNPC);
  }

private func DlgSheriff14()
  {
  MessageEx("$Dlg_Sheriff14$", pNPC);
  }

private func DlgSheriff15()
  {
  MessageEx("$Dlg_Sheriff15$", pNPC);
  // Alle Gebäude für den Wiederaufbau vorbereiten
  AddEffect("Rebuild", FindObject(SWWB, 1203, 287, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(SHOB, 1377, 295, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(PMWB, 1459,  80, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(DRSB, 1613, 243, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(WRWB, 1588, 410, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(BNKB, 1788, 236, -1, -1), 1, 1);
  AddEffect("Rebuild", FindObject(TGS2, 2264, 292, -1, -1), 1, 1);

  var pClonk, iEffect;
  pClonk = pNPC;
  SetCommand(pClonk,"MoveTo",0,1351,307);
  AddCommand(pClonk,"Wait",0,0,0,0,0,28);
  AddCommand(pClonk,"MoveTo",0,1300,495);
  AddCommand(pClonk,"Wait",0,0,0,0,0,105);
  iEffect = GetEffect("StayThere",pClonk);
  EffectVar(0,pClonk,iEffect) = 1351;
  EffectVar(1,pClonk,iEffect) =  307;

  pClonk = FindCrewByName(COWB, "Smith");
  SetCommand(pClonk,"MoveTo",0,1637,428);
  AddCommand(pClonk,"MoveTo",0,1351,307);
  AddCommand(pClonk,"Wait",0,0,0,0,0,10);
  AddCommand(pClonk,"MoveTo",0,1300,495);
  AddCommand(pClonk,"Wait",0,0,0,0,0,105);
  iEffect = GetEffect("StayThere",pClonk);
  EffectVar(0,pClonk,iEffect) = 1637;
  EffectVar(1,pClonk,iEffect) =  426;

  pClonk = FindCrewByName(STLR, "Henry");
  SetCommand(pClonk,"MoveTo",0,1185,304);
  AddCommand(pClonk,"Wait",0,0,0,0,0,125);
  AddCommand(pClonk,"MoveTo",0,1300,495);
  AddCommand(pClonk,"Wait",0,0,0,0,0,105);
  iEffect = GetEffect("StayThere",pClonk);
  EffectVar(0,pClonk,iEffect) = 1185;
  EffectVar(1,pClonk,iEffect) =  304;

  pClonk = FindCrewByName(STLR, "Gerry");
  SetCommand(pClonk,"MoveTo",0,1759,255);
  AddCommand(pClonk,"Jump");
  AddCommand(pClonk,"MoveTo",0,1681,259);
  AddCommand(pClonk,"MoveTo",0,1351,307);
  AddCommand(pClonk,"Wait",0,0,0,0,0,81);
  AddCommand(pClonk,"MoveTo",0,1300,495);
  AddCommand(pClonk,"Wait",0,0,0,0,0,105);
  iEffect = GetEffect("StayThere",pClonk);
  EffectVar(0,pClonk,iEffect) = 1759;
  EffectVar(1,pClonk,iEffect) =  255;

  pClonk = FindCrewByName(STLR, "Dorkan");
  SetCommand(pClonk,"MoveTo",0,1587,255);
  AddCommand(pClonk,"Wait",0,0,0,0,0,64);
  AddCommand(pClonk,"MoveTo",0,1300,495);
  AddCommand(pClonk,"Wait",0,0,0,0,0,105);
  iEffect = GetEffect("StayThere",pClonk);
  EffectVar(0,pClonk,iEffect) = 1587;
  EffectVar(1,pClonk,iEffect) =  255;

  g_fSheriffSpoken=2;
  StopDialog();
  }

private func DlgSheriff16()
{
  MessageEx("$Dlg_Sheriff16$", pNPC);
}

private func DlgSheriff17()
  {
  if(FindObject(SHOB, 1377-GetX(), 295-GetY()))
  {
    MessageEx("$Dlg_Sheriff17b$", pPlayer);
    return(StopDialog());
  }
  MessageEx("$Dlg_Sheriff17$", pPlayer);
  // Banditen mit Boss erzeugen und aus dem Saloon gehen lassen
  var pSaloon = FindObject(SALO);
  var pBandit = CreateObject(BNDT, 2094, 80, -1);
  pBandit->SetAI("BanditSaloon", 3);
  pBandit->SetColorDw(RGB(150));
  pBandit->Enter(pSaloon);
  pBandit = CreateObject(BNDT, 2094, 80, -1);
  pBandit->SetAI("BanditBossSaloon", 3);
  pBandit->SetColorDw(RGB(150));
  pBandit->MakeBoss();
  pBandit->Enter(pSaloon);
  }

static g_fSherifIsCrewMember;

private func DlgSheriff18()
{
  if(GetEffect("IntPause",pNPC)) {
    var pObj;
    if(pObj = FindObject2(Find_Container(pPlayer),Find_Func("GetFeedValue"),Find_Not(Find_Func("CanBeCooked")))) {
      RemoveObject(pObj);
      Sound("ClonkMunch",0,this());
      RemoveEffect("IntPause",pNPC);
    }
    else {
      MessageEx("$Dlg_SheriffPause2$",pNPC);
      StopDialog();
    }
  }
  MessageEx("$Dlg_Sheriff18$", pNPC);
  SetOwner(GetOwner(pPlayer), pNPC);
  SetColorDw(RGB(255,255), pNPC);
  ChangeDef(_SRF,pNPC);
  pNPC->SetAction("Walk");
  MakeCrewMember(pNPC,GetOwner(pPlayer));
  RemoveEffect("StayThere",pNPC);
  SetGraphics(0,pNPC,SHRF);
  SetObjectLayer(0,pNPC);
  if(!g_fSherifIsCrewMember) {
    CreateContents(HDCF,pNPC);
    CreateContents(AMBO,pNPC,2);
    CreateContents(WINC,pNPC);
  }
  AddEffect("WatchEnergy",pNPC,1,0,0,GetID());
  SetObjectLayer(this(),FindObject(_TLK,0,0,0,0,0,"Attach",this()));
  g_fSherifIsCrewMember=1;
  g_fSheriffSpoken=3;
  StopDialog();
}

func FxWatchEnergyStop(pTarget, iNumber, iReason, fTmp)
{
  if(fTmp) return();
  ChangeDef(SHRF,pTarget);
  SelectCrew(GetOwner(pTarget),pTarget,0);
  SetCrewStatus(GetOwner(pTarget),false,pTarget);
  DoEnergy(100, pTarget); SetAlive(1, pTarget); ObjectSetAction(pTarget, "Walk", 0, 0, 1); Message("", pTarget);
  // Spruch loslassen
  if(EffectVar(0, pTarget, iNumber)) {
    Message("$Dlg_SheriffPause$", pTarget);
    AddEffect("IntPause",pTarget,1);
    ObjectSetAction(pTarget,"Drink"); Sound("Drink",0,pTarget);
  }
  SetObjectLayer(0, FindObject(_TLK, 0, 0, 0, 0, 0, "Attach", pTarget));
//  g_fSherifIsCrewMember=0;
  Enter(FindObject(SHOF), pTarget);
  SetCommand(pTarget, "Exit");
  SetComDir(COMD_Stop, pTarget);
  SetPlrView(GetOwner(pTarget), pTarget);
  SetOwner(-1, pTarget);
  AddEffect("StayThere", pTarget, 1, 35, pTarget);
}

func FxWatchEnergyDamage(pTarget, iEffectNumber, iDmgEngy, iCause)
{
  if(iDmgEngy>0) return(iDmgEngy);
  // Energie geht nicht unter 1
  if(GetEnergy(pTarget)>-iDmgEngy/1000) return(iDmgEngy);
  EffectVar(0, pTarget, iEffectNumber) = 1;
  RemoveEffect(0,pTarget,iEffectNumber);
  return(0);
}

private func DlgSheriff19()
  {
  MessageEx("$Dlg_Sheriff19$", pNPC);
  }

private func DlgSheriff20()
  {
  MessageEx("$Dlg_Sheriff20$", pNPC);
  }

private func DlgSheriff21()
  {
  MessageEx("$Dlg_Sheriff21$", pPlayer);
  }
private func DlgSheriff22()
  {
  MessageEx("$Dlg_Sheriff22$", pNPC);
  }
private func DlgSheriff23()
  {
  MessageEx("$Dlg_Sheriff23$", pNPC);
  }
private func DlgSheriff24()
  {
  MessageEx("$Dlg_Sheriff24$", pNPC);
  }
private func DlgSheriff25()
  {
  MessageEx("$Dlg_Sheriff25$", pNPC);
  }
private func DlgSheriff26()
  {
  MessageEx("$Dlg_Sheriff26$", pNPC);
  }
private func DlgSheriff27()
  {
  MessageEx("$Dlg_Sheriff27$", pPlayer);
  }
private func DlgSheriff28()
  {
  MessageEx("$Dlg_Sheriff28$", pNPC);
  }
private func DlgSheriff29()
  {
  MessageEx("$Dlg_Sheriff29$", pNPC);
  }
private func DlgSheriff30()
  {
  MessageEx("$Dlg_Sheriff30$", pNPC);
  SetPlrKnowledge(GetOwner(pPlayer), WIRR);
  SetPlrKnowledge(GetOwner(pPlayer), TEL2);
  }

private func DlgSheriff31()
  {
  MessageEx("$Dlg_Sheriff31$", pNPC);
  g_fSheriffSpoken=5;
  }

private func DlgSheriff32()
  {
  if(GetCon(FindObject(TGST))<100)
  {
    MessageEx("$Dlg_Sheriff32b$", pPlayer);
    return(StopDialog());
  }
  if(!FindObject(TGST)->IsConnectedTo(FindObject(TEL1, 2994, 111, -1, -1)))
  {
    MessageEx("$Dlg_Sheriff32b$", pPlayer);
    return(StopDialog());
  }
  MessageEx("$Dlg_Sheriff32$", pPlayer);
  }
private func DlgSheriff33()
  {
  MessageEx("$Dlg_Sheriff33$", pNPC);
  g_fSheriffSpoken=6;
  StopDialog();
  }
private func DlgSheriff34()
  {
  MessageEx("$Dlg_Sheriff34$", pPlayer);
  }
private func DlgSheriff35()
  {
  // Erst müssen alle Gebäude wieder aufgebaut sein	  
  if(FindObject(SWWB,AbsX(1203),AbsY(287)) 
  || FindObject(SHOB,AbsX(1377),AbsY(295)) 
  || FindObject(PMWB,AbsX(1459),AbsY( 80)) 
  || FindObject(DRSB,AbsX(1613),AbsY(243)) 
  || FindObject(WRWB,AbsX(1588),AbsY(410)) 
  || FindObject(BNKB,AbsX(1788),AbsY(236)) 
  || FindObject(TGS2,AbsX(2264),AbsY(292)) 
  || FindObject(ELVB,AbsX(1520),AbsY( 71)) 
  || FindObject(ELVB,AbsX(1710),AbsY(241)) 
  || FindObject(ELVB,AbsX(2360),AbsY(291)))
  {
    MessageEx("$Dlg_Sheriff35b$", pNPC);
    StopDialog();
    return();
  }
  MessageEx("$Dlg_Sheriff35$", pNPC);
  var pKey = CreateObject(_KEY);
  LocalN("iRefID", pKey) = 1;
  SetClrModulation(RGB(255,255), pKey);
  Enter(pPlayer, pKey);
  g_fSheriffSpoken=7;
  }
private func DlgSheriff36()
  {
  MessageEx("$Dlg_Sheriff36$", pPlayer);
  StopDialog();
  }
private func DlgSheriff37()
  {
  MessageEx("$Dlg_Sheriff37$", pNPC);
  StopDialog();
  }
