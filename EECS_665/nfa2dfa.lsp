;David Menager
;Compiler Construction EECS_665
;Dr. Prasad Kulkarni
;3 October 2014

;Variable Definitions

; *DSTATES* (state0 state1 ... staten)
; *E-STACK (state0 state1 ... staten)
; Mark states by incrementing a counter
(defvar *DSTATES* '())
(defvar *E-STACK* '())
; alphabet = list of input characters
; states = list of State structures
(defstruct DTran name alphabet states)
(defstruct State name transitions)


(defun nfa2dfa ()
  (setq transTable (make-Dtran
		    :alphabet '(a b)
		    :states '()))

  (princ "made transition table")
  (do ((line (read-line)
	     (read-line)))
      ((null line))
    ;body begins here
    (princ (first line))
    (princ "done"))
  (let ((numStates 0))
    (setf numStates (+ numStates 1))))


; returns set of all states T reachable from state on E
; states = list of state structures
(defun e-closure (states)
  (map 'list #'(lambda (x)
	       (push x *E-STACK*))
       states)
  (setq e-closure-set (map 'list #'(lambda (x)
				     (State-name x))
			   states))
  (map '() #'(lambda (s)
	       (map '() #'(lambda (u)
			    (let ((teTrans (third (State-transitions s))))
			      (if (and (member (State-name u) teTrans) (not (member u e-closure-set)))
				      (progn
					(setf e-closure-set (append (list (State-name u)) e-closure-set))
					(setf *E-STACK* (append (list u) *E-STACK*))))))
			    (DTran-states transTable)))
		    *E-STACK*)
  e-closure-set)

; tt is temporary var for transTable
(defun move (set in tt)
  (loop for x in set
       do
       (let* ((state (car (member x (DTran-states tt) :key 'State-name :test 'eq)))
	      (trans (State-transitions state))
	      (idx (position in DTran-alphabet))
	      (tMove (nth idx trans)))
	 ; see if it has any transition from it to another state on in
	 ;(if (not (null tMove))
	     ;())))
	 )))

(defun clear ()
  (setq *E-STACK* '())
  (setq e-closure-set '()))

#|Tests
(setq start (make-State :name 'start :transitions '('() '() (zero))))
(setq zero (make-State :name 'zero :transitions '(one '() '())))
(setq one (make-State :name 'one :transitions '('() zero (two three))))
(setq two (make-State :name 'two :transitions '(two '() '())))
(setq three (make-State :name 'three :transitions '('() three '())))

(setq transTable (make-DTran :name 'nfa :alphabet '(a b E) :states (list zero one two three)))

(e-closure (list (second (DTran-states transTable))))|#
