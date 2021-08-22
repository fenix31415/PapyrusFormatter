Scriptname ;/com/; WSN_WorldShaman_Script ;/com/; extends ;/com/; activemagiceffect ;/com/;
;/com/; message ;/com/; property WSN_Effects_Message_TheAllMaker_SpellStored auto
;/com/; formlist ;/com/; property WSN_Misc_TheAllMaker_Boon1_Formlist_PowersUsed auto
;/com/; spell ;/com/; [;/com/;] ;/com/; property WSN_Spells ;/com/; auto ;/com/;
;/com/; function ;/com/; OnSpellCast ;/com/; (;/com/; Form akSpell ;/com/;) ;/com/;
  ;/com/; if;/com/;(;/com/; akSpell;/com/;as;/com/;spell;/com/;) ;/com/;as;/com/; Bool;/com/; && ;/com/;WSN_Spells . find;/com/;(;/com/;akSpell;/com/; as ;/com/;spell;/com/;, ;/com/;0;/com/;);/com/; != ;/com/;-1;/com/;
    WSN_Effects_Message_TheAllMaker_SpellStored.Show(0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000)
    WSN_Misc_TheAllMaker_Boon1_Formlist_PowersUsed.Addform(akSpell)
  endif;/com/; 
endfunction;/com/; 
