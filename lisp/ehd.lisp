#|
======================================================================
File: ehd.lisp
Author: Vladimir Kulyukin
Description: Extended Hamming Distance of two
             bit strings. Bit strings are represented as one-dimensional
             arrays of 1's and 0's.
Copyright (c) 2000
All rights reserved.

The software is provided "AS IS" without any express or implied
warranties. Any use of this software for education and/or research
is hereby granted, provided that the above copyright information 
is explicitly stated in all versions of the source code. Any 
use of this software for commercial purposes requires written 
permission of the copyright holder.
=======================================================================
|#

(in-package "USER")

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

(define-condition ehd-tableau-condition (simple-error)
  ((i :initarg :i :reader get-i)
   (j :initarg :j :reader get-j))
  (:report (lambda (condition stream)
             (format stream "Uknown case in tableau at C[~D, ~D]"
               (get-i condition)
               (get-j condition)))))
    
(defun ehd (sbitv tbitv ci cd a)
  "Compute ehd between source bit vector sbitv and
target bit vector tbitv; ci is the cost of insertion;
cd is the cost of deletion; a is the constant that
determines the cost of shift; sample values: ci = cd = 1.0,
a = .5."
  (declare (type bit-vector sbitv) (type bit-vector tbitv)
           (type single-float ci) (type single-float cd)
           (type single-float a))
  (declare (optimize (speed 3) (safety 1)))
  (let* ((rowv (compute-1-bit-pos-vec sbitv))
         (colv (compute-1-bit-pos-vec tbitv t))
         (rows (fill-pointer rowv))
         (cols (fill-pointer colv)))
    (declare (type (simple-array (unsigned-byte 8) 1) rowv)
             (type (simple-array (unsigned-byte 8) 1) colv)
             (type fixnum rows) (type fixnum cols))
    (if (or (zerop rows) (zerop cols))
        (values nil 0.0)
      (let ((tableau 
             (compute-ehd
              (init-ehd-tableau rows cols ci cd) 
              rowv colv
              rows cols
              ci cd a)))
        (values
         tableau
         (aref tableau 
               (+ (* (1- rows) cols) (1- cols))))))))

(defun fast-ehd (sbitv tbitv ci cd a)
  "A faster version of ehd. It returns only
the edit distances. It does not return the
tableau."
  (declare (type bit-vector sbitv) (type bit-vector tbitv)
           (type single-float ci) (type single-float cd)
           (type single-float a))
  (declare (optimize (speed 3) (safety 1)))
  (let* ((rowv (compute-1-bit-pos-vec sbitv))
         (colv (compute-1-bit-pos-vec tbitv t))
         (rows (fill-pointer rowv))
         (cols (fill-pointer colv)))
    (declare (type (simple-array (unsigned-byte 8) 1) rowv)
             (type (simple-array (unsigned-byte 8) 1) colv)
             (type fixnum rows) (type fixnum cols))
    (if (or (zerop rows) (zerop cols))
        0.0
      (aref 
       (compute-ehd
        (init-ehd-tableau rows cols ci cd)
        rowv colv
        rows cols
        ci cd a)
       (+ (* (1- rows) cols) (1- cols))))))

(defun bit-img-ehd (bit-img img-width tbitv row from-c upto-c ci cd a)
  (declare (type bit-vector bit-img) (type fixnum img-width)
           (type bit-vector tbitv) (type fixnum row)
           (type fixnum from-c) (type fixnum upto-c)
           (type single-float ci) (type single-float cd) (type single-float a))
  (declare (optimize (speed 3) (safety 1)))
  (let* ((rowv (compute-1-bit-pos-vec-from-bit-img bit-img img-width 
                                                   row
                                                   from-c upto-c))
         (colv (compute-1-bit-pos-vec tbitv))
         (rows (fill-pointer rowv))
         (cols (fill-pointer colv)))
    (declare (type (simple-array (unsigned-byte 8) 1) rowv)
             (type (simple-array (unsigned-byte 8) 1) colv)
             (type fixnum rows)
             (type fixnum cols))
    (if (or (zerop rows) (zerop cols))
        .0
      (aref 
       (compute-ehd
        (init-ehd-tableau rows cols ci cd)
        rowv colv
        rows cols
        ci cd a)
       (+ (* (1- rows) cols) (1- cols))))))

(let ((1-bit-pos-vec
       (make-array 100
                   :element-type '(unsigned-byte 8)
                   :initial-element 0
                   :adjustable t
                   :fill-pointer 1)))
  (declare (type (vector (unsigned-byte 8) 1) 1-bit-pos-vec))
       
  (defun compute-1-bit-pos-vec (bitv &optional (new nil))
    (declare (type bit-vector bitv))
    (declare (optimize (speed 3) (safety 1)))
    (if new
        (compute-1-bit-pos-new-vec bitv)
      (loop
        initially (setf (fill-pointer 1-bit-pos-vec) 1)
        for b across bitv
        as i = 0 then (1+ i)
        when (eql b 1)
        do (vector-push-extend (1+ i) 1-bit-pos-vec)
        finally (return 1-bit-pos-vec))))
  )

