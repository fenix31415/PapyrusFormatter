Scriptname a

function a()
  while a
  endwhile
  while a
    foo()
  endwhile
  while a
    ; com
  endwhile
  while a
    ; com
    foo()
    foo()
    a = 1

    ; com
  endwhile

  ;large
  while a
    ; com
    while a
      ; com
      foo()
      foo()
      a = 1

      ; com
    endwhile
    foo()
    foo()
    while a
      ; com
      foo()
      foo()
      a = 1

      ; com
    endwhile
    a = 1
    while a
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile

      ; com
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile
      foo()
      foo()
      while a
        ; com
        foo()
        while a
          ; com
          foo()
          foo()
          while a
            ; com
            foo()
            foo()
            while a
              ; com
              foo()
              while a
                ; com
                foo()
                while a
                  ; com
                  foo()
                  while a
                    while a
                      ; com
                      foo()
                      while a
                        ; com
                        foo()
                        while a
                          ; com
                          foo()
                          while a
                            ; com
                            foo()
                            foo()
                            a = 1

                            ; com
                          endwhile
                          foo()
                          a = 1

                          ; com
                        endwhile
                        foo()
                        a = 1

                        ; com
                      endwhile
                      foo()
                      a = 1

                      ; com
                    endwhile

                    ; com
                    foo()
                    foo()
                    a = 1

                    ; com
                  endwhile
                  foo()
                  a = 1

                  ; com
                endwhile
                foo()
                a = 1

                ; com
              endwhile
              foo()
              a = 1

              ; com
            endwhile
            a = 1

            ; com
          endwhile
          a = 1

          ; com
        endwhile
        foo()
        a = 1

        ; com
      endwhile
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile
      a = 1

      ; com
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile
    endwhile
    while a
      ; com
      foo()
      foo()
      a = 1

      ; com
    endwhile

    ; com
    while a
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile

      ; com
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile
      foo()
      foo()
      a = 1
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile

      ; com
    endwhile
    while a
      ; com
      foo()
      foo()
      a = 1

      ; com
    endwhile
    while a
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile

      ; com
      while a
        ; com
        foo()
        foo()
        a = 1

        ; com
      endwhile
      foo()
      foo()
      a = 1

      ; com
    endwhile
  endwhile

  ; coms, nospaces
  ;/com6/; while ;/com1/; a ;/com2/;
    ;/com3/;
    ;/com4/;
    ;/com8/; foo() ;/com9/;
  ;/com7/; endwhile ;/com5/;

  ; coms, spaces
  ;/com6/; while ;/com1/; a ;/com2/;
    ;/com3/;
    ;/com4/;
    ;/com8/; foo() ;/com9/;
  ;/com7/; endwhile ;/com5/;
endfunction
