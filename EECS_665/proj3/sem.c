# include <stdio.h>
# include <stdlib.h>
# include "cc.h"
# include "semutil.h"
# include "sem.h"
# include "sym.h"

extern int formalnum;
extern char formaltypes[]; // 
extern int localnum;
extern char localtypes[];
extern int localwidths[];

int numlabels = 0;                      /* total labels in file */
int numblabels = 0;                     /* total backpatch labels in file */
int isFirst = 1;
struct node {
  struct node* prev;
  char* name;
  int labelId;
};

struct node* root;
struct node* end;

struct sem_rec* root_b;
struct sem_rec* end_b;

struct sem_rec* root_c;
struct sem_rec* end_c;

struct sem_rec* worker;
/*
 * backpatch - backpatch list of quadruples starting at p with k
 */
void backpatch(struct sem_rec *p, int k)
{
  //fprintf(stderr, "sem: backpatch not implemented\n");
  printf("B%d = L%d\n", p->s_place, k);
}

/*
 * bgnstmt - encountered the beginning of a statement
 */
void bgnstmt()
{
  extern int lineno;

  printf("bgnstmt %d\n", lineno);
}

/*
 * call - procedure invocation
 */
struct sem_rec *call(char *f, struct sem_rec *args)
{
  struct id_entry* p;
  int argcount = 0;
  while(args) {
    if(args->s_mode != T_INT) {
      printf("argf t%d\n", args->s_place);
    }
    else {
      printf("argi t%d\n", args->s_place);
    }
    args = args->back.s_link;
    argcount++;
  }
  
  printf("t%d := global %s\n", nexttemp(), f);
  
  if((p = lookup(f, 0)) == NULL) {
    p = install(f, -1);
    p->i_type = T_INT;
    p->i_scope = LOCAL;
    p->i_defined = 1;
  }
  
  int cur = currtemp();

  if(p->i_type == T_INT) {
    printf("t%d := fi t%d %d\n", nexttemp(), cur, argcount);
  }
  else {
    printf("t%d := ff t%d %d\n", nexttemp(), cur, argcount);
  }
  argcount = 0;

  return (node (cur, p->i_type | T_ADDR, (struct sem_rec*) NULL, (struct sem_rec*) NULL));
}

/*
 * ccand - logical and
 */
struct sem_rec *ccand(struct sem_rec *e1, int m, struct sem_rec *e2)
{
  //fprintf(stderr, "sem: ccand not implemented\n");
  backpatch(e1->back.s_true, m);
  e2->s_false = e1->s_false;
  return e2;
}

/*
 * ccexpr - convert arithmetic expression to logical expression
 */
struct sem_rec *ccexpr(struct sem_rec *e)
{
   struct sem_rec *t1;

   if(e){
   
     t1 = gen("!=", e, cast(con("0"), e->s_mode), e->s_mode);
     
     printf("bt t%d B%d\n", t1->s_place, ++numblabels);
     printf("br B%d\n", ++numblabels);
     return (node(0, 0,
		  node(numblabels-1, 0, (struct sem_rec *) NULL, 
		       (struct sem_rec *) NULL),
		  node(numblabels, 0, (struct sem_rec *) NULL, 
		       (struct sem_rec *) NULL)));
   }
   else
     fprintf(stderr, "Argument sem_rec is NULL\n");
}

/*
 * ccnot - logical not
 */
struct sem_rec *ccnot(struct sem_rec *e)
{
  //fprintf(stderr, "sem: ccnot not implemented\n");
  struct sem_rec* p;
  p = e->back.s_true;
  e->back.s_true = e->s_false;
  e->s_false = p;
  
  return e;
}

/*
 * ccor - logical or
 */
struct sem_rec *ccor(struct sem_rec *e1, int m, struct sem_rec *e2)
{
  //fprintf(stderr, "sem: ccor not implemented\n");
  backpatch(e1->s_false, m);
  e2->back.s_true = e1->back.s_true;
  return e2;
}

/*
 * con - constant reference in an expression
 */
struct sem_rec *con(char *x)
{
  struct id_entry *p;

  if((p = lookup(x, 0)) == NULL) {
    p = install(x, 0);
    p->i_type = T_INT;
    p->i_scope = GLOBAL;
    p->i_defined = 1;
  }

  /* print the quad t%d = const */
  printf("t%d = %s\n", nexttemp(), x);
  
  /* construct a new node corresponding to this constant generation 
     into a temporary. This will allow this temporary to be referenced
     in an expression later*/
  return(node(currtemp(), p->i_type, (struct sem_rec *) NULL,
	      (struct sem_rec *) NULL));
}

