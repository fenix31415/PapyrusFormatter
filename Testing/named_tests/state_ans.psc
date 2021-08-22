Scriptname a

state a
endstate

state a
  function foo()
  endfunction
endstate

state a
  function foo1()
  endfunction

  function foo2()
  endfunction

  function foo3()
  endfunction
endstate

state a
  function foo1()
  endfunction

  function foo2()
    foo()
    foo()
  endfunction

  function foo3()
    foo()
  endfunction
endstate

; coms
state a ; coms
  function foo1() ; coms
  endfunction ; coms

  function foo2() ; coms
  endfunction ; coms

  function foo3() ; coms
  endfunction ; coms
endstate ; coms

; coms
;/com1/; state ;/com2/; a ;/com3/;
  ;/com11/; function ;/com12/; foo1 ;/com13/; (;/com14/;) ;/com15/;
  ;/com16/; endfunction ;/com17/;

  ;/com17/; function ;/com18/; foo2 ;/com19/; (;/com20/;) ;/com21/;
  ;/com22/; endfunction ;/com23/;

  ;/com4/; function ;/com5/; foo3 ;/com6/; (;/com7/;) ;/com8/;
  ;/com9/; endfunction ;/com10/;
;/com24/; endstate ;/com25/;

; coms
;/com/; state ;/com/; a ;/com/;
  ;/com/; function ;/com/; foo1 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;

  ;/com/; function ;/com/; foo2 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;

  ;/com/; function ;/com/; foo3 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;
;/com/; endstate ;/com/;

; coms
;/com/; state ;/com/; a ;/com/;
  ;/com/; function ;/com/; foo1 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;

  ;/com/; function ;/com/; foo2 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;

  ;/com/; function ;/com/; foo3 ;/com/; (;/com/;) ;/com/;
  ;/com/; endfunction ;/com/;
;/com/; endstate ;/com/;

state autos
  function foo1()
  endfunction

  function foo2()
  endfunction

  function foo3()
  endfunction
endstate

;/com1/; state ;/com2/; autos ;/com3/;
  function foo1()
  endfunction

  function foo2()
  endfunction

  function foo3()
  endfunction
endstate
