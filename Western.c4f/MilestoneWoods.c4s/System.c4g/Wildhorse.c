/* Wildpferd */

#strict

#appendto WHRS

private func EnemyNearby() {
  var pEnemy=FindObject(0,0,0,-1,-1, OCF_CrewMember());
  var iDist=ObjectDistance(pEnemy);
  var pContainer=Contained(pEnemy);

  // Vor den lieben Banditen hier keine Angst haben
  if(GetID(pEnemy)==BNDT) return();
  
  if(pContainer) if(GetID(pContainer)==GetID()) return();
  if(pEnemy)
   if(GetAction(pEnemy) ne "Crawl")	  
    if(iDist<300)
      if(!Contained(pEnemy) || iDist<150)
        if(PathFree(GetX(),GetY(),GetX(pEnemy),GetY(pEnemy)) || iDist<100 )
          return(pEnemy);
}
