#strict 
local target;

public func Activate(caster1, caster2) {
  // Tatsächliches Ziel ermitteln
  target = caster1;

  // caster2 auf den Container vom Zauberer setzen falls nicht übergeben
  if (!caster2) caster2 = Contained(caster1);

  // Wirklichen Caster überprüfen
  if (caster2) {
    // Container lebt?
    if (GetOCF(caster2) & OCF_Alive()) {
      // Zielobjekt = Container
      target = caster2;
    }
    // Sonst Fehlermeldung
    else 
      return(CastError("$NotInFree$", target));
  }
  
  // Nur auf festem Grund
  if (!GBackSolid(0, AbsY(GetDefBottom(target))))
    return(CastError("$NotWalking$", target, 1));
  
  // Einen Frame verzögert springen
  SetAction("Jump");
  
  // Fertig
  return(1);
}

protected func DoJump() {
  // Sound
  Sound("ToadJump");
  SetPosition(GetX(target), GetY(target) - 1, target);

  // Aktivität setzen
  ObjectSetAction(target, "Jump");
  
  // Richtung rauskriegen
  var dir = -1;
  if (GetDir(target) == 1) dir = +1;

  // X/Y-Dir setzen
  SetSpeed(35 * dir, -70, target); // Einheitlicher Sprung

  // Zerstören
  RemoveObject();
}

protected func CastError(msg, caster, silent) 
{
  // Meldung ausgeben
  if (!silent) Message(msg, caster, GetName(caster));
  // Sound
  if (!silent) Sound("Error");
  // Action zurücksetzen
  if (GetAction(caster) eq "Magic") ObjectSetAction(caster, "Walk");
  // Selbstzerstörung
  RemoveObject();
  // Fehlschlag
  return(0);
}

public func GetSpellClass() { return(AIR1); }
public func GetSpellCombo() { return("555"); }
