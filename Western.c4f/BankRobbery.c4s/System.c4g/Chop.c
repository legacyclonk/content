/*-- Bäume fällen --*/

#strict
#appendto BNDT

// Bäume zerlegen
public func ControlDigDouble()
{
  // nur im Stehen
  if(GetAction() ne "Walk" && GetAction() ne "SpearWalk") return(_inherited());
  // Nicht aus einem Gebäude heraus
  if(Contained()) return(_inherited());

  var pTree;
  // gefällte bäume zerlegen
  while( pTree = FindObject(0,0,1,0,0,OCF_Grab(),0,0,NoContainer(),pTree) )
    if(pTree->~IsTree())
      return(1,SetAction("Chop",pTree), AddEffect("Chop2", this(), 1, 1, this()));
  return(_inherited());
}

protected func CheckChop() {            // EndCall von Chop2 (Bäume zerlegen)
  Sound("Chop*");
  CastParticles("Dust",Random(3)+1,6,-8+16*GetDir(),1,10,12);
  var pA = GetActionTarget();
  // Aktionsziel auf mysteriöse Art und Weise verschwunden
  if(pA)
    if(ObjectDistance(pA) < Sqrt(GetDefWidth(GetID(pA))*GetDefHeight(GetID(pA)))/2 + 8)
    {
      DoDamage(+5,pA);
      // lange genug dran rumgeschnippelt
      if(GetDamage(pA) > ((pA->TreeStrength())*Max(GetCon(),30)/125)*2 )
      {
        // SetCon(BoundBy(GetCon(pA)-30,30,100),pA);
        Split2Components(GetActionTarget());
        // Wart-Kommandos hier abbrechen
        if(GetCommand() eq "Wait") FinishCommand(this(), 1);
      }
      else { return(); }
    }
  SetAction("Walk");
  RemoveEffect("Chop2", this());
}

protected func Chopping()
{	
  if(GetEffect("Chop2", this())) return();	
  if(!GetEffect("Chop", this())) AddEffect("Chop", this(), 1, 8*3, this());	
  return(_inherited());	
}

func FxChopTimer(pTarget, iNumber)
{
  if(!(GetAction(pTarget) S= "Chop"))
  {
    if( (GetOCF(GetActionTarget(0,pTarget)) & OCF_Grab) && GetAction(pTarget)S="Walk")
    {
      EffectVar(0, pTarget, iNumber) = 1;
      // Nur, wenn ein Baum gefällt wurde (nicht bei Telegrafenmasten)
      if(!(GetActionTarget(0,pTarget)->~IsTree())) return(-1);
      // Zerlegen
      pTarget->SetAction("Chop", GetActionTarget(0,pTarget));
      AddEffect("Chop2", this(), 1, 1, this());
    }
    return(-1);
  }
}

func FxChop2Timer(pTarget, iNumber)
{
  EffectVar(0, pTarget, iNumber)++;
  pTarget->~SetAction("Chop");
  SetPhase((EffectVar(0, pTarget, iNumber)/3)%8, pTarget);
  if(!Mod(EffectVar(0, pTarget, iNumber),8*3)) pTarget->~CheckChop();	
}

func GetProducerOf (def) {
  // Automatische Holzherstellung
  if (def == WOOD) return (this ());
  return (inherited (def));
}

func HowToProduce (clonk, def) {
  if (def == WOOD) {
    // Baum suchen
    var pTree;
    if (!(pTree = FindWorkTree(clonk)))
      // Kein geeigneter Baum (fertig)
      { AddMessage("$TxtNomoresuitabletrees$",clonk); FinishCommand (clonk, 0, 1); return(0); }
    // Baum zerlegen...
    AddCommand(clonk,"Wait",0,0,0,0,0,5*(pTree->TreeStrength())*Max(GetCon(),30)/100);
    AddCommand(clonk,"Call",clonk,0,0,0,0,"ControlDigDouble");
//    AddCommand(clonk,"Wait",0,0,0,0,0,5*(pTree->TreeStrength())*Max(GetCon(),30)/125);
//    AddCommand(clonk,"Grab",pTree,0,0,0, 500);
    // ...aber erst fällen
    if (!(GetOCF(pTree) & OCF_Grab()))
      AddCommand(clonk,"Chop",pTree);
  }
}

/* Importiert von Sägewerk */

private func FindWorkTree(object clonk)
{
  // Arbeiter fällt bereits einen Baum
  if (GetAction(clonk) eq "Chop") 
    return(GetActionTarget(0, clonk));
  // Arbeiter hat bereits einen Baum angefaßt
  if (GetAction(clonk) eq "Push") 
    if (GetActionTarget(0,clonk)->~IsTree())
      return(GetActionTarget(0,clonk));
  // Baum zum Anfassen suchen
  var obj;
  if (obj = FindTreeToGrab()) return(obj);
  // Baum zum Fällen suchen
  if (obj = FindTreeToChop()) return(obj);
  // Kein Baum gefunden
  return(0);
}

private func FindTreeToChop()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Chop(),0,0,0,obj))
    // Ausgewachsen
    if (GetOCF(obj) & OCF_Fullcon())
    // Im freien Bereich
    if (GetOCF(obj) & OCF_InFree())
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

private func FindTreeToGrab()
{
  var obj;
  while (obj = FindObject(0,0,0,-1,-1,OCF_Grab(),0,0,0,obj))
    // Ist ein Baum
    if (obj->~IsTree())
    // Nicht in einem Behälter
    if (!Contained(obj))
    // Nicht zu weit weg
    if (ObjectDistance(obj) < 500)
    // Brennt nicht
    if (!OnFire(obj))
    // Steckt nicht fest
    if (!Stuck(obj))
    // Baum wird nicht bereits von einem Clonk angefaßt
    if (!FindObject(0,0,0,0,0,OCF_CrewMember(),"Push",obj))
      // Passenden Baum gefunden
      return(obj);
  // Keinen passenden Baum gefunden
  return(0);
}
