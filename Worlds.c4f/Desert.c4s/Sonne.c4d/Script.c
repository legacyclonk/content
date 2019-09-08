/*-- Sonne --*/

#strict

local SonneX, SonneY;

// Initialisierung
protected func Initialize()
{
	// In den Hintergrund mit dir.
	SetCategory(C4D_StaticBack()|C4D_Background());
	// Action setzen
	SetAction("SeiBrav");
	// Nur für den Herrn und Meister sichtbar
	SetVisibility (VIS_Owner(), this());
	if(!FindObject(GetID())) 
	// Das erste Sonnen-Objekt prüft nur, dass jeder Spieler auch sein Umweltobjekt hat
	{
		SetOwner(-1,this()); // Unparteiisch sein!
		SetCategory(1,this()); // Nicht bewegen
		SetAction("IchWillNurSpielerZaehlen");
		return();
	}
}

// Sucht einen Spieler, der keine Sonne hat (-1 für Erfolglos)
private func FindeFreienSpieler()
{
	for(var i=0; i<16; ++i)
		if(GetPlayerName(i)&&(!FindObjectOwner(GetID(),i)))
			return(i);
	return(-1);
}

// Prüft alle Spieler durch, ob denn auch jeder seine Sonne hat
protected func PruefeAlleSpieler()
{
	var FreierSpieler;
	while((FreierSpieler=FindeFreienSpieler())!=-1)
		ErstelleLenseflare(FreierSpieler);
}

// Erstellt eine Sonne und weist ihm einen Besitzer zu
private func ErstelleLenseflare(FuerDiesenSpieler)
{
	CreateObject(GetID(),0,0,FuerDiesenSpieler);
}
			

