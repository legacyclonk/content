/*-- Tunnelzauber --*/

#strict

// Aktivierung durch Zaubern
public func Activate(object caster, object real_caster)
{
  // Eigentlichen Zauberer rausfinden
  var base_caster = caster;
  if(real_caster) caster = real_caster;

  // Zielen wenn möglich
  if(caster->~DoSpellAim(this(), base_caster) ) return(1);

  // Clonk kann nicht zielen: Tunnel geht geradeaus
  StartTunnel(caster, GetDir(caster)*180 - 90);

  // Zauberobjekt ist nicht mehr vonnoeten
  RemoveObject();

  // Zauber erfolgreich
  return(1);
}

// Aktivierung nach Zielen
public func ActivateAngle(object caster, int angle)
{
  // Akustik muss sein
  Sound("Magic*");

  // Auf gehts
  StartTunnel(caster, angle);

  // Objekt löschen
  RemoveObject();

  // Zauber erfolgreich
  return(1);
}

// Starten des Tunnels
public func StartTunnel(object caster, int angle)
{
  // Effekt anwerfen, der übernimmt den Rest
  AddEffect("TunnelUSpell", 0, 260, 1, 0, GetID(), caster, angle);
}

// Effektparameter
static const MTNL_HalfHeight = 15; // Halbe Höhe des Tunnels
static const MTNL_Jitter = 1; // Franseliger Rand am Gang
static const MTNL_Length = 120; // Länge des Tunnels
static const MTNL_Duration = 250; // Wartezeit zwischen Auf- und Abbau
static const MTNL_Precision = 12; // Rechengenauigkeit bei Nachkommastellen, in Bit

// Überprüft, ob der Punkt x,y in einem Tunnel der gerade nach unten geht
// innerhalb des Tunnels liegt
public func InTunnel(int x, int y)
{
  if(y < 0 || y > MTNL_Length) return false;

  if(y < MTNL_HalfHeight)
  {
    return (MTNL_HalfHeight - y)*(MTNL_HalfHeight - y) + x*x <= MTNL_HalfHeight*MTNL_HalfHeight;
  }
  else if(y > MTNL_Length - MTNL_HalfHeight)
  {
    return (y - (MTNL_Length - MTNL_HalfHeight))*(y - (MTNL_Length - MTNL_HalfHeight)) + x*x <= MTNL_HalfHeight*MTNL_HalfHeight;
  }
  else return x >= -MTNL_HalfHeight && x <= MTNL_HalfHeight;
}

public func TunnelJitterShift(int seed, int y)
{
  // Wir können hier nicht einfach Random() benutzen, da wir deterministischen
  // Zufall brauchen: Beim Öffnen und beim Schließen des Tunnels soll sich genau
  // das gleiche Muster ergeben.
  return Abs((((seed ^ (y<<2) ^ (y<<5))^((seed>>8)+1+y+(y<<2)))/17))%(MTNL_Jitter*2+1) - MTNL_Jitter;
}

// Und los gehts!
public func FxTunnelUSpellStart(object target, int effect_number, int temp, object caster, int angle)
{
  // Temporärer Aufruf? Nix für uns.
  if(temp) return(0);

  // Startpunkt und -winkel speichern
  EffectVar(0, target, effect_number) = Sin(angle, -10) + GetX(caster); // Start-X
  EffectVar(1, target, effect_number) = -Cos(angle, -10) + GetY(caster); // Start-Y
  // Um nicht so viele Vorzeichen mitschleppen zu müssen rechnet der
  // Algorithmus ausgehend von einem Tunnel nach unten. Da 0 Grad aber nach
  // oben zeigen passen wir das hier an:
  EffectVar(2, target, effect_number) = angle + 180; // Winkel
  EffectVar(3, target, effect_number) = 0; // Aktueller Daten-Index
  EffectVar(4, target, effect_number) = Random(32768); // Jitter-Seed
  EffectVar(5, target, effect_number) = 0; // prev. jitter (left)
  EffectVar(6, target, effect_number) = 0; // prev. jitter (right)

  // Groß genuges Array für die Pixeldaten
  EffectVar(7, target, effect_number) = CreateArray(4 * MTNL_Length * MTNL_HalfHeight);

  // Fertig, der Timer übernimmt den Rest
  return(1);
}

