/*-- Keycard-Terminal --*/

#strict

local cardid, target;

/* Bedienung */

protected func ControlThrow(object pClonk)
{
  [$Control$]

  var kc = Contents(0,pClonk);
  // Sonst Nullpointer -_-
  if(!kc) {
    Sound("CommandFailure1");
    return(Message("$Keycard$",pClonk));
  }

  if(!(kc->~IsKeycard())) {
    Sound("CommandFailure1");
    Message("$Keycard$",pClonk);
  }
  else {
    // erstmal annehmen
    kc->Exit();
    kc->Enter(this());
    ScheduleCall(0,"CheckCard",40);
  }
  return(1);
}

public func CheckCard() {
  if(!Contents()) return();

  if(Contents()->GetKeyNumber() & cardid) {
    // es ist die richtige!
    if(target) target->~Access();

    // Effekt
    Sound("Beep");
    CreateParticle("NoGravSpark", 5, -7, 0, 0, 50, RGBa(0, 255, 0, 50));

    // der Einfachheit halber werden Karten nach Gebrauch zerstört
    // if(Contents()) RemoveObject(Contents());
    // doch nicht
  }
  else {
    // die falsche... :(
    Sound("Error");
    CreateParticle("NoGravSpark", 5, -7, 0, 0, 50, RGBa(255, 0, 0, 50));

    // ausspucken
    if(Contents()) Contents()->Exit();
  }
}

public func CreateKeyCard() {
  var kc = CreateObject(KCRD,0,0,GetOwner());
  kc -> SetKeyNumber(cardid);
  return(kc);
}

public func Set(object pTarget, int iNumber)
{
  cardid = iNumber;
  target = pTarget;
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[GetLength(extra)] = [Format("Set(Object(%%d),%i)", cardid), target];
}
