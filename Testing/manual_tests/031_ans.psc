Scriptname WSN_WorldShaman_Script extends activemagiceffect
message property WSN_Effects_Message_TheAllMaker_SpellStored auto
formlist property WSN_Misc_TheAllMaker_Boon1_Formlist_PowersUsed auto
spell[] property WSN_Spells auto
function OnSpellCast(Form akSpell)
  if (akSpell as spell) as Bool && WSN_Spells.find(akSpell as spell, 0) != -1
    WSN_Effects_Message_TheAllMaker_SpellStored.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
    WSN_Misc_TheAllMaker_Boon1_Formlist_PowersUsed.Addform(akSpell)
  endif
endfunction
