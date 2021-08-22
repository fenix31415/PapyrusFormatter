Scriptname a

function foo()
  a = true || false * 10.0 + "hsdb" + none
  a = a && a

  ;/ nospace /;
  x = a < b && c > c || d != s + a == -1 * 2 + 3
  x = (a < b) && (c > c || d) != (s) + (a == -1) * (((((((2 + 3)))))))

  ; coms
  ;/com/; x ;/com/; = ;/com/; a ;/com/; < ;/com/; b ;/com/; && ;/com/; c ;/com/; > ;/com/; c ;/com/; || ;/com/; d ;/com/; != ;/com/; s ;/com/; + ;/com/; a ;/com/; == ;/com/; -1 ;/com/; * ;/com/; 2 ;/com/; + ;/com/; 3 ;/com/;
  ;/com/; x ;/com/; = ;/com/; (;/com/; a ;/com/; < ;/com/; b ;/com/;) ;/com/; && ;/com/; (;/com/; c ;/com/; > ;/com/; c ;/com/; || ;/com/; d ;/com/;) ;/com/; != ;/com/; (;/com/; s ;/com/;) ;/com/; + ;/com/; (;/com/; a ;/com/; == ;/com/; -1 ;/com/;) ;/com/; * ;/com/; (;/com/; (;/com/; (;/com/; (;/com/; (;/com/; (;/com/; (;/com/; 2 ;/com/; + ;/com/; 3 ;/com/;) ;/com/;) ;/com/;) ;/com/;) ;/com/;) ;/com/;) ;/com/;) ;/com/;

  ; unars
  a = -x - (-(x)) + !(x) && !none + (+x) + (+123) * (-15.0) + (+"????????")
  ;/com/; a ;/com/; = ;/com/; - ;/com/; x ;/com/; - ;/com/; (;/com/; - ;/com/; (;/com/; x ;/com/;) ;/com/;) ;/com/; + ;/com/; ! ;/com/; (;/com/; x ;/com/;) ;/com/; && ;/com/; ! ;/com/; none ;/com/; + ;/com/; (;/com/; + ;/com/; x ;/com/;) ;/com/; + ;/com/; (;/com/; + ;/com/; 123 ;/com/;) ;/com/; * ;/com/; (;/com/; - ;/com/; 15.0 ;/com/;) ;/com/; + ;/com/; (;/com/; + ;/com/; "????????" ;/com/;) ;/com/;
  b = (s as a) as b + c

  ; calls
  foo(1, a=(10) as bool + 10 as int, none, foo(), (v.d.g[67].d.foo()[10]))
  ;/com/; foo ;/com/; (;/com/; 1 ;/com/;, a ;/com/; = ;/com/; (;/com/; 10 ;/com/;) ;/com/; as ;/com/; bool ;/com/; + ;/com/; 10 ;/com/; as ;/com/; int ;/com/;, none ;/com/;, foo ;/com/; (;/com/;), (;/com/; v.d.g[;/com/; 67 ;/com/;].d.foo ;/com/; (;/com/;) ;/com/; [;/com/; 10 ;/com/;] ;/com/;) ;/com/;) ;/com/;
  foo(1, 1, 1, 1, 1)
  ;/com/; foo ;/com/; (;/com/; 1 ;/com/;, ;/com/; 1 ;/com/;, ;/com/; 1 ;/com/;, ;/com/; 1 ;/com/;, ;/com/; 1 ;/com/;) ;/com/;

  ; cant handle it :(
  ;  a;/com/;.;/com/;b;/com/;.;/com/;c;/com/;.;/com/;d;/com/;.;/com/;e ;/com/;= ;/com/;f;/com/;
  a.b.c.d.e = f

  ; arrays
  a = new int[10]
  a[a] = 10
  a = new a[314]

  ; coms
  ;/com/; a ;/com/; = ;/com/; new ;/com/; int ;/com/;[;/com/; 10 ;/com/;] ;/com/;
  ;/com/; a ;/com/; [;/com/; a ;/com/;] ;/com/; = ;/com/; 10 ;/com/;
  ;/com/; a ;/com/; = ;/com/; new ;/com/; a ;/com/;[;/com/; 314 ;/com/;] ;/com/;

  ; init
  int a
  int a = 10
  int a = e + x * p && r
  int[] a = new int[10]
  a[] a = new a[10]
  ;/com/; int ;/com/; a ;/com/;
  ;/com/; int ;/com/; a ;/com/; = ;/com/; 10 ;/com/;
  ;/com/; int ;/com/; a ;/com/; = ;/com/; e ;/com/; + ;/com/; x ;/com/; * ;/com/; p ;/com/; && ;/com/; r ;/com/;
  ;/com/; int ;/com/; [;/com/;] ;/com/; a ;/com/; = ;/com/; new ;/com/; int ;/com/;[;/com/; 10 ;/com/;] ;/com/;
  ;/com/; a ;/com/; [;/com/;] ;/com/; a ;/com/; = ;/com/; new ;/com/; a ;/com/;[;/com/; 10 ;/com/;] ;/com/;

  ;ret
  ;/com/; return ;/com/;
  ;/com/; return ;/com/; a ;/com/;
  ;/com/; return ;/com/; a ;/com/; + ;/com/; a ;/com/;
  return
  return a
  return a + a
endfunction
