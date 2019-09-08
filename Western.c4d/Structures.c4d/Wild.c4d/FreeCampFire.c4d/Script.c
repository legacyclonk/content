/*-- Lagerfeuer --*/

#strict

local meat;
local pMeatOut;
local iCook;

protected func Initialize()
{
  Sound("Inflame");
  SetAction("Burn");
}

private func Smoking() {
    Smoke(Random(3),Random(3),Random(8)+5);
}

protected func CheckContents()
{
  var pMeat;
  if(!ActIdle()) {
    if(pMeat = FindObject2(Sort_Random(), Find_Container(this()), Find_Func("CanBeCooked"))) {
      iCook++;
      if(iCook >= 25) {
        pMeat->~Cook();
        AddEffect("CampFire", pMeat, 101, 1, 0, GetID(), this());
        Sound("Fuse",0,this());
        iCook = 0;
      }
    }
    else iCook = 0;
    if(GetActTime() > 6000)
      SetAction("Idle");
  }
  if(ActIdle()) {
    if(ContentsCount(WOOD))
      return(BurnWood());
    if(iCook > 0) iCook--;
  }
}

private func BurnWood() {
  var pWood;
  if(pWood=FindContents(WOOD)) RemoveObject(pWood);
  Sound("Inflame");
  SetAction("Burn");
}

protected func ControlLeft(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk, "Get", this(), 0, 0, 0, 1);
}

protected func ControlRight(object pClonk)
{
  [$DescTake$]
  // Menü öffnen
  SetCommand(pClonk, "Get", this(), 0, 0, 0, 1);
}


// außer Holz nichts aufnehmen
protected func RejectCollect(idType, pObj)
{
  // Holz
  if(idType==WOOD) return(0);
  // Fleisch
  if(!pObj->~CanBeCooked()) return(Message("$MsgCantBeGrilled$", this(), GetName(pObj)));
  if(pObj->~CanBeCooked() && (ContentsCount()-ContentsCount(WOOD))<5 )
    return(0);
  else
    return(Message("$MsgFull$", this()));
  // Sonst nix aufnehmen
  return(1);
}

/* Fleischzubereitung */

protected func MeatPrepared(pMeat)
{
  // Kein Fleisch? Huh?
  if(!pMeat) return();
  // Fleisch raushängen
  Exit(pMeat, 0, -5);
  pMeatOut = pMeat;
}

public func FxCampFireStart(object pTarget, int iNumber, bool fTmp, object pFire)
{
  if(fTmp) return();
  EffectVar(0, pTarget, iNumber) = pFire;
}

public func FxCampFireTimer(object pTarget, int iNumber)
{
  // Feuer ist weg?
  if(!EffectVar(0, pTarget, iNumber)) return(-1);
  // Ist der Platz frei?
  if(!LocalN("pMeatOut", EffectVar(0, pTarget, iNumber)))
    EffectVar(0, pTarget, iNumber)->MeatPrepared(pTarget);
  if(LocalN("pMeatOut", EffectVar(0, pTarget, iNumber)) != pTarget) return();   
  // Fleisch ist weg?
  if(Contained(pTarget)) return(-1);
  // Position setzen
  var x,y;
  x = GetX(EffectVar(0, pTarget, iNumber));
  y = GetY(EffectVar(0, pTarget, iNumber));
  SetPosition(x, y-5, pTarget);
  SetXDir(0, pTarget);
  SetYDir(0, pTarget);
}

public func FxCampFireStop(pTarget, iNumber, reason, temp)
{
  if(temp) return();
  if(!EffectVar(0,pTarget,iNumber)) return();
  LocalN("pMeatOut", EffectVar(0, pTarget, iNumber)) = 0;
}

/* Besonderheit: Feuer kaputtmachen, um brennendes Holz zu erhalten */

public func ControlDig(pClonk)
{
  [$Destroy$|Image=WOOD]
  // Nicht leer?
  if(Contents()) return(Message("$MsgNotEmpty$", this()));
  // Holz erzeugen
  var pWood = CreateObject(WOOD, 10,0, GetOwner(pClonk));
  Collect(pWood, pClonk);
  // Holz brennt
  Incinerate(pWood);
  // Und weg
  return(RemoveObject());
}

/* Kann vom Indianer hergestellt werden */
public func IsIndianHandcraft() { return(1); }

/* Kann vom Trapper hergestellt werden */
public func IsTrapperHandcraft() { return(1); }
