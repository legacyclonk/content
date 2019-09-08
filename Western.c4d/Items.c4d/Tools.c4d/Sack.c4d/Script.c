/*-- Sack --*/

#strict 2

protected func Initialize() { if(GetID() != SAC1) SetAction("Be"); }

protected func Hit() { Sound("RockHit*"); }

/* Wird eingesammelt */

protected func Entrance()
{
  // Im einem Behälter ist der Sack nehmbar
  if(GetID() == SACK) ChangeDef(SAC1);
  SetAction("Idle");
  return(1);
}
  
/* Ins Freie */

protected func Departure() {

  // Im Freien ist der Sack nicht nehmbar
  if (GetID()==SAC1) ChangeDef(SACK);

  SetAction("Be");
  //Sound("Winning");

  return(1);
}

/* Nicht mehr aufnehmen als er kann */

protected func RejectCollect(id idObj, object pObj)
{
  if(idObj == SACK || idObj == SAC1) return(1);
  if(ContentsCount() < 10) return(0);
  return(1);
}

protected func RejectEntrance(object pIntoObj)
{
  if(GetOCF(pIntoObj) & OCF_Alive)
    if(FindContents(SAC1,pIntoObj)) return(1);
  return(0);
}

protected func ControlLeft(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk,"Get",this(),0,0,0,1);
}

protected func ControlRight(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk,"Get",this(),0,0,0,1);
}

public func ControlDigDouble(object pClonk)
{
  [$TxtCollect$|Image=CXIV]
  if(FindContents(SAC1,pClonk)) return(0);
  if(!pClonk->~RejectCollect(GetID(),this())) Enter(pClonk);
  return(1);
}

public func ControlThrow(pClonk)
{
  if(Contents(0,pClonk) == this()) return(_inherited(pClonk));
  if(Contents(0,pClonk))
    if(RejectCollect(GetID(Contents(0,pClonk))))
      return(SetCommand(pClonk,"Get",this(),0,0,0,1));
}

public func Check()
{
  if(!Contained()) return(Departure());	
}

/* Kleine Anzeige für die Objektanzahl */

public func Collection2(object pObj) { return(UpdatePicture()); }
public func Ejection(object pObj) { return(UpdatePicture()); }

public func UpdatePicture()
{
  var iCount = ContentsCount();
  if(iCount) {
    if(iCount > 9) {
      SetGraphics(0,0,GetNumberID(iCount/10),1,GFXOV_MODE_Picture);    
      SetObjDrawTransform(400,0,-12000,0,400,+10000,this(),1);
    }
    else SetGraphics(0,0,0,1);
    SetGraphics(0,0,GetNumberID(iCount%10),2,GFXOV_MODE_Picture);   
    SetObjDrawTransform(400,0,-5000,0,400,+10000,this(),2);
  }
  else SetGraphics(0,0,0,2);
}

private func GetNumberID(int iNumber) { return(C4Id(Format("SNB%d",iNumber))); }

/* Kann vom Trapper in Handarbeit hergestellt werden */
public func IsTrapperHandcraft() { return(1); }

/* Kann von Indianern erforscht werden */
public func IsIndianResearch() { return(1); }

/* Kann vom Indianer in Handarbeit hergestellt werden */
public func IsIndianHandcraft() { return(1); }