/*
 * dobreak - break statement
 */
void dobreak()
{
  printf("br B%d\n", ++numblabels);
  
  if(root_b == NULL) {
    root_b = (struct sem_rec*)malloc(sizeof(struct sem_rec));
    end_b = root_b;
  }
  else {
    end_b->back.s_link = (struct sem_rec*)malloc(sizeof(struct sem_rec));
    end_b = end_b->back.s_link;
  }

  end_b->s_place = numblabels;
   leaveblock();
}

/*
 * docontinue - continue statement
 */
void docontinue()
{
    printf("br B%d\n", ++numblabels);
  
  if(root_c == NULL) {
    root_c = (struct sem_rec*)malloc(sizeof(struct sem_rec));
    end_c = root_c;
  }
  else {
    end_c->back.s_link = (struct sem_rec*)malloc(sizeof(struct sem_rec));
    end_c = end_c->back.s_link;
  }

  end_c->s_place = numblabels;
  leaveblock();
}

/*
 * dodo - do statement
 */
void dodo(int m1, int m2, struct sem_rec *e, int m3)
{
  backpatch (e->back.s_true, m1);
  backpatch (e->s_false, m3);

  worker = root_b;
  while(worker) {
    backpatch(worker, m3);
    worker = worker->back.s_link;
  }

  worker = root_c;
  while(worker) {
    backpatch(worker, m2);
    worker - worker->back.s_link;
  }
  
}

/*
 * dofor - for statement
 */
void dofor(int m1, struct sem_rec *e2, int m2, struct sem_rec *n1,
           int m3, struct sem_rec *n2, int m4)
{
  backpatch(e2->back.s_true, m3);
  backpatch(e2->s_false, m4);
  backpatch(n1, m1);
  backpatch(n2, m2);

  worker = root_b;
  while(worker) {
    backpatch(worker, m4);
    worker = worker->back.s_link;
  }

  worker = root_c;
  while(worker) {
    backpatch(worker, m1);
    worker - worker->back.s_link;
  }
}

/*
 * dogoto - goto statement
 */
void dogoto(char *id)
{
  //fprintf(stderr, "sem: dogoto not implemented\n");
  struct node* it = end;

  while(it) {
    if(it->name == id) {
      printf("br L%d\n", it->labelId);
      break;
    }
    it = it->prev;
  }  
}

/*
 * doif - one-arm if statement
 */
void doif(struct sem_rec *e, int m1, int m2)
{
  //fprintf(stderr, "sem: doif not implemented\n");

  backpatch(e->back.s_true, m1);
  backpatch(e->s_false, m2);
}

/*
 * doifelse - if then else statement
 */
void doifelse(struct sem_rec *e, int m1, struct sem_rec *n,
                         int m2, int m3)
{
  //fprintf(stderr, "sem: doifelse not implemented\n");
  backpatch(e->back.s_true, m1);
  backpatch(e->s_false, m2);
  backpatch(n, m3);
}

/*
 * doret - return statement
 */
void doret(struct sem_rec *e)
{
  //check type of e
  printf("ret");

  switch(e->s_mode) {
  case T_INT:
    printf("i t%d\n", e->s_place);
    break;
  case T_DOUBLE:
    printf("f t%d\n", e->s_place);
    break;
  }
}

/*
 * dowhile - while statement
 */
void dowhile(int m1, struct sem_rec *e, int m2, struct sem_rec *n,
             int m3)
{
  backpatch(e->back.s_true, m2); 
  backpatch(e->s_false, m3);
  backpatch(n, m1);

  worker = root_b;
  while(worker) {
    backpatch(worker, m3);
    worker = worker->back.s_link;
  }

  worker = root_c;
  while(worker) {
    backpatch(worker, m1);
    worker - worker->back.s_link;
  }
}

/*
 * endloopscope - end the scope for a loop
 */
void endloopscope(int m)
{
  leaveblock();
}

/*
 * exprs - form a list of expressions
 */
struct sem_rec *exprs(struct sem_rec *l, struct sem_rec *e)
{
  e->back.s_link = l;
  return e;
}

/*
 * fhead - beginning of function body
 */
void fhead(struct id_entry *p)
{
  int i = 0;

  //print the formal parameter janks
  for(i; i < formalnum; i++) {
    switch(formaltypes[i]) {
    case 'i':
      printf("formal %d\n", 4);
      break;
    case 'f':
      printf("formal %d\n", 8);
      break;
    }
  }
  
  i = 0;
  //print the local janks
  for(i; i < localnum; i++) {
    switch(localtypes[i]) {
    case 'i':
      printf("localloc %d\n", 4);
      break;
    case 'f':
      printf("localloc %d\n", 8);
      break;
    }
  }  
}

