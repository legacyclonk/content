#strict

func Initialize() 
{
  SetSkyParallax(0,14,30);

  PlaceVegetations(BUSH,25);
  PlaceVegetations(PLM1,45);
  PlaceVegetations(PLM2,45);
  PlaceVegetations(TRE3,45);
  
  PlaceVines();

  PlaceCauldron(1899,326);
  PlaceCauldron(1194,446);
}

func InitializePlayer(int iPlr)
{
  // Werkzeuge und Waffen direkt in Clonks erschaffen  
  GetCrew(iPlr, 0)->CreateContents(MLLT);
  GetCrew(iPlr, 0)->CreateContents(SPR1);

  GetCrew(iPlr, 1)->CreateContents(MLLT);
  GetCrew(iPlr, 1)->CreateContents(AMUL);

  GetCrew(iPlr, 2)->CreateContents(BLTB);
  GetCrew(iPlr, 2)->CreateContents(ARP2);
  GetCrew(iPlr, 2)->CreateContents(SPR1);

}  

func PlaceVegetations(id idTree, int iCount) 
{
  var x=0, y=0, wdt=LandscapeWidth(), hgt=LandscapeHeight();
  for(var i=0;i<iCount;++i)
    PlaceVegetation(idTree,x,y,wdt,hgt,100000);
}

func PlaceCauldron(int iX,int iY)
{
  var pCauldron = CreateObject(CLD1,iX,iY);
  var pFire = LocalN("pFire",pCauldron);
  // Alles Holz aus dem Lagerfeuer werfen
  while(ContentsCount(0,pFire)) RemoveObject(Contents(0,pFire));
  // LAgerfeuer löschen
  ObjectSetAction(pFire,"Idle");
}

func PlaceVines()
{
  SetCon(54, CreateObject(VINE, 421, 671, -1));
  SetCon(74, CreateObject(VINE, 407, 682, -1));
  SetCon(87, CreateObject(VINE, 395, 695, -1));
  SetCon(84, CreateObject(VINE, 624, 701, -1));
  SetCon(56, CreateObject(VINE, 650, 701, -1));
  SetCon(60, CreateObject(VINE, 881, 819, -1));
  SetCon(52, CreateObject(VINE, 891, 825, -1));
  SetCon(83, CreateObject(VINE, 1130, 723, -1));
  SetCon(63, CreateObject(VINE, 1150, 724, -1));
  SetCon(77, CreateObject(VINE, 1164, 734, -1));
  SetCon(81, CreateObject(VINE, 1188, 724, -1));
  SetCon(100, CreateObject(VINE, 1210, 729, -1));
  SetCon(59, CreateObject(VINE, 1249, 718, -1));
  SetCon(83, CreateObject(VINE, 1114, 718, -1));
  SetCon(63, CreateObject(VINE, 1096, 710, -1));
  SetCon(85, CreateObject(VINE, 1049, 708, -1));
  SetCon(61, CreateObject(VINE, 1038, 700, -1));
  SetCon(56, CreateObject(VINE, 1323, 630, -1));
  SetCon(100, CreateObject(VINE, 1347, 603, -1));
  SetCon(63, CreateObject(VINE, 1367, 697, -1));
  SetCon(84, CreateObject(VINE, 2208, 559, -1));
  SetCon(67, CreateObject(VINE, 2223, 550, -1));
  SetCon(70, CreateObject(VINE, 2083, 543, -1));
  SetCon(72, CreateObject(VINE, 2093, 560, -1));
  SetCon(80, CreateObject(VINE, 1731, 428, -1));
  SetCon(64, CreateObject(VINE, 1454, 329, -1));
  SetCon(77, CreateObject(VINE, 1538, 348, -1));
  SetCon(84, CreateObject(VINE, 1521, 349, -1));
  SetCon(100, CreateObject(VINE, 755, 329, -1));
  SetCon(90, CreateObject(VINE, 1505, 284, -1));
  SetCon(73, CreateObject(VINE, 1731, 512, -1));
  SetCon(70, CreateObject(VINE, 2127, 309, -1));
  SetCon(62, CreateObject(VINE, 2135, 313, -1));
  SetCon(59, CreateObject(VINE, 2557, 145, -1));
  SetCon(100, CreateObject(VINE, 2457, 465, -1));
  SetCon(75, CreateObject(VINE, 1691, 601, -1));
  SetCon(92, CreateObject(VINE, 1848, 384, -1));
  SetCon(71, CreateObject(VINE, 1866, 422, -1));
  SetCon(100, CreateObject(VINE, 1882, 433, -1));
  SetCon(80, CreateObject(VINE, 2472, 360, -1));
  SetCon(86, CreateObject(VINE, 2482, 351, -1));
  SetCon(99, CreateObject(VINE, 2400, 362, -1));
  SetCon(90, CreateObject(VINE, 2155, 278, -1));
  SetCon(50, CreateObject(VINE, 1744, 424, -1));
  SetCon(100, CreateObject(VINE, 1764, 384, -1));
  SetCon(77, CreateObject(VINE, 1727, 291, -1));
  SetCon(91, CreateObject(VINE, 1767, 276, -1));
  SetCon(55, CreateObject(VINE, 1778, 262, -1));
  SetCon(100, CreateObject(VINE, 1823, 251, -1));
  SetCon(80, CreateObject(VINE, 1510, 320, -1));
  SetCon(64, CreateObject(VINE, 1114, 397, -1));
  SetCon(100, CreateObject(VINE, 1249, 361, -1));
  SetCon(76, CreateObject(VINE, 1241, 547, -1));
  SetCon(57, CreateObject(VINE, 1202, 538, -1));
  SetCon(52, CreateObject(VINE, 1192, 534, -1));
  SetCon(100, CreateObject(VINE, 1137, 502, -1));
  SetCon(57, CreateObject(VINE, 850, 291, -1));
  SetCon(65, CreateObject(VINE, 974, 290, -1));
  SetCon(100, CreateObject(VINE, 816, 267, -1));
  SetCon(80, CreateObject(VINE, 754, 399, -1));
  SetCon(100, CreateObject(VINE, 155, 430, -1));
  SetCon(62, CreateObject(VINE, 89, 513, -1));
  SetCon(68, CreateObject(VINE, 113, 515, -1));
  SetCon(83, CreateObject(VINE, 474, 415, -1));
  SetCon(87, CreateObject(VINE, 488, 428, -1));
  SetCon(50, CreateObject(VINE, 336, 284, -1));
  SetCon(100, CreateObject(VINE, 978, 264, -1));
  SetCon(100, CreateObject(VINE, 814, 214, -1));
  SetCon(50, CreateObject(VINE, 1245, 338, -1));
  SetCon(100, CreateObject(VINE, 1827, 225, -1));
  SetCon(100, CreateObject(VINE, 1845, 360, -1));
  SetCon(100, CreateObject(VINE, 761, 356, -1));
  SetCon(60, CreateObject(VINE, 757, 314, -1));
  SetCon(100, CreateObject(VINE, 864, 292, -1));
  SetCon(100, CreateObject(VINE, 1504, 265, -1));
  SetCon(100, CreateObject(VINE, 1457, 305, -1));
  SetCon(100, CreateObject(VINE, 1764, 360, -1));
  SetCon(100, CreateObject(VINE, 1137, 466, -1));
  SetCon(60, CreateObject(VINE, 1114, 377, -1));
  SetCon(80, CreateObject(VINE, 2456, 431, -1));
}
