/*-- Aliens! --*/

#strict 2

local maxAliens;

protected func Activate(iPlr)
{
  if(GetID()==ALAR)
    MessageWindow(GetDesc(),iPlr);
  else
    _inherited(iPlr,...);
}

public func GetMaxAliens() { return maxAliens; }

public func SetMaxAliens(val) { maxAliens = val; }

// Kann nicht mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return false; }

public func Init(int iAliens, int iAliensNestSize) 
{
	  Log("Aliens: %d",iAliens);
	  var pAlien, pHive, iX, iY, aReturn, iSize;
	  while(iAliens--)
	  {
		  if(!(aReturn = GameCall("HivePos", iAliens))) {
			  pAlien = PlaceAnimal(ALN2);
			  iX = pAlien -> GetX();
			  iY = pAlien -> GetY();
		  }
		  else {
			  iX = aReturn[0];
			  iY = aReturn [1];
			  CreateObject(ALN2, iX, iY, NO_OWNER);
		  }
		  iSize = RandomX(10+iAliensNestSize/4, 50+iAliensNestSize*3);
		  pHive = CreateObject(HCON, iX, iY, NO_OWNER);
			if(!pHive) continue;
		  pHive -> Init(RandomX(15+iSize, 45+RandomX(iSize*2,iSize*4)), 75, 75, RandomX(20, 30));
      
      for(var i2 = Random(iSize/8); i2; i2--)
			  CreateObject(ALN2,GetX(pHive),GetY(pHive),-1);
      
      // Taktiksetup
      pHive -> AlienClear();
      pHive -> AlienAdd(ALN2, 60, 10+Random(iSize/5));
	    pHive -> AlienAdd(ALN4, 25, 5+Random(iSize/10));
	    pHive -> AlienAdd(ALN1, 15, 1+Random(iSize/30));
      if(Random(iSize/10+1)>3)
        pHive -> AlienAdd(ALN5, 10+Random(iSize/10-3), RandomX(1,1+iSize/100));
      
      ScheduleCall(pHive, "SetSpreadRate", RandomX(1000-iAliensNestSize*10, 5000-iAliensNestSize*20), 0, RandomX(10+iAliensNestSize/2, 50+iAliensNestSize*2));
		  ScheduleCall(pHive, "SetFertility", RandomX(1000-iAliensNestSize*10, 5000-iAliensNestSize*20), 0, RandomX(10+iAliensNestSize/4, 50+iAliensNestSize));
      
      //for(i2 = Random(5); i2; i2--)
			//  PlaceAnimal(ALN6);
		  //if(!Random(10))
			//  PlaceAnimal(ALN5);
	  }
}
