(quicklisp:quickload 'cl-heap)
(quicklisp:quickload 'cl-heap-tests)

#| Solves 8-puzzle problem |#

; puzzle = 8-puzzle init state
; goal = goal state of 8-puzzle
; heuristicf = heuristic function
; goalf = goal function
; movef = move function
; RETURN: Path to solve puzzle
(defun solve (puzzle goal heuristicf goalf movef)
  (let ((frontier (make-instance 'cl-heap:priority-queue))
	(explored (make-hash-table :test #'equal)))
    (cl-heap:enqueue frontier (list puzzle) (total-cost puzzle puzzle goal heuristicf))
    ;(format t "Added ~S to frontier ~%" (list puzzle))
    (A* goal frontier explored heuristicf goalf movef)))

(defun A* (goal front ex heuristicf goalf movef)
  (if (null (cl-heap:peep-at-queue front))
      (format t "No solution: Explored nodes: ~d~%" (hash-table-size ex))
      (let* ((path (cl-heap:dequeue front))
	     (s (first (last path)))
	     (moves (applicable-moves s)))
	;(format t "Number of moves: ~S~%" (length moves))
	(setf (gethash s ex) s)
	(if (funcall goalf s goal)
	    (return-from A* path))
	(map '()
	     #'(lambda (a)
		 (let ((r (funcall movef s a)))
		   (if (not (nth-value 1 (gethash r ex)))
		       (progn
			 (let ((g (cost (reverse (cons r (cons a (reverse path))))))
			       (h (funcall 
				   heuristicf 
				   r
				   goal))
			       (f (total-cost r 
					      (reverse (cons r (cons a (reverse path))))
					      goal
					      heuristicf)))
			   (setq a (reverse (cons f (cons h (cons g (reverse a))))))
			 ;(format t "result was not in frontier or explored list~%")
			 (cl-heap:enqueue front 
					  (reverse (cons r (cons a (reverse path)))) 
					  (total-cost r
						      (reverse (cons r (cons a (reverse path))))
						      goal
						      heuristicf)))))))
			 ;(format t "Added ~S to frontier~%" 
				; (reverse (cons r (cons a (reverse path))))))
		       ;(format t "Result was in frontier or explored list~%"))))
	     moves)
	(A* goal front ex heuristicf goalf movef))))

#| Find candidates to move around |#

; state = puzzle config
(defun applicable-moves (state)
  (let ((ap-moves '()))
    (do ((r 0 (+ r 1)))
	((= r (length state)))
      (let ((row (nth r state)))
	(if (member 0 row)
	    (cond
	      ((eq 0 (position 0 row))
	       (push (list :tile (nth (+ 1 (position 0 row)) row) :move 'left) ap-moves)
	       (when (= 0 r)
		 (push (list :tile (nth 0 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 1 r)
		 (push (list :tile (nth 0 (nth (- r 1) state)) :move 'down) ap-moves)
		 (push (list :tile (nth 0 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 2 r)
		 (push (list :tile (nth 0 (nth (- r 1) state)) :move 'down) ap-moves)))	       
	       ((eq 1 (position 0 row))
		(push (list :tile (nth (+ 1 (position 0 row)) row) :move 'left) ap-moves)
		(push (list :tile (nth (- (position 0 row) 1) row) :move 'right) ap-moves)
		(when (= 0 r)
		  (push (list :tile (nth 1 (nth (+ 1 r) state)) :move 'up) ap-moves))
		(when (= 1 r)
		  (push (list :tile (nth 1 (nth (- r 1) state)) :move 'down) ap-moves)
		  (push (list :tile (nth 1 (nth (+ 1 r) state)) :move 'up) ap-moves))
		(when (= 2 r)
		  (push (list :tile (nth 1 (nth (- r 1) state)) :move 'down) ap-moves)))
	       ((eq 2 (position 0 row))
		(push (list :tile (nth (- (position 0 row) 1) row) :move 'right) ap-moves)
	       (when (= 0 r)
		 (push (list :tile (nth 2 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 1 r)
		 (push (list :tile (nth 2 (nth (- r 1) state)) :move 'down) ap-moves)
		 (push (list :tile (nth 2 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 2 r)
		 (push (list :tile (nth 2 (nth (- r 1) state)) :move 'down) ap-moves)))))))
    (reverse ap-moves)))

#| Determines if state is a goal state |#

; st = state of 8-puzzle
; goal = 8-puzzle goal state 
; RETURN: T or '()
(defun goal? (st goal)
  (if (equal st goal)
      t
      '()))

#| Represents the move from input state |#

; st = input st
; a = action to take
; puzzle = 8-puzzle
; RETURN: state
(defun move (st a)
  (let* ((state (copy-tree st))
	(tile (getf a :tile))
	(res (find-tile tile state)))
    (cond 
      ((eq 'left (getf a :move))
       (let*
	   ((tgt (nth (position (first res) state :test #'equal) state))
	    (pos (second res))
	    (val (nth (second res) tgt)))
	 (setf (nth (- pos 1) tgt) tile)
	 (setf (nth pos tgt) 0)))
      ((eq 'right (getf a :move))
       (let*
	   ((tgt (nth (position (first res) state :test #'equal) state))
	    (pos (second res))
	    (val (nth (second res) tgt)))
	 (setf (nth (+ pos 1) tgt) tile)
	 (setf (nth pos tgt) 0)))
      ((eq 'up (getf a :move))
       (let*
	   ((tgt (nth (- (position (first res) state :test #'equal) 1) state))
	    (pos (second res))
	    (val (nth (second res) tgt)))
	 (setf (nth pos tgt) tile)
	 (setf (nth pos (nth (position (first res) state :test #'equal) state)) 0)))
      ((eq 'down (getf a :move))
       (let*
	   ((tgt (nth (+ (position (first res) state :test #'equal) 1) state))
	    (pos (second res))
	    (val (nth (second res) tgt)))
	 (setf (nth pos tgt) tile)
	 (setf (nth pos (nth (position (first res) state :test #'equal) state)) 0))))
  state))

(defun find-tile (tile st)
  (let ((pair '()))
    (map '()
	 #'(lambda (row)
	     (if (not (null (position tile row)))
		 (progn
		   (push row pair)
		   (push (position tile row) pair))))
	 st)
  (reverse pair)))

#| Null heuristic |#

; tile = tile in 8-puzzle
; goal = 8-puzzle goal state
; RETURN: 0 for all distances to goal for tile
(defun null-heuristic (tile goal)
  0)

#| Manhattan distance heuristic |#

; st = state of puzzle
; goal = 8-puzzle goal state
; RETURN: Manhattan distance of tile to goal position
(defun man-dist (st goal)
  (let ((res '()))
    (do ((r 0 (+ r 1)))
	((= r (length st)))
      (let ((row (nth r st)))
	(do ((st-t 0 (+ st-t 1)))
	    ((= st-t (length row)))
	  (let* ((tile (nth st-t row))
		 (goal-r '())
		 (goal-t  '()))
	    (do ((goal-r 0 (+ goal-r 1)))
		(( = goal-r (length goal)))
	      (if (member tile (nth goal-r goal))
		  (progn
		    (setq goal-t (position tile (nth goal-r goal)))
		    (setq res (cons (+ (abs (- r goal-r)) (abs (- st-t goal-t))) res)))))))))
    (reduce #'+ res)))

#|Cost to get from start to current state |#

; path = a path on frontier
; RETURN: cost of path from start
(defun cost (path)
  (floor (/ (length path) 2)))

(defun total-cost (st path goal heuristicf)
  (+ (cost path) (funcall heuristicf st goal)))

(defun run ()
  (let ((st '((6 4 2) (1 5 3) (7 0 8)))
	(goal '((1 2 3) (4 5 6) (7 8 0))))
    (format t "Hueristic~S~%No Heuristic ~S~%~%" 
	    (time (solve st goal 'man-dist 'goal?'move))
	    (time (solve st goal 'null-heuristic 'goal?'move))))
    
  (let ((st '((6 4 2) (8 5 3) (1 0 7)))
	(goal '((1 2 3) (4 5 6) (7 8 0))))
    (format t "Hueristic~S~%No Heuristic ~S~%~%" 
	    (time (solve st goal 'man-dist 'goal?'move))
	    (time (solve st goal 'null-heuristic 'goal?'move))))
 
  (let ((st '((8 0 7) (6 5 4) (3 2 1)))
	(goal '((1 2 3) (4 5 6) (7 8 0))))
    (format t "Hueristic~S~% No Heuristic ~S~%~%"
	    (time (solve st goal 'man-dist 'goal?'move))
	    (time (solve st goal 'null-heuristic 'goal?'move))))
 
  (let ((st '((6 4 7) (8 5 0) (3 2 1)))
	(goal '((1 2 3) (4 5 6) (7 8 0))))
    (format t "Hueristic~S~%No Heuristic ~S~%~%" 
	    (time (solve st goal 'man-dist 'goal?'move))
	    (time (solve st goal 'null-heuristic 'goal?'move))))
  
  (let ((st '((1 2 3) (4 5 6) (8 7 0)))
	(goal '((1 2 3) (4 5 6) (7 8 0))))
    (format t "Hueristic~S~%No Heuristic ~S~%~%" 
	    (time (solve st goal 'man-dist 'goal?'move))
	    (time (solve st goal 'null-heuristic 'goal?'move)))))

#|
TESTS
(let ((st '((6 4 2) (1 5 3) (7 0 8)))
	       (goal '((1 2 3) (4 5 6) (7 8 0))))
	   (time (solve st goal 'man-dist 'goal?'move)))

(let ((st '((6 4 2) (8 5 3) (1 0 7)))
	       (goal '((1 2 3) (4 5 6) (7 8 0))))
	   (time (solve st goal 'man-dist 'goal?'move)))

(let ((st '((8 0 7) (6 5 4) (3 2 1)))
	       (goal '((1 2 3) (4 5 6) (7 8 0))))
	   (time (solve st goal 'man-dist 'goal?'move)))

(let ((st '((6 4 7) (8 5 0) (3 2 1)))
	       (goal '((1 2 3) (4 5 6) (7 8 0))))
	   (time (solve st goal 'man-dist 'goal?'move)))

(let ((st '((1 2 3) (4 5 6) (8 7 0)))
	       (goal '((1 2 3) (4 5 6) (7 8 0))))
	   (time (solve st goal 'man-dist 'goal?'move)))
|#