// Von "SeiBrav" aufgerufen
protected func TuWasBraves()
{
	// Bei vorhandenem Lenseflare (mit integrierter Sonne) kann dieses Objekt gelöscht werden
	if(ObjectCount(LENS))
	{
		RemoveObject(this());
		return();
	}
	// Hat es seinen Besitzer verloren? :(
	if((!GetPlayerName(GetOwner())) || GetOwner()==-1)
	{
		RemoveObject(this()); 
		return();
	}
	// Sonnenposition anpassen
	BerechneSonne();
	SetPosition(SonneX,SonneY);
	// Nachts ausblenden
	if(IsDay())
		SetVisibility (VIS_Owner());
	else
		SetVisibility (VIS_None());
	// Sofern der Spieler alleine ist, blenden
	var VektorDist=GibVektorDist(GetOwner());
	if(GetPlayerCount()==1)
		if(IsDay())
		{
			var Gelbstich = BoundBy((500-VektorDist)/5,0,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
			SetGamma (RGB(000+(30*Gelbstich)/100,000+(30*Gelbstich)/100,000),
					  RGB(128+(60*Gelbstich)/100,128+(60*Gelbstich)/100,128), 
					  RGB(255,255,255), GammaRampe_Lenseflare());
		}
}

// Interner Kruscht!
// Bildschirmmittelpunkt
private func GibMittelpunktX(iSpieler)
{
	if(IsNetwork())
		return(GetX(GetCursor(iSpieler)));
	else
		return(GetPlrViewX(iSpieler));
}

private func GibMittelpunktY(iSpieler)
{
	if(IsNetwork())
		return(GetY(GetCursor(iSpieler)));
	else
		return(GetPlrViewY(iSpieler));
}

// Sonnenkoordinate
private func BerechneSonne()
{
	BerechneSonneX();
	BerechneSonneY();
}
private func BerechneSonneX()
{
	var ZeitObjekt = FindObject(TIME);
	if(!ZeitObjekt)
		SonneX = LandscapeWidth() / 2;
	else
	{
		// Da die Winkelangaben aufgrund ihres Integer-Wesens zu ungenau für eine schöne Laufbahn sind
		// muss hier getrickst werden. Über Sin/Cos werden die Eckpunkte eines n-Ecks errechnet, auf den Linien
		// dazwischen fährt dann die Sonne.
		var Winkel = GibTageszeit()*360/100 +90;
		var VonEck = LandscapeWidth()/2+Cos(Winkel,LandscapeWidth()/2);
		Winkel = (GibTageszeit()+1)*360/100 +90;
		var ZuEck = LandscapeWidth()/2+Cos(Winkel,LandscapeWidth()/2);
		// Wieviele Ticks verstreichen während einem "Punkt" bei Tageszeit?
		// Sprich: Wieviele Zwischenschritte liegen zwischen VonEck und ZuEck?
		var Zwischenschritte = PrivateCall(ZeitObjekt,"TimeResolution") / 100;
		// Schrittweite in 1/100000 Pixel
		var Schrittweite = (ZuEck-VonEck)*100000 / (Zwischenschritte*TIME_Tick());
		// Wieviel TIME-Ticks sind seit der letzten "vollen" Tageszeit vergangen?
		var Vergangen = Local(1, ZeitObjekt) - GibTageszeit()*100;
		Vergangen=Vergangen*TIME_Tick()+FrameCounter()%10;
		SonneX = VonEck + (Vergangen*Schrittweite)/100000; // 100000 = 100000 wegen der 1/100000 Schrittweite
	}
}

private func BerechneSonneY()
{
	var ZeitObjekt = FindObject(TIME);
	if(!ZeitObjekt)
		SonneY = 150;
	else
	{		
		// Da die Winkelangaben aufgrund ihres Integer-Wesens zu ungenau für eine schöne Laufbahn sind
		// muss hier getrickst werden. Über Sin/Cos werden die Eckpunkte eines n-Ecks errechnet, auf den Linien
		// dazwischen fährt dann die Sonne.
		var Winkel = GibTageszeit()*360/100 +90;
		var VonEck = GetHorizonHeight(0)-Sin(Winkel,300)-50;
		Winkel = (GibTageszeit()+1)*360/100 +90;
		var ZuEck = GetHorizonHeight(0)-Sin(Winkel,300)-50;
		// Wieviele Ticks verstreichen während einem "Punkt" bei Tageszeit?
		// Sprich: Wieviele Zwischenschritte liegen zwischen VonEck und ZuEck?
		var Zwischenschritte = PrivateCall(ZeitObjekt,"TimeResolution") / 100;
		// Schrittweite in 1/100000 Pixel
		var Schrittweite = (ZuEck-VonEck)*100000 / Zwischenschritte;
		Schrittweite/=TIME_Tick(); // Jedes Time-Tick besteht aus 10 einzelnen Ticks
		// Wieviel TIME-Ticks sind seit der letzten "vollen" Tageszeit vergangen?
		var Vergangen = Local(1, ZeitObjekt) - GibTageszeit()*100;
		Vergangen=Vergangen*TIME_Tick()+FrameCounter()%10;
		SonneY = VonEck + (Vergangen*Schrittweite)/100000; // 100000 = 100000 wegen der 1/100000 Schrittweite
	}
}

private func TIME_Tick()
{
	return(10);
}

private func GibTageszeit()
{
	var ZeitObjekt = FindObject(TIME);
	if (!ZeitObjekt) return(-1);
	return(Local(1, ZeitObjekt)*100/PrivateCall(ZeitObjekt,"TimeResolution")); // 0-100=Mittag-Mittag
}

private func GibVektorDist(iSpieler)
{
	return(Distance(GibMittelpunktX(iSpieler),GibMittelpunktY(iSpieler),SonneX,SonneY));
}

// Bodenhöhe. Aus dem Schmetterling übernommen und aktuelle Syntax und global gemacht und so. k?
private func GetHorizonHeight(iX)
{
	var iY = - GetY(); // Zu globaler Koordinate wandeln
	iX -= GetX(); // Zu globaler Koordinate wandeln
	while ( (iY<LandscapeHeight())&&(!GBackSemiSolid(iX,iY)))
	{
		iY+=10;
	}
	return(iY+GetY());
}