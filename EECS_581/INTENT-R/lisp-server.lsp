(ql:quickload :usocket)
(ql:quickload :trivial-timers)

(defstruct atomic-action 
  (action 'nil :type symbol) 
  (with 'inv) 
  (who 'inv)  
  (what 'inv))

(defstruct entity
  (name 'nil :type symbol)
  (type 'nil :type symbol)
  (owner 'nil :type symbol))

(defstruct node 
  (state-name 0 :type integer)
  (state '() :type list)
  (edges '() :type list))

; consider changing this to a hash table of states to action
(defstruct graph 
  (nodes '() :type list)
  (cur-state 0 :type integer)
  (start-state 0 :type integer))

(defstruct (mdpr (:include graph)) 
  (states '() :type list) 
  (actions '() :type list)  
  (gamma 0.0 :type short-float))

(defstruct edge  
  (to 0 :type integer)
  (option-name 'nil :type symbol)
  (probability 0.00 :type short-float))

(defstruct option 
  (name 'nil :type symbol)
  (policy '() :type list)
  (termination-conditions '() :type list))

(defstruct term-cond
  (state-name 'nil :type symbol)
  (termination-prob 0.00 :type short-float))

(defstruct state
  (name '() :type integer)
  (data '() :type list))

(defstruct action
  (sub-actions '() :type list))

(defvar *thread-variables* '())

(defvar *atomic-actions* '())

(push (make-atomic-action :action 'attack :with '() :who '())
      *atomic-actions*)
(push (make-atomic-action :action 'barter :who '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'create :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'escort :who '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'gather :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'graze :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'heal :who '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'lock :what '())
      *atomic-actions*)
(push (make-atomic-action :action 'loot :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'move :who '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'patrol :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'repair :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'scout :what '()) 
      *atomic-actions*)
(push (make-atomic-action :action 'trade :what '() :who '())
      *atomic-actions*)

(defvar *entities* '((make-entity :name 'infantry-units :type 'infantry :owner '())
		    (make-entity :name 'cavalry-units :type 'cavalry :owner '())
		    (make-entity :name 'support-units :type 'support :owner '())
		    (make-entity :name 'siege-units :type 'siege :owner '())
		    (make-entity :name 'naval-units :type 'ship :owner '())
		    (make-entity :name 'structures :type 'structure :owner '())
		    (make-entity :name 'resources :type 'resourc :owner '())))
;(defvar *entities* '((make-entity :name 'swordsman :type 'infantry :owner '())
;		     (make-entity :name 'spearman :type 'infantry :owner '())
;		     (make-entity :name 'javelinist :type 'infantry :owner '())
;		     (make-entity :name 'archer :type 'infantry :owner '())
;		     (make-entity :name 'slinger :type 'infantry :owner '())
;		     (make-entity :name 'swordsman :type 'cavalry :owner '())
;		     (make-entity :name 'spearman :type 'cavalry :owner '())
;		     (make-entity :name 'javelinist :type 'cavalry :owner '())
;		     (make-entity :name 'archer :type 'cavalry :owner '())
;		     (make-entity :name 'female :type 'support :owner '())
;		     (make-entity :name 'trader :type 'support :owner '())
;		     (make-entity :name 'healer :type 'support :owner '())
;		     (make-entity :name 'onager :type 'siege :owner '())
;		     (make-entity :name 'ballista :type 'siege :owner '())
;		     (make-entity :name 'ram :type 'siege :owner '())
;		     (make-entity :name 'merchant-ship :type 'ship :owner '())
;		     (make-entity :name 'light-warship :type 'ship :owner '())
;		     (make-entity :name 'medium-warship :type 'ship :owner '())
;		     (make-entity :name 'heavy-warship :type 'ship :owner '())
;		     (make-entity :name 'animals :type 'herd)
;		     (make-entity :name 'animals :type 'hunt)
;		     (make-entity :name 'tree :type 'resource)
;		     (make-entity :name 'stone :type 'resource)
;		     (make-entity :name 'ore :type 'resource)
;		     (make-entity :name 'civic-centre :type 'structure :owner '())
;		     (make-entity :name 'house :type 'structure :owner '())
;		     (make-entity :name 'farmstead :type 'structure :owner '())
;		     (make-entity :name 'field :type 'structure :owner '())
;		     (make-entity :name 'corral :type 'structure :owner '())
;		     (make-entity :name 'mill :type 'structure :owner '())
;		     (make-entity :name 'outpost :type 'structure :owner '())
;		     (make-entity :name 'pallisade-wall :type 'structure :owner '())
;		     (make-entity :name 'pallisade-joint :type 'structure :owner '())
;		     (make-entity :name 'pallisade-gate :type 'structure :owner '())
;		     (make-entity :name 'dock :type 'structure :owner '())
;		     (make-entity :name 'market :type 'structure :owner '())
;		     (make-entity :name 'barracks :type 'structure :owner '())
;		     (make-entity :name 'temple :type 'structure :owner '())
;		     (make-entity :name 'defense-tower :type 'structure :owner '())
;		     (make-entity :name 'wall :type 'structure :owner '())
;		     (make-entity :name 'wall-turret :type 'structure :owner '())
;		     (make-entity :name 'city-gate :type 'structure :owner '())
;		     (make-entity :name 'commercial-port :type 'structure :owner '())
;		     (make-entity :name 'naval-shipyard :type 'structure :owner '())
;		     (make-entity :name 'embassy :type 'structure :owner '())))

(defun tcp-test-client (port)
  (setq conn (usocket:socket-connect usocket:*wildcard-host* port)))

(defun tcp-test-send ()
  (format (usocket:socket-stream conn) "Hello World!~%")
  (force-output (usocket:socket-stream conn)))

(defun tcp-server (port) 
  ; clear residual thread variables
  (setq *thread-variables* '())
 
  (let ((socket (usocket:socket-listen usocket:*wildcard-host*
				       port
				       :reuse-address t))
	(count 9))
    (unwind-protect 
	 (progn
	   (with-open-file (count-file "lastCount.txt"
				       :direction :input
				       :if-does-not-exist :error)
	     (setq count (read count-file nil))
	     (format *standard-output* "read ~d from counter!~%" count)
	     (clear-input count-file))
	   (loop 
	      (format *standard-output* "Waiting for input on ~A~%" socket)
	      (usocket:wait-for-input socket)
	      (let* ((connection (usocket:socket-accept socket))
		     (stream (usocket:socket-stream connection))
		     (thread-index count))
		
		(format *standard-output* "Connection made to client ~%")
		(setq *thread-variables* 
		      (append *thread-variables* (list (list '() t))))
		  ; handle the request on thread and let main accept new clients
		  (sb-thread:make-thread 
		   (lambda(std-out cnt)
		     (let* ((*standard-output* std-out)
			    (timer (trivial-timers:make-timer 
				    #'(lambda ()
					(format *standard-output* 
						"Connection timeout. Closing ~A.~%" 
						connection)
					(usocket:socket-close connection)	 
					(setf (second (nth cnt *thread-variables*)) '())
					(setf (first (nth cnt *thread-variables*)) '())))))
		       
		       ; 5 minute timeout
		       (trivial-timers:schedule-timer timer (* 5 60))
		     
		       (loop while (not (null (second (nth cnt *thread-variables*)))) do
			    (handle-request stream cnt std-out timer)
			    (clear-input stream)))) 
		   :arguments (list *standard-output* thread-index))
		  (incf count)
		  (with-open-file (write-count "lastCount.txt"
					       :direction :output
					       :if-exists :supersede
					       :if-does-not-exist :create)
		    (format write-count "~d~%" count)
		    (format *standard-output* "Wrote ~d to file~%" count)
		    (force-output write-count)))))
      (progn
	; wait for data to implement message handlers before the next 2 lines
	;(format stream "Connection closed due to inactivity.~%")
	(usocket:socket-close socket)))))

#| Service a request from a client |#

; stream = socket stream to client
; t-idx = index of thread variables for thread
; ostream = reference to standard out
; timer = timeout timer 
(defun handle-request (stream t-idx ostream timer)
  (trivial-timers:schedule-timer timer (* 5 60))
  (with-open-file (clientData (concatenate 'string 
					   "clientData/clientData"
					   (write-to-string t-idx) ".txt")
			      :direction :output
			      :if-exists :append
			      :if-does-not-exist :create)
    (let ((line (read-line stream nil 'the-end))
	  (*standard-output* ostream))
      (setf (first (nth t-idx *thread-variables*)) t)
      (format *standard-output* "Handling request ~%")
      (format *standard-output* "You said: ~S~%" line)
      (format clientData "You said: ~S~%" line))
    (force-output stream)
    (force-output clientData)))


(defun to-syms (inp)
  (let ((syms '()))
    (with-input-from-string (x inp)
      (do ((word (read x nil '()) (read x nil '())))
	  ((null word))
	(push word syms)))
    (reverse syms)))

#| Initialize MDP and expert's feature expectations |#

; init-file = states, transition probabilities, m trajectories
; return reward function   
(defun init-apprentice (init-file)
  
  ; create MDP/R
  (let ((mdp-r (make-mdpr :states '()
			  :actions '()
			  :nodes '()
			  :cur-state 0
			  :start-state 0)))
    
  
    ; update :owner for each player in game
    ; fill state/action space
    (setf (mdpr-states mdp-r) (make-state-space '()))
    (setf (mdpr-actions mdp-r) (subsets *atomic-actions* (mdpr-actions mdp-r)))
  
    ; create transition probabilites
    (make-nodes mdp-r)
    ; create expert's feature expectations

    ;return reward function
    mdp-r
    
    ;(discover-reward mdp-r '() '())
  ))

#| Define the state space for the MDP |#

; states = state list to fill
(defun make-state-space (states)
  (with-open-file (client-data "testStates.txt"
			       :direction :input
			       :if-does-not-exist :error)
    (let ((count 0))
      
      (do ((line (read-line client-data nil) 
		 (read-line client-data nil)))
	  ((null line))
	(let ((list (to-syms line)))
	  (setq states (reverse (cons (make-state :name count :data list)
				      (reverse states)))))
	(incf count)))
    states))

#| Convert decimal to binary list|#

; n = number to convert to binary
(defun binary-list (n &optional acc)
  (cond ((zerop n) (or acc (list 0)))
        ((plusp n)
         (binary-list (ash n -1) (cons (logand 1 n) acc)))
        (t (error "~S: non-negative argument required, got ~s" 'binary-list n))))

#| Define action space for MDP |#

; list = list of atomic actions
; atts = string list of atomic-action attributes
(defun make-action-space (actions atts)
  (if (null atts)
      (let ((ret '()))
	(setq *atomic-actions* 
	      (append *atomic-actions* (combinations 2 *atomic-actions*) '(())))
	 
	(loop while (>= 0 *total-actions*) do
	     (let ((item (cons (binary-list *total-actions*) 
			       (list (getf (nth *total-actions* *atomic-actions*) :action)
				     (getf (nth *total-actions* *atomic-actions*) :with)
				     (getf (nth *total-actions* *atomic-actions*) :who)
				     (getf (nth *total-actions* *atomic-actions*):what)))))
	       (cons item ret))
	     (decf *total-actions*))
	 ret)
      (let* ((att (first atts))
	    (set (remove-if #'(lambda (action)
				(eq (eval (reverse (cons action 
							 (reverse 
							  (to-syms (concatenate 'string 
										"atomic-action-" 
										att)))))) 'inv)) 
			    actions))
	    (applicable (remove-if-not  
			 #'(lambda (entity)
			     (cond
			       ((or (string= "with" att) 
				    (string= "who" att)) 
				(or (eq 'infantry (entity-type (eval entity))) 
				    (eq 'cavalry (entity-type (eval entity)))
				    (eq 'siege (entity-type (eval entity)))
				    (eq 'support (entity-type (eval entity)))
				    (eq 'ship (entity-type (eval entity)))
				    (eq 'structure (entity-type (eval entity)))))
			       ((string= "what" att) 
				(or (eq 'infantry (entity-type (eval entity))) 
				    (eq 'cavalry (entity-type (eval entity)))
				    (eq 'siege (entity-type (eval entity)))
				    (eq 'support (entity-type (eval entity)))
				    (eq 'ship (entity-type (eval entity)))))
			       (t (format *standard-output* "Invalid action attribute~%"))))
			 *entities*)))
	
	(format t "Actions: ~S~%Applicable Entities: ~S~%~%" set applicable)
    
	(map '() 
	     #'(lambda (x) 
		 (setq *atomic-actions* 
		       (remove x *atomic-actions* :test #'equal)))
	     set)
	
	(map '() 
	     #'(lambda (action)
		 (map '() 
		      #'(lambda (entity)
			  (let ((a (copy-structure action)))
			    (cond
			      ((string= "with" att)
			       (setf (atomic-action-with a) (eval entity)))
			      ((string= "who" att) 
			       (setf (atomic-action-who a) (eval entity)))
			      ((string= "what" att) 
			       (setf (atomic-action-what a) (eval entity))))
			    (push  a *atomic-actions*)))
		      applicable))
	     set)
    (make-action-space *atomic-actions* (rest atts)))))

; action space with 4 actions per move: 11,725,156 actions. This assumes action space of 130: intractable
; action space with 3 actions per move: 36,6276 actions. This assumes action space of 130: intractable
; action space with 2 actions per move: 8,516 actions. This assumes action space of 130
; action space with no generalizations: intractable.
(defun combinations (count list)
  (cond
    ((zerop count) '(())) ; one combination of zero element.
    ((endp list)   '())   ; no combination from no element.
    (t (nconc (mapcar (let ((item (first list))) 
			(lambda (combi) 
			  (cons item combi)))
                      (combinations (1- count) (rest list)))
              (combinations count (rest list))))))


#| build transition graph |#

; mdpr = mdpr simulation
(defun make-nodes (mdpr)
  ; make a node for each state
  (let ((count 0))
    (map 'list 
	 #'(lambda (s)
	       (setf (mdpr-nodes mdpr) 
		     (reverse (cons (make-node :state-name count :state (state-data s)) 
				    (reverse (mdpr-nodes mdpr)))))
	     (incf count))
	 (mdpr-states mdpr))))

#| Assign the transition probabilities for each node |#

; TODO: figure real transition probabilities
; TODO: (edge-to)
(defun make-transition-probs (mdpr))

#| Discovers reward function |#

; mdpr = mdp w/o reward funtion -- simulator
; phi = feature mapping -- (state -> state features)
; mue = expert's feature expectations
; returns reward
(defun discover-reward (mdpr phi mue)
  
  (let ((p (make-hash-table)))
    ;add elements to p
    (map '() 
	 #'(lambda (state)	    
	    (setf (gethash (state-name state) p) 
		  (nth (+ 1 (random (length (mdpr-actions mdpr)))) 
		       (mdpr-actions mdpr))))
	 (mdpr-states mdpr))
    (format t "Pi: ~A~%" p)))
    ;(mu p)))

#| Compute feature expectation of policy, pi |#

; mdpr = MDP simulator
; p = policy
; returns feature expectations of p
(defun mu (mdpr p)
  ;expected value of the sum of the discount * phi
  (let* ((err .01)
	 (gamma .5)
	 ;e-horizon time
	 (He (log (* err (- 1 gamma)) gamma)))
    
    (mapcar #'(lambda (&rest x) (reduce #'+ x))  
	    (map 'list 
		 #'(lambda (st)
		     (do ((i 0 (1+ i))
			  (>= i He))
			 (map 'list 
			      #'(lambda (comp)
				  (* (expt gamma i) comp)) 
			      st)))
		 (act p)))))
  
#| Get a random percent |#

(defun random-percent ()
  (/ (+ 0 (random (+ 1 (- 100 0)))) 100))

#| Simulate an action in the MDP-R|#

; p = policy
; mdpr = mdpr
; returns state
(defun act (p mdpr) 
  )


#| SCRATCH 

; test I did for mu functionality
(mapcar #'(lambda (&rest x)
		     (reduce #'+ x)) '(1 2 3 4) '(1 2 3 4))

; server printfs
(format *standard-output* 
		 "THREAD VARIABLES ~A~%COUNT ~d~%" 
		 *thread-variables* 
		 thread-index)

(format ostream "conn-timeout: Thread Variables ~A~%conn-timeout: t-idx ~d~%" 
	    *thread-variables*
	    t-idx)

(format ostream "Using count: ~d~%" t-idx) 

; Filling action space
(remove-if-not #'(lambda (x)
			    (getf x :a)) '((:a 1) (:b 2)))
; modifying plist
(let ((x '(:a 1 :b 2)))
	   (setf (getf x :a) 'artist)
	   x)|#
