{-# LANGUAGE GADTs, KindSignatures, OverloadedStrings #-}
import Data.List
import Graphics.Blank hiding (fillStyle, strokeStyle)
import Graphics.Blank.Style
import qualified Data.Text as T
import Data.Text(Text)
import Control.Applicative
import Control.Monad
import Text.Printf
import Debug.Trace
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

execST :: s -> State s a -> s
execST s (State_ f) = snd (f s)
-- -------------------------------------------------

type DS = [(Int, Int)]
type Disjoint a = State DS a
type Color = Text

init_ds :: DS
init_ds = []

-- Finds root representative for x
find_ds :: Int -> DS -> Int 
find_ds x adt = if  fst (adt !! x) == x then fst (adt !! x)  else find_ds (fst(adt !! x)) adt

index_of :: Int -> DS -> Int
index_of x adt = if fst (adt !! x) == x then snd (adt !! x) else index_of (fst (adt !! x)) adt

-- Unions two sets together
fast_union :: Int -> Int -> DS -> DS
--fast_union x y adt = take (find_ds x adt) adt ++ [(find_ds y adt, index_of x adt)] ++ drop((find_ds x adt) + 1) adt
fast_union x y adt = take (find_ds x adt) adt ++ [(x, index_of y adt)] ++ drop((find_ds x adt) + 1) adt 


addNode :: Int -> Int-> DS -> DS
addNode idx p ds = ds ++ [(p, idx)]

runDisjoint :: DS -> Disjoint Int -> Int
runDisjoint = runST

findDS :: Int -> Disjoint Int
findDS x = do
  s <- get 
  return (find_ds x s)
-- union values together...not idecies
funion :: Int -> Int -> Disjoint ()
funion x y = do
  s <- get
  trace (show s) $ return ()
  trace (show x) $ return ()
  trace (show y) $ return ()
  set (fast_union x y s)
  return ()

addNodeM :: Int -> Int -> Disjoint Int
addNodeM idx r = do
  s <- get
  --trace (show s) $ return ()
  set(addNode idx r s)
  return r

main :: IO ()
main = do
     let x = execST init_ds $ do                                                               
                               addNodeM 0 0
                               addNodeM 1 1
                               addNodeM 2 2
                               addNodeM 3 3
                               addNodeM 4 4
                               addNodeM 5 5
                               addNodeM 6 6
                               addNodeM 7 7
                               addNodeM 8 8
                               funion 1 2
                               funion 2 4
         
     blankCanvas 3000 $ \ context ->
         send context $ do
              forM_ x  $ \ item -> do
                      let offset = 200
                      let centerX = fromIntegral $ 300 + offset * ((fst item) - 1)
                      let yoffset = 100
                      let centerY = height context / 2
                      let radius = 79
                      let r_base = 25
                      let g_base = 100
                      let b_base = 75
                      let num = fst item
                      let idx = snd item
                      
                      beginPath()
                      fillStyle $ rgba (r_base * fromIntegral idx) (g_base * fromIntegral idx) (b_base * fromIntegral idx) 0.5 
                      arc(centerX, centerY, radius, 0, 2 * pi, False)
                      closePath ()
                      lineWidth 5
                      strokeStyle ("black":: Text)
                      stroke()
                      font "30pt Calibri"
                      fillText ((T.pack (show idx)), centerX, centerY)
                      fill()
                      
