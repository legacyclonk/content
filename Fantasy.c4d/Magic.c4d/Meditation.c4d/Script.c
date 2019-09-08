/* Meditation */

#strict

func Activate(pCaster,pRealcaster)
{
  var pClonk=pRealcaster;
  if(!pClonk) pClonk=pCaster;
  
  if(FindObject(NMGE)) return(0);
  
  // Clonk fängt an zu Meditieren
  AddEffect("MeditationPSpell",pClonk,250,2,0,GetID());

  // entfernen
  RemoveObject();
  return(1);
}

func FxMeditationPSpellTimer(pClonk, iEffectNumber)
{
  var iMLength=GetActMapVal("Length",GetAction(pClonk),GetID(pClonk));

  // Bei Menü will der Benutzer abbrechen
  if(GetMenu(pClonk)) return(-1); // normales Menü
  if(GetEffect("IntCBMUCheck", pClonk)) return(-1); // Kombomenü
  
  // magische Aktion vorhanden
  if(GetActMapVal("Name","Magic",GetID(pClonk)))
  {
    // z.B. während des Zauberns durch einen Stein getroffen
    if(!DoesMagic(pClonk))
    {
        // Ohnmächtig werden und Zauberenergie verlieren	    
        ObjectSetAction(pClonk, "Dead", 0, 0, 1);	    
        AddEffect("RemoveMagicEnergy", pClonk, 1, 2, 0, MMED);
	return(-1);
    }
    // Zauberaktion für das zaubernde Objekt: Es wird angenommen, dass diese
    // in einem Raum von etwa 2/6 bis 5/6 der Zauberaktion spielt
    if(GetPhase(pClonk) >= iMLength*5/6) SetPhase(iMLength*2/6,pClonk);
  }

  // Abbruch wenn voll
  if(GetMagicEnergy(pClonk)==GetPhysical("Magic",0,pClonk)/1000) return(-1);

  // Abbruch wenn zu schwach
  if(GetEnergy(pClonk)<=5) return(-1);
  
  // Partikeleffekte für den betroffenen
  CreateParticle("PSpark",GetX(pClonk),GetY(pClonk),0,0,180,RGB(0,0,20),pClonk);
  var i=2;
  while(i--)
  {
    var angle = RandomX(-180, 180);
    CreateParticle("PSpark",GetX(pClonk)+Sin(angle, 30),GetY(pClonk)-Cos(angle, 30),
		    -Sin(angle, 10),Cos(angle, 10),RandomX(50,70), 
		    RGBa(RandomX(0,100), RandomX(0,100), RandomX(150, 250), 75),pClonk);
  }
  // Clonk Mana geben
  if (!Random(3)) DoMagicEnergy(+1,pClonk);
  // Dabei verliert er auch ein bischen Lebensenergie
  else if(!Random(8)) DoEnergy(-1, pClonk);
}

func FxMeditationPSpellInfo() { return ("{{MMED}} $SpellInfo$"); }

func DoesMagic(pObj) { return(WildcardMatch(GetAction(pObj), "*Magic")); }

func FxRemoveMagicEnergyTimer(pClonk, iEffectNumber)
{
  // Liegen bleiben!	
  if(!(GetAction(pClonk)S="Dead")) 
  { ObjectSetAction(pClonk, "Dead", 0, 0, 1); SetPhase(5, pClonk);}
  // Hat er noch Zauberenergie?
  if(GetMagicEnergy(pClonk))
  {
    // Zauberenergie entfernen...
    DoMagicEnergy(-1, pClonk);
    // ...und verteilen
    var pObj;
    while(pObj=FindObject(0,0,0,-1,-1,OCF_Alive(), 0,0,0,pObj))
    {
      if(ObjectDistance(pObj, pClonk)>300) break;
      if(GetPhysical("Magic", 0, pObj))
	if(pObj!=pClonk)
          if(DoMagicEnergy(1, pObj)) break;
    }
    // Schöner Effekt dazu
    var color = RGBa(RandomX(0,100), RandomX(0,100), RandomX(150, 250), 75);
    CastParticles("PSpark", 3, 30, GetX(pClonk), GetY(pClonk)+8, 50, 70, color, color, pClonk);
  }
  else
  {
    // Sind wir fertig? dann aufstehen	  
    if(EffectVar(0, pClonk, iEffectNumber)++>10) { ObjectSetAction(pClonk, "FlatUp", 0, 0, 1); return(-1); }
    // Energie verlieren
    if(GetEnergy(pClonk)>5) DoEnergy(-1, pClonk); // nicht sterben
    // Effekt
    var color = RGBa(RandomX(150,250), RandomX(0,100), RandomX(0, 100), 75);
    CastParticles("PSpark", 3, 30, GetX(pClonk), GetY(pClonk)+8, 50, 70, color, color, pClonk);
  }
}

// Nicht im Turm zu zaubern
func NoTowerMagic() { return(true); }

// Bei Regel keine Mana soll der Spruch nicht verwendet werden
//public func DoesBlock(pClonk) { return(FindObject(NMGE)); }

/* Zaubercombo */

public func GetSpellClass() { return(WATR); }
public func GetSpellCombo() { return ("165"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