/*
 * fname - function declaration
 * t = type
 * id = name of function
 */
struct id_entry *fname(int t, char *id)
{
  enterblock();
  printf("func %s\n", id);
  return ((struct id_entry *) NULL);
}

/*
 * ftail - end of function body
 */
void ftail()
{
  printf("fend\n");
  leaveblock();
   
  //reset bounds
  formalnum = 0;
  localnum = 0;
}

/*
 * id - variable reference
 */
struct sem_rec *id(char *x)
{
   struct id_entry *p;

   if ((p = lookup(x, 0)) == NULL) {
      yyerror("undeclared identifier");
      p = install(x, -1);
      p->i_type = T_INT;
      p->i_scope = LOCAL;
      p->i_defined = 1;
   }
   if (p->i_scope == GLOBAL)
     printf("t%d := global %s\n", nexttemp(), x);
   else if (p->i_scope == LOCAL)
      printf("t%d := local %d\n", nexttemp(), p->i_offset);
   else if (p->i_scope == PARAM) {
      printf("t%d := param %d\n", nexttemp(), p->i_offset);
      if (p->i_type & T_ARRAY) {
         (void) nexttemp();
         printf("t%d := @i t%d\n", currtemp(), currtemp()-1);
      }
   }

   /* add the T_ADDR to know that it is still an address */
   return (node(currtemp(), p->i_type|T_ADDR, (struct sem_rec *) NULL,
                (struct sem_rec *) NULL));
}

/*
 * index - subscript
 */
struct sem_rec *tom_index(struct sem_rec *x, struct sem_rec *i)
{
  return (gen("[]", x, cast(i, T_INT), x->s_mode&~(T_ARRAY)));
}

/*
 * labeldcl - process a label declaration
 */
void labeldcl(char *id)
{
  //fprintf(stderr, "sem: labeldcl not implemented\n");
  m();
  if(isFirst == 1) {
    root = (struct node*)malloc(sizeof(struct node));
    end = root;
    isFirst = 0;
  }
  root->name = id;
  root->labelId = numlabels;
  root->prev = (struct node*)malloc(sizeof(struct node));
  root = root->prev;
}

/*
 * m - generate label and return next temporary number
 */
int m()
{
  printf("label L%d\n",++numlabels);
   return (numlabels);
}

/*
 * n - generate goto and return backpatch pointer
 */
struct sem_rec *n()
{
  printf("br B%d\n", ++numblabels);
  struct sem_rec *p = (struct sem_rec *)malloc(sizeof(struct sem_rec));

  p->s_place = numblabels;
  return p;
}

/*
 * op1 - unary operators
 */
struct sem_rec *op1(char *op, struct sem_rec *y)
{
  if (*op == '@' && !(y->s_mode&T_ARRAY)){
    /* get rid of T_ADDR if it is being dereferenced so can handle
       T_DOUBLE types correctly */
    y->s_mode &= ~T_ADDR;
    return (gen(op, (struct sem_rec *) NULL, y, y->s_mode));
  }
  else{
    fprintf(stderr, "sem: op1 not implemented\n");
    return ((struct sem_rec *) NULL);
  }
}

/*
 * op2 - arithmetic operators
 */
struct sem_rec *op2(char *op, struct sem_rec *x, struct sem_rec *y)
{
  //fprintf(stderr, "sem: op2 not implemented\n");
  struct sem_rec* p = cast(y, x->s_mode);
  struct sem_rec* q = gen(op, x, p, x->s_mode);
  
  return q;
}

/*
 * opb - bitwise operators
 */
struct sem_rec *opb(char *op, struct sem_rec *x, struct sem_rec *y)
{
  struct sem_rec* p = cast(y, x->s_mode);
  struct sem_rec* q = gen(op, x, p, x->s_mode);
  
  return q;
}

/*
 * rel - relational operators
 */
struct sem_rec *rel(char *op, struct sem_rec *x, struct sem_rec *y)
{
  if(y->s_mode != x->s_mode) {
    
    /*cast y to a double*/
    y = cast(y, x->s_mode);
  }
	   struct sem_rec* sr = gen(op, x, y, x->s_mode);

  printf("bt t%d B%d \n", sr->s_place, ++numblabels);
  sr->back.s_true = (struct sem_rec *) malloc(sizeof(struct sem_rec));
  sr->back.s_true->s_place = numblabels;
  
