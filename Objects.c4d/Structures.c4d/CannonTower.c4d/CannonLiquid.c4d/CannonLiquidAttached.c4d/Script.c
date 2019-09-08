/*--- Flüssigkeitsgeschütz (angeschraubt) ---*/

#strict

#include CTW2

protected func RotationSpeed() { return(2); }
protected func CannonMobileID() { return(LICN); }

public func ComFire(object clonk)
{
  Sound("Click");

  inherited(clonk, ...);

  return(1);
}

protected func DoMenu(object clonk, int iSelection)
{
  var tower = GetActionTarget();
  CreateMenu(tower->GetID(), clonk, this(), 0, "$TxtNoammo$",0,0,0,CannonShootMenuID());
  
  // Fässer suchen
  var i, obj, last_id, barrel_cnt, type_cnt;
  while (obj = tower->Contents(i++)) {
    var obj_id = obj->GetID();
    if (IsFilledBarrel(obj)) {
      barrel_cnt++;
      // Erstes Objekt des Typs?
      if (obj_id != last_id) {
        type_cnt++;
        last_id = obj_id;
        AddMenuItem("$TxtShoots$", "Shoot", obj_id, clonk, tower->ContentsCount(obj_id), 0, 0, 4, obj);
      }
    }
  }

  if (type_cnt > 1)
    AddMenuItem("$TxtShootsAll$", "Shoot", BARL, clonk, barrel_cnt);

  SelectMenuItem(iSelection, clonk);
  return(1);
}

private func IsFilledBarrel(object obj)
{
  return(obj->~GetAmount() && obj->~BarrelMaterialName());
}

private func Shoot(id barrel_type)
{
  var tower = GetActionTarget();
  var r = GetR();

  if (barrel_type == BARL)
    barrel_type = 0;

  var i, obj, n;
  for(var obj in FindObjects(Find_Container(GetActionTarget()))) {
    if(!IsFilledBarrel(obj)) continue;
    if (barrel_type && barrel_type != obj->GetID()) continue;

    var mat = obj->BarrelMaterial();
    var amt = obj->GetAmount();
    obj->BarrelDoFill(-amt);
    obj->ChangeDef(obj->EmptyBarrelID());

    for (var j = 0; j < amt; j++) {
      n++;
      var f = n / 50;
      var dist = 20 + f;
      var angle = r + RandomX(f / -3, f / 3);
      InsertMaterial(mat, Sin(angle, dist), -Cos(angle, dist), Sin(r, 200), -Cos(r, 200));
    }
  }

  Sound("Splash*");
}