(quicklisp:quickload 'cl-heap)
(quicklisp:quickload 'cl-heap-tests)

#| Solves 8-puzzle problem |#

; puzzle = 8-puzzle init state
; goal = goal state of 8-puzzle
; RETURN: Path to solve puzzle
(defun solve (puzzle goal)
  (let ((frontier (make-instance 'cl-heap:priority-queue))
	(explored (make-hash-table)))
    (cl-heap:enqueue frontier puzzle (cost puzzle goal))
    (solve-manager puzzle goal frontier explored)))

(defun solve-manager (puzzle goal front ex)
  (if (null front)
      '()
      (let* ((path (A* front))
	     (s (first (last path)))
	     (moves (applicable-moves s)))
	(setf (gethash s ex) s)
	(if (goal? s goal)
	    (return-from solve-manager path))
	(map '()
	     #'(lambda (a)
		 (let ((r (move s a)))
		   (unless (or (gethash r ex)
			       (find-q r (copy-structure front)))
		     (cl-heap:enqueue front 
				      (cons (reverse (cons a (reverse path))) r) 
				      (cost (cons (reverse (cons a (reverse path))) r)
					    goal)))))
	     moves)
	(solve-manager puzzle goal front ex))))

#|Pop priority queue till we find item |#

; item = item to find in queue
; priority = priority queue to pop
; RETURN: T / '()
(defun find-q (item priority)
  (let ((q priority))
    (if (null (cl-heap:dequeue q))
	'()
	(if (equal item (cl-heap:dequeue q))
	    t
	    (find-q item q)))))

#| Find the path that minimizes the sum of the actual and heuristic cost |#

; frontier = frontier
; RETURN: min cost path. frontier = frontier - path
(defun A* (frontier)
  (cl-heap:dequeue frontier))

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
		 (push (list :tile (nth 0 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 1 r)
		 (push (list :tile (nth 0 (nth (- r 1) state)) :move 'down) ap-moves)
		 (push (list :tile (nth 0 (nth (+ 1 r) state)) :move 'up) ap-moves))
	       (when (= 2 r)
		 (push (list :tile (nth 0 (nth (- r 1) state)) :move 'down) ap-moves)))))))
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
; ex = explored list
; puzzle = 8-puzzle
; RETURN: state, action, weight triples
(defun move (st ex puzzle action)
  )

#| Null heuristic |#

; tile = tile in 8-puzzle
; goal = 8-puzzle goal state
; RETURN: 0 for all distances to goal for tile
(defun null-heuristic (tile goal)
  0)

#| Manhattan distance heuristic |#

; tile = tile in 8-puzzle
; goal = 8-puzzle goal state
; RETURN: Hattan distance of tile to goal position
(defun man-dist (path)
  )

#|Cost to get from state to goal state |#

; path = a path on frontier
; goal = goalstate
; RETURN: cost to goal on path
(defun cost (path goal)
  )

(defun total-cost (front)
  (let ((ret '()))
    (map '()
	 #'(lambda (path)
	     (cons (+ (cost path) (man-dist path)) ret))
	 front)
    ret))
