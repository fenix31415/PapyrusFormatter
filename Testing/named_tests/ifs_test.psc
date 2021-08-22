Scriptname a
function a()

;com1 a

;com2 a

;com3 a

;com4 a

;com5 a

if a
endif

if a
else
endif

if a
elseif b
endif

if a
elseif b
else
endif

if a
elseif b
elseif c
elseif d
endif

if a
elseif b
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
elseif d
else
endif
elseif d
else
endif
elseif d
if a
elseif b
elseif c
elseif d
else
endif
else
endif
if a
elseif b
elseif c
elseif d
else
endif
if a
elseif b
elseif c
elseif d
else
endif
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
elseif d
else
endif
if a
elseif b
if a
if a
elseif b
elseif c
elseif d
else
endif
elseif b
elseif c
elseif d
else
endif
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
elseif d
else
endif
elseif d
else
endif
if a
elseif b
elseif c
elseif d
else
endif
elseif d
else
endif
elseif d
if a
elseif b
elseif c
elseif d
else
endif
else
if a
if a
elseif b
elseif c
elseif d
else
endif
elseif b
elseif c
elseif d
else
endif
if a
elseif b
elseif c
elseif d
else
endif
endif
if a
elseif b
elseif c
if a
elseif b
elseif c
elseif d
else
endif
elseif d
else
endif
if a
elseif b
elseif c
elseif d
else
endif
elseif d
if a
elseif b
elseif c
elseif d
else
endif
if a
elseif b
elseif c
elseif d
else
endif
else
if a
elseif b
elseif c
elseif d
else
endif
endif


if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
if a
elseif b
elseif c
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif
elseif d
else
endif

; with comments
if a  ; com1
; com2
endif ; com3

if a  ; com1
; com4
else  ; com2
; com5
endif  ; com3

if a  ; com1
; com4
elseif b  ; com2
; com5
endif  ; com3

if a  ; com1
; com2
elseif b  ; com3
; com4
else  ; com5
; com6
endif  ; com7

if a  ; com1
  ; com2
elseif b  ; com3
  ; com4
elseif c  ; com5
  ; com6
elseif d  ; com7
  ; com8
endif  ; com9

if a  ; com1
  ; com2
elseif b  ; com3
  ; com4
elseif c  ; com5
  ; com6
elseif d  ; com7
  ; com8
else  ; com9
  ; com10
endif
; ;/ this comms ;/
;/com1/;if;/com2/;a;/com3/;
;/com4/;elseif;/com5/;b;/com6/;
;/com7/;elseif;/com8/;c;/com9/;
;/com10/;elseif;/com11/;d;/com12/;
;/com13/;else;/com13/;
;/com14/;endif;/com15/;

; ;/ this comms ;/ + spaces
   ;/com1/;   if       ;/com2/;   a   ;/com3/;   
   ;/com4/;   elseif   ;/com5/;   b   ;/com6/;    
   ;/com7/;   elseif   ;/com8/;   c   ;/com9/;   
   ;/com10/;  elseif   ;/com11/;  d   ;/com12/;   
   ;/com13/;  else     ;/com13/;   
   ;/com14/;  endif    ;/com15/;   

; ;/ this comms ;/ + spaces + inside
   ;/com1/;   if       ;/com2/;   a   ;/com3/;   
   ;/com 1/;    
   foo()
   ;/com 2/;    
   ;/com 3/;   
   foo()
   ;/com 4/;    
   ;/com4/;   elseif   ;/com5/;   b   ;/com6/;    
   foo()
   foo()
   foo()
   ;/com7/;   elseif   ;/com8/;   c   ;/com9/;    
   ;/com 5/;
   ;/com10/;  elseif   ;/com11/;  d   ;/com12/;   
   
   ;/com 5/;   
   
   ;/com13/;  else     ;/com13/;   
   ;/com 9/;   
   ;/com 8/; foo() ;/com 6/;     
   ;/com 7/;      
   ;/com14/;  endif    ;/com15/;     

; ; this comments
if a; com1
elseif b; com2
elseif c; com3
elseif d; com4
else; com5
endif; com6

; ; this comments ; + spaces
if a  ; com1
elseif b  ; com2
elseif c  ; com3
elseif d  ; com4
else  ; com6
endif  ; com5

; ; this comments ; + spaces + inside
if a  ; com1
   ;/com 1/;
   foo()
   ;/com 2/;
   ;/com 3/;
   foo()
   ;/com 4/;
elseif b  ; com2
   foo()
   foo()
   foo()
elseif c  ; com3
   ;/com 5/;
elseif d  ; com4
   
   ;/com 5/;
   
else  ; com5
   ;/com 9/;
   ;/com 8/; foo() ;/com 6/;
   ;/com 7/;
endif  ; com6

endfunction

