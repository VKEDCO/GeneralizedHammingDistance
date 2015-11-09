To run ehd:

1. compile ehd.lisp.
2. change package to USER.
3. load ehd.fasl.
4. Evaluate:

> (ehd parm-tbitv parm-sbitv-1 1.0 1.0 .5)
#(0.0 1.0 2.0 3.0 1.0 0.0 1.0 2.0 2.0 1.0 ...)
0.5
> (ehd parm-tbitv parm-sbitv-2 1.0 1.0 .5)
#(0.0 1.0 2.0 3.0 1.0 0.0 1.0 2.0 2.0 1.0 ...)
2.0
> (ehd parm-tbitv parm-tbitv 1.0 1.0 .5)
#(0.0 1.0 2.0 3.0 1.0 0.0 1.0 2.0 2.0 1.0 ...)
0.0
> (fast-ehd parm-tbitv parm-sbitv-1 1.0 1.0 0.5)
0.5

parm-tbitv, parm-sbitv-1, and parm-sbitv-2 are
bit arrays defined in ehd.lisp as follows:

(defparameter parm-tbitv
  (make-array '(10)
              :element-type 'bit
              :initial-contents '(1 1 0 0 1 0 0 0 0 0)))

(defparameter parm-sbitv-1 
  (make-array '(10)
              :element-type 'bit
              :initial-contents '(1 1 0 0 0 1 0 0 0 0)))

(defparameter parm-sbitv-2 
  (make-array '(10)
              :element-type 'bit
              :initial-contents '(1 1 0 0 0 0 0 0 0 1)))

In general, the smaller the ehd score the closer
the two bit vectors are.

