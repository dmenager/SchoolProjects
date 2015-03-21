(load "proj1.lsp")

(defun variablep (x) 
  (symbolp x))

(defun negatedp (x)
  (eq '¬ x))
	 
(defun negated-termp (x)
  (negatedp (first x)))

(defun value-of (x θ)
  (cdr (assoc x θ)))
	 
(defun boundp (v θ)
  (pairp (assoc v θ)))

(defun pairp (l)
  (not (listp (cdr l))))
	 
(defun get-binding (v b)
  (assoc v b))

(defun binding-var (b)
  (car b))
	 
(defun binding-val (b)
  (cdr b))

(defun make-binding (var binding)
  (cons var binding))

(defun lookup (v bs)
  (cdr (assoc v bs)))

(defun extend-bindings (var val bindings)
  (cons (cons var val) bindings))

(defun reuse-cons (x y xy)
  (if (and (eq x (car xy))
	   (eq y (cdr xy)))
      xy
      (cons xy)))

(defun subst-bindings (bindings x)
  (cond
    ((eq x '()) '())      
    ((eq bindings '()) '())
    ((eq bindings '()) x)
    ((and (variablep x) 
	  (assoc x bindings))
     (subst-bindings bindings (cdr (assoc x bindings))))
    ((atom x) x)
    (t (reuse-cons (subst-bindings bindings (first x))
		   (subst-bindings bindings (rest x))
		   x))))

(defun unify  (x y θ)
  (cond
    ((eq θ '()) '())
    ((and (variablep x)
	  (apply boundp x θ)) 
     (unify (value-of x θ) y θ))
    ((and (variablep y)
	  (boundp y θ)) 
     (unify x (value-of y θ) θ))
    ((eq x y) θ)
    ((variablep x) 
     (if (occur? x y θ) 
	 '() 
	 (cons (cons y x) θ)))
    ((variablep y) 
     (if (occur? y x θ) 
	 '() 
	 (cons (cons y x) θ)))
    ((equal x y) θ)
    ((not (and (pairp x) (pairp y))) '())
    ((not (eq (car x) (car y))) '())
    ((not (= (length x) (length y))) '())
    (t (reduce #'unify (list θ (cdr x) (cdr y))))))

(defun mormap (fun list)
  (if (remove-duplicates (mapcar fun list))))

(defun occur? (v x θ)
  (cond
    ((and (variablep x)
	  (boundp x θ)) 
     (occur? v (value-of x θ) θ))
    ((eq v x) t)
    ((variablep v) '())
    ((not (pairp x)) '())
    (t 
     (let ((res (mapcar (lambda (&rest y) 
			  (occur? v y θ)) 
			(cdr x))))
       (if (null (not (remove-duplicates res)))
	   '()
	   (first res))))))
  
  (define (replace-term a-list exp)
    (cond
      [(variable? exp) (let [(replacement (assoc exp a-list))] (if replacement (cdr replacement) exp))]
      [(pair? exp) (cons (replace-term a-list (car exp)) (replace-term a-list (cdr exp)))]
      [else (map (λ(x) (replace-term a-list x)) exp)]
      ))
  (define (instantiate x e) (subst-bindings e x))
  (define (unique-find-anywhere-if exp found-so-far)
    (cond
      [(eq? exp '()) found-so-far]
      [else (if (atom? exp)
                (if (and (variable? exp) (not (member exp found-so-far))) (cons exp found-so-far) found-so-far)
                (unique-find-anywhere-if (rest (first exp)) (unique-find-anywhere-if (rest (rest exp)) found-so-far)))])
    )
  (define (freevarsin x) (unique-find-anywhere-if x '()))
  (define (rename-variables x) (subst-bindings (map (λ(y) (cons y (gensym y))) (freevarsin x)) x))
  (define (instantiate-clause c a) (map (λ(x) (instantiate x a)) c))
  (define (rename-clause c) (map (λ(x) (rename-variables x)) c))
  (define (freevarsin-clause c) (append (map (λ(x) (rename-variables x))c)))
  (define (unifier x y) (instantiate x (unify x (rename-variables y) '())))
  (define (resolve lc kd)
    (if (is-not-term? (first lc))
        (let* ([renamed-lc (rename-clause lc)] [unifiable (unify (first (rest (first renamed-lc))) (first kd) '())])
          (if unifiable (rename-clause (instantiate-clause (append (cdr renamed-lc) (cdr kd)) unifiable)) #f)
          )
        #f ))
  
  (define (res-moves s) 
    (filter-map 
     (λ(rhs) (let ([resolved (resolve s rhs)]) (if resolved (list resolved (first rhs) 1) #f))) 
     definite-horn-clauses) 
    )


(defun deduce (definite-horn-clauses top-clause)
  (let* (
	 
    


;(defun fol-bc-ask (kb query)
;  fol-bc-or(kb query '()))
;
;(defun fol-bc-or (kb, goal theta)
;  (let ((subsitutions '()))
;    (dolist (rule (fetch-rules-for-goal kb goal))
;      (setq rule (standardize-variables rule))
;      (dolist (theta-prime (fol-bc-and kb 
;				       (cdr rule) 
;				       goal 
;				       (unify (cdr rule) 
;					      goal 
;					      theta)))
;	(cons theta-prime substitutions)))
;    subsittutions))
;
;(defun fetch-rules-for-goal (kb goal))
;
;(defun standardize-variables (rule))
;
;(defun fol-bc-and (kb, goals theta))
