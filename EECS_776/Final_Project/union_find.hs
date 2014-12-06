{-# LANGUAGE GADTs, KindSignatures, OverloadedStrings #-}
import Data.List
import Graphics.Blank
-- -----------------------------------------------------------------
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
set n  = State_ (\ st -> ((), n))

runST :: s -> State s a -> a
runST s (State_ f) = fst (f s)
-- -------------------------------------------------

type DS = [Int]
type Disjoint a = State DS a

init_ds :: DS
init_ds = []

-- Finds root representative for x
find_ds :: Int -> DS -> Int 
find_ds x adt = if adt !! x == x then adt !! x else find_ds (adt !! x) adt

-- Unions two sets together
fast_union :: Int -> Int -> DS -> DS
fast_union x y adt = take (find_ds x adt) adt ++ [find_ds y adt] ++ drop(find_ds x adt + 1) adt

addNode :: Int-> DS -> DS
addNode p ds = ds ++ [p]

runDisjoint :: DS -> Disjoint Int -> Int
runDisjoint = runST

findDS :: Int -> Disjoint Int
findDS x = do
  s <- get 
  return (find_ds x s)

funion :: Int -> Int -> Disjoint ()
funion x y = do
  s <- get
  set (fast_union x y s)
  return ()

addNodeM :: Int -> Disjoint Int
addNodeM r = do
  s <- get
  set(addNode r s)
  return r

main :: IO()
main = blankCanvas 3000 $ \ context -> do
         send context $ do
           sequence_
              [do beginPath()
                  moveTo(200, height context / 2 + n)
                  lineTo(200, height context / 2 + n)
                  lineWidth 200
                  strokeStyle "#000ff"
                  lineCap cap
                  stroke()
               | (cap, n) <- zip["round"] [-50,0,50]
              ]
