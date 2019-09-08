/*-- Lorenwipf --*/

#strict

local count;

public func AttachTo(obj) {
  if (!count) {
    count = 1;
    UpdateGraphic();
  }
  SetAction(GetAction(), obj);
}

public func IncreaseCount() {
  count++;
  UpdateGraphic();
}

public func DecreaseCount() {
  count--;
  UpdateGraphic();
}

public func UpdateGraphic() {
  // Keine Wipfe mehr in der Lore -> Entfernen
  if (count <= 0) return(RemoveObject());
  // Grafik anpassen
  SetAction(Format("Wipf%d", BoundBy(count, 1, 3)));
}

protected func UpdateRotation() {
  var lorry = GetActionTarget();
  // Lore ist verschwunden -> entfernen
  if (!lorry) RemoveObject();
  // Sonst Rotation anpassen
  else SetR(GetR(lorry));
}