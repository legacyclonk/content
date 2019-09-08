/*-- Sägewerk --*/

#strict

#include SAWM
#include BAS9


// Überladungen

private func Sawing()
{
  // Noch nicht fertig
  if (GetActTime() < 30) return(0);
  // Fertig
  SetAction("Idle");
  // Inhalt auswerfen
  Exit(ContentsNotCrewMember(),-35,20,0,-1);
  return(1);
}

private func SawingDust()
{
  if (!Random(5)) 
    CastParticles("Dust",4+Random(3),8,-26,12,10,12);
}

private func FindTreeToChop()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Chop(),0,0,0,obj))
    // Ausgewachsen
    if (GetOCF(obj) & OCF_Fullcon())
    // Im freien Bereich
    if (GetOCF(obj) & OCF_InFree())
    // Nicht markiert
    if (!GetEffectCount ("IntSawmillTreeMarker", obj))
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Wichtig: Baum ist ein Baum (und kein Telegrafenmast)
    if(obj->~IsTree())
    // Baum wird nicht bereits von einem Clonk gefällt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Chop",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}
