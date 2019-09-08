/*-- Busch --*/

#strict

#include TREE

private func ReproductionAreaSize() { return(400); }
private func ReproductionRate()     { return(500); }
private func MaxTreeCount()         { return(15); }
private func TreeStrength()  { return(40); }
private func FruitRate()     { return(15); }
private func FruitID()       { return(WMPF); }
private func MaxFruitCount() { return(4); }

private func Seed()		// alle 350 Frames
{
  if(IsStanding()) if(GetCon()>99) if(!Random(FruitRate())) Fruit();
  return(_inherited());
}

public func ChopDown()		// Früchte fallen runter wenn gefällt
{
  var pFruit;
  while(pFruit = FindObject(FruitID(),0,0,0,0,0,0,this(),0,pFruit)) pFruit->~Drop();
  return(_inherited());
}

public func Incineration()	// Früchte versengen alle in den Flammen!!!1
{
  var pFruit;
  while(pFruit = FindObject(FruitID(),0,0,0,0,0,0,this(),0,pFruit)) RemoveObject(pFruit);
  // statt BurnTo in der Defcore
  ChangeDef(BUSB);
}

/* Funktionen zum Platzieren der Früchte */

private func FruitRetangle(&iWidth, &iHeight, &iOffset)
{
  var iFireTop = GetDefFireTop(GetID());
  iWidth =   GetDefWidth(GetID())*GetCon()/100;
  iHeight =  GetDefHeight(GetID())*GetCon()/100;
  iHeight -= iFireTop;

  // Frucht soll nicht "überhängen"
  iWidth  -= iWidth/10;
  iHeight -= iHeight/10;

  // ganz am Rand nicht
  //iWidth  -= GetDefWidth(FruitID());
  //iHeight -= GetDefHeight(FruitID());

  var iOffset = -iFireTop/2;
}

public func Fruit()
{

  var pFruit,i;
  while(pFruit = FindObject(FruitID(),0,0,0,0,0,0,this(),0,pFruit)) ++i;

  if(i < MaxFruitCount())
  {
    // Zielrechteck in dem die Früchte erstellt werden sollen
    var iWidth, iHeight, iOffset;
    FruitRetangle(iWidth, iHeight, iOffset);

    var iAngle = Random(360);
    // im Kreis: wirkt besser
    return(PlaceFruit(FruitID(),Cos(iAngle,RandomX(-iWidth/2,iWidth/2)),Sin(iAngle,RandomX(-iHeight/2,iHeight/2)),10));
  }
  // Kein Platz ;'(
  return(0);
}

private func PlaceFruit(id idFruit, int iX, int iY, int iGrowth)
{
  var pFruit = CreateObject(idFruit,iX,iY,-1);
  pFruit->~Place(this(),iGrowth);
}
