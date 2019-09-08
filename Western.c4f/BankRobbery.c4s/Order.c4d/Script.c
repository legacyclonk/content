/*-- Häuptling mit Bestellung --*/

#strict
#appendto MTNT

static iWaitOrder;

protected func ContainedUp(object pClonk)
{
  [$MsgOrder$]
  if(CheckTeam(GetOwner(pClonk))!=1) return();	  
  CreateMenu(GetID(), pClonk, this(), C4MN_Extra_Value, "");
  AddMenuItem("$MsgOrderWhisky$", Format("Order(1,%d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 6, GetOrderPrice(1));
  AddMenuItem("$MsgOrderAmmo$",    Format("Order(2, %d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 1, GetOrderPrice(2));
  AddMenuItem("$MsgOrderDynamit$", Format("Order(3, %d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 2, GetOrderPrice(3));
  AddMenuItem("$MsgOrderCrew$",    Format("Order(4, %d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 3, GetOrderPrice(4));
  AddMenuItem("$MsgOrderHorse$",  Format("Order(5, %d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 4, GetOrderPrice(5));
  AddMenuItem("$MsgOrderWeapons$", Format("Order(6,%d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 5, GetOrderPrice(6));
  AddMenuItem("$MsgOrderMaterial$", Format("Order(7,%d)", ObjectNumber(pClonk)), ORDR, pClonk, 0, pClonk, "", 128+2, 7, GetOrderPrice(7));
}

private func GetOrderPrice(iOrder)
{
  if(iOrder==1) return(20); // Whisky   
  if(iOrder==2) return(30); // Munition
  if(iOrder==3) return(40); // Dynamit
  if(iOrder==4) return(50); // Crew
  if(iOrder==5) return(15); // Pferd
  if(iOrder==6) return(25); // Waffen
  if(iOrder==7) return(25); // Baumaterial
  // iOdrer 8 Relaunch
}

protected func Order(iOrder, iClonk)
{
  var pClonk = Object(iClonk);  
  // Hat der Spieler genug Geld?
  if(GetWealth(GetOwner(pClonk)) < GetOrderPrice(iOrder))
    {
    PlayerMessage(GetOwner(pClonk), "$MsgNoMoney$", pClonk);
    Sound("Error", 1, 0,0, GetOwner(pClonk));
    return();
    }
  // Geld abziehen, bestellen und Geräusch
  DoWealth(GetOwner(pClonk), -GetOrderPrice(iOrder));

  //HomebaseProduce(GetOwner(pClonk));
  AddEffect("WaitSupply", 0, 1, 1, 0, GetID(), iOrder, GetOwner(pClonk));
  //if(iOrder<=4) AddEffect("SupplyArrive", 0, 1, 1, 0, GetID(), iOrder, GetOwner(pClonk));
  //if(iOrder==5) AddEffect("HorseArrive", 0, 1, 1, 0, GetID(), iOrder, GetOwner(pClonk));
  Sound("Uncash");
  Message("$TxtOrdered$", this());
  return(1);
}

func FxWaitSupplyStart(pTarget, iNumber, fTmp, iOrder, iPlr)
{
  if(fTmp) return(0);   
  EffectVar(0, pTarget, iNumber) = iOrder;
  EffectVar(1, pTarget, iNumber) = iPlr;
  EffectVar(2, pTarget, iNumber) = iWaitOrder++;
}

func FxWaitSupplyTimer(pTarget, iNumber)
{
  if(!GetEffect("SupplyArrive", pTarget) && !GetEffect("HorseArrive", pTarget))
    if(!EffectVar(2, pTarget, iNumber)--) 
  {
    if(EffectVar(0, pTarget, iNumber)==5) 
      AddEffect("HorseArrive", 0, 1, 1, 0, GetID(), EffectVar(0, pTarget, iNumber), EffectVar(1, pTarget, iNumber));
    else
      AddEffect("SupplyArrive", 0, 1, 1, 0, GetID(), EffectVar(0, pTarget, iNumber), EffectVar(1, pTarget, iNumber));
    iWaitOrder--;
    return(-1);   
  }
}

func FxSupplyArriveStart(pTarget, iNumber, fTmp, iOrder, iPlr)
{
  if(fTmp) return(0);   
  EffectVar(6, pTarget, iNumber) = iOrder;
  EffectVar(7, pTarget, iNumber) = iPlr;
  EffectVar(1, pTarget, iNumber) = 0;
  EffectVar(2, pTarget, iNumber) = CreateObject(COAC, 37, 419,-1); // Planwagen
  EffectVar(3, pTarget, iNumber) = CreateObject(HORS, 27, 419,-1); // gesatteltes Pferd
  EffectVar(4, pTarget, iNumber) = CreateObject(BNDT, 76, 392,-1); // Reiter
  EffectVar(5, pTarget, iNumber) = 100*1000;
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(2, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(3, pTarget, iNumber));
  SetObjectLayer(EffectVar(2, pTarget, iNumber), EffectVar(4, pTarget, iNumber));

SetDir(1, EffectVar(2, pTarget, iNumber));
SetDir(1, EffectVar(3, pTarget, iNumber));
SetDir(1, EffectVar(4, pTarget, iNumber));

SetGraphics("1", EffectVar(3, pTarget, iNumber), HORS);
  
ObjectSetAction(EffectVar(4, pTarget, iNumber), "Ride", EffectVar(2, pTarget, iNumber));
//EffectVar(3, pTarget, iNumber)->~ControlDigDouble();
EffectVar(3, pTarget, iNumber)->~ConnectWagon(EffectVar(2, pTarget, iNumber));

 EffectVar(2, pTarget, iNumber)->SetObjDrawTransform(1000, 0, -100*1000, 0, 1000); 
 EffectVar(3, pTarget, iNumber)->SetObjDrawTransform(1000, 0, -50*1000, 0, 1000); 
 EffectVar(4, pTarget, iNumber)->SetObjDrawTransform(1000, 0, -100*1000, 0, 1000);  
 // Bei Relaunch, alle Banditen in die Kutsche stecken
 if(iOrder==8)
 {
   var pRelaunch = FindObjectOwner(_PLO, iPlr);	 
   while(Contents(0,pRelaunch)) Enter(EffectVar(2, pTarget, iNumber), Contents(0,pRelaunch));
   RemoveObject(pRelaunch);
 }
 // Wegfreimachen
 FreeRect(0, 374, 275,416-374);
}

func FxSupplyArriveTimer(pTarget, iNumber, iTime)
{
  // Variablen hohlen   
  var pCoach = EffectVar(2, pTarget, iNumber), 
      pHorse = EffectVar(3, pTarget, iNumber), 
      pRider = EffectVar(4, pTarget, iNumber),
      iTime = EffectVar(0, pTarget, iNumber),
      iState = EffectVar(1, pTarget, iNumber),
      iShift = EffectVar(5, pTarget, iNumber),
      iOrder = EffectVar(6, pTarget, iNumber),
      iPlr = EffectVar(7, pTarget, iNumber);
  if(!Mod(iTime, 40))  FreeRect(0, 374, 275,416-374);
  if(!pHorse) return(-1); 
  if(iState==0)
  {
    if(EffectVar(5, pTarget, iNumber)<=50*1000) 
    {
      iTime = EffectVar(0, pTarget, iNumber)++;
      EffectVar(1, pTarget, iNumber)++;
      SetPosition(37, 399, pCoach);
      SetPosition(27, 399, pHorse);
      ObjectSetAction(pCoach, "Drive2");
      ObjectSetAction(pHorse, "Pull");
      SetPhase(Mod(iTime, 20), pHorse);
      SetPhase(Mod(iTime, 4), pCoach);
      EffectVar(5, pTarget, iNumber) -= 1000;
      pCoach->SetObjDrawTransform(1000, 0, -iShift, 0, 1000); 
      pRider->SetObjDrawTransform(1000, 0, -iShift,0, 0, 1000); 
      SetCommand(pHorse, "MoveTo", 0, 236, 402);
      return();
    }
    iTime = EffectVar(0, pTarget, iNumber)++;
    SetPosition(37, 399, pCoach);
    SetPosition(27, 399, pHorse);
    SetPosition(37+24, 399+1, pRider);
    SetDir(1, pRider);
    ObjectSetAction(pCoach, "Drive2");
    ObjectSetAction(pHorse, "Pull");
    SetPhase(Mod(iTime, 20), pHorse);
    SetPhase(Mod(iTime, 4), pCoach);
    EffectVar(5, pTarget, iNumber) -= 1500;
    pCoach->SetObjDrawTransform(1000, 0, -iShift, 0, 1000); 
    pHorse->SetObjDrawTransform(1000, 0, Min(-iShift+50*1000,0), 0, 1000); 
    pRider->SetObjDrawTransform(1000, 0, -iShift, 0, 1000);   
    return();
  }
  else if(iState==1)
  {
    if(Distance(GetX(pHorse), GetY(pHorse), 236, 402)<10)   
    {       
      SetCommand(pHorse, "None");
      pHorse->~SetGait();
      SetDir(1, EffectVar(2, pTarget, iNumber));
      ObjectSetAction(pRider, "Jump");
      SetCommand(pRider, "Enter", pCoach);
      if(iOrder==8)
      {
        for(var i=0;Contents(i, pCoach);i++) 
		SetCommand(Contents(i, pCoach), "MoveTo", 0, GetX(FindObject(_SCS))+RandomX(-20,20), GetY(FindObject(_SCS)));
        AddCommand(pRider, "Wait", 0, 0, 0, 0, 0, 50);
      }
      else if(iOrder!=4) AddCommand(pRider, "Grab", FindObject(_SCS));
      else
      {
        EffectVar(8, pTarget, iNumber) = CreateObject(BNDT, 0, 0, iPlr);
        Enter(pCoach, EffectVar(8, pTarget, iNumber));
        SetCommand(EffectVar(8, pTarget, iNumber), "MoveTo", 0, GetX(FindObject(_SCS))+RandomX(-20,20), GetY(FindObject(_SCS)));
        EffectVar(9, pTarget, iNumber) = CreateObject(BNDT, 0, 0, iPlr);
        Enter(pCoach, EffectVar(9, pTarget, iNumber));
        SetCommand(EffectVar(9, pTarget, iNumber), "MoveTo", 0, GetX(FindObject(_SCS))+RandomX(-20,20), GetY(FindObject(_SCS)));
        AddCommand(pRider, "Wait", 0, 0, 0, 0, 0, 50);
      }
      EffectVar(1, pTarget, iNumber)++;
      return();
    }
    if(!GetCommand(pHorse)) SetCommand(pHorse, "MoveTo", 0, 236, 402);
    if(EffectVar(5, pTarget, iNumber))
    {
    iTime = EffectVar(0, pTarget, iNumber)++;
    SetPosition(37, 399, pCoach);
    SetPosition(37+24, 399+1, pRider);
    SetDir(1, pRider);
    ObjectSetAction(pCoach, "Drive2");
    SetPhase(Mod(iTime, 4), pCoach);
    EffectVar(5, pTarget, iNumber) -= 1500;
    pCoach->SetObjDrawTransform(1000, 0, Min(-iShift,0), 0, 1000);   
    pRider->SetObjDrawTransform(1000, 0, Min(-iShift,0), 0, 1000); 
    }
    return();
  }
  else if(iState==2)
  {
    if(GetAction(pRider)S="Push" && iOrder)
    {
      var pChest = GetActionTarget(0, pRider);      
      if(iOrder==1)
      {
        for(var i=0;i<5;i++)
          CreateContents(WHKY, pChest);
      }
      if(iOrder==2)
      {
        for(var i=0;i<8;i++)
          CreateContents(AMBO, pChest);
      }
      if(iOrder==3)
      {
        for(var i=0;i<3;i++)
          CreateContents(DYNB, pChest);
        CreateContents(IGNB, pChest);
      }
      if(iOrder==6)
      {
        for(var i=0;i<3;i++)          
          CreateContents(REVR, pChest);
        CreateContents(WINC, pChest);
      }
      if(iOrder==7)
      {
      for(var i=0;i<2;i++)
          CreateContents(TENP, pChest);
      for(var i=0;i<3;i++)
          CreateContents(METL, pChest);
      for(var i=0;i<3;i++)
          CreateContents(WOOD, pChest);
      for(var i=0;i<3;i++)
          CreateContents(LOAM, pChest);
      }
      EffectVar(6, pTarget, iNumber)=0;
    }
    if(Contained(pRider))
    {
      if(EffectVar(8, pTarget, iNumber)) MakeCrewMember(EffectVar(8, pTarget, iNumber), iPlr);
      if(EffectVar(9, pTarget, iNumber)) MakeCrewMember(EffectVar(9, pTarget, iNumber), iPlr);
      ObjectSetAction(pRider, "Ride", pCoach);      
      EffectVar(1, pTarget, iNumber)++;
      SetCommand(this(), "MoveTo", 0, 27, 402);
      pCoach->~TurnRight();
    }
  }
  else if(iState==3)
  {
    if(GetX(pHorse)<30)
    {
      EffectVar(0, pTarget, iNumber) = GetPhase(pHorse);
      SetPhysical("Walk", 0, 2, pHorse);
      RemoveObject(FindObject(CHBM, 0, 0, 0, 0, 0, "Connect", pCoach));
      EffectVar(1, pTarget, iNumber)++;
      return();
    }
    if(!GetCommand(pHorse)) SetCommand(pHorse, "MoveTo", 0, 27, 402);
  }
  else
  {
    if(EffectVar(5, pTarget, iNumber)>=100*1000) 
    {
      RemoveObject(pCoach);
      RemoveObject(pHorse);
      RemoveObject(pRider);
      return(-1);
    }
    iTime = EffectVar(0, pTarget, iNumber)++;
    ObjectSetAction(pCoach, "Drive2");
    SetPhase(Mod(iTime, 4), pCoach);
    SetDir(0, pCoach);
    SetPosition(GetX(pCoach)-24, GetY(pCoach)+1, pRider);
    SetDir(0, pRider);
    ObjectSetAction(pHorse, "Pull");
    SetPhase(Mod(iTime, 20), pHorse);
    EffectVar(5, pTarget, iNumber) += 1500;
    pCoach->SetObjDrawTransform(1000, 0, -iShift, 0, 1000); 
    pHorse->SetObjDrawTransform(1000, 0, -iShift, 0, 1000); 
    pRider->SetObjDrawTransform(1000, 0, -iShift, 0, 1000);   
  }
}

func FxHorseArriveStart(pTarget, iNumber, fTmp, iOrder, iPlr)
{
  if(fTmp) return(0);   
  EffectVar(6, pTarget, iNumber) = iOrder;
  EffectVar(3, pTarget, iNumber) = CreateObject(HRSS, 27, 419,-1); // gesatteltes Pferd
  EffectVar(4, pTarget, iNumber) = CreateObject(BNDT, 27, 395,-1); // Reiter
  EffectVar(5, pTarget, iNumber) = 50*1000;
  SetObjectLayer(EffectVar(3, pTarget, iNumber), EffectVar(3, pTarget, iNumber));
  SetObjectLayer(EffectVar(4, pTarget, iNumber), EffectVar(4, pTarget, iNumber));

  SetDir(1, EffectVar(3, pTarget, iNumber));
  SetDir(1, EffectVar(4, pTarget, iNumber));

  SetGraphics("1", EffectVar(3, pTarget, iNumber), HRSS);
  LocalN("iGraphics", EffectVar(3, pTarget, iNumber)) = 1;
  
  ObjectSetAction(EffectVar(4, pTarget, iNumber), "Ride", EffectVar(3, pTarget, iNumber));
  EffectVar(3, pTarget, iNumber)->SetObjDrawTransform(1000, 0, -50*1000, 0, 1000); 
  EffectVar(4, pTarget, iNumber)->SetObjDrawTransform(1000, 0, -50*1000, 0, 1000); 

  // Weg frei!
  FreeRect(0, 374, 275,416-374);
}

func FxHorseArriveTimer(pTarget, iNumber, iTime)
{
  // Variablen hohlen   
  var pHorse = EffectVar(3, pTarget, iNumber), 
      pRider = EffectVar(4, pTarget, iNumber),
      iTime = EffectVar(0, pTarget, iNumber),
      iState = EffectVar(1, pTarget, iNumber),
      iShift = EffectVar(5, pTarget, iNumber),
      iOrder = EffectVar(6, pTarget, iNumber),
      iPlr = EffectVar(7, pTarget, iNumber);
  if(!Mod(iTime, 40))  FreeRect(0, 374, 275,416-374);
  if(!pRider) return(-1);
  if(!pHorse) return(-1);
  if(iState==0)
  {
    if(EffectVar(5, pTarget, iNumber)<0) 
    {
      EffectVar(1, pTarget, iNumber)++;
      SetPosition(27, 399, pHorse);
      ObjectSetAction(pHorse, "Walk");
      SetPhase(Mod(iTime, 20), pHorse);
      SetCommand(pHorse, "MoveTo", 0, 236, 402);
      return();
    }
    iTime = EffectVar(0, pTarget, iNumber)++;
    SetPosition(27, 399, pHorse);
    pHorse->~SetGait(1);
    ObjectSetAction(pHorse, "Walk");
    SetPhase(Mod(iTime, 20), pHorse);
    EffectVar(5, pTarget, iNumber) -= 1500;
    pHorse->SetObjDrawTransform(1000, 0, Min(-iShift,0), 0, 1000); 
    pRider->SetObjDrawTransform(1000, 0, Min(-iShift,0), 0, 1000);   
    return();
  }
  else if(iState==1)
  {
    if(Distance(GetX(pHorse), GetY(pHorse), 236, 402)<20)   
    {       
      SetCommand(pHorse, "None");
      pHorse->~SetGait();
      SetObjectLayer(0, pHorse);
      ObjectSetAction(pRider, "Jump");
      SetCommand(pRider, "MoveTo", 0, 0, 402);
      EffectVar(1, pTarget, iNumber)++;
      return();
    }
    if(!GetCommand(pHorse)) SetCommand(pHorse, "MoveTo", 0, 236, 402);
    return();
  }
  else if(iState==2)
  {
    if(GetX(pRider)<10)
    {
      RemoveObject(pRider);
      return(-1);
    }
    if(!GetCommand(pRider)) SetCommand(pRider, "MoveTo", 0, 0, 402);
  }
}