  printf("br B%d\n", ++numblabels);
  sr->s_false = (struct sem_rec *) malloc(sizeof(struct sem_rec));
  sr->s_false->s_place = numblabels;
  
  return sr;
}

/*
 * set - assignment operators
 */
struct sem_rec *set(char *op, struct sem_rec *x, struct sem_rec *y)
{
  /* assign the value of expression y to the lval x */
  struct sem_rec *p, *cast_y;

  if(*op!='\0' || x==NULL || y==NULL){
    int xType = x->s_mode;
    int yType = y->s_mode;

    if( x->s_mode & T_INT && !(y->s_mode &T_INT)) {
      xType = T_INT;
      yType = T_DOUBLE;
    }
    else if(!(x->s_mode & T_INT) && y->s_mode & T_INT) {
      yType = T_INT;
      xType = T_DOUBLE;
    }
    else if(!(x->s_mode & T_INT) && !(y->s_mode &T_INT)) {
      yType = T_DOUBLE;
      xType = T_DOUBLE;
    }

    struct sem_rec *q = gen("@", (struct sem_rec *) NULL, x, xType);
    struct sem_rec *r;
    if(y->s_mode == T_ARRAY) {
      r = gen("@", (struct sem_rec *) NULL, cast(y, xType), yType);
    }
    else {
      r = cast(y, xType);
    }
    
    struct sem_rec *t = gen(op, q, r, xType);
    return gen("=", x, t, xType);
  }

  /* if for type consistency of x and y */
  cast_y = y;
  if((x->s_mode & T_DOUBLE) && !(y->s_mode & T_DOUBLE)){
    
    /*cast y to a double*/
    printf("t%d = cvf t%d\n", nexttemp(), y->s_place);
    cast_y = node(currtemp(), T_DOUBLE, (struct sem_rec *) NULL,
		  (struct sem_rec *) NULL);
  }
  else if((x->s_mode & T_INT) && !(y->s_mode & T_INT)){

    /*convert y to integer*/
    printf("t%d = cvi t%d\n", nexttemp(), y->s_place);
    cast_y = node(currtemp(), T_INT, (struct sem_rec *) NULL,
		  (struct sem_rec *) NULL);
  }

  /*output quad for assignment*/
  if(x->s_mode & T_DOUBLE)
    printf("t%d := t%d =f t%d\n", nexttemp(), 
	   x->s_place, cast_y->s_place);
  else
    printf("t%d := t%d =i t%d\n", nexttemp(), 
	   x->s_place, cast_y->s_place);

  /*create a new node to allow just created temporary to be referenced later */
  return(node(currtemp(), (x->s_mode&~(T_ARRAY)),
	      (struct sem_rec *)NULL, (struct sem_rec *)NULL));
}

/*
 * startloopscope - start the scope for a loop
 */
void startloopscope()
{
  enterblock();
}

/*
 * string - generate code for a string
 */
struct sem_rec *string(char *s)
{
  printf("t%d := %s\n", nexttemp(), s);
  struct sem_rec* p = (struct sem_rec*)malloc(sizeof(struct sem_rec));
  p->s_place = currtemp();
  return p;
}



/************* Helper Functions **************/

/*
 * cast - force conversion of datum y to type t
 */
struct sem_rec *cast(struct sem_rec *y, int t)
{
   if (t == T_DOUBLE && y->s_mode != T_DOUBLE)
      return (gen("cv", (struct sem_rec *) NULL, y, t));
   else if (t != T_DOUBLE && y->s_mode == T_DOUBLE)
      return (gen("cv", (struct sem_rec *) NULL, y, t));
   else
      return (y);
}

/*
 * gen - generate and return quadruple "z := x op y"
 */
struct sem_rec *gen(char *op, struct sem_rec *x, struct sem_rec *y, int t)
{
   if (strncmp(op, "arg", 3) != 0 && strncmp(op, "ret", 3) != 0)
      printf("t%d := ", nexttemp());
   if (x != NULL && *op != 'f')
      printf("t%d ", x->s_place);
   printf("%s", op);
   if (t & T_DOUBLE && (!(t & T_ADDR) || (*op == '[' && *(op+1) == ']'))) {
      printf("f");
      if (*op == '%')
         yyerror("cannot %% floating-point values");
   }
   else
      printf("i");
   if (x != NULL && *op == 'f')
      printf(" t%d %d", x->s_place, y->s_place);
   else if (y != NULL)
      printf(" t%d", y->s_place);
   printf("\n");
   return (node(currtemp(), t, (struct sem_rec *) NULL,
           (struct sem_rec *) NULL));
}
