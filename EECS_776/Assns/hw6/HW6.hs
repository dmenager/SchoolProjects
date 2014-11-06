{-# LANGUAGE GADTs, KindSignatures #-}

------------------------------------------------------
type Graph r = [(Int, r, [Int])]

empty :: Graph r
empty = []

count :: Graph r -> Int
count = length

-- Consider changing
addNode :: Int -> Graph r -> Graph r
addNode n r gs = (n, r, []) : gs
{-
addEdge :: Int -> Int -> Graph r -> Graph r
addEdge from to gs = [ if n == from
                       then (n,to : es)
                       else (n,es)
                     | (n,es) <- gs ]
 -}                    
------------------------------------------------------

type GM a r  = State (Graph a) r  

newtype State ::  * -> (* -> *) where
  State_ :: (s-> (a,s)) -> State s a

instance Monad (State s) where
  -- return :: a -> State s a
  return a = State_ (\ st0 -> (a,st0))
  -- (>>=) :: State s a -> (a -> State s b) -> State s b
  (State_ f) >>= k = State_ (\ st0 ->
        -- f :: (s -> (a,s))
          let (a,st1) = f st0 in
          case (k a) of
            State_ g -> g st1)

                     
get :: State s s
get = State_ (\ st -> (st,st))

set :: s -> State s ()
set n r = State_ (\ st -> ((), (n, r)))


runST :: s -> State s a -> a
runST s (State_ f) = fst (f s)

-------------------------------------------------------
-- type GM a = State Graph a

--runGM :: Graph -> State Graph a -> a
runGM :: Graph r -> GM a r -> a
runGM = runST

--addNodeM :: State Graph Int
addNodeM :: GM Int r
addNodeM = do
   gs <- get
   let n = if null gs then 0 else maximum (map fst gs) + 1
   set (addNode n gs)
   return n
   
reifyM :: GM (Graph a) r
reifyM = get
   