global func FxTunnelUSpellTimer(object target, int effect_number, int effect_time)
{
  // Aktueller Status (0: Aufbau, 1: Warten, 2: Abbau)
  var cur_state = 0;
  if(effect_time >= 1 + MTNL_Length) cur_state = 1;
  if(effect_time >= 1 + MTNL_Length + MTNL_Duration) cur_state = 2;
  if(effect_time >= 1 + MTNL_Length * 2 + MTNL_Duration) return(-1);

  // Warten?
  if(cur_state == 1)
  {
    // Daten-Index resetten, damit er beim Abbau ausgelesen werden kann.
    EffectVar(3, target, effect_number) = 0;
    EffectVar(5, target, effect_number) = 0; // prev. jitter (left)
    EffectVar(6, target, effect_number) = 0; // prev. jitter (right)
    // Warten.
    return;
  }

  // Tunnel-Y anhand Effekt-Zeit rausfinden
  var adv_y = effect_time - 1;
  if(cur_state == 2) adv_y = effect_time - 1 - (MTNL_Length + MTNL_Duration);

  // Rotationswinkel
  var angle = EffectVar(2, target, effect_number);

  var aa = Cos(angle, 1 << MTNL_Precision), ab = Sin(angle, 1 << MTNL_Precision);
  var ba = -ab,                             bb = aa;
  
  var y1 = adv_y,                          y2 = adv_y;
  var x1 = -MTNL_HalfHeight - MTNL_Jitter, x2 = MTNL_HalfHeight + MTNL_Jitter;

  var x1_ = (aa*x1 >> MTNL_Precision) - (ab*y1 >> MTNL_Precision), y1_ = -(ba*x1 >> MTNL_Precision) + (bb*y1 >> MTNL_Precision);
  var x2_ = (aa*x2 >> MTNL_Precision) - (ab*y2 >> MTNL_Precision), y2_ = -(ba*x2 >> MTNL_Precision) + (bb*y2 >> MTNL_Precision);

  var bx1 = Min(x1_, x2_), by1 = Min(y1_, y2_);
  var bx2 = Max(x1_, x2_), by2 = Max(y1_, y2_);

  // Verschiebung ermitteln, die der Rand in dieser Zeile ausgefranst wird
  var seed = EffectVar(4, target, effect_number);

  var old_lshift = EffectVar(5, target, effect_number);
  var lshift = MTNL->TunnelJitterShift(seed, adv_y);
  lshift = BoundBy(lshift, old_lshift - 1, old_lshift + 1);
  EffectVar(5, target, effect_number) = lshift;

  var old_rshift = EffectVar(6, target, effect_number);
  var rshift = MTNL->TunnelJitterShift(seed, adv_y + MTNL_Length);
  rshift = BoundBy(rshift, old_rshift - 1, old_rshift + 1);
  EffectVar(6, target, effect_number) = rshift;

  var mvec = Material("Vehicle");
  var mtun = Material("Tunnel");

  var px = EffectVar(0, target, effect_number);
  var py = EffectVar(1, target, effect_number);

  for(var y = by1 - 1; y <= by2 + 1; ++ y)
  {
    for(var x = bx1 - 1; x <= bx2 + 1; ++ x)
    {
      var y_ = (ba*x >> MTNL_Precision) + (bb*y >> MTNL_Precision);
      if(y_ != adv_y) continue;

      // Rand ausfransen
      var x_ = (aa*x >> MTNL_Precision) + (ab*y >> MTNL_Precision);
      if(x_ < 0)
        x_ += lshift;
      else
        x_ += rshift;

      var did_draw = false;

      // Die auskommentierten Codezeilen machen die Ausführung des Codes um 40
      // bis 70% schneller, benötigen jedoch die engineinternen Funktionen
      // GBackPix und SBackPix, die die Engine dann ans Script weitergeben
      // müsste.
      if(MTNL->InTunnel(x_, y_))
      {
        var tx = x + px;
        var ty = y + py;
        if(!cur_state)
				{
          var mat_data = GetMaterial(tx, ty);
          if(mat_data == mvec || mat_data == mtun || mat_data == -1)
            pixel_data = -1;
          else
            pixel_data = mat_data | (1 << 7); //
//          pixel_data = GBackPix(tx, ty);

	        EffectVar(7, target, effect_number)[EffectVar(3, target, effect_number)++] = pixel_data;

	        if(pixel_data != -1)
	        {
            EffectVar(7, target, effect_number)[EffectVar(3, target, effect_number)++] = GetTexture(tx, ty); //
            FreeRect(tx, ty, 1, 1);
            did_draw = true;
	        }
	      }
        else
        {
          var pixel_data = EffectVar(7, target, effect_number)[EffectVar(3, target, effect_number)++];
          if(pixel_data != -1)
          {
            var texture_data = EffectVar(7, target, effect_number)[EffectVar(3, target, effect_number)++]; //
            // Ein naives DrawMaterialQuad("...", tx, ty, tx+1, ty, tx+1, ty+1, tx, ty+1) zeichnet zwei Pixel in
            // x-Richtung statt nur einem. Ich habe mit den Parametern ein bisschen rumgespielt damit sie nur ein
            // Pixel erzeugen und das ist dabei rausgekommen. Siehe auch #2369.
            DrawMaterialQuad(Format("%s-%s", MaterialName(pixel_data & ~(1 << 7)), texture_data), tx, ty, tx, ty, tx, ty+1, tx, ty, pixel_data & (1 << 7)); //
            //SBackPix(tx, ty, pixel_data);
            did_draw = true;
          }
        }

        if(did_draw)
          if(!Random(11))
            CreateParticle("MaterialSpark", tx, ty, 0, RandomX(-1, 1), RandomX(5*3, 5*14), RGBa(255, 128, 128, Random(128)) );
      }
    }
  }
}

public func IsAimerSpell() { return(1); }

// Zaubereigenschaften
public func GetSpellClass() { return(EART); }
public func GetSpellCombo() { return("633"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
