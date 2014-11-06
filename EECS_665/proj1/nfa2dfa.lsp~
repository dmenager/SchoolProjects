;David Menager
;Compiler Construction EECS_665
;Dr. Prasad Kulkarni
;3 October 2014

;Variable Definitions

; *DSTATES* = list of e-closures
; *E-STACK* = list of states
; Mark states by incrementing a counter
(defvar *DSTATES* '())
(defvar *E-STACK* '())

; alphabet = list of input characters
; states = list of State structures
(defstruct DTran name alphabet states i-state f-states)
(defstruct State name transitions)

(defun split-str (string &optional (separator " "))
  (split-str-1 string separator))

(defun split-str-1 (string &optional (separator " ") (r nil))
  (setq string (remove #\} (remove #\{ string)))
  (let ((n (position separator string
		     :from-end t
		     :test #'(lambda (x y)
			       (find y x :test #'string=)))))
    (if n
	(split-str-1 (subseq string 0 n) separator (cons (subseq string (1+ n)) r))
	(cons string r))))

(defun parse-commas (string)
  (setq lst (split-str string ","))
  (setq ret '())
  (map '() #' (lambda (x)
		(push (parse-integer x) ret))
       lst)
  (setq ret (reverse ret))
  ret)

(defun build-nfa ()
  (setq transTable (make-DTran :name 'nfa))
  (setq *STANDARD-INPUT* (open "test.txt" :direction :input :element-type :default))
  (setq desc '())

  (do ((line (read-line *STANDARD-INPUT* nil)
	     (read-line *STANDARD-INPUT* nil)))
      ((null line))
    (push (split-str line " ") desc))
  (setq desc (reverse desc))
  
  (setf (Dtran-i-state transTable) (parse-integer (first (last (first desc)))))
  (pop desc)
  
  (setf (Dtran-f-states transTable) (parse-commas (first (last (first desc)))))
  (pop desc)

  (pop desc)
  (pop (first desc))
  
  (setf (first desc) (map 'list #'string-upcase (first desc)))
  
  (setf (DTran-alphabet transTable) (map 'list #'intern (first desc)))
  
  (if (eq (first (last (DTran-alphabet transTable))) 'E)
      (progn
	(setf (DTran-alphabet transTable) (remove 'E (Dtran-alphabet transTable)))
	(setf (DTran-alphabet transTable) (append (DTran-alphabet transTable) (list '~)))))
  (pop desc)
  
  (setq trans '())
  
  (map '() #'(lambda (x)
	       (setq nfa-state-name (parse-integer (first x)))
	       (setq trans '())
	       
	       ;(princ x)
	       ;(princ #\newline)
	       
	       (pop x)
	       ;(princ "*** begin ***")
	       ;(princ #\newline)
	       ;(princ #\newline)
	       (do ((value (first x) (first x)))
		   ((null x))
		 ;(princ "sublist: ") (princ x)
		 ;(princ #\newline)
		 ;(princ "list element: ")
		 (if (not (string= "" value))
		     (progn
		       ;(princ value)
		       (push (parse-commas value) trans))
		     (progn
		       ;(princ "null")
		       (push '() trans)))
		 ;(princ #\newline)
		 ;(princ "trans = ") (princ trans)
		 ;(princ #\newline)
		 ;(princ #\newline)
		 (pop x))
	       
	       (setq trans (reverse trans))
	       
	       (setf (DTran-states transTable) (append (DTran-states transTable) (list (make-State :name nfa-state-name :transitions trans)))))
       desc)
  ;(princ "----------------------------------------------------------------")
  ;(princ #\newline)
  ;(princ #\newline)
  (nfa2dfa))


(defun nfa2dfa ()
  (setq dfa (make-DTran :name 'dfa :alphabet (remove '~ (DTran-alphabet transTable))))
  (let ((n-states (DTran-states transTable))
	(marked -1)
	(trans '())
	(s '()))
    (setq s (e-closure (list (first n-states))))

    (setq *DSTATES* s)
    (setf (DTran-states dfa) (append (DTran-states dfa) s))
    (do ((u-state (nth (1+ marked) *DSTATES*) (nth (1+ marked) *DSTATES*)))
	((= (length *DSTATES*) (1+ marked)))
      
      (princ "Working on unmarked state: ") (princ u-state)
      (princ #\newline)
      (PRINC #\newline)

      (setq marked (1+ marked))

      (map '() #'(lambda (x)
		   (if (not (eq '~ x))
		       (progn
			 (setq state (first (e-closure (move (list u-state) x))))
			 ;(princ state)
			 ;(princ #\newline)
			 ;(princ #\newline)
			 (setq insert t)
			 
			 (if (not (null state))
			     (progn
			       (map '() #'(lambda (x)
					    (if (equal (State-name state) (State-name x))
						(setq insert '())))
				    *DSTATES*)
			 
			       (if (and insert (not (null state)))
				   (progn
				   (setq *DSTATES* (append *DSTATES* (list state)))
				   (if (not (listp (State-name state)))
				       (setf (State-name state) (list (State-name state))))
				   (setf (DTran-states dfa) (append (DTran-states dfa) (list state))))))))))
	   (DTran-alphabet transTable))
      
   
      (princ "---------------------------------------------")
      (princ #\newline)
      (princ #\newline)))
  (map '() #'(lambda (st)
	       (map '() (lambda (name-bit)
			  (if (eq name-bit (DTran-I-State transTable))
			      (setf (DTran-I-State dfa) (State-name st)))
			  (if (member name-bit (DTran-F-States transTable))
			      (setf (DTran-F-States dfa) (push (State-name st) (DTRan-F-States dfa)))))
		    (State-name st))) 
	       (DTran-States dfa))
  (if (null (Dtran-states dfa))
      (progn
	(princ "Machine is already a DFA")
	(return-from nfa2dfa)))
  dfa)

; returns set of all states T reachable from state on E
; states = list of state structures
(defun e-closure (states)

  ;(if (not (eql (first (last (DTran-alphabet transTable))) '~))
  ;    (return-from e-closure))


  (if (null states)
      (progn 
	;(princ "State is null. Returning")
	(setq s-transition '(nil))
	(setq *E-STACK* '())
	(princ #\newline)
	(return-from e-closure)))

  (princ "e-elosure for ") (princ states)
  (princ #\newline)
 
  (setq *E-STACK* '())
  (map 'list #'(lambda (x)
	       (push x *E-STACK*))
       states)
  (setq e-closure-set states) 
	
  (loop while (not (null *E-STACK*)) do
       (setq s (pop *E-STACK*))
       (map '() #'(lambda (u)
		    (let ((teTrans (first (last (State-transitions s)))))
		      (if (and (member (State-name u) teTrans) (not (member u e-closure-set :test #'tree-equal)))
			  (progn
			    ;(princ "Appending ") (princ u) (princ " to e-closure-set of ") (princ states)
			    ;(princ #\newline)
			    (setq e-closure-set (append e-closure-set (list u)))
			    
			    ;smash the first two states
			    (setq e-closure-trans (mapcar #'append (State-transitions (first e-closure-set)) (State-transitions (second e-closure-set))))
			    
			    (setq e-closure-trans (map 'list #'remove-duplicates e-closure-trans))
			    ;(princ e-closure-trans)
			    ;(princ #\newline)
			    
			    (setq e-closure-name (append (list (State-name (first e-closure-set))) (list (State-name (second e-closure-set)))))
			    (if (eq (type-of (list 1)) (type-of (first e-closure-name)))
				    (setq e-closure-name (append (first (butlast e-closure-name)) (last e-closure-name))))

			    ;(princ e-closure-name)
			    ;(princ #\newline)
			    ; remove the first two states
			    (pop e-closure-set)
			    ;(princ e-closure-set) (princ #\newline)
			    (pop e-closure-set)
			    ;(princ e-closure-set) (princ #\newline)
			    (push (make-State :name e-closure-name :transitions e-closure-trans) e-closure-set)
			    (setq *E-STACK* (append (list u) *E-STACK*))))))
	    (DTran-states transTable)))
e-closure-set)

; set is set of states
(defun move (set in)
  (setq class '())
  (map '() #' (lambda (x)
		(let* ((trans (State-transitions x))
		       (idx (position in (DTran-alphabet transTable)))
		       (tMove (nth idx trans)))
		  (map '() #'(lambda (y)
			       (map '() #'(lambda (z)
					    (if (eql (State-name z) y)
						(push z class)))
				    (DTran-states transTable)))
		       tMove)))
       set)
  (princ set) (princ " --") (princ in) (princ "--> ") (princ class)
  (princ #\newline)
  class)

(defun clear ()
  (setq *E-STACK* '())
  (setq e-closure-set '())
  (setq class '())
  (setq *DSTATES* '())
  (setq transTable '())
  (setq dfa '()))

(defun debug-make ()
  (clear)
  ;(setq start (make-State :name 'start :transitions '('() '() (zero))))
  (setq zero (make-State :name 'zero :transitions '((one) '() '())))
  (setq one (make-State :name 'one :transitions '('() (zero) (two three))))
  (setq two (make-State :name 'two :transitions '((two) '() '())))
  (setq three (make-State :name 'three :transitions '('() (three) '())))

  (setq transTable (make-DTran :name 'nfa :alphabet '(a b ~) :states (list zero one two three))))
  

#|Tests
(setq start (make-State :name 'start :transitions '('() '() (zero))))
(setq zero (make-State :name 'zero :transitions '((one) '() '())))
(setq one (make-State :name 'one :transitions '('() (zero) (two three))))
(setq two (make-State :name 'two :transitions '((two) '() '())))
(setq three (make-State :name 'three :transitions '('() three '())))

(setq transTable (make-DTran :name 'nfa :alphabet '(a b E) :states (list start zero one two three)))

(e-closure (list (second (DTran-states transTable))))
(move (list (first (Dtran-states transTable))) 'a)

(e-closure (move (list (second (DTran-states transTable))) 'a))|#
