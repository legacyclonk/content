/*-- Tor (Zapper kann nicht coden) --*/

#strict
local iTeam; // Hier ist das Team drin gespeichert.

protected func Initialize()
{
  SetAction("Exist");
}

private func SoundScore()
{
  Sound("Energize");
}

private func SoundOut()
{
  Sound("DeEnergize");
}

protected func CheckGoal()
{
	var pObj = FindObject2(Find_InRect(-19,-30,38,49), Find_ID(GBRB), Find_Not(Find_Owner(NO_OWNER))); // Bombe suchen.
	if( pObj )
	{
		var iBombTeam = GetPlayerTeam(pObj->GetOwner()); // Team speichern
		if(iBombTeam != GetGoalTeam()) // Eigentore gibt es nicht!
		{
			// Punktevergabe
			if(pObj->Contained()) 
			{
				DoTeamScore(iBombTeam, +2);
				Log("$TeamMultiScore$", GetPlayerName(pObj->GetOwner()), 2, GetTeamName(iBombTeam));
			}
			else 
			{
				DoTeamScore(iBombTeam, +1);
				Log("$TeamScored$", GetPlayerName(pObj->GetOwner()), GetTeamName(iBombTeam));
			}

			// Effekte
			var dir = GetDir();
			SetAction("FlashUp");
			SetDir(dir);
			pObj -> CastParticles("MSpark", 15, 25, 0, 0, 20, 30, RGB(200, 230, 255), RGB(255,255,255));

			// Bombe zurücksetzen
			pObj -> ReturnToSpawn();
      
      Sound("TeamChange",1);
			
		}
	}
}

public func IsBombGoal(){return(true);}

public func GetGoalTeam()
{
	return(iTeam);
}

public func SetGoalTeam(int t)
{
	iTeam = t;
}

//Erzeugt ein Bombentor
global func CreateBombGate(int iT,int iX,int iY, int iDir)
{
 var gate=CreateObject(GBRG,AbsX(iX),AbsY(iY),-1);
 gate->SetGoalTeam(iT);
 gate->SetDir(iDir);
 return (gate);
}