(defun compute-1-bit-pos-new-vec (bitv)
  (declare (type bit-vector bitv))
  (declare (optimize (speed 3) (safety 1)))
  (let ((1-bit-pos-vec
         (make-array 10 :element-type '(unsigned-byte 8)
                     :initial-element 0
                     :adjustable t
                     :fill-pointer 1)))
    (declare (type (vector (unsigned-byte 8) 1) 1-bit-pos-vec))
    (loop
      for b across bitv
      as i = 0 then (1+ i)
      when (eql b 1)
      do (vector-push-extend (1+ i) 1-bit-pos-vec)
      finally (return 1-bit-pos-vec))))

(let ((1-bit-pos-vec
       (make-array 100 :initial-element 0
                   :element-type '(unsigned-byte 8)
                   :adjustable t
                   :fill-pointer 1)))
  (declare (type (simple-array (unsigned-byte 8) 1) 1-bit-pos-vec))
  
  (defun compute-1-bit-pos-vec-from-bit-img (bit-img img-width 
                                                     row from-c upto-c)
    (declare (type (simple-array (unsigned-byte 4) 2) bit-img)
             (type fixnum img-width)
             (type fixnum row)
             (type fixnum from-c)
             (type fixnum upto-c))
    (declare (optimize (speed 3) (safety 1)))
    (loop
      initially (setf (fill-pointer 1-bit-pos-vec) 1)
      for col from from-c upto upto-c
      as i = 0 then (1+ i)
      when (eql (aref bit-img (+ (* row img-width) col)) 1)
      do (vector-push-extend (1+ i) 1-bit-pos-vec)
      finally (return 1-bit-pos-vec)))
  
  )

(let ((tableau (make-array 2500 :initial-element .0
                           :element-type 'single-float
                           :adjustable t
                           :fill-pointer 0)))
  (declare (type (vector single-float 1) tableau))
  
  (defun init-ehd-tableau (rows cols ci cd)
    (declare (type fixnum rows) (type fixnum cols)
             (type single-float ci) (type single-float cd))
    (declare (optimize (speed 3) (safety 1)))
    (setf (fill-pointer tableau) (* rows cols)) 
    (loop
      for c from 0 below cols
      do (setf (aref tableau c) ;;; c = (+ (* 0 cols) c)
           (float (* c ci))))
    (loop
      for r from 0 below rows
      do (setf (aref tableau (* r cols)) ;;; (* r cols) = (+ (* r cols) 0)
           (float (* r cd))))
    tableau)
  
  (defun get-tableau () tableau)
  
  )
  
(defun compute-ehd (tableau rowv colv rows cols ci cd a)
  "Tableu is a one-dim array."
  (loop
    initially (when (or (eql rows 1) (eql cols 1))
                (return tableau))
    with width = cols
    for i from 1 below rows
    as ri = (aref rowv i)
    do (loop 
         for j from 1 below cols
         as cj = (aref colv j)
         do (if (eql ri cj)
                (setf (aref tableau (+ (* i width) j))
                  (aref tableau (+ (* (1- i) width) (1- j))))
              (let ((sc (shift-cost a ri cj 
                                    (aref tableau (+ (* (1- i) width) (1- j)))))) 
                (if (> ri cj)
                    (setf (aref tableau (+ (* i width) j))
                      (min (del-cost cd (aref tableau (+ (* (1- i) width) j)))
                           sc))
                  (if (< ri cj)
                      (setf (aref tableau (+ (* i width) j))
                        (min (ins-cost ci (aref tableau (+ (* i width) (1- j))))
                             sc))
                    (error (make-instance 'ehd-tableau-condition
                             :i i :j j)))))))
    finally (return tableau)))

(defun shift-cost (a ri cj t-entry)
  "Compute the cost of shifting a bit."
  (declare (type single-float a)
           (type fixnum ri)
           (type fixnum cj)
           (type single-float t-entry))
  (declare (optimize (speed 3) (safety 1)))
  (+ (* a (abs (- ri cj))) t-entry))

(defun ins-cost (ci t-entry)
  "Compute the cost of inserting a bit."
  (declare (type single-float ci)
           (type single-float t-entry))
  (declare (optimize (speed 3) (safety 1)))
  (+ ci t-entry))

(defun del-cost (cd t-entry)
  "Compute the cost of deleting a bit."
  (declare (type single-float ci)
           (type single-float t-entry))
  (declare (optimize (speed 3) (safety 1)))
  (+ cd t-entry))

;;; end-of-file

              

         
