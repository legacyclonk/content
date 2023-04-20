/* Ammosystem */

#strict

global func DoAmmo(id ammoid, int change, object target)
{ 
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(0);

  // Kann 0 sein bei Objektlokalen Aufrufen.
  if(!target) target=this();
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  // no ammo rule
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return(0);
  var ammo=FindContents(ammoid, obj);
  // Gibts schon ein Objekt der Munitionsart? OMGOMG, nein!
  if(!ammo)
    // Schnell eines erstellen
    ammo=CreateContents(ammoid, obj);
  //if(!ammo) Log("Panik - Munitionsobjekt wird nicht erstellt");
  // Alten Munitionsstand speichern
  var oldammoamount = obj->GetAmmo(ammoid);
  var truechange;
  var maxamount = ammo->~MaxAmount();
  // Der neue Wert wird ausgerechnet, darf aber nicht größer als eventuelles MaxAmount() und nicht kleiner als 0 sein
  if(maxamount == 0)
    truechange= Max(oldammoamount+change, 0);
  else
    truechange= Max(Min(maxamount, oldammoamount+change), 0);
    
  // Neuer Wert dem Objekt geben, in Lokale ammoamount
  ammo->Local(0)=truechange;
  // Wenn Muni alle, Objekt entfernen
  if(truechange==0) RemoveObject(ammo);
	
  // Callback an das Objekt, dass sich sein Ammo geändert hat
  target->~ OnAmmoChange(ammoid, truechange);
	
  // Differenz zurückgeben: Tatsächliche Änderung.
  return(truechange-oldammoamount);
}

global func GetAmmo(id ammoid, object target)
{
  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(0);

  // Kann 0 sein bei Objektlokalen Aufrufen.
  if(!target) target=this();
  // Entsprechendes Munitionslagerobjekt suchen
  var obj = target ->~ AmmoStoring();
  if(!obj) obj = target;
  // no ammo rule
  if(ObjectCount(NOAM))
    if(obj ->~ IsAmmoStorage())
      return(0);
  var ammo;
  if(ammo=FindContents(ammoid, obj))
    return(ammo->Local(0));
  else
    return(0);
}

global func CheckAmmo(id ammoid, int count) {

  if(!ammoid) return(false);

  // gar keine Munition
  if(!(ammoid->~IsAmmo())) return(false);

  if(!Par(2)) Par(2) = this();
  var iSum = 0;

  // search
  for(var i=2; i<10 && Par(i); ++i) {
    // no ammo rule
    var obj = Par(i) ->~ AmmoStoring();
    if(!obj) obj = Par(i);
    if(ObjectCount(NOAM))
      if(obj ->~ IsAmmoStorage())
        return(true);
    iSum += GetAmmo(ammoid,obj);
  }


  if(iSum >= count)
    return(true);
  return(false);
}
