/*-- Verbranntes Ledergemälde --*/

#strict 2

protected func Destruction()
{
  // Der Clan bekommt den jeweiligen Bauplan zurück
  if(!TTPL->~RejectResearch(this(),-1)) SetClanKnowledge(GetClan(),GetDefConstructTo(GetID()),0);
}