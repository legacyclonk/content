/* Vögel in den Hintergrund versetzen wenn sie sich in dem Tunnelhinderniss da befinden */
#strict
#appendto BIRD

func Activity()
{
  if(Inside(GetX(),336*GetScenMapZoom(),LandscapeWidth()) && Inside(GetY(),40*GetScenMapZoom(),98*GetScenMapZoom())) {
        SetCategory(69640|1048576);
        Local(0)=1;
    } else 
        if(Local(0)) {
            SetCategory(69640);
          Local(0)=0;
      }
  return(_inherited());
}
