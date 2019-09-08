#strict

protected func Sparkle() {
  if(!Contained())
    //if(GBackLiquid())
      CreateParticle("PxSpark",RandomX(-5,5),RandomX(-5,5),0,0,RandomX(80,200),RGB(255,40,0));
}