/* K¸nstliche Intelligenzen */

#strict

#appendto CLNK

local iOwner;

public func RejectCollect()
{
  if(GetEffect("AIBandit*",this())) return(1);
  return(_inherited(...));
}

public func SetAI(szName, iInterval)
{
  SetDifficultyPhysicals(iDifficulty);
  RemoveEffect("Life", this());
  return(AddEffect(Format("AI%s", szName), this(), 1, iInterval, this()));
}

func SetDifficultyPhysicals(iDifficulty)
{
  if(iDifficulty==0) SetPhysical("Energy", 25000+25000*this()->~IsCaptain(), 2);
  if(iDifficulty==1) SetPhysical("Energy", 75000+45000*this()->~IsCaptain(), 2);
  if(iDifficulty==2) SetPhysical("Energy",100000+50000*this()->~IsCaptain(), 2);
  if(iDifficulty==3) SetPhysical("Energy",150000+50000*this()->~IsCaptain(), 2);
  DoEnergy(100);
}

func FxAIBanditStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  // Nur schieﬂen und stehen	
  this()->~ContextDefend();
//  SetPhysical("Walk", 0, 2);
  CreateContents(AMBO);
  CreateContents(AMBO);
  CreateContents(WINC);
  iOwner=-2;
}

func FxAIBanditTimer()
{
  // Immer genug Munition	
  if(!FindContents(AMBO)) CreateContents(AMBO);	
  // Gegner in Reichweite f¸r Nahkampf?
  var pTarget;
  while(pTarget = FindObject(0, -15,-15,30,30, OCF_Alive, 0, 0, pTarget)) if(Hostile(GetOwner(pTarget), GetOwner())) break;
  if (pTarget) if (!Random(3))
    if (Contained() == Contained(pTarget))
      return(FightWith(pTarget));
}

func FxAIBanditStop()
{
  // Nichts fallen lassen	
  if(LocalN("idWeapon",this())) LocalN("idWeapon",this()) = 0;
  while(Contents()) RemoveObject(Contents());
  // nur eine Muni schachtel
  CreateContents(AMBO);
  if(this()->~IsCaptain()) CreateContents(WINC);
}

func FxAIBanditRevolverStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  // Nur schieﬂen und stehen	
  this()->~ContextDefend();
  CreateContents(AMBO);
  CreateContents(AMBO);
  CreateContents(REVR);
  iOwner=-2;
}

func FxAIBanditRevolverTimer()
{
  // Immer genug Munition	
  if(!FindContents(AMBO)) CreateContents(AMBO);	
  // Gegner in Reichweite f¸r Nahkampf?
  var pTarget;
  while(pTarget = FindObject(0, -15,-15,30,30, OCF_Alive, 0, 0, pTarget)) if(Hostile(GetOwner(pTarget), GetOwner())) break;
  if (pTarget) if (!Random(3))
    if (Contained() == Contained(pTarget))
      return(FightWith(pTarget));
}

func FxAIBanditRevolverStop()
{
  // Nichts fallen lassen
  if(LocalN("idWeapon",this())) LocalN("idWeapon",this()) = 0;
  while(Contents()) RemoveObject(Contents());
  // nur eine Muni schachtel
  CreateContents(AMBO);
}

func FxAIBanditNoMoveStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  // Nur schieﬂen und stehen	
  this()->~ContextDefend();
  SetPhysical("Walk", 0, 2);
  CreateContents(AMBO);
  CreateContents(AMBO);
  CreateContents(WINC);
  iOwner=-2;
}

func FxAIBanditNoMoveTimer()
{
  if(GetCommand()) SetCommand(this(), "None");	
  // Immer genug Munition	
  if(!FindContents(AMBO)) CreateContents(AMBO);	
  // Gegner in Reichweite f¸r Nahkampf?
  var pTarget;
  while(pTarget = FindObject(0, -15,-15,30,30, OCF_Alive, 0, 0, pTarget)) if(Hostile(GetOwner(pTarget), GetOwner())) break;
  if (pTarget) if (!Random(3))
    if (Contained() == Contained(pTarget))
      return(FightWith(pTarget));
}

func FxAIBanditNoMoveStop()
{
  // Nichts fallen lassen
  while(Contents()) RemoveObject(Contents());
  if(LocalN("idWeapon",this())) LocalN("idWeapon",this()) = 0;
  // nur eine Muni schachtel
  CreateContents(AMBO);
}

func FxAIBanditBoss(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  SetCommand(this(), "Enter", FindObject(MTNT, 0, 0, -1, -1));
//  iOwner=-2;
}

func FxAIBanditBossTimer()
{
  if(!Local(4))
  {
    var pBandit;
    // Nach den Teamkamaraden suchen
    while(pBandit = FindObject(BNDT, 0, 0, 0, 0, OCF_Alive, 0, 0, pBandit)) if(pBandit != this()) break;
    if(pBandit) return();
    // Nur wenn alle schon tot sind, dann selber rauskommen
    SetCommand(this(), "UnGrab");
//    AddCommand(this(), "Call", this(), 0, 0, 0, 0,"ContextDefend");
    AddCommand(this(), "Call", FindObject(_SWT, 0, 0, -1, -1), 0, 0, 0, 0, "ControlLeft");
    AddCommand(this(), "Grab", FindObject(_SWT, 0, 0, -1, -1));
//    Enter(this(), FindObject(CALU));
    Local(4)=1;
    return();	  
  }
  if(!GetCommand())
  {
    SetAI("BanditNoMove", 3);	  
    return(-1);
  }
}

func FxAIBanditSaloonStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
}

func FxAIBanditSaloonTimer()
{
  if(!Local(4))
  {
    var pSheriff;
    // Nach Sheriff suchen
    while(pSheriff = FindObject(_SRF, 0, 0, -1, -1, OCF_Alive, 0, 0))
    if(!pSheriff || ObjectDistance(pSheriff)>100) return();
    // Nur wenn Sheriff in der N‰he ist
    SetCommand(this(), "MoveTo", 0, 1985, 99);
    Local(4)=1;
    return();	  
  }
  if(!GetCommand())
  {
    SetAI("BanditNoMove", 3);	  
    return(-1);
  }
}

func FxAIBanditBossSaloonStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  AddEffect("NoDying", this(), 1, 0, this());
}

func FxAIBanditBossSaloonTimer()
{
  if(!Local(4))
  {
    var pSheriff;
    // Nach Sheriff suchen
    while(pSheriff = FindObject(_SRF, 0, 0, -1, -1, OCF_Alive, 0, 0))
    if(!pSheriff) return();
    if(ObjectDistance(pSheriff)>100) return();
    // Nur wenn Sheriff in der N‰he ist
    SetCommand(this(), "MoveTo", 0, 2168, 87);
    Local(4)=1;
    return();	  
  }
  if(!GetCommand())
  {
    SetAI("BanditNoMove", 3);
    return(-1);
  }
}

func FxNoDyingDamage(pTarget, iEffectNumber, iDmgEngy, iCause)
{
  // Energie geht nicht unter 1	
  if(iDmgEngy>0) return(iDmgEngy);	
  if(GetEnergy(pTarget)>-iDmgEngy/1000) return(iDmgEngy);
  return(-GetEnergy(pTarget)*1000+1000);
}

local iX, iY, pHelpClonk;

func FxAIMagicStart(pThis, iNumber, fTmp)
{
  if(fTmp) return();
  iX = GetX();
  iY = GetY();
}

func FxAIMagicTimer()
{
  return(-1); // Wird nicht mehr verwendet	
  if(GetAction()S="Magic") return();
  if(pHelpClonk) return();
  // Alle Clonks in der Liste durchgehen, ob man ihnen helfen kann
  var i, pClonk;
  while(pClonk = Local(i++))
  {
  if(OnFire(pClonk)) MagicHelp(pClonk, EXTG);//{ SetAction("Magic"); CreateObject(EXTG,0,0,GetOwner())->~Activate(this(), pClonk); }
  if(GetEnergy(pClonk)<40) MagicHelp(pClonk, MGHL);//{ SetAction("Magic"); CreateObject(MGHL,0,0,GetOwner())->~Activate(this(), pClonk); }
  }
//  if(!Random(10)) if(GetAction()S="Walk") SetAction("Magic");	
}

public func MagicHelp(pClonk, idMagic)
{
//  var iX = GetX(), iY = GetY();	
  if(pClonk == this()) return(DoHelpMagic(this(), idMagic, this()));
  var pCave;
  var iDir = 1;
  if(GetX()<GetX(pClonk)) iDir = -1;
  if(!GetPathLength(GetX(), GetY(), GetX(pClonk), GetY(pClonk)))
  {
    pCave = FindObject(CAVE,0,0,-1,-1);	  
    iDir = 1;
    if(GetX(LocalN("pLink",pCave))<GetX(pClonk)) iDir = -1;
  }
  pHelpClonk = pClonk;
  SetCommand(this(), "MoveTo", 0, iX, iY);
  if(pCave)
  { 
	  AddCommand(this(), "Wait", 0, 0, 0, 0, 0, 32);
	  AddCommand(this(), "Enter",LocalN("pLink",pCave));
  }
  AddCommand(this(), "Call", this(), idMagic, pClonk, 0, 0, "DoHelpMagic");
  AddCommand(this(), "MoveTo", 0, GetX(pClonk)+50*iDir, GetY(pClonk));
  if(pCave) 
  { 
	  AddCommand(this(), "Wait", 0, 0, 0, 0, 0, 32);
	  AddCommand(this(), "Enter",pCave);
  }
}

public func DoHelpMagic(pThis, idMagic, pClonk)
{
  SetAction("Magic"); 
  CreateObject(idMagic,0,0,GetOwner())->~Activate(this(), pClonk);
  pHelpClonk = 0;
}

func FxAIDrunkTimer()
{
  // Alkoholeinfluﬂ ist sichtbar
  if(!Contained()) CreateParticle("AlcoholBubble", 0, -9, RandomX(-3,3), -7, 30, RGBa(50,100,155,0));
}

global func GetOwner(pObj) 
{
  if(!pObj) pObj = this();
  if(LocalN("iOwner", pObj)) return(LocalN("iOwner", pObj));
  return(_inherited(pObj));
}

global func Hostile(iPlr1, iPlr2)
{
  if(iPlr1==-1 || iPlr2==-1) return(0);
  if(iPlr1==-2 || iPlr2==-2) return(iPlr1 != iPlr2);
  return(_inherited(iPlr1, iPlr2));
}
