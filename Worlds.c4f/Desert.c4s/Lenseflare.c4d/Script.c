/*-- Lenseflare --*/
// Sonnenskript includieren
#include SONE

#strict

local SonneX, SonneY;

// Initialisierung
protected func Initialize()
{
	// Nur für den Herrn und Meister sichtbar
	SetVisibility (VIS_Owner(), this());
	if(!FindObject(GetID())) 
	// Das erste Lenseflare-Objekt prüft nur, dass jeder Spieler auch sein Umweltobjekt hat
	{
		SetOwner(-1,this()); // Unparteiisch sein!
		SetCategory(1 | C4D_MouseIgnore(),this()); // Nicht bewegen
		SetAction("IchWillNurSpielerZaehlen");
		return();
	}
}

// Initialisierung als Haupt-Lenseflare
public func WerdeHauptlenseflareFuerDeinenHerrn()
{ 
	SetCategory(C4D_StaticBack()|C4D_Background()| C4D_MouseIgnore());
	for(var i=0; i<MIJON(); ++i)
	{
		Local(i) = CreateObject(GetID());
		SetOwner(GetOwner(),Local(i));
		SetCategory(C4D_StaticBack()| C4D_MouseIgnore()|C4D_Foreground(),Local(i));
		ObjectSetAction(Local(i),Format("Fleck%d",i%14));
	}
	SetAction("SeiBrav");
}

// Erstellt einen Lenseflare und weist ihm einen Besitzer zu
private func ErstelleLenseflare(FuerDiesenSpieler)
{
	var Flare = CreateObject(GetID());
	SetOwner(FuerDiesenSpieler,Flare);
	Flare->WerdeHauptlenseflareFuerDeinenHerrn();
}
			

// Von "SeiBrav" aufgerufen
protected func TuWasBraves()
{
	// Hat es seinen Besitzer verloren? :(
	if((!GetPlayerName(GetOwner())) || GetOwner()==-1)
	{
		for(var i=0; i<MIJON(); ++i)
			RemoveObject(Local(i));
		RemoveObject(this()); 
		return();
	}
	// Sonnenposition anpassen
	BerechneSonne();
	SetPosition(SonneX,SonneY);
	// unskalierten Vektor ermitteln
	var VektorX=GibVektorX(GetOwner());
	var VektorY=GibVektorY(GetOwner());
	var VektorDist=GibVektorDist(GetOwner());
	// Die Modulation ist für alle Flares gleich
	var FarbModulation = RGBa(GibFarbMod(),GibFarbMod(),GibFarbMod(),GibAlphaMod(VektorDist));
	// Genauso auch die Abstandsangabe
	var Abstaende = 1000-BoundBy((3000-VektorDist*10)/3,0,1000);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
	var AbstaendeAbsolut = (50*Abstaende)/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lenseflares sind
	// Ist unsere liebe Sonne überhaupt sichtbar (also Tag + unverdeckt)
	var SonneSichtbar = IsDay() && (!GBackSemiSolid(0,0));
	// Tagsüber die Sonne einblenden
	if(IsDay())
	{
		SetVisibility (VIS_Owner());
	}
	else
	{
		SetVisibility (VIS_None());
	}
	// Alle Lenseflares durchgehen
	for(var i=0; i<MIJON(); ++i)
	{
		if(SonneSichtbar)
		{
			// Tags sichtbar
			SetVisibility (VIS_Owner(), Local(i));
			SetClrModulation(FarbModulation,Local(i));
			var LensDist = (i*AbstaendeAbsolut)/100;
			SetPosition(GibLensPosX(LensDist,VektorX,VektorDist), GibLensPosY(LensDist,VektorY,VektorDist),Local(i));
		}
		else
			// Nachts unsichtbar ODER bei verdeckter Sonne
			SetVisibility (VIS_None(), Local(i));
	}
	// Sofern der Spieler alleine ist
	if(GetPlayerCount()==1)
		if(IsDay())
		{
			var Gelbstich = BoundBy((500-VektorDist)/5,0,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
			SetGamma (RGB(000+(30*Gelbstich)/100,000+(30*Gelbstich)/100,000),
					  RGB(128+(60*Gelbstich)/100,128+(60*Gelbstich)/100,128), 
					  RGB(255,255,255), GammaRampe_Lenseflare());
		}
}

// Für die Farbmodulation (Entfernung + Nacht)
private func GibHelligkeit()
{
	var ZeitObjekt = FindObject(TIME);
	if(!ZeitObjekt)
		return(100);
	return(Local(2, ZeitObjekt));
}

private func GibSonnenMod() // Dämmerung 
{
	var Helligkeit = GibHelligkeit();
	// Folgende Farbwerte sind in Promille
	var Rot = 1000;
	var Grun = 1000;
	var Blau = 1000;
	if(Inside(Helligkeit,1,20)) // Dämmerung -> Rotstich
	{
		var RotStich = Cos(Helligkeit*4,500);
		Grun -= RotStich/2;
		Blau -= RotStich;
	}
	// Nun die Farbe berechnen
	return(RGB((255*Rot)/1000,(255*Grun)/1000,(255*Blau)/1000));
}

private func GibFarbMod()
{
	return((255*GibHelligkeit())/100);
}

private func GibAlphaMod(iEntfernung)
{
	var Staerke = BoundBy((2000-iEntfernung)/20,75,100); // Prozentangabe 75 = weit weg, 100 = nahe; 2000 Pixel sind dabei "weit weg"
	return(255-(255*Staerke)/100); 
}

// Genutzte Gammarampe
global func GammaRampe_Lenseflare()
{
	return(2);
}

// Den (unskalierten) Vektor. Er wird skaliert, um die Lenseflare-Positionen zu errechnen
private func GibVektorX(iSpieler)
{
	return(GibMittelpunktX(iSpieler) - SonneX);
}

private func GibVektorY(iSpieler)
{
	return(GibMittelpunktY(iSpieler) - SonneY);
}

// Skaliere den Vektor mit der gewünschten Entfernung Lenseflare->Sonne
private func GibLensPosX(iEntfernung, VektorX, VektorDist)
{
	// Skalierung in Promille
	var Skalierung = (iEntfernung*1000) / VektorDist;
	var TempX = (VektorX * Skalierung)/1000;
	return(TempX + SonneX);
}

private func GibLensPosY(iEntfernung, VektorY, VektorDist)
{
	// Skalierung in Promille
	var Skalierung = (iEntfernung*1000) / VektorDist;
	var TempY = (VektorY * Skalierung)/1000;
	return(TempY + SonneY);
}

// Wieviele Lenseflares hätten's denn gerne
private func MIJON()
{
	// etwa alle 50 Pixel einen Lenseflare
	return(LandscapeWidth() / 2 / 50);
}