/*-- Leerer Kelch --*/

#strict 3

local iFill, iTimer;

func Initialize()
{
	iFill = 0;
	iTimer = 0;
}

func FillCheck()
{
	if (++iTimer >= 5)
	{
		iTimer = 0;
		if (!Contained())
		{
			var iX =  Sin(GetR(), RandomX(-4, 8));
			var iY = -Cos(GetR(), RandomX(-4, 4));
			CreateParticle("NoGravSpark", iX, iY, 0, 0, RandomX(25, 55), RGBa(255, 255, 0, 120));
		}
	}
	if (GetMaterial() == Material("Water"))
	{
		if ((iFill += ExtractMaterialAmount(0, 2, Material("Water"), 6)) >= 10)
		{
			SetFull();
		}
	}
}

func SetFull()
{
	ChangeDef(GBLT);
	this->Initialize();
	return true;
}

public func IsMagicProduct() { return true; }
