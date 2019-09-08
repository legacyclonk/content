/*-- Drachenei --*/

#strict

static const DEGG_hardness = 10000;

local lair;

/* Initialisierung */

protected func Initialize()
{
  DoCon(-Random(51) );
  AddEffect("IntFireCounter", this(), 1, 20, this() );
}

/* Drachenhorst */

public func SetLair(object new_lair)
{
  lair = new_lair;
}

/* Aufprall */

protected func Hit()
{
  Sound("RockHit*");
}

/* Aufbrechen */

public func Break()
{
  SetAction("Break");
  var drgn = CreateConstruction(DRGN, 0, GetObjHeight() / 2, GetOwner(), 20);
  drgn->Birth(lair);
  RemoveEffect("IntFireCounter", this() );
}

/* Entfernen */

private func Remove()
{
  RemoveObject();
}

/* Effekt */

protected func FxIntFireCounterTimer(object target, int number, int time)
{
  var obj;
  while(obj = FindObject(0, -50, -50, 100, 100, OCF_OnFire(), 0,0,0, obj))
  {
    var distance = ObjectDistance(obj);
    if(distance < 50)
    {
      EffectVar(0, target, number) += Sqrt(Sqrt(obj->GetObjWidth() * obj->GetObjHeight() ) * (50 - distance) * 12);
    }
  }

  var material = GetMaterial();
  if(material != -1)
  {
    if(GetMaterialVal("Incindiary", "Material", material) )
      EffectVar(0, target, number) += 100;
    if(material == Material("Water") )
      EffectVar(0, target, number) = Max(EffectVar(0, target, number) - 50, 0);
  }

  if(EffectVar(0, target, number) >= DEGG_hardness)
    Break();
}

