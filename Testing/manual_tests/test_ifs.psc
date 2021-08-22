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
elseif c
elseif d
else


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

endfunction