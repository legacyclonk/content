/*-- Magus --*/

#strict

#include SCLK

static const MAGE_MaxHoverDuration = 100; // Maximale Zeit, die der Zauberer zur Zauber- und Zielwahl schweben kann

static const MagicStaff     = 1,
             MagicAmulet    = 2,
             MagicRing      = 3,
             MagicClothing  = 4;

/* Initialisierung */

/* Status */
  
public func IsCasting() 
{ 
  return(GetAction() eq "Cast" || GetAction() eq "HoverCast" || GetAction() eq "RideCast"); 
}

private func SetCastAction()
{
  return(); // Magus auch erstmal ohne Cast-Aktivität...
  // Gehen
  if (GetAction() S= "Walk") 
    SetAction("Cast"); 
  // Reiten
  else if (IsRiding())
    SetAction("RideCast");
  // Schweben
  else 
    SetAction("HoverCast");  
  return(true);
}


// Leuchten während des Zauberns in der Luft
protected func HoverGlow()
  {
  // Nach zu langer Zeit wieder runterfallen (kein Schwebecheat)
  if (GetActTime() >= MAGE_MaxHoverDuration) return(EndMagicAction());
  // Effekt
  var i=2, dwClr;
  /*if (idSelectedElement) dwClr = idSelectedElement->GetElementColor(); else*/ dwClr = RGB(20,50,255);
  while (i--) CreateParticle("PxSpark", Random(17)-8,11, Random(10)-5,-Random(10), Random(50)+20,dwClr);
  // Fertig
  return(1);
  }

// Leuchten während des Zauberns
protected func Glow()
  {
  // Effekt
  var dwClr;
  /*if (idSelectedElement) dwClr = idSelectedElement->GetElementColor(); else*/ dwClr = RGB(20,50,255);
  CreateParticle("PxSpark", Sin(GetActTime()*10, 20), Cos(GetActTime()*10, 20), Random(10)-5,-Random(10), Random(50)+20,dwClr);
  // Fertig
  return(1);
  }


// Ausrüstung ablegen
protected func ContextMagicEquipment(object pObject)
{
  [$CtrlTakeoffDesc$|Image=WEAR|Condition=WearsSomething]

  // Clonk soll anhalten
  SetComDir(COMD_Stop());

  // Kontextmenü erzeugen
  CreateMenu(WEAR,0,0,0,"$CtrlTakeoffDesc$",0,1,0);

  // alle attachten Objekte
  var pObj;
  while(pObj = FindObject(0,0,0,0,0,0,0,this(),0,pObj))
    if(pObj->~IsMagicEquipment())
      AddMenuItem(GetName(pObj),"MnuTakeOff",GetID(pObj),0,0,pObj);

  return(1);
}

protected func MnuTakeOff(id idItem, object pObj) { pObj->~TakeOff(); }

protected func WearsSomething()
{
  var pObj;
  while(pObj = FindObject(0,0,0,0,0,0,0,this(),0,pObj))
    if(pObj->~IsMagicEquipment())
      return(1);
  return(0);
}

public func CanWear(int type) { return(true); }

