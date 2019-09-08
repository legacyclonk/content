/*-- Vogel --*/

#strict

#appendto BIRD

// Von nun an bleiben sie oben auf der Ebene

protected func Activity()
{
  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();

  // Rechts bleiben!
  if(GetX()<4580 && !GetDir()) return(TurnRight());

  // Oben bleiben!
  if(GetY()>150) return(COMD_Up());	
  
  // Geräusche machen
  if (!Random(25)) Sound("Raven*");

  // Nichts machen
  if (Random(2) || GetAction() ne "Fly") return(1);

  // Nach oben oder unten fliegen
  SetComDir(COMD_Up());
  if (Random(2) && GetY()>50) SetComDir(COMD_Down());

  // Nichts machen
  if (!Random(4)) return(1);

  // Umdrehen
  if (Random(2) || GetX()<4600) return(TurnRight());
  return(TurnLeft());
}
