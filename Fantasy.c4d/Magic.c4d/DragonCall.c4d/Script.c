/* Drachenruf */

#strict

/* Aufruf */

public func Activate(object caster, object real_caster)
{
  // Effekt prüfen
  var iResult;
  if (iResult = CheckEffect("DragonCallNSpell", 0, 125)) return(iResult!=-1 && RemoveObject());
  
  if(real_caster) caster = real_caster;

  var dragon = 0;
  while(dragon = FindObject(DRGN, caster->GetX() - GetX(), caster->GetY() - GetY(), -1, -1, 0, 0,0, NoContainer(), dragon) )
  {
    // Nah genug
    if (ObjectDistance(caster, dragon) < 750)
      // Drache hat keinen Reiter
      if (!dragon->GetRider())
        // Drache ist ausgewachsen
        if (dragon->GetCon() >= 100)
        {
          // Drache ist wütend
          if (dragon->IsAngry())
          {
            Message("$MsgDragonAngry$", caster);
            caster->Sound("Error");
            return(false);
          }            
          // Kontrolle übernehmen
          dragon->ResetControl();
          // Den Drachen rufen
          //if (!dragon->MoveTo(caster->GetX(), caster->GetY(), true))
          if (!dragon->SetCommand(dragon, "Follow", caster))
          {
            // Nicht moeglich: Dann hat der Drache hier wohl kein Platz.
            Message("$MsgNoPlace$", caster);
            caster->Sound("Error");
            return(false);
          }
          // Hörbare Reaktion vom Drachen
          dragon->Sound("DragonGrowl");
          // Effekte
          CallEffects(caster->GetX()-GetX(), caster->GetY()-GetY(), dragon->GetX()-GetX()+dragon->GetVertex(0,0), dragon->GetY()-GetY()+dragon->GetVertex(0,1)+20);
          // Erfolg
          return(true);
        }
  }

  // Kein rufbarer Drache in Reichweite
  Message("$MsgNoDragon$", caster);
  caster->Sound("Error");
  return(false);
}

private func CallEffects(x1, y1, x2, y2, color1, color2) 
{
  CastParticles("PxSpark", 10, 30, x2, y2, 5*2, 5*10, RGB(196, 196, 196), RGB(255, 255, 255));
  for (var i = 0; i < 5; i++)
    DrawParticleLine("PSpark", x1+Random(20)-10,y1+Random(20)-10, x2+Random(20)-10,y2+Random(20)-10, 10+Random(20),50, RGBa(255,50+Random(100),50+Random(100),50), RGBa(50+Random(100),255,50+Random(100),100), -Random(3));
  return(1);
}
    
/* Zaubercombo */

public func GetSpellClass(object pMage) { return(FIRE); }
public func GetSpellCombo(pMage) { return ("433"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
