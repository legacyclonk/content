/*-- Teams --*/

#strict

protected func Activate(iPlr)
{
  var pClonk = GetCursor(iPlr);
  var iPlayer;
  CreateMenu(_PLO, pClonk, this(), 0, "Teams", 0, 1);
  for(var iTeam = 1; GameCall("GetTeamName", iTeam); iTeam++)
  {
    if(!GetTeamCount(iTeam)) continue;
    AddMenuItem(GameCall("GetTeamName", iTeam), "", _PLO, pClonk);
    var iPlayer;
    for(var i = 0; i < GetTeamCount(iTeam); i++)
    {
      iPlayer = GetPlrByTeam(iTeam, i);
      AddMenuItem(Format("   <c %x>%s</c>", GetPlrColorDw(iPlayer), GetPlayerName(iPlayer)), "", GetID(), pClonk, 0, 0, 0, 2, 1);
    }
  }  
}
