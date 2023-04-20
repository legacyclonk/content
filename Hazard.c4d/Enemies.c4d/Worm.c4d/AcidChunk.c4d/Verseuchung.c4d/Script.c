/*-- Verseuchung --*/

#strict

local fade;

func Initialize()
{
  SetAction("Be");
  SetPhase(Random(6));
}

func Check()
{
	for(var enemy in FindTargets(this(),40)) {
		if(enemy->GetAlive()) {
			var abzug = fade/50;
			DoDmg(Max(0,4-abzug),DMG_Bio,enemy);
		}
	}
	
	if(GetActTime() > 200)
		DoFade(5);
}

func OnHit(int dmg, int type, object from)
{
	if(type == DMG_Fire)
		DoFade(5);
	if(type == DMG_Explosion)
		DoFade(25);
}

func DoFade(int change)
{
	fade += change;
	
	if(fade > 250)
		RemoveObject();
	
	SetClrModulation(RGBa(255,255,255,fade));
}

public func IsBulletTarget(id ID)
{
	if(ID == FFLM)
		return(true);
	return(false);
}
