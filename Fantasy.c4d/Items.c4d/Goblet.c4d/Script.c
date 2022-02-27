/*-- Kelch --*/

#strict 3

local iTimer;

func Initialize()
{
	iTimer = 0;
}

func FillCheck()
{
	if (!Contained() && Inside(GetR(), 20, 340))
	{
		if (Hit())
		{
			return true;
		}
	}

	if (!Contained() && !(iTimer % 5))
	{
		var iX =  Sin(GetR(), RandomX(-4, 4));
		var iY = -Cos(GetR(), RandomX(-4, 1));
		CreateParticle("NoGravSpark", iX, iY, 0, 0, RandomX(25, 55), RGBa(255, 255, 0, 120));
	}

	if (++iTimer >= 17)
	{
		var contained = Contained();
		if (GetOCF(contained) & OCF_CrewMember)
		{
			DoEnergy(+1, contained);
		}
		iTimer = 0;
	}
}

func Hit()
{
	if (!GBackLiquid())
	{
		for (var i = 15; i > 0; --i)
		{
			var iX =  Sin(GetR(), RandomX(4,8));
			var iY = -Cos(GetR(), RandomX(4,8));
			var iXDir = GetWind() * 30 / 100;
			var iYDir = -Cos(GetR() + RandomX(-2, 2), Random(7));
			InsertMaterial(Material("Water"), iX, iY, iXDir, iYDir);
		}
		return SetEmpty();
	}
}

private func SetEmpty()
{
	ChangeDef(EGBL);
	this->Initialize();
	return true;
}

// Mit Kelch: Aufwertung zum Magus
public func GetRevaluationCombo() { return MAGE; }

protected func UpdateTransferZone()
{
	// this is necessary for backwards compatibility with Goblets stored in Objects.txt
	iTimer ??= 0;
}
